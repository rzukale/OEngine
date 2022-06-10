#include "Keyboard.h"

bool Keyboard::m_Keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::m_KeysChanged[GLFW_KEY_LAST] = { 0 };

void Keyboard::KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
    if (Action != GLFW_RELEASE)
    {
        if (!m_Keys[Key])
        {
            m_Keys[Key] = true;
        }
    }
    else
    {
        m_Keys[Key] = false;
    }

    m_KeysChanged[Key] = Action != GLFW_REPEAT;
}

bool Keyboard::Key(int Key)
{
    return m_Keys[Key];
}

bool Keyboard::KeyChanged(int Key)
{
    bool Ret = m_KeysChanged[Key];
    m_KeysChanged[Key] = false;
    return Ret;
}

bool Keyboard::KeyWentUp(int Key)
{
    return !m_Keys[Key] && KeyChanged(Key);
}

bool Keyboard::KeyWentDown(int Key)
{
    return m_Keys[Key] && KeyChanged(Key);
}
