#include "Hooks.h"

#include "Client.h"
#include "utils/ProcessHelper.h"
#include "utils/Memory.h"

Hooks g_Hooks;

void Hooks::Init()
{
    auto mod = ProcessHelper::getModuleBaseLoad("opengl32.dll");
    auto target = ProcessHelper::getExport(mod, "wglSwapBuffers");
    if (!target)
        return;

    g_Hooks.m_pWglSwapBuffersHook = std::make_unique<TFuncHook>(target, (void *)Hooks::hkwglSwapBuffers);
}

void Hooks::Enable()
{
    if (g_Hooks.m_pWglSwapBuffersHook)
        g_Hooks.m_pWglSwapBuffersHook->Enable();
}

void Hooks::Restore()
{
    if (g_Hooks.m_pWglSwapBuffersHook)
        g_Hooks.m_pWglSwapBuffersHook->Restore();

    if (g_hWnd && oWndProc)
        SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    oWndProc = nullptr;
    g_hWnd = nullptr;
    g_hwndInitialized = false;
}

LRESULT CALLBACK Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Client::GetInstance().OnWndProc(hWnd, msg, wParam, lParam);
    return CallWindowProcW(oWndProc, hWnd, msg, wParam, lParam);
}

BOOL WINAPI Hooks::hkwglSwapBuffers(HDC hdc)
{
    if (!g_hwndInitialized)
    {
        g_hWnd = WindowFromDC(hdc);
        if (g_hWnd)
            g_hwndInitialized = true;
    }

    if (g_hwndInitialized && !oWndProc)
    {
        SetLastError(0);
        LONG_PTR prev = SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc);

        if (prev != 0 || GetLastError() == 0)
            oWndProc = (WNDPROC)prev;
    }

    Client::GetInstance().OnSwapBuffers(hdc);

    return g_Hooks.m_pWglSwapBuffersHook->OriginalAs<BOOL(WINAPI *)(HDC)>()(hdc);
}
