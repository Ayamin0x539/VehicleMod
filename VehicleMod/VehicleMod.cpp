// VehicleMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "constants.h"
#include "machine_id.h"

class Utilities {

public:
	Utilities() {}

	static void message(std::string s) {
		MessageBoxA(NULL, s.c_str(), "DEBUG MESSAGE", MB_OK);
	}

	static bool checkExpirationIsValid() {
		time_t t = time(0);
		struct tm now;
		localtime_s(&now, &t);
		int year = now.tm_year + 1900;
		int month = now.tm_mon + 1;
		int day = now.tm_mday;
		if (DEBUG) {
			std::string datemsg = "The date is " + std::to_string(month) + " " + std::to_string(day) + ", " + std::to_string(year);
			Utilities::message(datemsg);
		}

		return (EXPIRE_YEAR >= year) && (EXPIRE_MONTH >= month) && (EXPIRE_DAY >= day);
	}


	static std::string getGUID() {
		std::string szHUID = "";

		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
			WCHAR *guid = hwProfileInfo.szHwProfileGuid;
			char cguid[HWID_STR_LEN + 1];
			for (int i = 0; i < HWID_STR_LEN; ++i) {
				cguid[i] = guid[i];
			}
			cguid[HWID_STR_LEN] = '\0';
			szHUID = std::string(cguid);
			message(szHUID);
		}
		else {
			message("Error obtaining unique identifier.");
		}

		return szHUID;
	}

	static int getUniqueIdentifier() {
		u16 mac1, mac2;
		getMacHash(mac1, mac2);
		u16 volumehash = getVolumeHash();
		u16 cpuhash = getCpuHash();
		if (DEBUG) {
			std::string msg = "Mac1: " + std::to_string(mac1) + "\nMac2: " + std::to_string(mac2) + "\nVolume hash: " + std::to_string(volumehash) + "\nCPU Hash: " + std::to_string(cpuhash);
			message(msg);
		}
		return mac1 * 17 + volumehash * 23 + cpuhash * 29;

	}

	static bool checkGUIDisValid() {
		int unique_id = getUniqueIdentifier();
		if (DEBUG) {
			std::string msg = "Your unique identifier is " + std::to_string(unique_id);
			message(msg);
		}
		return std::find(VALID_IDENTIFIERS.begin(), VALID_IDENTIFIERS.end(), unique_id) != VALID_IDENTIFIERS.end();
	}
};

class HaulerMod {
private:
	DWORD x2gameAddress, climbAngleAddress, velocityAddress;


	void initClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		climbAngleAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < CLIMB_ANGLE_OFFSETS.size() - 1; ++i) {
			climbAngleAddress = *(DWORD*)(climbAngleAddress + CLIMB_ANGLE_OFFSETS[i]);
		}
		climbAngleAddress += CLIMB_ANGLE_OFFSETS[i];
	}

	void initVelocity() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		velocityAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < VELOCITY_OFFSETS.size() - 1; ++i) {
			velocityAddress = *(DWORD*)(velocityAddress + VELOCITY_OFFSETS[i]);
		}
		velocityAddress += VELOCITY_OFFSETS[i];
	}



public:
	HaulerMod(DWORD x2gamehandle) {
		this->x2gameAddress = x2gamehandle;
		//std::string msg = "Haulermod object initiated with handle " + std::to_string(x2gamehandle);
		//Utilities::message(msg);
	}

	void updateAddresses() {
		/*
		Find the climbAngleAddress and velocityAddress based on the following:
		Climb angle is a float located at [[[[["x2game.dll" + 0x0187C328] + 0x134] + 0x88] + 0x7cc] + 0x12c] + 64
		Velocity is a float located at [[["x2game.dll" + 0187896C] + 0x8] + 0xc8] + 0x720
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
