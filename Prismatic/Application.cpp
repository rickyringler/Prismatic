#include "Application.hpp"

Application::Application(){}
Application::Application(int win_width, int win_height) : m_Application(nullptr), m_inputHandler(nullptr), m_D3D(nullptr), m_application_name(L"ENGINE"), m_hinstance(nullptr), m_hwnd(nullptr), width(width), height(height) {}
Application::Application(const Application& other) = default;
Application::~Application() {Shutdown();}

int Application::Init() 
{

    InitWin(width, height);
    m_inputHandler = new InputHandler();
    m_inputHandler->Init();

    //m_D3D = new D3D;
    //bool result;
    //result = m_D3D->Init(hwnd, width, screen, VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    //if (!result) { MessageBox(hwnd, L"DX FAILURE", L"Error", MB_OK); return false; }

    return 0;
}

bool Application::Shutdown() 
{
    if (m_Application) 
    {
        m_Application->ShutdownWin();
        delete m_Application;
        m_Application = nullptr;
        return true;
    }
    if (m_inputHandler) 
    {
        delete m_inputHandler;
        m_inputHandler = nullptr;
        return true;
    }

    if(m_D3D)
    {
       // m_D3D->Shutdown();
        delete m_D3D;
    }

    return false;
}

void Application::Run()
{
    MSG msg = { 0 };
    bool done = false;

    while (!done)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                done = true;
                break;
            }
        }

        if (done) break;
        //if (!Render()) { done = true; }
    }
}

bool Application::Render() 
{

    if (m_inputHandler->IsKeyDown(VK_ESCAPE)){return false;}

    bool result;
    result = m_Application->Render();

    if (!result){return false;}

    return true;
}

LRESULT Application::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
    switch (msg) 
    {
    case WM_KEYDOWN:
        m_inputHandler->KeyDown(static_cast<unsigned int>(wparam));
        return 0;
    case WM_KEYUP:
        m_inputHandler->KeyUp(static_cast<unsigned int>(wparam));
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

void Application::InitWin(int& width, int& height) 
{
    WNDCLASSEX wc;
    ApplicationHandle = this;
    DEVMODE dmScreenSettings = { 0 };
    int posX, posY;

    m_hinstance = GetModuleHandle(NULL);
    m_application_name = L"ENGINE";

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_application_name;
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    if (FULL_SCREEN) 
    {
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(width);
        dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(height);
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
        posX = posY = 0;
    }
    else 
    {
        posX = (GetSystemMetrics(SM_CXSCREEN) - this->width) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - this->height) / 2;
    }

    m_hwnd = CreateWindowEx
    (
        WS_EX_APPWINDOW, 
        m_application_name, 
        m_application_name,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, this->width, this->height, 
        NULL, 
        NULL, 
        m_hinstance, 
        NULL
    );

    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);
    ShowCursor(false);
}

void Application::ShutdownWin() 
{
    ShowCursor(true);

    if (FULL_SCREEN) 
    {
        ChangeDisplaySettings(NULL, 0);
    }

    DestroyWindow(m_hwnd);
    UnregisterClass(m_application_name, m_hinstance);
    m_hwnd = nullptr;
    m_hinstance = nullptr;
    ApplicationHandle = nullptr;
}

LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ApplicationHandle) 
    {
        return ApplicationHandle->MessageHandler(hwnd, msg, wparam, lparam);
    }
    else 
    {
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}
