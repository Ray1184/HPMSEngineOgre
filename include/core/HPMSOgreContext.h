/*!
 * File HPMSOgreContext.h
 */


#pragma once

#include <Ogre.h>
#include <OgreFileSystemLayer.h>
#include <common/HPMSNamedWrapper.h>
#include <SDL2/SDL.h>
#include <input/HPMSInputHandler.h>

namespace hpms
{
    struct WindowSettings
    {
        unsigned int width{320};
        unsigned int height{200};
        std::string name{"HPMS Template"};
    };
    struct NativeWindowPair
    {
        Ogre::RenderWindow* render{nullptr};
        SDL_Window* native{nullptr};
    };

    class OgreContext : public Ogre::FrameListener
    {
    public:
        OgreContext(const WindowSettings& settings = {});

        virtual ~OgreContext();

        virtual void Run();

    protected:
        virtual bool Setup();

        virtual void InitRoot();

        virtual bool CreateWindowPair();

        virtual void CreateFrameListener();

        virtual void CreateViewports();

        virtual void SetupResources();

        virtual void CreateResourceListener();

        virtual void LoadResources();

        virtual void CustomCreateScene() = 0;

        virtual void CustomDestroyScene() = 0;

        virtual void CustomInput(const std::vector<hpms::KeyEvent>& keyEvents,
                                 const std::vector<hpms::MouseEvent>& mouseBussonEvents, unsigned int x,
                                 unsigned int y) = 0;

        virtual void CustomUpdate() = 0;

        bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;


    public:
        inline Ogre::FileSystemLayer* GetFsLayer() const
        {
            return fsLayer->GetWrappedObject();
        }

        inline Ogre::Root* GetRoot() const
        {
            return root->GetWrappedObject();
        }

        inline Ogre::LogManager* GetLogManager() const
        {
            return logManager->GetWrappedObject();
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

        inline const WindowSettings& GetWindowSettings() const
        {
            return windowSettings;
        }

        inline void SetShutDown(bool shutDown)
        {
            OgreContext::shutDown = shutDown;
        }

    private:
        NamedWrapper<Ogre::FileSystemLayer>* fsLayer;
        NamedWrapper<Ogre::LogManager>* logManager;
        NamedWrapper<Ogre::Root>* root;
        Ogre::String resourcesCfg;
        Ogre::String pluginsCfg;
        Ogre::Camera* camera;
        Ogre::SceneManager* sceneMgr;
        NativeWindowPair windowPair;
        WindowSettings windowSettings;
        char polygonRenderingMode;
        bool shutDown;
        hpms::InputHandler inputHandler;
        std::vector<KeyEvent> keyStates{};
        std::vector<MouseEvent> mouseButtonStates{};
        unsigned int x;
        unsigned int y;

    };
}