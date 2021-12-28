#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
/*X is bomb O is empty*/
DWORD getprocID(const wchar_t* procname)
{
    DWORD procid = 0;

    HANDLE hsnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

    if (hsnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procentry;
        procentry.dwSize = sizeof(procentry);

        if (Process32First(hsnap, &procentry))
        {
            do
            {
                if (!wcscmp(procentry.szExeFile, procname))
                {

                    procid = procentry.th32ProcessID;
                    break;
                }


            } while (Process32Next(hsnap, &procentry));

        }

    }

    CloseHandle(hsnap);
    return procid;
}

uintptr_t get_module_base_addr(DWORD procID, const wchar_t* modname)
{
    uintptr_t modbaseadr = 0;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE, procID);

    if (hsnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modentry;
        modentry.dwSize = sizeof(modentry);

        if (Module32First(hsnap, &modentry))
        {
            do
            {
                if (!wcscmp(modentry.szModule, modname))
                {
                    modbaseadr = (uintptr_t)modentry.modBaseAddr;
                    break;
                }

            } while (Module32Next(hsnap, &modentry));

        }

    }

    CloseHandle(hsnap);
    return modbaseadr;
}

uintptr_t findDMAaddr(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;

    for (unsigned int i = 0; i < offsets.size(); i++)
    {
        ReadProcessMemory(hproc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;

}

int main()
{
    DWORD procid = getprocID(L"winmine.exe");

    uintptr_t modulebase = get_module_base_addr(procid, L"winmine.exe");
    if (modulebase == 0)
    {
        std::cerr << "Error: game not running" << '\n';
        return 1;
    }
    HANDLE hprocess = 0;
    hprocess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procid);

    uintptr_t field = modulebase + 0x6690;

    unsigned char booz[840] = {};

    bool flag = 0;

    ReadProcessMemory(hprocess, (BYTE*)field, booz, sizeof(booz), 0);
    int counter = 0;
    for (int i = 0; i < 831; i++)
    {
        if (flag && booz[i] != 0x10)
        {

            if (booz[i] == 0x8f)
            {
                std::cout << std::hex << 'X' << ' ';
                counter++;
            }
            else
            {
                std::cout << std::hex << 'O' << ' ';
                counter++;
            }
            if ((counter % 8 == 0 && counter != 0))
            {
                std::cout << "\n";
            }
        }

        if (booz[i] == 0x10)
        {
            flag = !flag;
        }
    }
    return 0;
}
