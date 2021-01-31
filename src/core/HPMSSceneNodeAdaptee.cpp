/*!
 * File HPMSSceneNodeAdaptee.cpp
 */

#include <core/HPMSSceneNodeAdaptee.h>
#include <core/HPMSEntityAdaptee.h>

std::string hpms::SceneNodeAdaptee::GetName()
{
    Check(ogreNode);
    return ogreNode->getName();
}

void hpms::SceneNodeAdaptee::SetPosition(const glm::vec3& position)
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        ogreNode->getParentSceneNode()->setPosition(position.x, position.y, position.z);
    }
}

glm::vec3 hpms::SceneNodeAdaptee::GetPosition()
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        auto oVec = ogreNode->getParentSceneNode()->getPosition();
        return glm::vec3(oVec.x, oVec.y, oVec.z);
    }
    return glm::vec3();
}

void hpms::SceneNodeAdaptee::SetRotation(const glm::quat& rot)
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        ogreNode->getParentSceneNode()->setOrientation(rot.w, rot.x, rot.y, rot.z);
    }
}

glm::quat hpms::SceneNodeAdaptee::GetRotation()
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        auto oQuatc = ogreNode->getParentSceneNode()->getOrientation();
        return glm::quat(oQuatc.w, oQuatc.x, oQuatc.y, oQuatc.z);
    }
    return glm::quat();
}

void hpms::SceneNodeAdaptee::SetScale(const glm::vec3& scale)
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        ogreNode->getParentSceneNode()->setScale(scale.x, scale.y, scale.z);
    }
}

glm::vec3 hpms::SceneNodeAdaptee::GetScale()
{
    Check(ogreNode);
    if (ogreNode->getParentSceneNode() != nullptr)
    {
        auto oVec = ogreNode->getParentSceneNode()->getScale();
        return glm::vec3(oVec.x, oVec.y, oVec.z);
    }
    return glm::vec3();
}

void hpms::SceneNodeAdaptee::SetVisible(bool visible)
{
    // Nothing to do.
}

bool hpms::SceneNodeAdaptee::IsVisible()
{
    return true;
}


hpms::SceneNodeAdapter* hpms::SceneNodeAdaptee::CreateChild(const std::string& name)
{
    Check(ogreNode);
    auto* rawChildNode = ogreNode->createChildSceneNode(name);
    auto* childNodeAdaptee = hpms::SafeNew<hpms::SceneNodeAdaptee>(rawChildNode, name, this);
    return childNodeAdaptee;
}

void hpms::SceneNodeAdaptee::AttachObject(hpms::ActorAdapter* actor)
{
    Check(ogreNode);
    if (auto* e = dynamic_cast<EntityAdaptee*>(actor))
    {
        ogreNode->attachObject(e->GetOgreEntity());
    }
}


hpms::SceneNodeAdapter* hpms::SceneNodeAdaptee::RemoveChild(const std::string& name)
{
    Check(ogreNode);
    ogreNode->removeChild(name);
}


hpms::SceneNodeAdapter* hpms::SceneNodeAdaptee::GetParent()
{
    return parent;
}


hpms::SceneNodeAdaptee::SceneNodeAdaptee(hpms::OgreContextAdaptee* ctx, const std::string& name) : AdapteeCommon(ctx), parent(nullptr)
{
    HPMS_ASSERT(ctx->GetSceneManager(), "Scene manager cannot be null.");
    ogreNode = ctx->GetSceneManager()->getRootSceneNode()->createChildSceneNode(name);
}

hpms::SceneNodeAdaptee::SceneNodeAdaptee(Ogre::SceneNode* ogreSceneNode, const std::string& name, SceneNodeAdapter* parent)
        : AdapteeCommon(nullptr),
        ogreNode(ogreSceneNode),
        parent(parent)
{
}

hpms::SceneNodeAdaptee::~SceneNodeAdaptee()
{
    hpms::SafeDeleteRaw(ogreNode);
}
