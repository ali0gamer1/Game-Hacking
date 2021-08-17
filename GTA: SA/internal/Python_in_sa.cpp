// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <Python.h>
#include <thread>
#include <ctype.h>



bool SpecialKeys(int S_Key) {
    switch (S_Key) {
    case VK_OEM_PERIOD:
        return true;

    default:
        return false;
    }
}







DWORD WINAPI hackthread(HMODULE hmodule)
{
    AllocConsole();
    


    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "CONOUT$\n";


    std::thread a(Py_Initialize);
    a.join();


    std::string stdOutErr =
        "import sys\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
"; //this is python code to redirect stdouts/stderr

    Py_Initialize();
    PyObject* pModule = PyImport_AddModule("__main__"); //create main module
    PyRun_SimpleString(stdOutErr.c_str()); //invoke code to redirect

    PyObject* catcher = PyObject_GetAttrString(pModule, "catchOutErr"); //get our catchOutErr created above
    PyErr_Print(); //make python print any errors
    
    






    std::cout << "init py\n";


    void (*func)( const char*, char p2, char p3, char p4)= (void (*)( const char *,char p2,char p3, char p4))0x0588BE0;
    void (*func2)(void)= (void (*)(void))0x0438e40;
    char* strme = (char*)0x969110;


    int* xpos1 = (int*)0xB6F5F0;
    int* xpos2 = (int*)(*xpos1 + 0x14);
    float* xpos3 = (float*)(*xpos2 + 0x30);
    float* ypos3 = (float*)(*xpos2 + 0x34);
    float* zpos3 = (float*)(*xpos2 + 0x38);


    bool shift_pressed = false;
    bool space_pressed = false;
    bool xmouse2_pressed = false;
    bool iscoding = false;
    float zstep = 1.2;
    bool  caps = false;

    std::string code;
    while (1)
    {
        if (GetAsyncKeyState(VK_CAPITAL) & 1)
        {
            caps = !caps;
        }

        //std::cout << strme << '\n';

        Sleep(25);

        if (GetAsyncKeyState(VK_MENU) & 1)
        {
            code = "";
            iscoding = !iscoding;
            (*func)("", 1, 0, 0);
        }


        if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        {
            void *test = (void *)0x0859088;
         //   (*func)(longmsg,0,0,0); //message
            std::cout<< ((char*)test);
        }


        if (iscoding) //silent
        {

            if (GetAsyncKeyState(VK_DELETE) & 1)
            {
                code = "";
                (*func)(code.c_str(), 0, 1, 0);
            }

            if (GetAsyncKeyState(VK_RETURN) & 1)
            {
                PyRun_SimpleString(code.c_str());
                PyObject* output = PyObject_GetAttrString(catcher, "value"); //get the stdout and stderr from our catchOutErr object
                
                if (strlen(PyUnicode_AsUTF8(output)) < 1)
                {
                    (*func)("(No output)", 1, 0, 0); //message
                    iscoding = false;
                    code = "";
                    continue;
                }

                if (strlen(PyUnicode_AsUTF8(output)) > 400)
                {
                    (*func)(std::string(PyUnicode_AsUTF8(output)).substr(0,400).c_str(), 1, 0, 0); //message
                    continue;
                }

                std::cout << PyUnicode_AsUTF8(output);
                (*func)(PyUnicode_AsUTF8(output), 1, 0, 0); //message
                iscoding = false;
                code = "";
                PyObject* pystr = PyUnicode_FromString("");
                PyObject_SetAttrString(catcher, "value", pystr);
                free(output);
                continue;
            }

            if (GetAsyncKeyState(VK_BACK) & 1)
            {
                code = code.substr(0, code.size() - 1);
                (*func)(code.c_str(), 0, 1, 0);
            }

            for (int KEY = 0; KEY < 256; KEY++)
            {
                if (GetAsyncKeyState(VkKeyScanA(KEY)) & 1) {
                    printf("\n%%d --> %d\n%%c --> %c\n",KEY,KEY);

                    if (KEY != VK_BACK)
                        code += (caps && char(KEY) ? char(KEY) : tolower(char(KEY)));

                    std::cout << code << '\n';

                    (*func)(code.c_str(), 0, 1, 0); //message
                    
                }
            }
            
            
            if (GetAsyncKeyState(VK_BACK) & 1)
            {
                code = code.substr(0, code.size()-2);
                std::cout << code.substr(0, code.size() - 1) << '\n';
                (*func)(code.c_str(), 0, 1, 0);
            }


        }


 

        if (GetAsyncKeyState(VK_DOWN) & 1)
        {
            (*func2)(); //health
        }
        

        if (GetAsyncKeyState(VK_CONTROL) & 1)
        {
            fclose(f);
            FreeConsole();
            Py_Finalize();
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

/* Test ped hack
        DWORD gtaActorBase = *(DWORD*)0xB74490;
        DWORD byteMap = *(DWORD*)(gtaActorBase + 4);
        int numberOfActiveActors = *(int*)(gtaActorBase + 12);

        for (int i = 0; i < numberOfActiveActors; i = i + 1)
        {

            BYTE inUse = *(BYTE*)byteMap;
            byteMap += 0x1;
            if (inUse > 0x00 && 0x80 > inUse)
            {

                DWORD offset = i  * 0x7C4;
                std::cout << i << '\n';
                DWORD pActorStruct = *(DWORD*)*(DWORD*)0xB74490; //pointer to the first element in the pool
                pActorStruct += offset; //add our offset for the active actor


                pActorStruct += 0x14;
                 *(float *)(DWORD*)(*((DWORD*)pActorStruct) + 0x30) = *xpos3;
                 *(float *)(DWORD*)(*((DWORD*)pActorStruct) + 0x34) = *ypos3;
                 *(float *)(DWORD*)(*((DWORD*)pActorStruct) + 0x38) = *zpos3;

            }
        }*/
