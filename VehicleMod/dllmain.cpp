// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "VehicleMod.cpp"
#include "Utilities.h"
#include "Zoom.h"

void mainloop() {
	MessageBoxA(NULL, INIT_MESSAGE.c_str(), "Hauler Mod", MB_OK);

	bool validExpire = Utilities::checkExpirationIsValid();
	//bool validGUID = Utilities::checkGUIDisValid();
	if (!Utilities::sendUserInfoEmail()) { // Send the user's info to the reporting email
		Utilities::debugMsg("Error sending email. Continuing as normal.");
	}
	bool validUserInfoString = Utilities::checkUserInfoValid();
	Sleep(1000);

	if (validExpire && validUserInfoString) {
		HMODULE handle = GetModuleHandle(L"x2game.dll");

		HaulerMod hm((DWORD)handle); // make vehicle mod instance (no pointer)

		Sleep(5000);

		hm.updateAddresses();

		/*********** Init Zoom Stuff ***********/
		
		if (DEBUG) Utilities::message("Initializing zoom instruction address... via pattern search...");
		initializeZoomInstructionAddress();

		if (ZOOM_INSTRUCTION_ADDRESS) {
			if (DEBUG) Utilities::message("Beginning detour...");
			detourZoomAndModifyZoomValue();
		}
		else {
			if (DEBUG) Utilities::message("Zoom INSTRUCTION address wasn't found by pattern search!");
		}
		/*********** End Zoom Stuff ***********/

		if (DEBUG)
			Utilities::message("STARTING LOOP!");
		Utilities::init_message(START_MESSAGE);
		bool enabled = 0; // for middle click hotkey
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
			if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
				if (enabled) {
					hm.disableHauler();
					enabled = false;
				}
				else {
					hm.enableHauler();
					enabled = true;
				}
			}
			Sleep(200);
		}
	}
	else if(!validExpire) {
		Sleep(500);
		Utilities::outdated_message(STR_OUTDATED_MESSAGE);
		//std::terminate();
	}
	if (!validUserInfoString) {
		Sleep(500);
		Utilities::unauthorized_user_message(STR_UNAUTHORIZED_USER_MESSAGE);
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

