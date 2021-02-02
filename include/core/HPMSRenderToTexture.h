/*!
 * File HPMSRenderToTexture.h
 */


#pragma once

#include <OgreRectangle2D.h>
#include <core/HPMSOgreContext.h>

#define RTT_MATERIAL_NAME "RTTMaterial"
#define RTT_TEXTURE_NAME "RTTTex"

namespace hpms
{
    class RenderToTexture : public hpms::Object
    {
    private:
        unsigned int fbWidth;
        unsigned int fbHeight;
        hpms::NamedWrapper<Ogre::Rectangle2D>* renderScreen;
        hpms::OgreContext* ctx;
        Ogre::SceneNode* fbNode;
        bool initialized;

        void Initialize();

    public:
        RenderToTexture(hpms::OgreContext* ctx, unsigned int fbWidth, unsigned int fbHeight);

        virtual ~RenderToTexture();

        void Activate();

        void Deactivate();

        inline size_t GetFbWidth() const
        {
            return fbWidth;
        }

        inline size_t GetFbHeight() const
        {
            return fbHeight;
        }

        inline Ogre::Rectangle2D* GetRenderScreen() const
        {
            return renderScreen->GetWrappedObject();
        }

        inline const std::string Name() const override
        {
            return "RenderToTexture";
        }

        static RenderToTexture* GetOrCreate(hpms::OgreContext* ctx, unsigned int fbWidth, unsigned int fbHeight);

    };
}