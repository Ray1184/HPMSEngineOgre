/*!
 * File HPMSSimulatorAdaptee.cpp
 */

#include <core/HPMSSimulatorAdaptee.h>

void hpms::SimulatorAdaptee::Run()
{
    Check();
    HPMS_ASSERT(logic, "Logic implementation cannot be null.");
    OgreWindowSettings ogreSettings;
    ogreSettings.name = windowSettings.name;
    ogreSettings.width = windowSettings.width;
    ogreSettings.height = windowSettings.height;
    if (!ctx->Setup(ogreSettings))
    {
        LOG_ERROR("Error setting up OGRE subsystems.");
        return;
    }
    logic->OnCreate();
    ctx->GetRoot()->addFrameListener(this);
    LOG_DEBUG("OGRE subsystems initialization completed, starting rendering.");
    ctx->GetRoot()->startRendering();

    logic->OnDestroy();
}

hpms::SimulatorAdaptee::SimulatorAdaptee(hpms::OgreContext* ctx, hpms::CustomLogic* logic,
                                         hpms::WindowSettings& windowSettings) : AdapteeCommon(ctx),
                                                                                 logic(logic),
                                                                                 windowSettings(windowSettings)
{

}

hpms::SimulatorAdaptee::~SimulatorAdaptee()
{
    ctx->GetRoot()->removeFrameListener(this);
}

bool hpms::SimulatorAdaptee::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    ctx->ManageClose();
    logic->OnUpdate(evt.timeSinceLastFrame);
    inputHandler.Update();
    inputHandler.HandleKeyboardEvent(keyStates);
    inputHandler.HandleMouseEvent(mouseButtonStates, &x, &y);
    logic->OnInput(keyStates, mouseButtonStates, x, y);
    logic->OnUpdate(evt.timeSinceLastFrame);

    return true;
}
