#pragma once

#include "stdafx.h"
#define DEBUG 0
#define UNIQ_ID_DEBUG 0
#define PUBLIC_DEBUG 0
#define RESTRICTED_VERSION 0
#define EMAIL_DEBUG 0
// "Good until" this day. (Will still work on this day.)
#define EXPIRE_MONTH 3
#define EXPIRE_DAY 1
#define EXPIRE_YEAR 2016

#define HWID_STR_LEN 39


/* These need to be updated after every patch. */
const std::string VERSION_NUMBER = "3.00d";

const DWORD USER_INFO_ADDRESS = 0x3A05C708;

const std::vector<DWORD> CLIMB_ANGLE_OFFSETS = { 0x3A8DE218, 0x2d4, 0x38, 0x4 };
const std::vector<DWORD> VELOCITY_OFFSETS = { 0x3A8DE218, 0x2d4, 0x74 };

const std::vector<DWORD> RED_CLIMB_ANGLE_OFFSETS = { 0x3A8DE218, 0x2f0, 0x38, 0x4 };
const std::vector<DWORD> RED_VELOCITY_OFFSETS = { 0x3A8DE218, 0x2f0, 0x74 };

/* ------------------------------------------- */

// const std::vector<int> VALID_IDENTIFIERS = { 861564013, 233328561, 1602667984, 1252689912 }; // deprecated

// Valid users
const std::vector<std::string> VALID_CHARACTERS = { //"Ollo/Samshaq", 
													//"Ollo/Wafi", 
													//"Ollo/Wafii", 
													"Ollo/Anastacio", 
													"Ollo/Tryzub", 
													//"Ollo/Chienthan", 
													"Ollo/Itsjustwendi",
													"Ollo/Piki", 
													"Ollo/Cse"
};

// Strings
const std::string INIT_MESSAGE = "\
								 Ultimate Hauler Mod DLL Injected.\n\
								 AA 2.5 Can't Keep Me Out!!!\n\
								 Now with zoom feature!\n\
								 (Which took me 12 hours to implement T_T)";
const std::string START_MESSAGE = "\
									Version " + VERSION_NUMBER + ".\n\
									Now working.\n\
									Numpad 8 to enable.\n\
									Numpad 7 to disable.\n\
									Or middle click to enable/disable.\n\
									Zoom is enabled automatically.";
const std::string STR_OUTDATED_MESSAGE = "HACK OUTDATED.\n\nIt's been patched.\n\nExiting...";
const std::string STR_UNAUTHORIZED_USER_MESSAGE = "Sorry, you are not an authorized user of this hack.\n\nThe dll will now close.\n\nExiting...";

// All are little endian
const char HAULER_CLIMB_ANGLE_MOD_VALUE[] = "\x00\x00\x00\x00"; // 0 in float
const char HAULER_CLIMB_ANGLE_RESET_VALUE[] = "\x00\x00\x34\x42"; // 45.0 in float
const char HAULER_VELOCITY_MOD_VALUE[] = "\x00\x00\x50\x41"; // 13.0 in float
const char HAULER_VELOCITY_RESET_VALUE[] = "\x00\x00\xa0\x40"; // 5.0 in float
const char ZOOM_VALUE_TWO_HUNDRED[] = "\x00\x00\x48\x43"; // 200.0 in float

/* For pattern search. */
const DWORD START_ADDRESS = 0x392D0000;
const DWORD DELTA_ADDRESS = 0x10000;
extern const char *ZOOM_PATTERN;
extern const char *ZOOM_MASK;



// Zoom
extern DWORD ZOOM_ADDRESS;
extern DWORD ZOOM_INSTRUCTION_ADDRESS;
extern DWORD ZOOM_RETURN_ADDRESS; // for zoom codecave. can't think of a better way to do this...
// maybe I can push this onto the stack before calling the naked function?! we'll try it after I get this to work with the global.