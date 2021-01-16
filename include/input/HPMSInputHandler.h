/*!
 * File HPMSInputHandler.h
 */


#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <common/HPMSObject.h>
#include <common/HPMSUtils.h>
#include <thirdparty/InputHelper.h>


namespace hpms
{
    struct MouseEvent : public Object
    {
        enum MouseState
        {
            NONE,
            PRESSED_FIRST_TIME,
            PRESSED
        };

        std::string name;
        MouseState state{NONE};

        inline const std::string Name() const override
        {
            return "MouseEvent";
        }
    };

    struct KeyEvent : public Object
    {
        enum KeyState
        {
            NONE,
            PRESSED_FIRST_TIME,
            PRESSED
        };

        std::string name;
        KeyState state{NONE};


        inline const std::string Name() const override
        {
            return "KeyEvent";
        }

    };


    class InputHandler
    {
    private:

        std::map<SDL_Scancode, std::string> keyToString;
        std::map<SDL_Scancode, bool> keyState;

        std::map<tp::MouseButton, std::string> mouseButtonToString;
        std::map<tp::MouseButton, bool> mouseButtonState;

    public:
        InputHandler();

        inline void Update()
        {
            tp::InputHelper::Instance().update();
        }

        void HandleKeyboardEvent(std::vector<KeyEvent>& keys);

        void HandleMouseEvent(std::vector<MouseEvent>& mouseButtons, unsigned int* x, unsigned int* y);


    };
}