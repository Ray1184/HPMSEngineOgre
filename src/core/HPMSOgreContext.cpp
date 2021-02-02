/*!
 * File HPMSOgreContextAdaptee.cpp
 */

#include <core/HPMSOgreContext.h>
#include <SDL2/SDL_syswm.h>

hpms::OgreContext::OgreContext() : root(nullptr),
                                   camera(nullptr),
                                   sceneMgr(nullptr),
                                   resourcesCfg(HPMS_OGRE_CONFIG_FOLDER "Resources.ini"),
                                   pluginsCfg(HPMS_OGRE_CONFIG_FOLDER "Plugins.ini"),
                                   shutDown(false)
{
    logManager = hpms::SafeNewRaw<Ogre::LogManager>();
    logManager->createLog(HPMS_OGRE_CONFIG_FOLDER "Ogre.log", true, false, false);
    fsLayer = hpms::SafeNewRaw<Ogre::FileSystemLayer>("FSData");
    root = hpms::SafeNewRaw<Ogre::Root>(pluginsCfg,
                                        fsLayer->getWritablePath(
                                                HPMS_OGRE_CONFIG_FOLDER "Ogre.ini"),
                                        fsLayer->getWritablePath(
                                                HPMS_OGRE_CONFIG_FOLDER "Ogre.log"));

}

hpms::OgreContext::~OgreContext()
{
    root->destroySceneManager(GetSceneManager());
    hpms::SafeDeleteRaw(root);
    hpms::SafeDeleteRaw(fsLayer);
    hpms::SafeDeleteRaw(logManager);
}

bool hpms::OgreContext::CreateWindowPair(const OgreWindowSettings& settings)
{
    try
    {
        auto miscParams = Ogre::NameValuePairList();

        if (!SDL_WasInit(SDL_INIT_VIDEO))
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
        }

        auto p = root->getRenderSystem()->getRenderWindowDescription();
        miscParams.insert(p.miscParams.begin(), p.miscParams.end());
        p.miscParams = miscParams;
        p.name = settings.name;
        p.useFullScreen = settings.fullScreen;


        if (settings.width > 0 && settings.height > 0)
        {
            p.width = settings.width;
            p.height = settings.height;
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

        windowPair.render = root->createRenderWindow(p);
    } catch (std::exception& e)
    {
        return false;
    }
    return true;
}


void hpms::OgreContext::CreateViewports()
{
    sceneMgr = root->createSceneManager();
    camera = sceneMgr->createCamera(DEFAULT_CAMERA_NAME);
    Ogre::Viewport* vp = windowPair.render->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void hpms::OgreContext::InitRoot()
{
    root->setRenderSystem(
            root->getRenderSystemByName("OpenGL Rendering Subsystem"));
    root->initialise(false);
}

void hpms::OgreContext::SetupResources()
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

void hpms::OgreContext::LoadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void hpms::OgreContext::CreateResourceListener()
{

}

bool hpms::OgreContext::Setup(const OgreWindowSettings& settings)
{
    LOG_DEBUG("Initializing OGRE root.");
    InitRoot();

    LOG_DEBUG("Setting up OGRE resources.");
    SetupResources();

    LOG_DEBUG("Creating OGRE window.");
    bool ok = CreateWindowPair(settings);
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

    return true;
}


bool hpms::OgreContext::ManageClose() const
{
    if (windowPair.render->isClosed())
    {
        return false;
    }

    if (shutDown)
    {
        return false;
    }

    return true;
}
