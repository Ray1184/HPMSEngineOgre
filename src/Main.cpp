/*!
 * File Main.cpp
 */



#include <core/HPMSOgreContextAdaptee.h>
#include <iostream>
#include <OgreRectangle2D.h>
#include <core/HPMSEntityHelper.h>
#include <core/HPMSRenderToTexture.h>
#include <resource/HPMSLuaScript.h>
#include <resource/HPMSLuaScriptManager.h>
#include <resource/HPMSWalkmap.h>
#include <resource/HPMSWalkmapManager.h>

using namespace Ogre;
using namespace std;

class TestContext : public hpms::OgreContext
{
public:

protected:

    Ogre::SceneNode* node;

    Ogre::RenderTexture* renderTexture;

    Ogre::Rectangle2D* fbScreen;

public:
    TestContext(const hpms::WindowSettings& windowSettings) : hpms::OgreContext(windowSettings)
    {}

protected:

    void CustomCreateScene() override
    {
        GetSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
        GetSceneManager()->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
        Ogre::Camera* camera = GetCamera();
        camera->setNearClipDistance(1.0);
        camera->setFarClipDistance(50.0);
        camera->setPosition(5, 5, 5);
        camera->lookAt(0, 0, 0);


        Ogre::Entity* entity = GetSceneManager()->createEntity("Cube.mesh");
        node = GetSceneManager()->getRootSceneNode()->createChildSceneNode("EntityNode");
        node->attachObject(entity);
        //hpms::DepthGeometry* entityDepth = hpms::SafeNew<hpms::DepthGeometry>("DepthCube.mesh", this);
        Ogre::Entity* entityDepth = GetSceneManager()->createEntity("DepthCube.mesh");
        hpms::EntityHelper::SetWriteDepthOnly(entityDepth);
        GetSceneManager()->getRootSceneNode()->createChildSceneNode("DepthNode")->attachObject(entityDepth);
        Ogre::Light* light = GetSceneManager()->createLight();
        light->setType(Ogre::Light::LT_POINT);
        light->setPosition(-5, 10, 10);
        light->setDiffuseColour(1.0f, 1.0f, 1.0f);


        hpms::WalkmapPtr map = hpms::WalkmapManager::GetSingleton().Create("Basement.hrdat");

        auto* rtt = hpms::RenderToTexture::GetOrCreate(this, 320, 200);
        rtt->Activate();

        hpms::LuaScriptPtr script = hpms::LuaScriptManager::GetSingleton().Create("TestScript.lua");

        std::cout << script->GetScriptContent() << std::endl;









    }

    void CustomDestroyScene() override
    {

    }

    void CustomInput(const std::vector<hpms::KeyEvent>& keyEvents,
                     const std::vector<hpms::MouseEvent>& mouseBussonEvents, unsigned int x,
                     unsigned int y) override
    {
        if (!keyEvents.empty())
        {
            std::cout << "----------" << std::endl;
        }
        for (auto& k : keyEvents)
        {
            if (k.name == "S" && k.state == hpms::KeyEvent::PRESSED_FIRST_TIME)
            {

                renderTexture->update();

                renderTexture->writeContentsToFile("Screenshot.png");
                std::cout << "Got screenshot!" << std::endl;
            }
        }
        if (!keyEvents.empty())
        {
            std::cout << "----------" << std::endl;
        }


        if (!mouseBussonEvents.empty())
        {
            std::cout << "##########" << std::endl;
        }
        for (auto& k : mouseBussonEvents)
        {
            std::cout << k.name << " --> " << k.state << std::endl;
        }
        if (!mouseBussonEvents.empty())
        {
            std::cout << "x --> " << x << " y --> " << y << std::endl;
            std::cout << "##########" << std::endl;
        }
    }

    void CustomUpdate() override
    {
        node->rotate(Vector3::NEGATIVE_UNIT_Y, Degree(0.5f));
    }
};

int main(int argc, char* argv[])
{
    hpms::WindowSettings ws;
    ws.width = 1280;
    ws.height = 800;
    TestContext txt(ws);
    try
    {
        txt.Run();
    } catch (std::exception& ex)
    {
        LOG_ERROR(ex.what());
    }

    return 0;
}


