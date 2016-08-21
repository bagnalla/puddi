#ifndef SKELETON_H
#define SKELETON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

namespace puddi
{
	struct ObjectAnimation;

	struct BoneAnimation
	{
		std::string name;
		std::vector<std::pair<float, glm::vec4> > positionKeys;
		std::vector<std::pair<float, glm::quat> > rotationKeys;
		std::vector<std::pair<float, glm::vec4> > scaleKeys;

		BoneAnimation() {}
		BoneAnimation(std::string n) : name(n) {}
	};

    struct Bone
    {
        int index;
        std::string name;
        glm::mat4 bindPose;
        glm::mat4 inverseBindPose;
        std::vector<Bone> children;
		std::unordered_map<std::string, BoneAnimation> animations;

		Bone() {}
        Bone(const std::string& n) : name(n) {}

       /* void SetBindPose(const glm::mat4& bPose) */
    };

    namespace Skeleton
    {
        void Init();

        void Cleanup();

        int LoadSkeleton(const std::string& filepath, const std::string& name, const std::string& subdirectory = "");

		void PrintSkeleton(const Bone& skeleton);

        Bone GetSkeletonByName(const std::string& name);

        Bone GetBoneByName(const std::string& skeletonName, const std::string& boneName);

        Bone GetBoneByIndex(const std::string& skeletonName, int i);

		/*void SetBoneBindPoseInverse(const std::string& skeletonName, const std::string& boneName, const glm::mat4& bPose);*/

		std::vector<ObjectAnimation> GetAnimationsBySkeletonName(const std::string& skeletonName);

		size_t count_bones(const Bone& skel);

		void SendBoneTransformsToGPU(const std::vector<glm::mat4>& boneTransforms);
    }
}

#endif
