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
			computeBoneTransforms(skeleton);
			concatenateBindPoseInverses(skeleton);
		}
	}

	void AnimatedObject::SendTransformToGPU()
	{
		Skeleton::SendBoneTransformsToGPU(boneTransforms);
		DrawableObject::SendTransformToGPU();
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
		skeleton = skel;
		for (auto it = anims.begin(); it != anims.end(); ++it)
			animations.emplace(it->name, *it);
		// size of boneTransforms = number of bones
		boneTransforms = vector<mat4>(Skeleton::count_bones(skeleton));
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

	void AnimatedObject::updateBoneTransformsHelper(Bone& b)
	{
		// update transform and recursive call on each child
		for (auto it = b.children.begin(); it != b.children.end(); ++it)
		{
			boneTransforms[it->index] = boneTransforms[b.index] * it->bindPose;
			updateBoneTransformsHelper(*it);
		}
	}

	void AnimatedObject::computeBoneTransforms(Bone& b)
	{
		// compute b transform and write to buffer
		boneTransforms[b.index] = b.bindPose;

		/*vec4 pos = b.animations[""].positionKeys[0].second;
		mat4 t = translate(vec3(pos.x, pos.y, pos.z));

		quat rot = b.animations[""].rotationKeys[0].second;
		mat4 r = mat4_cast(rot);

		vec4 scal = b.animations[""].positionKeys[0].second;
		mat4 s = glm::scale(vec3(scal.x, scal.y, scal.z));

		boneTransforms[b.index] = t * r * s;*/

		updateBoneTransformsHelper(b);
	}
	
	void AnimatedObject::concatenateBindPoseInverses(Bone& b)
	{
		boneTransforms[b.index] = boneTransforms[b.index] * b.bindPoseInverse; /*inverse(b.bindPose);*/
		for (auto it = b.children.begin(); it != b.children.end(); ++it)
			concatenateBindPoseInverses(*it);
	}
}
