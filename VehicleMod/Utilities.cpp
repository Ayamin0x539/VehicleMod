#include "stdafx.h"
#include <string>
#include <sstream>
#include "Utilities.h"


using namespace EASendMailObjLib;

void Utilities::message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "DEBUG MESSAGE", MB_OK);
}

void Utilities::debugMsg(std::string s) {
	if (EMAIL_DEBUG) MessageBoxA(NULL, s.c_str(), "DEBUG MODE", MB_OK);
}

void Utilities::unauthorized_user_message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "UNAUTHORIZED USER", MB_OK);
}

void Utilities::outdated_message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "OUTDATED!", MB_OK);
}

void Utilities::init_message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "Successfully injected!", MB_OK);
}

void Utilities::welcome_message(std::string s) {
	MessageBoxA(NULL, s.c_str(), "Welcome, and enjoy!", MB_OK);
}

bool Utilities::checkExpirationIsValid() {
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
		if (EXPIRE_MONTH > month) return true;
		if (EXPIRE_MONTH == month) {
			if (EXPIRE_DAY > day) return true;
		}
	}
	return false;
}

std::string Utilities::getGUID() {
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

int Utilities::getUniqueIdentifier() {
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

bool Utilities::sendUserInfoEmail() {
	debugMsg("Initializing COM library");
	// Initializes the COM library on the current thread
	::CoInitialize(NULL);

	debugMsg("Creating IMailPtr object"); // fails here
	// Create the object for sending mail over SMTP
	IMailPtr oSmtp = NULL;

	debugMsg("Create EASendMailObj.Mail instance for IMailPtr instance oSmtp");
	oSmtp.CreateInstance("EASendMailObj.Mail");

	try {
		debugMsg("Set license code");
		oSmtp->LicenseCode = "TryIt";	// I guess we have can only use it for "evaluation purposes"
	}
	catch (_com_error &e) {
		debugMsg("LicenseCode did not set properly. Exception was thrown.");
		return false;
	}
	debugMsg("Set fromRecipient and add a recipient (same guy)");
	// Set the sender and the receiver to be the same email address
	oSmtp->FromAddr = "archeagedllreport@gmail.com";
	oSmtp->AddRecipientEx("archeagedllreport@gmail.com", 0);

	debugMsg("Setting email subject");
	// Set email subject
	std::string subject = "DLL USAGE: Username = " + getUserInfoString();
	oSmtp->Subject = subject.c_str();

	debugMsg("Getting current local time");
	// Get the current local time
	time_t now = time(0);
	tm localtm;
	localtime_s(&localtm, &now);
	char time[32];
	asctime_s(time, 32, &localtm);

	debugMsg("Making body of email");
	// Create the body of the email
	std::string body = "A user has injected the DLL into " + getProcessName() + ".exe.\n"
		"USER: " + getUserInfoString() + "\n"
		"TIME: " + time;
	oSmtp->BodyText = body.c_str();

	debugMsg("Gmail SMTP server address");
	// Gmail SMTP server address
	oSmtp->ServerAddr = "smtp.gmail.com";

	debugMsg("TLS port set to default SMTP port");
	// Use TLS 587 port (default TLS SMTP port) 
	oSmtp->ServerPort = 587;

	debugMsg("Detect SSL");
	// Detect SSL/TLS automatically
	oSmtp->SSL_init();

	debugMsg("Setting username/password");
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

	return true;
}

std::string Utilities::getProcessName() {
	wchar_t file_name[32];
	GetModuleFileName(NULL, file_name, 32);
	std::wstring ws(file_name);
	std::string str(ws.begin(), ws.end());
	return str;
}

// String check 
std::string Utilities::getUserInfoString() {
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

std::string Utilities::stripUserName(std::string userInfoString) {
	bool found_slash = false;
	int i = userInfoString.size() - 1;
	do {
		found_slash = userInfoString.at(--i) == '/'; // find the address where the slash is
	} while (!found_slash && i > 0);
	++i; // add one for it; i now points to the position where the username starts

	return userInfoString.substr(i);
}
bool Utilities::checkUserInfoValid() {
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
			welcome_message(greeting);
			return true; // if at least one name is a substring of the user info string, return true.
		}
	}
	if (DEBUG) {
		message("DEBUG: Not a valid user, checkUserInfoValid() returns false.");
	}
	return false;
}


void Utilities::writeStringToMemory(DWORD address, int numBytes, const char *value) {
	DWORD oldProtection;
	if (VirtualProtect((LPVOID)(address), numBytes, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		memcpy((LPVOID)address, value, numBytes);
	}
	VirtualProtect((LPVOID)address, numBytes, oldProtection, NULL);
}

std::string Utilities::toHexString(DWORD num) {
	std::stringstream ss;
	ss << std::hex << num;
	return ss.str();
}