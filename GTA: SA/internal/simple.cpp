// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>


DWORD WINAPI hackthread(HMODULE hmodule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "CONOUT$\n";
  //  uintptr_t modulebase = (uintptr_t) GetModuleHandle(L"gta_sa.exe");
    void (*func)(const char*, char p2, char p3, char p4)= (void (*)(const char *,char p2,char p3, char p4))0x0588BE0;//Custom popup messages
    void (*func2)(void)= (void (*)(void))0x0438e40; //HESOYAM cheat code call

    
    while (1)
    {
        Sleep(50);
        if (GetAsyncKeyState(VK_END) & 1)
        {
            void *test = (void *)0x0859088;
            (*func)("ali",1,0,0);
            std::cout<< ((char*)test);
        }
 

        if (GetAsyncKeyState(VK_DOWN) & 1)
        {
            (*func2)();
        }


        if (GetAsyncKeyState(VK_CONTROL) & 1)
        {
            fclose(f);
            FreeConsole();
            FreeLibraryAndExitThread(hmodule, 0);
            return 0;
        }

    }



}





BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hackthread, hModule,0,nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

