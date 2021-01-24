/*!
 * File HPMSOgreContextAdaptee.h
 */


#pragma once

#define DYNALO_EXPORT_SYMBOLS

#include <symbol_helper.hpp>
#include <Ogre.h>
#include <OgreFileSystemLayer.h>
#include <common/HPMSNamedWrapper.h>
#include <SDL2/SDL.h>
#include <input/HPMSInputHandler.h>
#include <api/HPMSContextAdapter.h>

namespace hpms
{

    struct NativeWindowPair
    {
        Ogre::RenderWindow* render{nullptr};
        SDL_Window* native{nullptr};
    };

class OgreContextAdaptee : public Ogre::FrameListener, public hpms::ContextAdapter
    {
    public:
        OgreContextAdaptee(hpms::CustomLogic* logic, const hpms::WindowSettings& settings = {});

        virtual ~OgreContextAdaptee();

        virtual void Run() override;



    protected:
        virtual bool Setup();

        virtual void InitRoot();

        virtual bool CreateWindowPair();

        virtual void CreateFrameListener();

        virtual void CreateViewports();

        virtual void SetupResources();

        virtual void CreateResourceListener();

        virtual void LoadResources();

        virtual void CustomCreateScene();

        virtual void CustomDestroyScene();

        virtual void CustomInput(const std::vector<hpms::KeyEvent>& keyEvents,
                                 const std::vector<hpms::MouseEvent>& mouseBussonEvents, unsigned int x,
                                 unsigned int y);

        virtual void CustomUpdate();

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
            OgreContextAdaptee::shutDown = shutDown;
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
        bool shutDown;
        hpms::InputHandler inputHandler;
        std::vector<KeyEvent> keyStates{};
        std::vector<MouseEvent> mouseButtonStates{};
        unsigned int x;
        unsigned int y;

    };
}

DYNALO_EXPORT hpms::ContextAdapter* DYNALO_CALL CreateContext(hpms::CustomLogic* logic, const hpms::WindowSettings& settings = {});

DYNALO_EXPORT void DYNALO_CALL DestroyContext(hpms::ContextAdapter* context);