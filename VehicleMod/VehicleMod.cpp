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

	static void unauthorized_user_message(std::string s) {
		MessageBoxA(NULL, s.c_str(), "UNAUTHORIZED USER", MB_OK);
	}
	
	static void outdated_message(std::string s) {
		MessageBoxA(NULL, s.c_str(), "OUTDATED!", MB_OK);
	}

	static void init_message(std::string s) {
		MessageBoxA(NULL, s.c_str(), "Successfully injected!", MB_OK);
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
		if (EXPIRE_YEAR > year) return true;
		if (EXPIRE_YEAR == year) {
			if (EXPIRE_MONTH >= month && EXPIRE_DAY >= day) {
				return true;
			}
		}
		return false;
	}

	static std::string getGUID() {
		std::string szHUID = "";

		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
			WCHAR *guid = hwProfileInfo.szHwProfileGuid;
			char cguid[HWID_STR_LEN + 1];
			for (unsigned int i = 0; i < HWID_STR_LEN; ++i) {
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
		if (UNIQ_ID_DEBUG) {
			std::string msg = "Mac1: " + std::to_string(mac1) + "\nMac2: " + std::to_string(mac2) + "\nVolume hash: " + std::to_string(volumehash) + "\nCPU Hash: " + std::to_string(cpuhash);
			message(msg);
		}
		//return mac1 * 17 + volumehash * 23 + cpuhash * 29;
		return volumehash * cpuhash;
	}

	static bool checkGUIDisValid() {
		int unique_id = getUniqueIdentifier();
		if (UNIQ_ID_DEBUG) {
			std::string msg = "Your unique identifier is " + std::to_string(unique_id);
			message(msg);
		}
		return std::find(VALID_IDENTIFIERS.begin(), VALID_IDENTIFIERS.end(), unique_id) != VALID_IDENTIFIERS.end();
	}

	// String check 
	static std::string getUserInfoString() {
		char user_info_buffer[100];
		DWORD oldPermissions;
		if (VirtualProtect((LPVOID)(USER_INFO_ADDRESS), 100, PAGE_EXECUTE_READ, &oldPermissions)) {
			for (unsigned int i = 0; i < 100; ++i) {
				user_info_buffer[i] = *(BYTE*)(USER_INFO_ADDRESS + i);
			}
		}
		std::string user_info_string(user_info_buffer);
		return user_info_string;
	}

	static bool checkUserInfoValid() {
		std::string userInfoString = getUserInfoString();
		if (userInfoString == "") {
			message("You injected too early.\nMake sure to inject after your character loads into the game.");
			return false;
		}
		if (DEBUG) {
			std::string msg = "The user info string is \n" + userInfoString;
			message(msg);
		}
		for (unsigned int i = 0; i < VALID_CHARACTERS.size(); ++i) {
			if (userInfoString.find(VALID_CHARACTERS[i]) != std::string::npos) {
				if (DEBUG) {
					message("DEBUG: Valid user. checkUserInfoValid() returns true.");
				}
				return true; // if at least one name is a substring of the user info string, return true.
			}
		}
		if (DEBUG) {
			message("DEBUG: Not a valid user, checkUserInfoValid() returns false.");
		}
		return false;
	}
};

class HaulerMod {
private:
	DWORD x2gameAddress,
		climbAngleAddress, velocityAddress,
		redClimbAngleAddress, redVelocityAddress,
		rudolphClimbAngleAddress, rudolphVelocityAddress;

	// Regular
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

