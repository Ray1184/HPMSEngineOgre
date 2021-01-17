/*!
 * File HPMSRenderToTexture.cpp
 */

#include <core/HPMSRenderToTexture.h>



hpms::RenderToTexture* hpms::RenderToTexture::GetOrCreate(hpms::OgreContext* ctx, unsigned int fbWidth, unsigned int fbHeight)
{
    auto* instance = hpms::SafeNew<RenderToTexture>(ctx, fbWidth, fbHeight);
    return instance;
}

hpms::RenderToTexture::RenderToTexture(hpms::OgreContext* ctx, unsigned int fbWidth, unsigned int fbHeight) : ctx(ctx),
                                                                                                  fbWidth(fbWidth),
                                                                                                  fbHeight(fbHeight),
                                                                                                  initialized(false),
                                                                                                  renderScreen(nullptr),
                                                                                                  fbNode(nullptr)
{

}

void hpms::RenderToTexture::Initialize()
{
    if (initialized) {
        return;
    }

    auto texture = Ogre::TextureManager::getSingleton().createManual(RTT_TEXTURE_NAME,
                                                                     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                     Ogre::TEX_TYPE_2D, this->fbWidth, this->fbHeight, 0,
                                                                     Ogre::PF_R8G8B8,
                                                                     (int) Ogre::TU_RENDERTARGET);
    auto* renderTexture = texture->getBuffer()->getRenderTarget();

    renderTexture->addViewport(ctx->GetCamera());
    renderTexture->getViewport(0)->setClearEveryFrame(true);
    renderTexture->getViewport(0)->setOverlaysEnabled(true);


    auto* iRenderScreen = hpms::SafeNewRaw<Ogre::Rectangle2D>(true);
    this->renderScreen = hpms::SafeNew<hpms::NamedWrapper<Ogre::Rectangle2D>>(iRenderScreen, "Ogre::Rectangle2D");
    this->renderScreen->GetWrappedObject()->setCorners(-1, 1.0, 1.0, -1);
    this->renderScreen->GetWrappedObject()->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

    this->fbNode = ctx->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
    //this->fbNode->attachObject(this->renderScreen->GetWrappedObject());

    Ogre::MaterialPtr renderMaterial =
            Ogre::MaterialManager::getSingleton().create(
                    RTT_MATERIAL_NAME,
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TextureUnitState* textState = renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(RTT_TEXTURE_NAME);
    textState->setTextureFiltering(Ogre::TFO_NONE);

    this->renderScreen->GetWrappedObject()->setMaterial(renderMaterial);
    initialized = true;
}


hpms::RenderToTexture::~RenderToTexture()
{
    if (initialized)
    {
        hpms::SafeDelete(renderScreen);
    }
}

void hpms::RenderToTexture::Activate()
{
    Initialize();
    if (!renderScreen->GetWrappedObject()->isAttached())
    {
        fbNode->attachObject(renderScreen->GetWrappedObject());
    }
}

void hpms::RenderToTexture::Deactivate()
{
    fbNode->detachObject(renderScreen->GetWrappedObject());
}


