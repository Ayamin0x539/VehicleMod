#include "stdafx.h"
#define DEBUG 0
// "Good until" this day. (Will still work on this day.)
#define EXPIRE_MONTH 12
#define EXPIRE_DAY 29
#define EXPIRE_YEAR 2015
// Users
#define NAGA 1
#define BABBAR 1
#define CASS 1


#define HWID_STR_LEN 39

const std::string VERSION_NUMBER = "1.10";

const std::vector<DWORD> CLIMB_ANGLE_OFFSETS = { 0x0187B990, 0x8, 0xd4, 0x408, 0x4 };
const std::vector<DWORD> VELOCITY_OFFSETS = { 0x018832B4, 0x8, 0x488, 0x64, 0x65c, 0x444 };

// Me, Cass, Naga, Babbar
const std::vector<int> VALID_IDENTIFIERS = { 1663625, 2214730, 2179965, 1859266 };
//const std::vector<int> VALID_IDENTIFIERS = { 2682386 }; // Me Only
//const std::vector<int> VALID_IDENTIFIERS = { 2813448 }; // Cass Only
//const std::vector<int> VALID_IDENTIFIERS = { 3324259 }; // Naga Only
//const std::vector<int> VALID_IDENTIFIERS = { 3021610 }; // Babbar Only
const std::string START_MESSAGE = "Version " + VERSION_NUMBER + ".\nNow working.\nNumpad 8 to enable.\nNumpad 7 to disable.";