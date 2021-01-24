/*!
 * File HPMSSceneManagerAdaptee.h
 */


#pragma once

#include <api/HPMSSceneManagerAdapter.h>
#include <core/HPMSAdapteeCommon.h>
#include <common/HPMSUtils.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Ogre.h>

namespace hpms
{
    class SceneManagerAdaptee : public SceneManagerAdapter, public AdapteeCommon<Ogre::SceneManager>
    {

    public:
        explicit SceneManagerAdaptee(Ogre::SceneManager* ogreSceneManager);

        virtual ~SceneManagerAdaptee();

        EntityAdapter* CreateEntity(const std::string& path) override;

        SceneNodeAdapter* GetRootSceneNode() override;

        LightAdapter* CreateLight() override;

        void SetAmbientLight(const glm::vec3& rgb) override;

    };
}