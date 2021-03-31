#include "proc.h"

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0; //for error checking
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //snapshot process
	if (hsnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hsnap, &procEntry)) {
			do 
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} 			
			while (Process32Next(hsnap, &procEntry));
		}
	}
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{

}

uintptr_t FindDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int>)
{

}
