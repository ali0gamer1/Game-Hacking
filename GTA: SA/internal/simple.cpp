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
    void (*func)(const char*, char p2, char p3, char p4)= (void (*)(const char *,char p2,char p3, char p4))0x0588BE0; //custom messages
    void (*func2)(void)= (void (*)(void))0x0438e40; //Activate hesoyam (no message)
    char* strme = (char*)0x969110; //TEST


    int* zpos1 = (int*)0xB6F5F0; //cped pointer (repeated each time for clarification)
    int* zpos2 = (int*)(*zpos1 + 0x14);
    float* zpos3 = (float*)(*zpos2 + 0x38);
    
    int* xpos1 = (int*)0xB6F5F0; //cped
    int* xpos2 = (int*)(*xpos1 + 0x14);
    float* xpos3 = (float*)(*xpos2 + 0x30);

    int* ypos1 = (int*)0xB6F5F0;//cped
    int* ypos2 = (int*)(*ypos1 + 0x14);
    float* ypos3 = (float*)(*ypos2 + 0x34);

    bool shift_pressed = false;
    bool space_pressed = false;
    bool xmouse2_pressed = false;
    float zstep = 1.2;
    while (1)
    {

        //std::cout << strme << '\n';

        Sleep(10);
        if (GetAsyncKeyState(VK_END) & 1)
        {
            void *test = (void *)0x0859088;
            (*func)("sss",1,0,0); //message
            std::cout<< ((char*)test);
        }
 

        if (GetAsyncKeyState(VK_DOWN) & 1)
        {
            (*func2)(); //health
        }



        if (GetAsyncKeyState(VK_XBUTTON1) & 1)
        {
            *xpos3 += 1.0;
            xmouse2_pressed = true;
            std::cout << *xpos3;

        }
        else if (xmouse2_pressed)
        {

            xmouse2_pressed = false;
        }
        
        if (GetAsyncKeyState(VK_SPACE) != 0)
        {
            space_pressed = true;
           *zpos3 -= zstep;
            zstep += 0.4;
        }
        else if (space_pressed)
        {
            space_pressed = false;
            zstep = 1.2;
        }

        if (GetAsyncKeyState(VK_SHIFT) != 0)
        {
            shift_pressed = true;
            *zpos3 += zstep;
            zstep += 0.4;
        }
        else if (shift_pressed)
        {
            shift_pressed = false;
            zstep = 1.2;
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
