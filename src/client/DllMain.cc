#include "Client.h"
#include <iostream>

static DWORD WINAPI init(LPVOID hModule)
{
    Client::GetInstance().Initialize();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, init, hModule, 0, nullptr);
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        Client::GetInstance().Shutdown();
    }
    return TRUE;
}