/*!
 * File HPMSSimulatorAdaptee.cpp
 */

#include <core/HPMSSimulatorAdaptee.h>

void hpms::SimulatorAdaptee::Run()
{
    Check();
    HPMS_ASSERT(logic, "Logic implementation cannot be null.");
    logic->OnCreate();
    ctx->GetRoot()->addFrameListener(this);
    LOG_DEBUG("OGRE subsystems initialization completed, starting rendering.");
    ctx->GetRoot()->startRendering();

    logic->OnDestroy();
}

hpms::SimulatorAdaptee::SimulatorAdaptee(hpms::OgreContext* ctx, hpms::CustomLogic* logic) : AdapteeCommon(ctx),
                                                                                             logic(logic)
{

}

hpms::SimulatorAdaptee::~SimulatorAdaptee()
{
    ctx->GetRoot()->removeFrameListener(this);
}

bool hpms::SimulatorAdaptee::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

    if (ctx->GetRenderWindow()->isClosed() || ctx->IsShutDown())
    {
        return false;
    }

    logic->OnUpdate(evt.timeSinceLastFrame);
    inputHandler.Update();
    inputHandler.HandleKeyboardEvent(keyStates);
    inputHandler.HandleMouseEvent(mouseButtonStates, &x, &y);
    logic->OnInput(keyStates, mouseButtonStates, x, y);
    logic->OnUpdate(evt.timeSinceLastFrame);

    return true;
}
