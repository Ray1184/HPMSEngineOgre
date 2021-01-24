/*!
 * File HPMSSceneManagerAdaptee.cpp
 */

#include <core/HPMSSceneManagerAdaptee.h>

hpms::SceneManagerAdaptee::SceneManagerAdaptee(Ogre::SceneManager* ogreSceneManager) : AdapteeCommon(ogreSceneManager)
{

}

hpms::SceneManagerAdaptee::~SceneManagerAdaptee()
{

}


hpms::EntityAdapter* hpms::SceneManagerAdaptee::CreateEntity(const std::string& path)
{
    return nullptr;
}

hpms::SceneNodeAdapter* hpms::SceneManagerAdaptee::GetRootSceneNode()
{
    return nullptr;
}

hpms::LightAdapter* hpms::SceneManagerAdaptee::CreateLight()
{
    return nullptr;
}

void hpms::SceneManagerAdaptee::SetAmbientLight(const glm::vec3& rgb)
{

}

