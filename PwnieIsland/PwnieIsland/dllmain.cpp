// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>

// -- BEGIN MEMORY FUNCTIONS -- 

// Retrieve the Base Memory Address of a DLL loaded into a given process by ID
// GetModuleBaseAddress solution from: https://stackoverflow.com/questions/39091948/c-get-the-base-address-of-a-dll-of-a-running-process
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

// PatchByte and LocateDirectMemoryAddress Components modified from https://guidedhacking.com/threads/how-to-hack-any-game-tutorial-c-trainer-1-external.10897/
// Set Permissions and Copy Bytes into Memory
void PatchByte(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

// Given Address and Relative Pointer Offsets, Locate Memory Address
uintptr_t LocateDirectMemoryAddress(uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t address = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i) {
		address = *(uintptr_t*)address;
		address += offsets[i];
	}
	return address;
}

// -- END MEMORY FUNCTIONS -- 

// -- BEGIN HACK FUNCTIONS -- 

DWORD APIENTRY CyberRaijuPwnIsland(HMODULE hModule) {

	// Define Variables 
	bool speedHack = false;
	bool jumpHoldHack = false;
	bool airHoppingHack = false;
	bool ghostHack = false;
	bool cowHack = false;
	float hackSpeedVal = 700.0f;
	float hackJumpHoldVal = 60.0f;
	DWORD procId = GetCurrentProcessId();
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");
	uintptr_t goldenEgg1 = LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x0C, 0x4 });
	uintptr_t procBase = GetModuleBaseAddress(procId, L"PwnAdventure3-Win32-Shipping.exe");
	uintptr_t unbearableAddress = LocateDirectMemoryAddress(moduleBase + 0x000752e8, {});
	uintptr_t cowAddress = LocateDirectMemoryAddress(moduleBase + 0x000751b8, {});
	float zAxisValue = 0.0f;
	float yAxisValue = 0.0f;
	float xAxisValue = 0.0f;
	float origJump = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x128 });
	float origSpeed = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x120 });


	// Attach Console for Debug Output
	AllocConsole();
	FILE* consoleStream;
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);
	std::cout << "Injection Successful!\nDetecting ProcessID: " << procId << "\n";
	std::cout << "GameLogic Module Address: " << std::hex << moduleBase << "\n";
	std::cout << "Unbearable Address: " << std::hex << unbearableAddress << "\n";
	std::cout << "Cow Level Address: " << std::hex << (cowAddress) << "\n";
	std::cout << "F1: Speed Hack\n";
	std::cout << "F2: Flying Hack (Jump and hold)\n";
	std::cout << "F3: Air Hopping Hack (Jump in midair)\n";
	std::cout << "F4: Ghost Hack (Use: Ins, Del, Home, End, Pg Up, Pg Down)\n";
	std::cout << "F5: Cow Level Hack (Replace UnbearableForest with Cowabungalow in fast travel)\n";

	while (true) {
		// Enable or Disable Speed Hack if F1 is Pressed
		if (GetAsyncKeyState(VK_F1) & 0x8000) {
			speedHack = !speedHack;
			std::cout << "Speed Hack: " << speedHack << "\n";
			if (speedHack) {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x120 }), (BYTE*)&hackSpeedVal, sizeof(hackSpeedVal));
				std::cout << "Player Speed: " << hackSpeedVal << "\n";
			}
			else {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x120 }), (BYTE*)&origSpeed, sizeof(origSpeed));
				std::cout << "Player Speed: " << origSpeed << "\n";
			}
			Sleep(500);
		}
		// Enable or Disable Jump Hold Hack if F2 is Pressed
		if (GetAsyncKeyState(VK_F2) & 0x8000) {
			jumpHoldHack = !jumpHoldHack;
			std::cout << "Jump Hack: " << jumpHoldHack << "\n";
			if (jumpHoldHack) {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x128 }), (BYTE*)&hackJumpHoldVal, sizeof(hackJumpHoldVal));
				std::cout << "Player Jump Hold Time: " << hackJumpHoldVal << "\n";
			}
			else {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x6C, 0x128 }), (BYTE*)&origJump, sizeof(origJump));
				std::cout << "Player Jump Hold Time: " << origJump << "\n";
			}
			Sleep(500);
		}
		// Enable or Disable Air Hopping Hack if F3 is Pressed
		if (GetAsyncKeyState(VK_F3) & 0x8000) {
			airHoppingHack = !airHoppingHack;
			std::cout << "Air Hopping Hack: " << airHoppingHack << "\n";
			if (airHoppingHack) {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00051680, {}), (BYTE*)"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x85\x09", 16);
			}
			else {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00051680, {}), (BYTE*)"\x8B\x49\x9C\x85\xC9\x74\x07\x8B\x01\x8B\x40\x50\xFF\xE0\x32\xC0", 16);
			}
			Sleep(500);
		}
		// Enable or Disable Ghost Hack if F4 is Pressed
		if (GetAsyncKeyState(VK_F4) & 0x8000) {
			ghostHack = !ghostHack;
			std::cout << "Ghost Hack: " << ghostHack << "\n";
			Sleep(500);
		}
		//Key Bindings and Logic for Ghost Hack
		if (GetAsyncKeyState(VK_PRIOR) & 0x8000) {
			if (ghostHack) {
				zAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x98 });
				zAxisValue += 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x98 }), (BYTE*)&zAxisValue, sizeof(zAxisValue));
				std::cout << "Z-Axis: " << zAxisValue << "\n";
			}
		}
		if (GetAsyncKeyState(VK_NEXT) & 0x8000) {
			if (ghostHack) {
				zAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x98 });
				zAxisValue -= 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x98 }), (BYTE*)&zAxisValue, sizeof(zAxisValue));
				std::cout << "Z-Axis: " << zAxisValue << "\n";
			}
		}
		if (GetAsyncKeyState(VK_HOME) & 0x8000) {
			if (ghostHack) {
				yAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x94 });
				yAxisValue += 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x94 }), (BYTE*)&yAxisValue, sizeof(yAxisValue));
				std::cout << "Y-Axis: " << yAxisValue << "\n";
			}
		}
		if (GetAsyncKeyState(VK_END) & 0x8000) {
			if (ghostHack) {
				yAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x94 });
				yAxisValue -= 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x94 }), (BYTE*)&yAxisValue, sizeof(yAxisValue));
				std::cout << "Y-Axis: " << yAxisValue << "\n";
			}
		}
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
			if (ghostHack) {
				xAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x90 });
				xAxisValue += 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x90 }), (BYTE*)&xAxisValue, sizeof(xAxisValue));
				std::cout << "X-Axis: " << xAxisValue << "\n";
			}
		}
		if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
			if (ghostHack) {
				xAxisValue = *(float*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x90 });
				xAxisValue -= 1.0f;
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00097D7C, { 0x1C, 0x4, 0x4c, 0x38, 0x8, 0x54, 0x90 }), (BYTE*)&xAxisValue, sizeof(xAxisValue));
				std::cout << "X-Axis: " << xAxisValue << "\n";
			}
		}
		// Enable or Disable Cow Level Hack if F5 is Pressed
		if (GetAsyncKeyState(VK_F5) & 0x8000) {
			cowHack = !cowHack;
			if (cowHack) {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x0005582E, {}), (BYTE*)"\x08", 1);
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00055830, {}), (BYTE*)&cowAddress, sizeof(cowAddress));
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00055838, {}), (BYTE*)"\x08", 1);
			}
			else {
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x0005582E, {}), (BYTE*)"\x0F", 1);
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00055830, {}), (BYTE*)&unbearableAddress, sizeof(unbearableAddress));
				PatchByte((BYTE*)LocateDirectMemoryAddress(moduleBase + 0x00055838, {}), (BYTE*)"\x0F", 1);
			}
			std::cout << "Cow Level Enabled: " << cowHack << "\n";
			Sleep(500);
		}
	}
	// Close Console and Exit Thread
	fclose(consoleStream);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}
// -- END HACK FUNCTIONS -- 

// -- BEGIN MAIN FUNCTION -- 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CyberRaijuPwnIsland, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// -- END MAIN FUNCTION -- 


