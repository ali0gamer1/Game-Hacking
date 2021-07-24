#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


DWORD getprocid(const char *procname)
{
    DWORD proid = 0;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hsnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procentry;
        procentry.dwSize = sizeof(procentry);

        if (Process32First(hsnap, &procentry))
        {
            do
            {
                if (!_stricmp(procentry.szExeFile, procname))
                {
                    proid = procentry.th32ProcessID;
                    puts(procname);
                    break;
                }
            } while (Process32Next(hsnap, &procentry));


        }

    }
    
    CloseHandle(hsnap);
    return proid;

}

int main()
{
    const char* path = "C:\\Users\\alii\\Documents\\TETRIS\\Dll1\\Debug\\dll2.dll";
    DWORD procid = 0;
    const char* procname = "gta_sa.exe";

    while (!procid)
    {
        procid = getprocid(procname);
        Sleep(25);
    }

    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, 0, procid);

    if (hproc && hproc != INVALID_HANDLE_VALUE)
    {

        void* loc = VirtualAllocEx(hproc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (loc)
        {
            WriteProcessMemory(hproc, loc, path, strlen(path) + 1, 0);

        }
        HANDLE hthread = CreateRemoteThread(hproc, 0,0,(LPTHREAD_START_ROUTINE)LoadLibraryA, loc,0,0);
        if (hthread)
            CloseHandle(hthread);
        
        if (hproc)
            CloseHandle(hproc);

        


    }
    return 0;
}

