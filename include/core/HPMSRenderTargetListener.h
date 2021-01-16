/*!
 * File HPMSRenderTargetListener.h
 */


#pragma once

#include <OgreRenderTargetListener.h>

namespace hpms
{
    class RenderTargetListener : public Ogre::RenderTargetListener
    {
    protected:
        Ogre::Rectangle2D* screen;
    public:
        inline explicit RenderTargetListener(Ogre::Rectangle2D* screen)
        {
            RenderTargetListener::screen = screen;
        }

        inline void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override
        {
            screen->setVisible(false);
        }

        inline void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override
        {
            screen->setVisible(true);
        }
    };
}