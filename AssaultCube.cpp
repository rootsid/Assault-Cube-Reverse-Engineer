// AssaultCube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    // Get ProcID for the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    // GetModulebase address

    // Get Handle to process

    // Resolve base address of the pointer chain

    // Resolve our ammo pointer chain

    // Read Ammo Value

    // Write to it

    // Read out again
    std::cout << procId;
}
