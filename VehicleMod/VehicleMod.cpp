// VehicleMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "constants.h"
#include "machine_id.h"
#include "easendmailobj.tlh"
#include <tchar.h>
using namespace EASendMailObjLib;

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

	static void sendUserInfoEmail() {
		// Initializes the COM library on the current thread
		::CoInitialize(NULL);

		// Create the object for sending mail over SMTP
		IMailPtr oSmtp = NULL;
		oSmtp.CreateInstance("EASendMailObj.Mail");
		oSmtp->LicenseCode = "TryIt";	// I guess we have can only use it for "evaluation purposes"

										// Set the sender and the receiver to be the same email address
		oSmtp->FromAddr = "archeagedllreport@gmail.com";
		oSmtp->AddRecipientEx("archeagedllreport@gmail.com", 0);

		// Set email subject
		std::string subject = "DLL USAGE: Username = " + getUserInfoString();
		oSmtp->Subject = subject.c_str();

		// Get the current local time
		time_t now = time(0);
		tm localtm;
		localtime_s(&localtm, &now);
		char time[32];
		asctime_s(time, 32, &localtm);

		// Create the body of the email
		std::string body = "A user has injected the DLL into " + getProcessName() + ".exe.\n"
			"USER: " + getUserInfoString() + "\n"
			"TIME: " + time;
		oSmtp->BodyText = body.c_str();

		// Gmail SMTP server address
		oSmtp->ServerAddr = "smtp.gmail.com";

		// Use TLS 587 port (default TLS SMTP port) 
		oSmtp->ServerPort = 587;

		// Detect SSL/TLS automatically
		oSmtp->SSL_init();

		// Username and password for acheageDLLreport email
		oSmtp->UserName = "archeagedllreport@gmail.com";
		oSmtp->Password = "archeagereport";

		if (oSmtp->SendMail() == 0 && DEBUG)
		{
			message("Email sent successfully.");
		}
		else if (DEBUG)
		{
			// Get the error
			_bstr_t error = oSmtp->GetLastErrDescription();
			const char* buf = error;
			int bstrlen = error.length();
			std::string errorString(buf ? buf : "", bstrlen);
			message("Email not sent. Error: " + errorString);
		}

		if (oSmtp != NULL)
			oSmtp.Release();
	}

	static std::string getProcessName() {
		wchar_t file_name[32];
		GetModuleFileName(NULL, file_name, 32);
		std::wstring ws(file_name);
		std::string str(ws.begin(), ws.end());
		return str;
	}


	/* // Deprecated
	static bool checkGUIDisValid() {
		int unique_id = getUniqueIdentifier();
		if (UNIQ_ID_DEBUG) {
			std::string msg = "Your unique identifier is " + std::to_string(unique_id);
			message(msg);
		}
		return std::find(VALID_IDENTIFIERS.begin(), VALID_IDENTIFIERS.end(), unique_id) != VALID_IDENTIFIERS.end();
	}
	*/

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

	static std::string stripUserName(std::string userInfoString) {
		bool found_slash = false;
		int i = userInfoString.size() - 1;
		do {
			found_slash = userInfoString.at(--i) == '/'; // find the address where the slash is
		} while (!found_slash && i > 0);
		++i; // add one for it; i now points to the position where the username starts

		return userInfoString.substr(i);
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
				std::string username = stripUserName(userInfoString);
				std::string greeting = "Hello, " + username + ". You are a valid user of this hack.";
				message(greeting);
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
		climbAngleAddress, velocityAddress;

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
	}

	void writeStringToMemory(DWORD address, int numBytes, const char *value) {
		DWORD oldProtection;
		if (VirtualProtect((LPVOID)(address), numBytes, PAGE_EXECUTE_READWRITE, &oldProtection)) {
			memcpy((LPVOID)address, value, numBytes);
		}
		VirtualProtect((LPVOID)address, numBytes, oldProtection, NULL);
	}

	void modClimbAngle(DWORD address) { // sets the float at the address to 0.0
		writeStringToMemory(address, 4, HAULER_CLIMB_ANGLE_MOD_VALUE);
	}
	void disableClimbAngle(DWORD address) { // sets the float at the address to 45.0
		writeStringToMemory(address, 4, HAULER_CLIMB_ANGLE_RESET_VALUE);
	}
	void modVelocity(DWORD address) { // sets the float at the address to 13.0
		writeStringToMemory(address, 4, HAULER_VELOCITY_MOD_VALUE);
	}
	void disableVelocity(DWORD address) { // sets the float at the address to 5.0
		writeStringToMemory(address, 4, HAULER_VELOCITY_RESET_VALUE);
	}
	void enableHauler() {
		/*
		Change hauler climb angle from 45.0 to 90.0, and velocity from 5.0 to 13.0
		At climbAngleAddress: 00 00 34 42 should be changed to 00 00 B4 42
		At velocityAddress: 00 00 A0 40 should be changed to 00 00 50 41
		*/
		modVelocity(velocityAddress);

		modClimbAngle(climbAngleAddress);
		if (DEBUG || PUBLIC_DEBUG)
			MessageBoxA(NULL, "Hauler hacks enabled.", "[ENABLED]", MB_OK);
	}

	void disableHauler() {
		disableVelocity(velocityAddress);

		disableClimbAngle(climbAngleAddress);
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
