/*!
 * File HPMSEntityAdaptee.cpp
 */

#include <core/HPMSEntityAdaptee.h>
#include <core/HPMSEntityHelper.h>
#include <core/HPMSAnimationAdaptee.h>

std::string hpms::EntityAdaptee::GetName()
{
    Check(ogreEntity);
    return ogreEntity->getName();
}

void hpms::EntityAdaptee::SetPosition(const glm::vec3& position)
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        ogreEntity->getParentSceneNode()->setPosition(position.x, position.y, position.z);
    }
}

glm::vec3 hpms::EntityAdaptee::GetPosition()
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        auto oVec = ogreEntity->getParentSceneNode()->getPosition();
        return glm::vec3(oVec.x, oVec.y, oVec.z);
    }
    return glm::vec3();
}

void hpms::EntityAdaptee::SetRotation(const glm::quat& rot)
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        ogreEntity->getParentSceneNode()->setOrientation(rot.w, rot.x, rot.y, rot.z);
    }
}

glm::quat hpms::EntityAdaptee::GetRotation()
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        auto oQuatc = ogreEntity->getParentSceneNode()->getOrientation();
        return glm::quat(oQuatc.w, oQuatc.x, oQuatc.y, oQuatc.z);
    }
    return glm::quat();
}

void hpms::EntityAdaptee::SetScale(const glm::vec3& scale)
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        ogreEntity->getParentSceneNode()->setScale(scale.x, scale.y, scale.z);
    }
}

glm::vec3 hpms::EntityAdaptee::GetScale()
{
    Check(ogreEntity);
    if (ogreEntity->getParentSceneNode() != nullptr)
    {
        auto oVec = ogreEntity->getParentSceneNode()->getScale();
        return glm::vec3(oVec.x, oVec.y, oVec.z);
    }
    return glm::vec3();
}

void hpms::EntityAdaptee::SetVisible(bool visible)
{
    Check(ogreEntity);
    ogreEntity->setVisible(visible);
}

bool hpms::EntityAdaptee::IsVisible()
{
    Check(ogreEntity);
    return ogreEntity->isVisible();
}

void hpms::EntityAdaptee::SetMode(hpms::EntityMode mode)
{
    if (hpms::EntityAdaptee::mode == mode)
    {
        return;
    }
    Check(ogreEntity);
    switch (mode)
    {
        case hpms::EntityMode::DEPTH_ONLY:
            hpms::EntityHelper::SetWriteDepthOnly(ogreEntity);
            break;
        case hpms::EntityMode::COLOR_ONLY:
            hpms::EntityHelper::SetWriteColorOnly(ogreEntity);
            break;
        default:
            hpms::EntityHelper::SetWriteDepthAndColor(ogreEntity);
            break;
    }
    hpms::EntityAdaptee::mode = mode;
}


std::vector<hpms::AnimationAdapter*>& hpms::EntityAdaptee::GetAllAnimations()
{
    return animList;
}

hpms::AnimationAdapter* hpms::EntityAdaptee::GetAnimationByName(const std::string& animName)
{
    return animMap[animName];
}

void hpms::EntityAdaptee::AttachObjectToBone(const std::string& boneName, hpms::ActorAdapter* object,
                                             const glm::vec3& offsetPosition, const glm::quat& offsetOrientation)
{
    Check(ogreEntity);
    Ogre::Vector3 posOff(offsetPosition.x, offsetPosition.y, offsetPosition.z);
    Ogre::Quaternion rotOff(offsetOrientation.w, offsetOrientation.x, offsetOrientation.y, offsetOrientation.z);

    if (auto* e = dynamic_cast<EntityAdaptee*>(object))
    {
        ogreEntity->attachObjectToBone(boneName, e->ogreEntity, rotOff, posOff);
    }
    // TODO Scene node management.
}

hpms::EntityAdaptee::EntityAdaptee(hpms::OgreContextAdaptee* ctx, const std::string& name) : AdapteeCommon(ctx),
                                                                                             mode(hpms::EntityMode::COLOR_AND_DEPTH)
{
    HPMS_ASSERT(ctx->GetSceneManager(), "Scene manager cannot be null.");
    ogreEntity = ctx->GetSceneManager()->createEntity(name);
    for (auto& it : ogreEntity->getAllAnimationStates()->getAnimationStates())
    {
        auto* animAdaptee = hpms::SafeNew<hpms::AnimationAdaptee>(it.second);
        animList.push_back(animAdaptee);
        animMap[it.first] = animAdaptee;
    }

}

hpms::EntityAdaptee::~EntityAdaptee()
{
    hpms::SafeDeleteRaw(ogreEntity);
    for (auto* animAdaptee : animList)
    {
        hpms::SafeDelete(animAdaptee);
    }
}

hpms::EntityAdapter* CreateEntity(hpms::ContextAdapter* ctx, const std::string& name) DYNALO_CALL {
    return hpms::SafeNew<hpms::EntityAdaptee>((hpms::OgreContextAdaptee*) ctx, name);
}

void DestroyEntity(hpms::EntityAdapter* entity)
{
    hpms::SafeDelete(entity);
}
