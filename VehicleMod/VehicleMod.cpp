// VehicleMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


class HaulerMod {
private:
	DWORD x2gameAddress, climbAngleAddress, velocityAddress;
	std::vector<DWORD> climbAngleOffsets, velocityOffsets;

	void message(std::string s) {
		MessageBoxA(NULL, s.c_str(), "DEBUG MESSAGE", MB_OK);
	}

	void initClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		climbAngleOffsets = { 0x0187C328, 0x134, 0x88, 0x7cc, 0x12c, 0x64 }; // C++11 only
		climbAngleAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < climbAngleOffsets.size() - 1; ++i) {
			climbAngleAddress = *(DWORD*)(climbAngleAddress + climbAngleOffsets[i]);
		}
		climbAngleAddress += climbAngleOffsets[i];
	}

	void initVelocity() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		velocityOffsets = { 0x0187896C, 0x8, 0xc8, 0x720 };
		velocityAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < velocityOffsets.size() - 1; ++i) {
			velocityAddress = *(DWORD*)(velocityAddress + velocityOffsets[i]);
		}
		velocityAddress += velocityOffsets[i];
	}



public:
	HaulerMod(DWORD x2gamehandle) {
		this->x2gameAddress = x2gamehandle;
		std::string msg = "Haulermod object initiated with handle " + std::to_string(x2gamehandle);
		message(msg);
	}

	void updateAddresses() {
		/*
		Find the climbAngleAddress and velocityAddress based on the following:
		Climb angle is a float located at [[[[["x2game.dll" + 0x0187C328] + 0x134] + 0x88] + 0x7cc] + 0x12c] + 64
		Velocity is a floatl located at [[["x2game.dll" + 0187896C] + 0x8] + 0xc8] + 0x720
		*/

		initClimbAngle();

		initVelocity();
		
	}

	void enableHauler() {
		/*
		Change hauler climb angle from 45.0 to 90.0, and velocity from 5.0 to 13.0
		At climbAngleAddress: 00 00 34 42 should be changed to 00 00 B4 42
		At velocityAddress: 00 00 A0 40 should be changed to 00 00 50 41
		*/
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

	DWORD getx2GameAddress() {
		return this->x2gameAddress;
	}

	DWORD getClimbAngleAddress() {
		return this->climbAngleAddress;
	}

	DWORD getVelocityAddress() {
		return this->velocityAddress;
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
};

