#include "Skeleton.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <iostream>

using namespace std;

namespace puddi
{
    namespace Skeleton
    {
        // PRIVATE
        namespace
        {
            std::unordered_map<string, Bone*> skeletonMap;

            Bone* buildSkeleton(const aiScene *scene, aiNode *node, std::string subdirectory)
            {
            }
        }

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
    }
}
