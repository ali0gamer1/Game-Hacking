#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

DWORD getprocID(const wchar_t *procname)
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
                if (!strcmp(procentry.szExeFile, (char * )procname))
                {

                    procid = procentry.th32ProcessID;
                    break;
                }


            }while(Process32Next(hsnap, &procentry));


        }

    }

    CloseHandle(hsnap);
    return procid;
}

uintptr_t get_module_base_addr(DWORD procID, const wchar_t *modname)
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
                if (!strcmp(modentry.szModule, (char *)modname))
                {
                    modbaseadr = (uintptr_t)modentry.modBaseAddr;
                    break;
                }


            }while(Module32Next(hsnap, &modentry));

        }

    }

    CloseHandle(hsnap);
    return modbaseadr;

}


uintptr_t findDMAaddr(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;

    for (unsigned int i = 0; i < offsets.size();i++)
    {
        ReadProcessMemory(hproc, (BYTE *)addr, &addr, sizeof(addr),0);
        addr += offsets[i];
    }
    return addr;



}




int main()
{
    DWORD procid = getprocID((wchar_t*)"ac_client.exe");

    uintptr_t modulebase = get_module_base_addr(procid, (wchar_t*)"ac_client.exe");

    if (modulebase == 0)
    {
        std::cerr<<"Error: game not running"<<'\n';
        return 1;
    }


    HANDLE hprocess = 0;
    hprocess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procid);

    uintptr_t dynamicptr = modulebase + 0x10f4f4;



    std::vector<unsigned int> ammos;
    ammos.push_back(0x374);
    ammos.push_back(0x14);
    ammos.push_back(0x0);


    std::vector<unsigned int> healthvec;
    healthvec.push_back(0xf8);

    uintptr_t healthaddr = findDMAaddr(hprocess, dynamicptr, healthvec);


    uintptr_t ammoaddr = findDMAaddr(hprocess, dynamicptr, ammos), tempammoaddr = 0;
    std::cout<<"Dynamic pointer base: "<< std::hex << dynamicptr<<'\n';
    std::cout<<"ammo addr:  " << ammoaddr<<'\n'<<std::dec;
    int ammo = 0, tempammo = 0, newammo = 100, health = 0, temphealth = 0, newhealth = 1000;
    ReadProcessMemory(hprocess, (BYTE *)healthaddr, &temphealth, sizeof(health), 0);
    ReadProcessMemory(hprocess, (BYTE *)ammoaddr, &tempammo, sizeof(ammo), 0);

    while(1)
    {
         ReadProcessMemory(hprocess, (BYTE *)healthaddr, &health, sizeof(health), 0);
         ReadProcessMemory(hprocess, (BYTE *)ammoaddr, &ammo, sizeof(ammo), 0);
         ammoaddr = findDMAaddr(hprocess, dynamicptr, ammos);

         Sleep(50);
         if (ammo != tempammo)
         {
             std::cout<<"Old ammo: "<<tempammo<<'\n';
             tempammo = ammo;
             WriteProcessMemory(hprocess,(BYTE *)ammoaddr,&newammo, sizeof(newammo),0);
             std::cout<<"New ammo: "<<newammo<<'\n';
         }
         if (temphealth != health)
         {
             std::cout<<"Old Health: "<<temphealth<<'\n';
             temphealth = health;
             WriteProcessMemory(hprocess,(BYTE *)healthaddr,&newhealth, sizeof(newhealth),0);
             std::cout<<"New Health: "<<newhealth<<'\n';
         }

    }

    std::cin.get();
    return 0;
}
