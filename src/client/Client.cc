#include "Client.h"
#include "Hooks.h"
#include "ModuleManager.h"

#include "modules/ImGuiModule.h"

void Client::Initialize()
{
    if (m_initialized)
        return;

    auto &mm = ModuleManager::GetInstance();

    mm.RegisterModule(std::make_unique<ImGuiModule>());
    mm.Initialize();

    for (auto &m : mm.GetModules())
        m->Enable();

    Hooks::Init();
    Hooks::Enable();

    m_initialized = true;
    m_running = true;
}

void Client::Shutdown()
{
    if (!m_initialized)
        return;

    Hooks::Restore();
    ModuleManager::GetInstance().Shutdown();

    m_running = false;
    m_initialized = false;
}

void Client::OnSwapBuffers(HDC hdc)
{
    if (!m_running)
        return;
    ModuleManager::GetInstance().OnSwapBuffers(hdc);
}

void Client::OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (!m_running)
        return;
    ModuleManager::GetInstance().OnWndProc(hWnd, msg, wParam, lParam);
}
