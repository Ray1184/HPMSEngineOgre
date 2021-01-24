/*!
 * File HPMSOgreContextAdaptee.cpp
 */

#include <core/HPMSOgreContextAdaptee.h>
#include <SDL2/SDL_syswm.h>

hpms::OgreContextAdaptee::OgreContextAdaptee(hpms::CustomLogic* logic, const hpms::WindowSettings& settings) : root(nullptr),
                                                                                                 camera(nullptr),
                                                                                                 sceneMgr(nullptr),
                                                                                                 resourcesCfg(HPMS_OGRE_CONFIG_FOLDER "Resources.ini"),
                                                                                                 pluginsCfg(HPMS_OGRE_CONFIG_FOLDER "Plugins.ini"),
                                                                                                 shutDown(false),
                                                                                                 windowSettings(windowSettings),
                                                                                                 logic(logic)
{
    auto* iLogManager = hpms::SafeNewRaw<Ogre::LogManager>();
    logManager = hpms::SafeNew<NamedWrapper<Ogre::LogManager>>(iLogManager, "Ogre::LogManager");
    logManager->GetWrappedObject()->createLog(HPMS_OGRE_CONFIG_FOLDER "Ogre.log", true, false, false);

    auto* iFsLayer = hpms::SafeNewRaw<Ogre::FileSystemLayer>("FSData");
    fsLayer = hpms::SafeNew<NamedWrapper<Ogre::FileSystemLayer>>(iFsLayer, "Ogre::FileSystemLayer");

    auto* iRoot = hpms::SafeNewRaw<Ogre::Root>(pluginsCfg,
                                               fsLayer->GetWrappedObject()->getWritablePath(
                                                       HPMS_OGRE_CONFIG_FOLDER "Ogre.ini"),
                                               fsLayer->GetWrappedObject()->getWritablePath(
                                                       HPMS_OGRE_CONFIG_FOLDER "Ogre.log"));
    root = hpms::SafeNew<NamedWrapper<Ogre::Root>>(iRoot, "Ogre::Root");

}

hpms::OgreContextAdaptee::~OgreContextAdaptee()
{
    root->GetWrappedObject()->removeFrameListener(this);
    hpms::SafeDelete(root);
    hpms::SafeDelete(fsLayer);
    hpms::SafeDelete(logManager);
}

void hpms::OgreContextAdaptee::Run()
{
    if (!Setup())
    {
        LOG_ERROR("Error setting up OGRE subsystems.");
        return;
    }
    LOG_DEBUG("OGRE subsystems initialization completed, starting rendering.");
    root->GetWrappedObject()->startRendering();

    CustomDestroyScene();
}

bool hpms::OgreContextAdaptee::CreateWindowPair()
{
    try
    {
        auto miscParams = Ogre::NameValuePairList();

        if (!SDL_WasInit(SDL_INIT_VIDEO))
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
        }

        auto p = root->GetWrappedObject()->getRenderSystem()->getRenderWindowDescription();
        miscParams.insert(p.miscParams.begin(), p.miscParams.end());
        p.miscParams = miscParams;
        p.name = windowSettings.name;


        if (windowSettings.width > 0 && windowSettings.height > 0)
        {
            p.width = windowSettings.width;
            p.height = windowSettings.height;
        }

        int flags = p.useFullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
        int d = Ogre::StringConverter::parseInt(miscParams["monitorIndex"], 1) - 1;
        windowPair.native = SDL_CreateWindow(p.name.c_str(), SDL_WINDOWPOS_UNDEFINED_DISPLAY(d),
                                             SDL_WINDOWPOS_UNDEFINED_DISPLAY(d), p.width, p.height, flags);

#if OGRE_PLATFORM != OGRE_PLATFORM_EMSCRIPTEN
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(windowPair.native, &wmInfo);
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        p.miscParams["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.x11.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        p.miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        assert(wmInfo.subsystem == SDL_SYSWM_COCOA);
        p.miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.cocoa.window));
