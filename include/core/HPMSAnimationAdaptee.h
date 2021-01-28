/*!
 * File HPMSAnimationAdaptee.h
 */

#pragma once

#include <api/HPMSAnimationAdapter.h>
#include <Ogre.h>

namespace hpms
{
    class AnimationAdaptee : public hpms::AnimationAdapter
    {
    private:
        Ogre::AnimationState* ogreAnim;
    public:
        explicit AnimationAdaptee(Ogre::AnimationState* ogreAnim);

        virtual ~AnimationAdaptee();

        virtual void Update(float tpf) override;

        virtual bool IsPlaying() override;

        virtual void SetPlaying(bool playing) override;

        virtual bool IsLoop() override;

        virtual void SetLoop(bool loop) override;
    };
}
