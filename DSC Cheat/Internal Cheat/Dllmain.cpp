#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "Offsets.h"
#include "Flags.h"
#include "Structs.h"

HINSTANCE hInstance;
HANDLE myThread = NULL;

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

void Aimbot() {
    if (GetAsyncKeyState(VK_XBUTTON1) >> 16) {
        uintptr_t pClientState = *(uintptr_t*)((uintptr_t)GetModuleHandleA("engine.dll") + hazedumper::signatures::dwClientState);
        Vector3D* pViewAngles = (Vector3D*)(pClientState + hazedumper::signatures::dwClientState_ViewAngles);

        uintptr_t pLocalPlayer = *(uintptr_t*)((uintptr_t)GetModuleHandleA("client.dll") + hazedumper::signatures::dwLocalPlayer);
        int localTeam = *(int*)(pLocalPlayer + hazedumper::netvars::m_iTeamNum);

        Vector3D localPlayerOrigin = *(Vector3D*)(pLocalPlayer + hazedumper::netvars::m_vecOrigin);
        Vector3D localPlayerViewOffset = *(Vector3D*)(pLocalPlayer + hazedumper::netvars::m_vecViewOffset);
        Vector3D eyePosition = localPlayerOrigin + localPlayerViewOffset;

        Vector3D* pClosestAngle = nullptr;

        for (int i = 0; i < 64; i++)
        {
            uintptr_t pEntity = *(uintptr_t*)((uintptr_t)GetModuleHandleA("client.dll") + hazedumper::signatures::dwEntityList + 0x10 * i);
            if (pEntity == NULL) continue;
            
            int entityTeam = *(int*)(pEntity + hazedumper::netvars::m_iTeamNum);
            int health = *(int*)(pEntity + hazedumper::netvars::m_iHealth);
            bool entityDormant = *(bool*)(pEntity + hazedumper::signatures::m_bDormant);

            if (localTeam == entityTeam || entityDormant || !(health > 0)) continue;

            uintptr_t pEnemyBones = *(uintptr_t*)(pEntity + hazedumper::netvars::m_dwBoneMatrix);
            int boneID = 8;
            Vector3D enemyHeadPosition = Vector3D(   // Bone 8 is the head
                *(float*)(pEnemyBones + 0x30 * boneID + 0x0C),
                *(float*)(pEnemyBones + 0x30 * boneID + 0x1C),
                *(float*)(pEnemyBones + 0x30 * boneID + 0x2C)
            );

            // Get vector from our eyes to enemy head
            Vector3D relativePosition = enemyHeadPosition - eyePosition;

            float flathypotenuse = sqrt(relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y);

            Vector3D relativeAngle = Vector3D(
                atanf(relativePosition.z / flathypotenuse) * -RadToDeg,
                atan2f(relativePosition.y, relativePosition.x) * RadToDeg,
                0.f        // INSTANT BAN IF NOT 0!
            );
                
            if (relativeAngle[0] <= 89 && relativeAngle[0] >= -89) // INSTANT BAN IF OUT OF RANGE
            {
                if (pClosestAngle == nullptr || abs(relativeAngle[1] - (*pViewAngles)[1]) < abs((*pClosestAngle)[1] - (*pViewAngles)[1]))    // Check if this player is closer to cursor based on y angle
                {
                    pClosestAngle = &relativeAngle;
                }
            }
        }

        if (pClosestAngle != nullptr) {
            //printf("x=%f, y=%f, z=%f\n", pClosestAngle->x, pClosestAngle->y, pClosestAngle->z);
            *pViewAngles = *pClosestAngle;
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
        Aimbot();
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