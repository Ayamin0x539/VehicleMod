// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

DWORD x2gameAddress, climbAngleAddress, velocityAddress;
std::vector<DWORD> climbAngleOffsets, velocityOffsets;
bool valid;

void message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "Alert", MB_OK);
}

void displayAddresses() {
	std::string msg = "---";
	if (x2gameAddress)
		msg += "\nx2game.dll address: " + std::to_string(x2gameAddress);
	if (velocityAddress)
		msg += "\nvelocity address: " + std::to_string(velocityAddress);
	if (climbAngleAddress)
		msg += "\nclimb angle address " + std::to_string(climbAngleAddress);
	MessageBoxA(NULL, msg.c_str(), "Address Info", MB_OK);
}

void initClimbAngle() {
	if (!x2gameAddress) {
		MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
		return;
	}
	if (DEBUG)
		message("Beginning address searching: climb angle.");

	climbAngleOffsets = { 0x0187C328, 0x134, 0x88, 0x7cc, 0x12c, 0x64 }; // C++11 only
	climbAngleAddress = x2gameAddress;
	unsigned int i = 0;
	for (; i < climbAngleOffsets.size() - 1; ++i) {
		Sleep(100);
		climbAngleAddress = (DWORD)(*(DWORD*)(climbAngleAddress + climbAngleOffsets[i]));
		Sleep(100);
	}
	climbAngleAddress += climbAngleOffsets[i];
}

void initVelocity() {
	if (!x2gameAddress) {
		MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
		return;
	}
	if (DEBUG)
		message("Beginning address searching: velocity.");

	velocityAddress = x2gameAddress;
	velocityOffsets = { 0x0187896C, 0x8, 0xc8, 0x720 };
	unsigned int i = 0;
	for (; i < velocityOffsets.size() - 1; ++i) {
		Sleep(100);
		velocityAddress = (DWORD)(*(DWORD*)(velocityAddress + velocityOffsets[i]));
		Sleep(100);
	}
	velocityAddress += velocityOffsets[i];
}

void enableHauler() {
	DWORD climbstore, velocitystore;
	if (VirtualProtect((LPVOID)(climbAngleAddress + 2), 1, PAGE_EXECUTE_READWRITE, &climbstore)) {
		*(BYTE*)(climbAngleAddress + 2) = 0xB4;
	}
	if (VirtualProtect((LPVOID)(velocityAddress + 2), 2, PAGE_EXECUTE_READWRITE, &velocitystore)) {
		*(BYTE*)(velocityAddress + 2) = 0x50;
		*(BYTE*)(velocityAddress + 3) = 0x41;
	}
	if (DEBUG)
		MessageBoxA(NULL, "Hauler hacks enabled.", "[ENABLED]", MB_OK);
}

void disableHauler() {
	DWORD climbstore, velocitystore;
	if (VirtualProtect((LPVOID)(climbAngleAddress + 2), 1, PAGE_EXECUTE_READWRITE, &climbstore)) {
		*(BYTE*)(climbAngleAddress + 2) = 0x34;
	}
	if (VirtualProtect((LPVOID)(velocityAddress + 2), 2, PAGE_EXECUTE_READWRITE, &velocitystore)) {
		*(BYTE*)(velocityAddress + 2) = 0xA0;
		*(BYTE*)(velocityAddress + 3) = 0x40;
	}
	if (DEBUG)
		MessageBoxA(NULL, "Hauler hacks disabled.", "[DISABLED]", MB_OK);
}

bool checkExpirationIsValid() {
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	int year = now.tm_year + 1900;
	int month = now.tm_mon + 1;
	int day = now.tm_mday;
	if (DEBUG) {
		std::string datemsg = "The date is " + std::to_string(month) + " " + std::to_string(day) + ", " + std::to_string(year);
		message(datemsg);
	}

	return (EXPIRE_YEAR >= year) && (EXPIRE_MONTH >= month) && (EXPIRE_DAY >= day);
}

void mainloop() {
	MessageBoxA(NULL, "For Naga, Babbar, and Cass only.", "Hauler Mod", MB_OK);
	valid = checkExpirationIsValid();

	Sleep(1000);

	if (valid) {
		HMODULE handle = GetModuleHandle(L"x2game.dll");
		x2gameAddress = (DWORD)handle;

		Sleep(5000);

		initClimbAngle();
		initVelocity();

		if (DEBUG)
			MessageBoxA(NULL, "Starting loop.", "Hauler Mod", MB_OK);
		message("Now working.\nNumpad 8 to enable.\nNumpad 7 to disable.");
		while (1) {
			if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
				if (DEBUG) {
					displayAddresses();
				}
			}
			if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
				enableHauler();
			}
			if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
				disableHauler();
			}
			Sleep(100);
		}
	}
	else {
		Sleep(500);
		message("HACK OUTDATED. \nIt's been patched.\nExiting...");
		//std::terminate();
	}

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&mainloop, 0, 0, 0);
	}
	return TRUE;
}

