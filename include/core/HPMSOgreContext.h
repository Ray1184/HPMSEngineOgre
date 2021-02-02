/*!
 * File HPMSOgreContext.h
 */


#pragma once

#define DYNALO_EXPORT_SYMBOLS

#include <symbol_helper.hpp>
#include <Ogre.h>
#include <OgreFileSystemLayer.h>
#include <common/HPMSNamedWrapper.h>
#include <SDL2/SDL.h>
#include <input/HPMSInputHandler.h>

namespace hpms
{

    struct NativeWindowPair
    {
        Ogre::RenderWindow* render{nullptr};
        SDL_Window* native{nullptr};
    };

    struct OgreWindowSettings
    {
        unsigned int width{320};
        unsigned int height{200};
        std::string name{"Ogre Template"};
        bool fullScreen{false};
    };

class OgreContext
    {
    public:
        explicit OgreContext();

        virtual ~OgreContext();

        bool Setup(const OgreWindowSettings& settings);

        void InitRoot();

        bool CreateWindowPair(const OgreWindowSettings& settings);

        void CreateViewports();

        void SetupResources();

        void CreateResourceListener();

        void LoadResources();

        bool ManageClose() const;


    public:
        inline Ogre::FileSystemLayer* GetFsLayer() const
        {
            return fsLayer;
        }

        inline Ogre::Root* GetRoot() const
        {
            return root;
        }

        inline Ogre::LogManager* GetLogManager() const
        {
            return logManager;
        }

        inline Ogre::Camera* GetCamera() const
        {
            return camera;
        }

        inline Ogre::SceneManager* GetSceneManager() const
        {
            return sceneMgr;
        }

        inline Ogre::RenderWindow* GetRenderWindow() const
        {
            return windowPair.render;
        }

        inline void SetShutDown(bool shutDown)
        {
            OgreContext::shutDown = shutDown;
        }

    private:
        Ogre::FileSystemLayer* fsLayer;
        Ogre::LogManager* logManager;
        Ogre::Root* root;
        Ogre::String resourcesCfg;
        Ogre::String pluginsCfg;
        Ogre::Camera* camera;
        Ogre::SceneManager* sceneMgr;
        NativeWindowPair windowPair;
        bool shutDown;

    };
}
