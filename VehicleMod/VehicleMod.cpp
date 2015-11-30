// VehicleMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;

class HaulerMod {
private:
	DWORD x2gameAddress, climbAngleAddress, velocityAddress;
	vector<DWORD> climbAngleOffsets, velocityOffsets;

	void initX2() {
		LPCWSTR proc = L"x2game.dll";
		HANDLE handle = GetModuleHandle(proc);
		if (!handle) {
			LoadLibrary(proc);
			handle = GetModuleHandle(proc);
		}
		x2gameAddress = (DWORD)handle;
	}
	void initClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		climbAngleOffsets = { 0x0187C328, 0x134, 0x88, 0x7cc, 0x12c, 0x64 }; // C++11 only
		climbAngleAddress = x2gameAddress;
		int i = 0;
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
		int i = 0;
		for (; i < velocityOffsets.size() - 1; ++i) {
			velocityAddress = *(DWORD*)(velocityAddress + velocityOffsets[i]);
		}
		velocityAddress += velocityOffsets[i];
	}

public:
	HaulerMod() {
		/*
			Constructor will find the climbAngleAddress and velocityAddress based on the following:
			Climb angle is a float located at [[[[["x2game.dll" + 0x0187C328] + 0x134] + 0x88] + 0x7cc] + 0x12c] + 64
			Velocity is a floatl located at [[["x2game.dll" + 0187896C] + 0x8] + 0xc8] + 0x720
		*/
		initX2();
		initClimbAngle();
		initVelocity();
	}

	void enableHauler() {
		/*
		Change hauler climb angle from 45.0 to 90.0, and velocity from 5.0 to 13.0
		At climbAngleAddress: 00 00 34 42 should be changed to 00 00 B4 42
		At velocityAddress: 00 00 A0 40 should be changed to 00 00 50 41
		*/
		*(BYTE*)(climbAngleAddress + 2) = 0xB4;
		*(BYTE*)(velocityAddress + 2) = 0x50;
		*(BYTE*)(velocityAddress + 3) = 0x41;
	}

	void disableHauler() {
		*(BYTE*)(climbAngleAddress + 2) = 0x34;
		*(BYTE*)(velocityAddress + 2) = 0xA0;
		*(BYTE*)(velocityAddress + 3) = 0x40;
	}
};

/*
class UtilityFunctions {
private:
	HANDLE handle = GetCurrentProcess();
public:
	static DWORD getModuleBase(LPSTR lpModuleName, DWORD procID) {
		MODULEENTRY32 lpModuleEntry = { 0 };
		HANDLE hSnapShot 
	}
};
*/
