#ifndef _INPUT_HANDLER_HPP
#define _INPUT_HANDLER_HPP

class InputHandler
{
public:
    InputHandler();
    InputHandler(const InputHandler&);
    ~InputHandler();

    void Init();
    void KeyDown(unsigned int key);
    void KeyUp(unsigned int key);
    bool IsKeyDown(unsigned int key);

private:
    bool m_keys[256];
};

#endif
