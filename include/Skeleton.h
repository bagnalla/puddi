#ifndef SKELETON_H
#define SKELETON_H

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

namespace puddi
{
    struct Bone
    {
        int index;
        std::string name;
        glm::mat4 bindPose;
        glm::mat4 bindPoseInverse;
        glm::mat4 animateTransform;
        Bone *parent;
        std::vector<Bone*> children;

        Bone(const std::string& n, Bone *par) : name(n), parent(par) {}

       /* void SetBindPose(const glm::mat4& bPose) */
    };

    namespace Skeleton
    {
        void Init();

        void Cleanup();

        int LoadSkeleton(const char *filepath, const std::string& name, const std::string& subdirectory = "");

		void PrintSkeleton(Bone *skeleton);

        Bone* GetSkeletonByName(const std::string& name);

        Bone* GetBoneByName(const std::string& skeletonName, const std::string& boneName);

        Bone* GetBoneByIndex(const std::string& skeletonName, int i);
    }
}

#endif
