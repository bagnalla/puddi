#include <iostream>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "AnimatedObject.h"
#include "Shader.h"
#include "Util.h"

using namespace std;
using namespace glm;

namespace puddi
{
    //  BONE PUBLIC

    /*void Bone::SetBindPose(const glm::mat4& bPose)
    {
        bindPose = bPose;
        bindPoseInverse = inverse(bPose);
    }*/

    namespace Skeleton
    {
        // PRIVATE
        namespace
        {
            unordered_map<string, Bone> skeletonMap;
            unordered_map<string, unordered_map<string, Bone> > skeletonBoneMap;
            unordered_map<string, vector<Bone> > skeletonBoneArrays;
            unordered_map<string, vector<ObjectAnimation> > animationMap;

            GLuint boneTransformBuffer;
            GLuint boneTransformTexture;

            vector<string> collectBoneNames(const aiScene *scene)
            {
                vector<string> boneNames;

                // this doesn't necessarily get all the bone names because some bones
                // only exist for the sake of hierarchical transforms but don't directly
                // influence any meshes
                /*if (scene->HasMeshes())
                {
                    for (size_t i = 0; i < scene->mNumMeshes; ++i)
                    {
                        auto mesh = scene->mMeshes[i];
                        if (mesh->HasBones())
                        {
                            for (size_t j = 0; j < mesh->mNumBones; ++j)
                            {
                                auto bone = mesh->mBones[j];
                                boneNames.push_back(bone->mName.C_Str());
                            }
                        }
                    }
                }*/

                // this seems to get all of the bones.
                if (scene->HasAnimations())
                {
                    for (size_t i = 0; i < scene->mNumAnimations; ++i)
                    {
                        auto anim = scene->mAnimations[i];
                        for (size_t j = 0; j < anim->mNumChannels; ++j)
                        {
                            auto nodeAnim = anim->mChannels[j];
                            boneNames.push_back(nodeAnim->mNodeName.C_Str());
                        }
                    }
                }

                return boneNames;
            }

            // assumes only one skeleton exists, so the first bone node we encounter by
            // doing a depth-first traversal should be the root of the skeleton
            aiNode* findSkeleton(aiNode *root, const vector<string>& boneNames)
            {
                // if current node is a bone, return it as the root of the skeleton
                if (find(boneNames.begin(), boneNames.end(), root->mName.C_Str()) != boneNames.end())
                    return root;

                // else, if no children, there is no skeleton
                if (!root->mNumChildren)
                    return nullptr;

                // if there are children, recursively search them for a bone
                for (size_t i = 0; i < root->mNumChildren; ++i)
                {
                    auto skeleton = findSkeleton(root->mChildren[i], boneNames);
                    if (skeleton != nullptr)
                        return skeleton;
                }

                // if no bone found in children, there is no skeleton
                return nullptr;
            }

            Bone buildSkeleton(aiNode *aiRoot, size_t& index, Bone *parent)
            {
                Bone bone = Bone(aiRoot->mName.C_Str());
                bone.index = index++;

                //bone.bindPose = Util::Mat4OfAiMat4(aiRoot->mTransformation);
                if (parent == nullptr)
                {
                    bone.bindPose = Util::Mat4OfAiMat4(aiRoot->mTransformation);
                    bone.inverseBindPose = inverse(bone.bindPose);
                }
                else
                {
                    bone.bindPose = parent->bindPose * Util::Mat4OfAiMat4(aiRoot->mTransformation);
                    bone.inverseBindPose = inverse(bone.bindPose);
                    //bone.inverseBindPose = inverse(parent->bindPose * bone.bindPose);
                    //bone.inverseBindPose = inverse(bone.bindPose) * inverse(parent->bindPose);
                }

                for (size_t i = 0; i < aiRoot->mNumChildren; ++i)
                    bone.children.push_back(buildSkeleton(aiRoot->mChildren[i], index, &bone));

                return bone;
            }

            void loadKeyFrames(const aiScene *scene, Bone& b)
            {
                for (size_t i = 0; i < scene->mNumAnimations; ++i)
                {
                    auto animation = scene->mAnimations[i];
                    for (size_t j = 0; j < animation->mNumChannels; ++j)
                    {
                        auto channel = animation->mChannels[j];
                        if (!strcmp(channel->mNodeName.C_Str(), b.name.c_str()))
                        {
                            BoneAnimation boneAnimation(animation->mName.C_Str());

                            for (size_t k = 0; k < channel->mNumPositionKeys; ++k)
                            {
                                auto positionKey = channel->mPositionKeys[k];
                                auto bonePosKey = make_pair(positionKey.mTime, vec4(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z, 1.0f));
                                boneAnimation.positionKeys.push_back(bonePosKey);
                            }

                            for (size_t k = 0; k < channel->mNumRotationKeys; ++k)
                            {
                                auto rotationKey = channel->mRotationKeys[k];
                                auto boneRotKey = make_pair(rotationKey.mTime, quat(rotationKey.mValue.w, rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z));
                                boneAnimation.rotationKeys.push_back(boneRotKey);
                            }

                            for (size_t k = 0; k < channel->mNumScalingKeys; ++k)
                            {
                                auto scalingKey = channel->mScalingKeys[k];
                                auto boneScaleKey = make_pair(scalingKey.mTime, vec4(scalingKey.mValue.x, scalingKey.mValue.y, scalingKey.mValue.z, 1.0f));
                                boneAnimation.scaleKeys.push_back(boneScaleKey);
                            }

                            b.animations.emplace(boneAnimation.name, boneAnimation);
                            break;
                        }
                    }
                }

                // recursive call on children
                for (auto it = b.children.begin(); it != b.children.end(); ++it)
                    loadKeyFrames(scene, *it);
            }

