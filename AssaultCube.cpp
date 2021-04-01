// AssaultCube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

/*int main()
{
    // Get ProcID for the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    // GetModulebase address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get Handle to process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;
    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    // Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
    std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    // Read Ammo Value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

    // Write to it
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
    
    // Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

    getchar();
    return 0;
}*/

int main() 
{
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0, ammoAddr=0;
    bool bHealth = false, bAmmo = false, bRecoil = false;

    const int newValue = 1337;
    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        localPlayerPtr = moduleBase + 0x10f4f4;

        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
        std::cout << healthAddr << std::endl;
    }
    else
    {
        std::cout << "Process not found, press enter to exit\n";
        getchar();
        return 0;
    }
    
    DWORD dwExit = 0;

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
            if (bAmmo)
            {
                //ff 06 = inc [esi];222222
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else
            {
                //ff 0E = dec [esi];
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            }
            else
            {
                mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            return 0;
        }

        if (bHealth)
        {
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit\n";
    getchar();
    return 0;
}


