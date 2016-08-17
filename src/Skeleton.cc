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

            Bone* buildSkeleton(const aiScene *scene, aiNode *node, std::string subdirectory)
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
                Bone *skeleton = buildSkeleton(scene, scene->mRootNode, subdirectory);

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
