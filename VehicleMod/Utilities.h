#pragma once

#include "constants.h"
#include "machine_id.h"
#include "easendmailobj.tlh"
#include <tchar.h>

class Utilities
{
public:
	Utilities();


	static void message(std::string);
	static void debugMsg(std::string);
	static void unauthorized_user_message(std::string);
	static void outdated_message(std::string);
	static void init_message(std::string);
	static void welcome_message(std::string);
	static bool checkExpirationIsValid();
	static std::string getGUID();
	static int getUniqueIdentifier();
	static bool sendUserInfoEmail();
	static std::string getProcessName();
	static std::string getUserInfoString();
	static std::string stripUserName(std::string);
	static bool checkUserInfoValid();
	static void writeStringToMemory(DWORD address, int numBytes, const char *value);
	static std::string toHexString(DWORD);
};