#endif

        windowPair.render = root->GetWrappedObject()->createRenderWindow(p);
    } catch (std::exception& e) {
        return false;
    }
    return true;
}

void hpms::OgreContextAdaptee::CreateFrameListener()
{
    root->GetWrappedObject()->addFrameListener(this);
}

void hpms::OgreContextAdaptee::CreateViewports()
{
    sceneMgr = root->GetWrappedObject()->createSceneManager();
    camera = sceneMgr->createCamera(DEFAULT_CAMERA_NAME);
    Ogre::Viewport* vp = windowPair.render->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void hpms::OgreContextAdaptee::InitRoot()
{
    root->GetWrappedObject()->setRenderSystem(
            root->GetWrappedObject()->getRenderSystemByName("OpenGL Rendering Subsystem"));
    root->GetWrappedObject()->initialise(false);
}

void hpms::OgreContextAdaptee::SetupResources()
{
    Ogre::ConfigFile cf;
    cf.load(resourcesCfg);

    Ogre::String name, locationType;
    Ogre::ConfigFile::SettingsBySection_ settingsBySection = cf.getSettingsBySection();
    for (const auto& p : settingsBySection)
    {
        for (const auto& r : p.second)
        {
            locationType = r.first;
            name = r.second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locationType);
        }
    }
}

void hpms::OgreContextAdaptee::LoadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void hpms::OgreContextAdaptee::CreateResourceListener()
{

}

bool hpms::OgreContextAdaptee::Setup()
{
    LOG_DEBUG("Initializing OGRE root.");
    InitRoot();

    LOG_DEBUG("Setting up OGRE resources.");
    SetupResources();

    LOG_DEBUG("Creating OGRE window.");
    bool ok = CreateWindowPair();
    if (!ok)
    {
        LOG_ERROR("Error creating native window handler.");
        return false;
    }

    LOG_DEBUG("Creating OGRE viewport.");
    CreateViewports();
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

    LOG_DEBUG("Creating OGRE resource listener.");
    CreateResourceListener();

    LOG_DEBUG("Loading OGRE resources.");
    LoadResources();

    CustomCreateScene();

    LOG_DEBUG("Creating OGRE frame listener.");
    CreateFrameListener();

    return true;
}


bool hpms::OgreContextAdaptee::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (windowPair.render->isClosed())
    {
        return false;
    }

    if (shutDown)
    {
        return false;
    }

    inputHandler.Update();
    inputHandler.HandleKeyboardEvent(keyStates);
    inputHandler.HandleMouseEvent(mouseButtonStates, &x, &y);
    CustomInput(keyStates, mouseButtonStates, x, y);

    CustomUpdate();

    return true;
}

void hpms::OgreContextAdaptee::CustomCreateScene()
{
    HPMS_ASSERT(logic, "Custom logic cannot be null.");
    logic->OnCreate();
}

void hpms::OgreContextAdaptee::CustomDestroyScene()
{
    HPMS_ASSERT(logic, "Custom logic cannot be null.");
    logic->OnDestroy();
}

void hpms::OgreContextAdaptee::CustomInput(const std::vector<hpms::KeyEvent>& keyEvents,
                                    const std::vector<hpms::MouseEvent>& mouseButtonEvents, unsigned int x,
                                    unsigned int y)
{
    HPMS_ASSERT(logic, "Custom logic cannot be null.");
    logic->OnInput(keyEvents, mouseButtonEvents, x, y);
}


void hpms::OgreContextAdaptee::CustomUpdate()
{
    HPMS_ASSERT(logic, "Custom logic cannot be null.");
    logic->OnUpdate();
}


hpms::ContextAdapter* CreateContext(hpms::CustomLogic* logic, const hpms::WindowSettings& settings) DYNALO_CALL {
    return hpms::SafeNew<hpms::OgreContextAdaptee>(logic, settings);
}

void DestroyContext(hpms::ContextAdapter* context)
{
    hpms::SafeDelete(context);
}