            void loadAnimations(const aiScene *scene, const string& skeletonName)
            {
                vector<ObjectAnimation> animations;
                for (size_t i = 0; i < scene->mNumAnimations; ++i)
                {
                    auto anim = scene->mAnimations[i];
                    animations.push_back(ObjectAnimation(anim->mName.C_Str(),
							 anim->mDuration,
							 anim->mTicksPerSecond));
                }
                animationMap.emplace(skeletonName, animations);
            }

            void addBonesToMapAndArray(const string& skeletonName, Bone b)
            {
                auto *boneMap = &skeletonBoneMap[skeletonName];
                auto *boneArray = &skeletonBoneArrays[skeletonName];
                b.index = boneArray->size();
                boneArray->push_back(b);
                boneMap->emplace(b.name, b);
                for (auto it = b.children.begin(); it != b.children.end(); ++it)
                {
                    addBonesToMapAndArray(skeletonName, *it);
                }
            }
        }

        // PUBLIC

        void Init()
        {
            // create and bind buffer
            glGenBuffers(1, &boneTransformBuffer);
            glBindBuffer(GL_TEXTURE_BUFFER, boneTransformBuffer);
            glBufferData(GL_TEXTURE_BUFFER, pow(2.0f, 16.0f), nullptr, GL_DYNAMIC_DRAW);

            // create and bind texture
            glGenTextures(1, &boneTransformTexture);
            glActiveTexture(GL_TEXTURE0 + TEXTURE_BONE_TRANSFORM);
            glBindTexture(GL_TEXTURE_BUFFER, boneTransformTexture);

            // associate texture with buffer
            glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, boneTransformBuffer);
        }

        void Cleanup()
        {
            aiDetachAllLogStreams();
        }

        int LoadSkeleton(const string& filepath, const string& name, const string& subdirectory)
        {
            if (skeletonMap.find(name) != skeletonMap.end())
            {
                std::cerr << "skeleton with name " << name << ". already exists\n";
                return -1;
            }

            const aiScene *scene = NULL;

            if ((scene = aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
            {
                auto boneNames = collectBoneNames(scene);
                aiNode *skeletonNode = findSkeleton(scene->mRootNode, boneNames);
                size_t index = 0;
                Bone skeleton = buildSkeleton(skeletonNode, index, nullptr);
                loadKeyFrames(scene, skeleton);
                loadAnimations(scene, name);

                addBonesToMapAndArray(name, skeleton);

                skeletonMap.emplace(name, skeleton);
            }
            else
            {
                std::cerr << aiGetErrorString() << std::endl;
                return -2;
            }

            aiReleaseImport(scene);
            return 0;
        }

        void PrintSkeleton(const Bone& skeleton)
        {
            cout << skeleton.name << endl;
            for (size_t i = 0; i < skeleton.children.size(); ++i)
                PrintSkeleton(skeleton.children[i]);
        }

        Bone GetSkeletonByName(const string& name)
        {
            return skeletonMap[name];
        }

        Bone GetBoneByName(const string& skeletonName, const string& boneName)
        {
            return skeletonBoneMap[skeletonName][boneName];
        }

        Bone GetBoneByIndex(const string& skeletonName, int i)
        {
            return skeletonBoneArrays[skeletonName][i];
        }

        /*void SetBoneBindPoseInverse(const string& skeletonName, const string& boneName, const mat4& bPose)
        {
            skeletonBoneMap[skeletonName][boneName].inverseBindPose = bPose;
        }*/

        vector<ObjectAnimation> GetAnimationsBySkeletonName(const string& skeletonName)
        {
            return animationMap[skeletonName];
        }

        size_t count_bones(const Bone& skel)
        {
            size_t children_count = 0;
            for (auto it = skel.children.begin(); it != skel.children.end(); ++it)
                children_count += count_bones(*it);
            return 1 + children_count;
        }

        void SendBoneTransformsToGPU(const vector<mat4>& boneTransforms)
        {
            glBindBuffer(GL_TEXTURE_BUFFER, boneTransformBuffer);
            glActiveTexture(GL_TEXTURE0 + TEXTURE_BONE_TRANSFORM);
            //glBindTexture(GL_TEXTURE_BUFFER, boneTransformTexture);
            glBufferSubData(GL_TEXTURE_BUFFER, 0, boneTransforms.size() * sizeof(mat4), &boneTransforms[0]);
            //glBufferData(GL_TEXTURE_BUFFER, boneTransforms.size() * sizeof(mat4), &boneTransforms[0], GL_DYNAMIC_DRAW);
        }
    }
}
