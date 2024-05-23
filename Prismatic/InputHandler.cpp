#include "InputHandler.hpp"

InputHandler::InputHandler()
{
    Init();
}

InputHandler::InputHandler(const InputHandler& other)
{
    for (int i = 0; i < 256; ++i)
    {
        m_keys[i] = other.m_keys[i];
    }
}

InputHandler::~InputHandler() {}

void InputHandler::Init()
{
    for (int i = 0; i < 256; ++i)
    {
        m_keys[i] = false;
    }
}

void InputHandler::KeyDown(unsigned int key)
{
    if (key < 256)
    {
        m_keys[key] = true;
    }
}

void InputHandler::KeyUp(unsigned int key)
{
    if (key < 256)
    {
        m_keys[key] = false;
    }
}

bool InputHandler::IsKeyDown(unsigned int key)
{
    if (key < 256)
    {
        return m_keys[key];
    }
    return false;
}
