// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>

DWORD WINAPI DoMagic2(LPVOID lpParameter)
{
    MessageBox(NULL, L"hijacked again", L"alert", MB_OK);

    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE threadHandle;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        threadHandle = CreateThread(NULL, 0, DoMagic2, NULL, 0, NULL);
        CloseHandle(threadHandle);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
