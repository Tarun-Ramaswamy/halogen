#include "../include/core/input.h"

namespace halogen
{
    Input::Input()
    {
        m_keys.clear();

        set_input_map();

        m_quit_application = false;
    }

    void Input::process_inputs()
    {
        if (SDL_PollEvent(&m_input_event) != 0)
        {
            m_keyboard_state = SDL_GetKeyboardState(nullptr);

            if (m_input_event.type == SDL_QUIT)
            {
                m_quit_application = true;
            }
        }
    }

    bool Input::is_key_pressed(InputMap::KeyboardInput::Keys key)
    {
        if (m_keyboard_state[key] == 1)
            return true;

        return false;
    }

    bool Input::quit()
    {
        return m_quit_application || is_key_pressed(InputMap::KeyboardInput::Quit);
    }

    void Input::set_input_map()
    {
        m_keys["Quit"] = SDL_SCANCODE_ESCAPE;

        m_keys["W"] = SDL_SCANCODE_W;
        m_keys["A"] = SDL_SCANCODE_A;
        m_keys["S"] = SDL_SCANCODE_S;
        m_keys["D"] = SDL_SCANCODE_D;

        m_keys["Right"] = SDL_SCANCODE_RIGHT;
        m_keys["Left"] = SDL_SCANCODE_LEFT;
        m_keys["Up"] = SDL_SCANCODE_UP;
        m_keys["Down"] = SDL_SCANCODE_DOWN;

        m_keys["Enter"] = SDL_SCANCODE_RETURN;
    }

    void Input::clean_up()
    {
        m_keys.clear();
        debug::log("Cleaning up input.");
    }

    Input::~Input()
    {
        clean_up();
    }
}