#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Offsets.h"
#include "Flags.h"

HINSTANCE hInstance;
HANDLE myThread = NULL;

struct glow_t {
    int m_nNextFreeSlot;
    int pEntity;
    float r;
    float g;
    float b;
    float a;
    bool m_bGlowAlphaCappedByRenderAlpha;
    float m_flGlowAlphaFunctionOfMaxVelocity;
    float m_flGlowAlphaMax;
    float m_flGlowPulseOverdrive;
    bool m_bRenderWhenOccluded;
    bool m_bRenderWhenUnoccluded;
    bool m_bFullBloomRender;
    int m_nFullBloomStencilTestValue;
    int m_nRenderStyle;
    int m_nSplitScreenSlot;
};

void Bhop() {
    if ((1 << 16) & GetAsyncKeyState(VK_SPACE))
    {
        uintptr_t pClient = (uintptr_t)GetModuleHandleA("client.dll");
        uintptr_t pLocalPlayer = *(uintptr_t*)(pClient + hazedumper::signatures::dwLocalPlayer);
        int flags = *(int*)(pLocalPlayer + hazedumper::netvars::m_fFlags);                  // Flags describe motion of player currently
        int* pForceJump = (int*)(pClient + hazedumper::signatures::dwForceJump);             // Variable which has been reverse engineered through SDK to make the player jump

        if (flags & FL_ONGROUND)
        {
            *pForceJump = 5;      // Reverse engineered number to make the player jump
            Sleep(15);
            *pForceJump = 4;      // Reset back to inital value
            Sleep(10);
        }
    }
}

void Glow() {
    glow_t* pGlowObject = *(glow_t**)((uintptr_t)GetModuleHandleA("client.dll") + hazedumper::signatures::dwGlowObjectManager);
    uintptr_t pLocalPlayer = *(uintptr_t*)((uintptr_t)GetModuleHandleA("client.dll") + hazedumper::signatures::dwLocalPlayer);
    int localTeam = *(int*)(pLocalPlayer + hazedumper::netvars::m_iTeamNum);

    for (int i = 0; i < 64; i++)
    {
        uintptr_t pEntity = *(uintptr_t*)((uintptr_t)GetModuleHandleA("client.dll") + hazedumper::signatures::dwEntityList + 0x10 * i);

        if (pEntity != NULL)
        {
            int entityTeam = *(int*)(pEntity + hazedumper::netvars::m_iTeamNum);

            int glowIndex = *(int*)(pEntity + hazedumper::netvars::m_iGlowIndex);
            glow_t* g = &pGlowObject[glowIndex];
            bool entityDormant = *(bool*)(pEntity + hazedumper::signatures::m_bDormant);

            if (entityDormant || entityTeam < 2)
            {
                continue;
            }

            if (entityTeam == localTeam)
            {
                g->r = 0.0f;
                g->g = 1.0f;
                g->b = 0.0f;
                g->a = 0.7f;
                g->m_bRenderWhenOccluded = true;
            }
            else
            {
                g->r = 1.0f;
                g->g = 0.0f;
                g->b = 0.0f;
                g->a = 0.7f;
                g->m_bRenderWhenOccluded = true;
            }
        }
    }
}

void MainThread() {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & (1 << 15)) {
            system("cls");
            std::cout << "Goodbye!";
            FreeConsole();
            FreeLibraryAndExitThread(hInstance, 0);
            break;
        }

        Bhop();
        Glow();
    }
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        hInstance = hinstDLL;
        myThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}