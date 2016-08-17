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
        std::string name;
        glm::mat4 bindPose;
        glm::mat4 bindPoseInverse;
        glm::mat4 animateTransform;
        Bone *parent;
        std::vector<Bone*> children;

        Bone(const std::string& n, const glm::mat4& bPose, Bone *par)
            : name(n), bindPose(bPose), parent(par)
        {
            bindPoseInverse = inverse(bindPose);
        }
    };

    namespace Skeleton
    {
        void Init();

        void Cleanup();

        int LoadSkeleton(const char *filepath, const std::string& name, const std::string& subdirectory = "");

        Bone* GetSkeletonByName(const std::string& name);
    }
}

#endif
