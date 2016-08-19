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
	};

	class AnimatedObject : public DrawableObject
	{
	public:
		AnimatedObject(Object *par, SchematicNode *schematic, Bone skel, const std::vector<ObjectAnimation>& anims);

		void Update();

		void EnableAnimation();
		void DisableAnimation();

		void SetActiveAnimation(const std::string& animationName);

		void SetAnimationTicksPerSecond(const std::string& animationName, float tps);

	private:
		bool animationActive;
		unsigned int animationTime;
		ObjectAnimation activeAnimation;
		std::unordered_map<std::string, ObjectAnimation> animations;
		Bone skeleton;

		void init(Bone& skel, const std::vector<ObjectAnimation>& anims);

		/*Bone* copySkeleton(Bone skel, Bone *parent);*/

		void updateBoneTransforms(Bone& b);
	};
}

#endif
