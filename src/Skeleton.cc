#include "Skeleton.h"
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <iostream>

using namespace std;

namespace puddi
{
    //  BONE PUBLIC

    void Bone::SetBindPose(const glm::mat4& bPose)
    {
        bindPose = bPose;
        bindPoseInverse = inverse(bPose);
    }

    namespace Skeleton
    {
        // PRIVATE
        namespace
        {
            unordered_map<string, Bone*> skeletonMap;
            unordered_map<string, Bone*> boneMap;
            vector<Bone*> bones;

            vector<string> collectBoneNames(const aiScene *scene)
            {
                vector<string> boneNames;

                if (scene->HasMeshes())
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
                }

                return boneNames;
            }

            aiNode* findSkeleton(aiNode *root, const vector<string>& boneNames)
            {
                if (find(boneNames.begin(), boneNames.end(), root->mName.C_Str()) != boneNames.end())
                    return root;

                if (!root->mNumChildren)
                    return nullptr;

                for (int i = 0; i < root->mNumChildren; ++i)
                {
                    auto skeleton = findSkeleton(root->mChildren[i], boneNames);
                    if (skeleton != nullptr)
                        return skeleton;
                }

                return nullptr;
            }

            Bone* buildSkeleton(aiNode *root)
            {

            }

            void addBonesToMapAndArray(Bone *skeleton)
            {
                skeleton->index = bones.size();
                bones.push_back(skeleton);
                boneMap.emplace(skeleton->name, skeleton);
                for (auto it = skeleton->children.begin(); it != skeleton->children.end(); ++it)
                {
                    addBonesToMapAndArray(*it);
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
                Bone *skeleton = buildSkeleton(skeletonNode);

                addBonesToMapAndArray(skeleton);

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

        Bone* GetSkeletonByName(const string& name)
        {
            return skeletonMap[name];
        }

        Bone* GetBoneByName(const string& name)
        {
            return boneMap[name];
        }

        Bone* GetBoneByIndex(int i)
        {
            return bones[i];
        }
    }
}
