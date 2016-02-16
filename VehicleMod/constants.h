#pragma once

#include "stdafx.h"
#define DEBUG 0
#define UNIQ_ID_DEBUG 0
#define PUBLIC_DEBUG 0
#define RESTRICTED_VERSION 0
// "Good until" this day. (Will still work on this day.)
#define EXPIRE_MONTH 2
#define EXPIRE_DAY 19
#define EXPIRE_YEAR 2016

#define HWID_STR_LEN 39


const std::string VERSION_NUMBER = "2.16";

const std::vector<DWORD> CLIMB_ANGLE_OFFSETS = { 0x018E8018, 0x2b8, 0x6e4, 0x4 };
const std::vector<DWORD> VELOCITY_OFFSETS = { 0x018E8018, 0x2c4, 0x444 };

// const std::vector<int> VALID_IDENTIFIERS = { 861564013, 233328561, 1602667984, 1252689912 }; // deprecated
const std::vector<std::string> VALID_CHARACTERS = { "Ollo/Samshaq", 
													//"Ollo/Wafi", 
													//"Ollo/Wafii", 
													"Ollo/Anastacio", 
													"Ollo/Tryzub", 
													"Ollo/Chienthan", 
													"Ollo/Itsjustwendi",
													"Ollo/Piki"
};
const std::string INIT_MESSAGE = "\
								 Ultimate Hauler Mod DLL Injected.\n\
								 AA 2.5 Can't Keep Me Out!!!\n\
									New efficient version.";
const std::string START_MESSAGE = "\
									Version " + VERSION_NUMBER + ".\n\
									Now working.\n\
									Numpad 8 to enable.\n\
									Numpad 7 to disable.\n\
									Or middle click to enable/disable.\n";
const std::string STR_OUTDATED_MESSAGE = "HACK OUTDATED.\n\nIt's been patched.\n\nExiting...";
const std::string STR_UNAUTHORIZED_USER_MESSAGE = "Sorry, you are not an authorized user of this hack.\n\nThe dll will now close.\n\nExiting...";

// All are little endian
const char HAULER_CLIMB_ANGLE_MOD_VALUE[] = "\x00\x00\x00\x00"; // 0 in float
const char HAULER_CLIMB_ANGLE_RESET_VALUE[] = "\x00\x00\x34\x42"; // 45.0 in float
const char HAULER_VELOCITY_MOD_VALUE[] = "\x00\x00\x50\x41"; // 13.0 in float
const char HAULER_VELOCITY_RESET_VALUE[] = "\x00\x00\xa0\x40"; // 5.0 in float
const DWORD USER_INFO_ADDRESS = 0x3A058450;