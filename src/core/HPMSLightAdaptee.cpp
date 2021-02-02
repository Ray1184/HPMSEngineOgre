/*!
 * File HPMSLightAdaptee.cpp
 */

#include <core/HPMSLightAdaptee.h>

std::string hpms::LightAdaptee::GetName()
{
    return std::string();
}

void hpms::LightAdaptee::SetPosition(const glm::vec3& position)
{

}

glm::vec3 hpms::LightAdaptee::GetPosition()
{
    return glm::vec3();
}

void hpms::LightAdaptee::SetRotation(const glm::quat& rotation)
{

}

glm::quat hpms::LightAdaptee::GetRotation()
{
    return glm::quat();
}

void hpms::LightAdaptee::SetScale(const glm::vec3& scale)
{

}

glm::vec3 hpms::LightAdaptee::GetScale()
{
    return glm::vec3();
}

void hpms::LightAdaptee::SetVisible(bool visible)
{

}

bool hpms::LightAdaptee::IsVisible()
{
    return false;
}

void hpms::LightAdaptee::SetColor(const glm::vec3& rgb)
{

}

hpms::LightAdaptee::LightAdaptee(hpms::OgreContext* ctx) : AdapteeCommon(ctx)
{
    Check();
    ogreLight = ((OgreContext*) ctx)->GetSceneManager()->createLight();
}

hpms::LightAdaptee::~LightAdaptee()
{
    Check();
    ((OgreContext*) ctx)->GetSceneManager()->destroyLight(ogreLight);
}
