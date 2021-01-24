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
#include "HPMSOgreContextAdaptee.h"

namespace hpms
{
    class EntityAdaptee : public EntityAdapter, public AdapteeCommon
    {
    private:
        Ogre::Entity* ogreEntity;
        hpms::EntityMode mode;

    public:
        explicit EntityAdaptee(hpms::OgreContextAdaptee* ctx, const std::string& name);

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

    };
}


DYNALO_EXPORT hpms::EntityAdapter* DYNALO_CALL CreateEntity(hpms::ContextAdapter* ctx, const std::string& name);

DYNALO_EXPORT void DYNALO_CALL DestroyEntity(hpms::EntityAdapter* entity);