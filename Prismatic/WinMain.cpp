#include "Application.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Application* app = new Application(300,300);
    app->Init();
    app->Run();

    if(app->Shutdown()){delete app; app = nullptr;}

    return 0;
}