#pragma once
#include <Windows.h>
#include <atomic>

class Client
{
public:
    static Client &GetInstance()
    {
        static Client instance;
        return instance;
    }

    void Initialize();
    void Shutdown();

    bool IsInitialized() const { return m_initialized; }
    bool IsRunning() const { return m_running; }

    void RequestShutdown() { m_shutdownRequested = true; }
    bool ShutdownRequested() const { return m_shutdownRequested; }

    void OnSwapBuffers(HDC hdc);
    void OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    Client() = default;
    ~Client() = default;

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

private:
    bool m_initialized = false;
    bool m_running = false;
    std::atomic<bool> m_shutdownRequested = false;
};
