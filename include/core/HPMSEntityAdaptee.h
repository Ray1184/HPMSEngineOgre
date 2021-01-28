/*!
 * File HPMSEntityAdaptee.h
 */


#pragma once

#include <api/HPMSEntityAdapter.h>
#include <core/HPMSAdapteeCommon.h>
#include <common/HPMSUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Ogre.h>
#include <core/HPMSOgreContextAdaptee.h>
#include <vector>
#include <map>

namespace hpms
{
    class EntityAdaptee : public EntityAdapter, public AdapteeCommon
    {
    private:
        Ogre::Entity* ogreEntity;
        hpms::EntityMode mode;
        std::map<std::string, hpms::AnimationAdapter*> animMap;
        std::vector<hpms::AnimationAdapter*> animList;

    public:
        EntityAdaptee(hpms::OgreContextAdaptee* ctx, const std::string& name);

        virtual ~EntityAdaptee();

        std::string GetName() override;

        void SetPosition(const glm::vec3& position) override;

        glm::vec3 GetPosition() override;

        void SetRotation(const glm::quat& rotation) override;

        glm::quat GetRotation() override;

        void SetScale(const glm::vec3& scale) override;

        glm::vec3 GetScale() override;

        void SetVisible(bool visible) override;

        bool IsVisible() override;

        void SetMode(EntityMode mode) override;

        virtual std::vector<hpms::AnimationAdapter*>& GetAllAnimations() override;

        virtual AnimationAdapter* GetAnimationByName(const std::string& animName) override;

        virtual void
        AttachObjectToBone(const std::string& boneName, hpms::ActorAdapter* object, const glm::vec3& offsetPosition,
                           const glm::quat& offsetOrientation) override;

    };
}


DYNALO_EXPORT hpms::EntityAdapter* DYNALO_CALL CreateEntity(hpms::ContextAdapter* ctx, const std::string& name);

DYNALO_EXPORT void DYNALO_CALL DestroyEntity(hpms::EntityAdapter* entity);