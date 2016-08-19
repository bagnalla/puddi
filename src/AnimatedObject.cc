#include "AnimatedObject.h"
#include "Skeleton.h"
#include "FpsTracker.h"

#include <iostream>

using namespace std;

namespace puddi
{
	// ObjectAnimation PUBLIC

	ObjectAnimation::ObjectAnimation()
	{
		name = "";
		duration = ticksPerSecond = trueDuration = 0.0f;
	}

	ObjectAnimation::ObjectAnimation(const string& n, float d, float tps)
	{
		name = n;
		duration = d;
		ticksPerSecond = tps;
		trueDuration = duration / ticksPerSecond;
	}

	// AnimatedObject PUBLIC

	AnimatedObject::AnimatedObject(Object *par, SchematicNode *schematic, Bone skel, const vector<ObjectAnimation>& anims)
		: DrawableObject(par, schematic)
	{
		init(skel, anims);
	}

	void AnimatedObject::Update()
	{
		if (animationActive)
		{
			animationTime += FpsTracker::GetFrameTimeMs() % static_cast<int>(animations[activeAnimation.name].trueDuration * 1000.0f);
			updateBoneTransforms(skeleton);
		}
	}

	void AnimatedObject::EnableAnimation()
	{
		animationActive = true;
	}

	void AnimatedObject::DisableAnimation()
	{
		animationActive = false;
	}

	void AnimatedObject::SetActiveAnimation(const std::string& animationName)
	{
		activeAnimation = animations[animationName];
	}

	void AnimatedObject::SetAnimationTicksPerSecond(const std::string& animationName, float tps)
	{
		animations[animationName].ticksPerSecond = tps;
	}

	// AnimatedObject PRIVATE

	void AnimatedObject::init(Bone& skel, const vector<ObjectAnimation>& anims)
	{
		animationActive = false;
		animationTime = 0.0f;
		/*skeleton = copySkeleton(skel, nullptr);*/
		for (auto it = anims.begin(); it != anims.end(); ++it)
			animations.emplace(it->name, *it);
	}

	/*Bone* AnimatedObject::copySkeleton(Bone *skel, Bone *parent)
	{
		Bone *copy = new Bone(*skel);
		copy->parent = parent;
		copy->children.clear();
		for (auto it = skel->children.begin(); it != skel->children.end(); ++it)
			copy->children.push_back(copySkeleton(*it, copy));
		return copy;
	}*/

	void updateBoneTransformsHelper(Bone& b)
	{
		// update transform and recursive call on each child
		for (auto it = b.children.begin(); it != b.children.end(); ++it)
		{
			// update child transform
			updateBoneTransformsHelper(*it);
		}
	}

	void AnimatedObject::updateBoneTransforms(Bone& b)
	{
		// update b transform and write to buffer

		updateBoneTransformsHelper(b);
	}
	
}