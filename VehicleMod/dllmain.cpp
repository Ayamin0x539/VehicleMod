// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "VehicleMod.cpp"

void mainloop() {
	std::string initMsg = "For ";
	if (NAGA) initMsg += "Naga";
	if (BABBAR) initMsg += ", Babbar";
	if (CASS) initMsg += ", Cass";
	initMsg += " only.";
	MessageBoxA(NULL, initMsg.c_str(), "Hauler Mod", MB_OK);

	bool validExpire = Utilities::checkExpirationIsValid();
	bool validGUID = Utilities::checkGUIDisValid();
	Sleep(1000);

	if (validExpire && validGUID) {
		HMODULE handle = GetModuleHandle(L"x2game.dll");

		HaulerMod hm((DWORD)handle); // make vehicle mod instance (no pointer)

		Sleep(5000);

		hm.updateAddresses();

		if (DEBUG)
			MessageBoxA(NULL, "Starting loop.", "Hauler Mod", MB_OK);
		Utilities::message(START_MESSAGE);
		while (1) {
			if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
				if (DEBUG) {
					hm.displayAddresses();
				}
			}
			if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
				hm.enableHauler();
			}
			if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
				hm.disableHauler();
			}
			Sleep(100);
		}
	}
	else {
		Sleep(500);
		Utilities::message("HACK OUTDATED. \nIt's been patched.\nExiting...");
		//std::terminate();
	}
	if (!validGUID) {
		Sleep(500);
		Utilities::message("You thought you could just leech this hack from a friend? \nMake your own. \nNice try.");
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

