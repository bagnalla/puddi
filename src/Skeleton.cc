#include "Skeleton.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <iostream>

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
            unordered_map<string, Bone*> skeletonMap;
			unordered_map<string, unordered_map<string, Bone*> > skeletonBoneMap;
            unordered_map<string, vector<Bone*> > skeletonBoneArrays;

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
                for (int i = 0; i < root->mNumChildren; ++i)
                {
                    auto skeleton = findSkeleton(root->mChildren[i], boneNames);
                    if (skeleton != nullptr)
                        return skeleton;
                }

				// if no bone found in children, there is no skeleton
                return nullptr;
            }

            Bone* buildSkeleton(aiNode *aiRoot, Bone *parent)
            {
				Bone *bone = new Bone(aiRoot->mName.C_Str(), parent);

				bone->bindPose = mat4(
					vec4(aiRoot->mTransformation.a1, aiRoot->mTransformation.a2, aiRoot->mTransformation.a3, aiRoot->mTransformation.a4),
					vec4(aiRoot->mTransformation.b1, aiRoot->mTransformation.b2, aiRoot->mTransformation.b3, aiRoot->mTransformation.b4),
					vec4(aiRoot->mTransformation.c1, aiRoot->mTransformation.c2, aiRoot->mTransformation.c3, aiRoot->mTransformation.c4),
					vec4(aiRoot->mTransformation.d1, aiRoot->mTransformation.d2, aiRoot->mTransformation.d3, aiRoot->mTransformation.d4)
					);

				for (int i = 0; i < aiRoot->mNumChildren; ++i)
					bone->children.push_back(buildSkeleton(aiRoot->mChildren[i], bone));

				return bone;
            }

            void addBonesToMapAndArray(const string& skeletonName, Bone *b)
            {
				auto boneMap = skeletonBoneMap[skeletonName];
				auto boneArray = skeletonBoneArrays[skeletonName];
                b->index = boneArray.size();
				boneArray.push_back(b);
				boneMap.emplace(b->name, b);
                for (auto it = b->children.begin(); it != b->children.end(); ++it)
                {
                    addBonesToMapAndArray(skeletonName, *it);
                }
            }
        }

        // PUBLIC

        void Init() {}

        void Cleanup()
        {
            aiDetachAllLogStreams();
        }

        int LoadSkeleton(const char *filepath, const string& name, const string& subdirectory)
        {
            if (skeletonMap.find(name) != skeletonMap.end())
            {
                std::cerr << "skeleton with name " << name << ". already exists\n";
                return -1;
            }

            const aiScene *scene = NULL;

            if ((scene = aiImportFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
            {
                auto boneNames = collectBoneNames(scene);
                aiNode *skeletonNode = findSkeleton(scene->mRootNode, boneNames);
                Bone *skeleton = buildSkeleton(skeletonNode, nullptr);

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

		void PrintSkeleton(Bone *skeleton)
		{
			cout << skeleton->name << endl;
			for (size_t i = 0; i < skeleton->children.size(); ++i)
				PrintSkeleton(skeleton->children[i]);
		}

        Bone* GetSkeletonByName(const string& name)
        {
            return skeletonMap[name];
        }

        Bone* GetBoneByName(const string& skeletonName, const string& boneName)
        {
            return skeletonBoneMap[skeletonName][boneName];
        }

        Bone* GetBoneByIndex(const string& skeletonName, int i)
        {
            return skeletonBoneArrays[skeletonName][i];
        }
    }
}