	// Red
	void initRedClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		redClimbAngleAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < RED_CLIMB_ANGLE_OFFSETS.size() - 1; ++i) {
			redClimbAngleAddress = *(DWORD*)(redClimbAngleAddress + RED_CLIMB_ANGLE_OFFSETS[i]);
		}
		redClimbAngleAddress += RED_CLIMB_ANGLE_OFFSETS[i];
	}

	void initRedVelocity() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		redVelocityAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < RED_VELOCITY_OFFSETS.size() - 1; ++i) {
			redVelocityAddress = *(DWORD*)(redVelocityAddress + RED_VELOCITY_OFFSETS[i]);
		}
		redVelocityAddress += RED_VELOCITY_OFFSETS[i];
	}

	// Rudolph
	void initRudolphClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		rudolphClimbAngleAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < RUDOLPH_CLIMB_ANGLE_OFFSETS.size() - 1; ++i) {
			rudolphClimbAngleAddress = *(DWORD*)(rudolphClimbAngleAddress + RUDOLPH_CLIMB_ANGLE_OFFSETS[i]);
		}
		rudolphClimbAngleAddress += RUDOLPH_CLIMB_ANGLE_OFFSETS[i];
	}

	void initRudolphVelocity() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		rudolphVelocityAddress = x2gameAddress;
		unsigned int i = 0;
		for (; i < RUDOLPH_VELOCITY_OFFSETS.size() - 1; ++i) {
			rudolphVelocityAddress = *(DWORD*)(rudolphVelocityAddress + RUDOLPH_VELOCITY_OFFSETS[i]);
		}
		rudolphVelocityAddress += RUDOLPH_VELOCITY_OFFSETS[i];
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

		// Regular
		initClimbAngle();
		initVelocity();
		// Red
		initRedClimbAngle();
		initRedVelocity();
		// Rudolph
		initRudolphClimbAngle();
		initRudolphVelocity();
	}

	void modClimbAngle(DWORD address) { // sets the float at the address to 0.0
		DWORD climbstore;
		if (PUBLIC_DEBUG) {
			Utilities::message("Attempting virtual protect on climb angle...");
		}
		if (VirtualProtect((LPVOID)(address), 4, PAGE_EXECUTE_READWRITE, &climbstore)) {
			*(BYTE*)(address) = 0x00;
			*(BYTE*)(address + 1) = 0x00;
			*(BYTE*)(address + 2) = 0x00;
			*(BYTE*)(address + 3) = 0x00;
		}
	}
	void disableClimbAngle(DWORD address) { // sets the float at the address to 45.0
		DWORD climbstore;
		if (VirtualProtect((LPVOID)(address), 4, PAGE_EXECUTE_READWRITE, &climbstore)) {
			*(BYTE*)(address) = 0x00;
			*(BYTE*)(address + 1) = 0x00;
			*(BYTE*)(address + 2) = 0x34;
			*(BYTE*)(address + 3) = 0x42;
		}
	}
	void modVelocity(DWORD address) { // sets the float at the address to 13.0
		DWORD velocitystore;
		if (PUBLIC_DEBUG) {
			Utilities::message("Attempting virtual protect on velocity...");
		}
		if (VirtualProtect((LPVOID)(address), 4, PAGE_EXECUTE_READWRITE, &velocitystore)) {
			*(BYTE*)(address) = 0x00;
			*(BYTE*)(address + 1) = 0x00;
			*(BYTE*)(address + 2) = 0x50;
			*(BYTE*)(address + 3) = 0x41;
		}
	}
	void disableVelocity(DWORD address) { // sets the float at the address to 5.0
		DWORD velocitystore;
		if (VirtualProtect((LPVOID)(address), 4, PAGE_EXECUTE_READWRITE, &velocitystore)) {
			*(BYTE*)(address) = 0x00;
			*(BYTE*)(address + 1) = 0x00;
			*(BYTE*)(address + 2) = 0xa0;
			*(BYTE*)(address + 3) = 0x40;
		}
	}
	void enableHauler() {
		/*
		Change hauler climb angle from 45.0 to 90.0, and velocity from 5.0 to 13.0
		At climbAngleAddress: 00 00 34 42 should be changed to 00 00 B4 42
		At velocityAddress: 00 00 A0 40 should be changed to 00 00 50 41
		*/
		modVelocity(velocityAddress);
		modVelocity(redVelocityAddress);
		modVelocity(rudolphVelocityAddress);

		modClimbAngle(climbAngleAddress);
		modClimbAngle(redClimbAngleAddress);
		modClimbAngle(rudolphClimbAngleAddress);
		if (DEBUG || PUBLIC_DEBUG)
			MessageBoxA(NULL, "Hauler hacks enabled.", "[ENABLED]", MB_OK);
	}

	void disableHauler() {
		disableVelocity(velocityAddress);
		disableVelocity(redVelocityAddress);
		disableVelocity(rudolphVelocityAddress);

		disableClimbAngle(climbAngleAddress);
		disableClimbAngle(redClimbAngleAddress);
		disableClimbAngle(rudolphClimbAngleAddress);
		if (DEBUG || PUBLIC_DEBUG)
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

	DWORD getRedClimbAngleAddress() {
		return this->redClimbAngleAddress;
	}

	DWORD getRedVelocityAddress() {
		return this->redVelocityAddress;
	}
	
	DWORD getRudolphClimbAngleAddress() {
		return this->rudolphClimbAngleAddress;
	}

	DWORD getRudolphVelocityAddress() {
		return this->rudolphVelocityAddress;
	}
	void displayAddresses() {
		std::string msg = "---";
		if (x2gameAddress)
			msg += "\nx2game.dll address: " + std::to_string(x2gameAddress);
		if (velocityAddress)
			msg += "\nvelocity address: " + std::to_string(velocityAddress);
		if (climbAngleAddress)
			msg += "\nclimb angle address " + std::to_string(climbAngleAddress);
		if (redVelocityAddress)
			msg += "\nred velocity address: " + std::to_string(redVelocityAddress);
		if (redClimbAngleAddress)
			msg += "\nred climb angle address: " + std::to_string(redClimbAngleAddress);
		if (rudolphVelocityAddress)
			msg += "\n rudolph velocity address: " + std::to_string(rudolphVelocityAddress);
		if (rudolphClimbAngleAddress)
			msg += "\nrudolph climb angle addres: " + std::to_string(rudolphClimbAngleAddress);
		MessageBoxA(NULL, msg.c_str(), "Address Info", MB_OK);
	}
};
