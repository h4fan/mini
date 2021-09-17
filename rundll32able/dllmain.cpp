// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

extern "C" __declspec(dllexport) void __cdecl runme(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    MessageBox(NULL, L"ok", L"alert", MB_OK);

    HANDLE hFile;
    char *DataBuffer = lpszCmdLine;
    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    hFile = CreateFile(L"test.txt",                // name of the write
        GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        CREATE_NEW,             // create new file only
        FILE_ATTRIBUTE_NORMAL,  // normal file
        NULL);                  // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("Terminal failure: Unable to open file \"\" for write.\n"));
        return;
    }


    bErrorFlag = WriteFile(
        hFile,           // open file handle
        DataBuffer,      // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        printf("Terminal failure: Unable to write to file.\n");
    }
    else
    {
        if (dwBytesWritten != dwBytesToWrite)
        {
            // This is an error because a synchronous write that results in
            // success (WriteFile returns TRUE) should write all data as
            // requested. This would not necessarily be the case for
            // asynchronous writes.
            printf("Error: dwBytesWritten != dwBytesToWrite\n");
        }
        else
        {
     
        }
    }

    CloseHandle(hFile);

    return ;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
   // MessageBox(NULL, L"in dllmain", L"info", MB_OK);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    //    MessageBox(NULL, L"1 in DLL_PROCESS_ATTACH", L"1", MB_OK);
        break;
    case DLL_THREAD_ATTACH:
    //    MessageBox(NULL, L"2 in DLL_THREAD_ATTACH", L"2", MB_OK);
        break;
    case DLL_THREAD_DETACH:
    //    MessageBox(NULL, L"3 in DLL_THREAD_DETACH", L"3", MB_OK);
        break;
    case DLL_PROCESS_DETACH:
    //    MessageBox(NULL, L"4 in DLL_PROCESS_DETACH", L"4", MB_OK);
        break;
    }
    return TRUE;
}
