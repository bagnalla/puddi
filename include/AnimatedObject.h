#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

#include "DrawableObject.h"
#include "Skeleton.h"
#include <unordered_map>

namespace puddi
{
	struct ObjectAnimation
	{
		std::string name;
		float duration; // in ticks
		float ticksPerSecond;
		float trueDuration; // in seconds

		ObjectAnimation();
		ObjectAnimation(const std::string& n, float d, float tps);

		void SetTicksPerSecond(float tps);
	};

	class AnimatedObject : public DrawableObject
	{
	public:
		AnimatedObject(Object *par, SchematicNode *schematic, Bone skel, const std::vector<ObjectAnimation>& anims);

		void Update();
		void SendTransformToGPU();

		void EnableAnimation();
		void DisableAnimation();

		void SetActiveAnimation(const std::string& animationName);

		void SetAnimationTicksPerSecond(const std::string& animationName, float tps);

	private:
		bool animationActive;
		float animationTicks;
		ObjectAnimation activeAnimation;
		std::unordered_map<std::string, ObjectAnimation> animations;
		Bone skeleton;
		std::vector<glm::mat4> boneTransforms;

		void init(Bone& skel, const std::vector<ObjectAnimation>& anims);

		void computeBoneTransformsHelper(Bone& b);
		void computeBoneTransforms(Bone& b);
		void concatenateBindPoseInverses(Bone& b);
	};
}

#endif
