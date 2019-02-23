#include <functional>
#include "AnimatedObject.h"
#include "FpsTracker.h"

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
        SetTicksPerSecond(tps);
    }

    void ObjectAnimation::SetTicksPerSecond(float tps)
    {
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
            animationTicks = fmodf(animationTicks + (FpsTracker::GetFrameTimeMs() / 1000.0f) * animations[activeAnimation.name].ticksPerSecond, animations[activeAnimation.name].duration);
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
        resetBoneTransforms(skeleton);
    }

    void AnimatedObject::SetActiveAnimation(const std::string& animationName)
    {
        activeAnimation = animations[animationName];
    }

    void AnimatedObject::SetAnimationTicksPerSecond(const std::string& animationName, float tps)
    {
        animations[animationName].SetTicksPerSecond(tps);
    }

    void AnimatedObject::SetAnimationProgress(float progress)
    {
        animationTicks = animations[activeAnimation.name].duration * progress;
    }

    // AnimatedObject PRIVATE

    void AnimatedObject::init(Bone& skel, const vector<ObjectAnimation>& anims)
    {
        animationActive = false;
        animationTicks = 0;
        skeleton = skel;
        for (auto it = anims.begin(); it != anims.end(); ++it)
            animations.emplace(it->name, *it);
        // size of boneTransforms = number of bones
        boneTransforms = vector<mat4>(Skeleton::count_bones(skeleton));
    }

    template<typename T>
    int findNearestKeyIndex(vector<pair<float, T> > keys, float ticks)
    {
        int nearestKeyIndex = -1;
        float smallestDistance = FLT_MAX;
        for (size_t i = 0; i < keys.size(); ++i)
        {
            float dist = abs(keys[i].first - ticks);
            if (dist < smallestDistance)
            {
                nearestKeyIndex = i;
                smallestDistance = dist;
            }
        }
        return nearestKeyIndex;
    }

    vec4 computeInterpolatedVec4Key(vector<pair<float, vec4> > keys, float ticks)
    {
        vec4 result;

        int nearestKeyIndex = findNearestKeyIndex(keys, ticks);

        auto key = keys[nearestKeyIndex];

        if (ticks <= key.first)
        {
            if (nearestKeyIndex == 0)
                result = key.second;
            else
            {
                auto predKey = keys[nearestKeyIndex - 1];
                if (predKey.first == key.first)
                    result = predKey.second;
                else
                {
                    float a = (ticks - predKey.first) / (key.first - predKey.first);
                    result = mix(predKey.second, key.second, a);
                }
            }
        }
        else
        {
            if (nearestKeyIndex == keys.size() - 1)
                result = key.second;
            else
            {
                auto succKey = keys[nearestKeyIndex + 1];
                if (succKey.first == key.first)
                    result = succKey.second;
                else
                {
                    float a = (ticks - key.first) / (succKey.first - key.first);
                    result = mix(key.second, succKey.second, a);
                }
            }
        }

        return result;
    }

    quat computeInterpolatedQuatKey(vector<pair<float, quat> > keys, float ticks)
    {
        quat result;

        int nearestKeyIndex = findNearestKeyIndex(keys, ticks);

        auto key = keys[nearestKeyIndex];

        if (ticks <= key.first)
        {
            if (nearestKeyIndex == 0)
                result = key.second;
            else
            {
                auto predKey = keys[nearestKeyIndex - 1];
                if (predKey.first == key.first)
                    result = predKey.second;
                else
                {
                    float a = (ticks - predKey.first) / (key.first - predKey.first);
                    result = lerp(predKey.second, key.second, a);
                }
            }
        }
        else
        {
            if (nearestKeyIndex == keys.size() - 1)
                result = key.second;
            else
            {
                auto succKey = keys[nearestKeyIndex + 1];
                if (succKey.first == key.first)
                    result = succKey.second;
                else
                {
                    float a = (ticks - key.first) / (succKey.first - key.first);
                    result = lerp(key.second, succKey.second, a);
                }
            }
        }

        return result;
    }

    void AnimatedObject::computeBoneTransformsHelper(Bone& b)
    {
        // update transform and recursive call on each child
        for (auto it = b.children.begin(); it != b.children.end(); ++it)
        {
            auto animation = it->animations[activeAnimation.name];

            auto pos = computeInterpolatedVec4Key(animation.positionKeys, animationTicks);
            auto rot = computeInterpolatedQuatKey(animation.rotationKeys, animationTicks);
            auto scal = computeInterpolatedVec4Key(animation.scaleKeys, animationTicks);

            mat4 t = translate(vec3(pos.x, pos.y, pos.z));
            mat4 r = mat4_cast(rot);
            mat4 s = glm::scale(vec3(scal.x, scal.y, scal.z));

            boneTransforms[it->index] = boneTransforms[b.index] * (t * r * s);

            computeBoneTransformsHelper(*it);
        }
    }

    void AnimatedObject::computeBoneTransforms(Bone& b)
    {
        // compute b transform

        auto animation = b.animations[activeAnimation.name];

        auto pos = computeInterpolatedVec4Key(animation.positionKeys, animationTicks);
        auto rot = computeInterpolatedQuatKey(animation.rotationKeys, animationTicks);
        auto scal = computeInterpolatedVec4Key(animation.scaleKeys, animationTicks);

        mat4 t = translate(vec3(pos.x, pos.y, pos.z));
        mat4 r = mat4_cast(rot);
        mat4 s = glm::scale(vec3(scal.x, scal.y, scal.z));

        boneTransforms[b.index] = t * r * s;

        // recursively compute children bone transforms
        computeBoneTransformsHelper(b);
    }

    void AnimatedObject::concatenateBindPoseInverses(Bone& b)
    {
        boneTransforms[b.index] = boneTransforms[b.index] * b.inverseBindPose; /** inverse(b.bindPose);*/
        for (auto it = b.children.begin(); it != b.children.end(); ++it)
            concatenateBindPoseInverses(*it);
    }

    void AnimatedObject::resetBoneTransforms(Bone& b)
    {
        boneTransforms[b.index] = b.bindPose;
        for (auto it = b.children.begin(); it != b.children.end(); ++it)
            resetBoneTransforms(*it);
    }
}
