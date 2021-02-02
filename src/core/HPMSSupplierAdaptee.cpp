/*!
 * File HPMSSupplierAdaptee.cpp
 */

#include <core/HPMSSupplierAdaptee.h>


hpms::EntityAdapter* hpms::SupplierAdaptee::CreateEntity(const std::string& path)
{
    Check();
    auto* entity = hpms::SafeNew<hpms::EntityAdaptee>((OgreContext*) ctx, path);
    entities.push_back(entity);
    return entity;
}

hpms::SceneNodeAdapter* hpms::SupplierAdaptee::GetRootSceneNode()
{
    Check();
    return rootNode;
}

hpms::CameraAdapter* hpms::SupplierAdaptee::GetCamera()
{
    Check();
    return camera;
}

hpms::LightAdapter* hpms::SupplierAdaptee::CreateLight(float r, float g, float b)
{
    Check();
    return hpms::SafeNew<hpms::LightAdaptee>((OgreContext*) ctx);
}

hpms::BackgroundImageAdapter*
hpms::SupplierAdaptee::CreateBackgroundImage(const std::string& path, unsigned int width, unsigned int height)
{
    return nullptr;
}

hpms::OverlayImageAdapter*
hpms::SupplierAdaptee::CreateOverlayImage(const std::string& path, unsigned int width, unsigned int height,
                                          unsigned int x, unsigned int y, int zOrder)
{
    return nullptr;
}

void hpms::SupplierAdaptee::SetAmbientLight(const glm::vec3& rgb)
{

}

void hpms::SupplierAdaptee::FreeItems()
{

}

hpms::SupplierAdaptee::SupplierAdaptee(hpms::OgreContext* ctx) : AdapteeCommon(ctx)
{
    Check();
    auto* ogreRootNode = ((OgreContext*) ctx)->GetSceneManager()->getRootSceneNode();
    rootNode = hpms::SafeNew<hpms::SceneNodeAdaptee>(ogreRootNode, ogreRootNode->getName(), nullptr);
    auto* ogreCamera = ((OgreContext*) ctx)->GetCamera();
    camera = hpms::SafeNew<hpms::CameraAdaptee>((OgreContext*) ctx, ogreCamera->getName());
}

hpms::SupplierAdaptee::~SupplierAdaptee()
{
    FreeItems();
}

std::string hpms::SupplierAdaptee::GetImplName()
{
#ifdef BACKEND_IMPL_NAME
    return BACKEND_IMPL_NAME;
#else
    return "Ogre 3D";
#endif
}


