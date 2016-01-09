#pragma once

#include "stdafx.h"
#define DEBUG 0
#define UNIQ_ID_DEBUG 0
#define PUBLIC_DEBUG 0
#define RESTRICTED_VERSION 0
// "Good until" this day. (Will still work on this day.)
#define EXPIRE_MONTH 1
#define EXPIRE_DAY 8
#define EXPIRE_YEAR 2016

#define HWID_STR_LEN 39


const std::string VERSION_NUMBER = "1.29";

const std::vector<DWORD> CLIMB_ANGLE_OFFSETS = { 0x0187B990, 0x8, 0xd4, 0x408, 0x4 };
const std::vector<DWORD> VELOCITY_OFFSETS = { 0x018832B4, 0x8, 0x488, 0x64, 0x65c, 0x444 };
const std::vector<DWORD> RED_CLIMB_ANGLE_OFFSETS = { 0x0187B990, 0x8, 0xcc, 0x4fc, 0xc4 };
const std::vector<DWORD> RED_VELOCITY_OFFSETS = { 0x0187B990, 0x8, 0xe8, 0x62c };
const std::vector<DWORD> RUDOLPH_CLIMB_ANGLE_OFFSETS = { 0x0187B990, 0x8, 0x1c0, 0x6e4, 0x4 };
const std::vector<DWORD> RUDOLPH_VELOCITY_OFFSETS = { 0x0187B990, 0x14, 0x8, 0x74 };

// Me, Samshaq, Samshaq2 Naga, Babbar
//const std::vector<int> VALID_IDENTIFIERS = { 1663625, 1166522, 1873926, 2179965, 1859266 };
const std::vector<int> VALID_IDENTIFIERS = { 861564013, 233328561, 1602667984, 1252689912 };
const std::vector<std::string> VALID_CHARACTERS = { "Ollo/Cse", "Ollo/Babbar", "Ollo/Spybar", "Ollo/Samshaq" };
const std::string INIT_MESSAGE = "Ultimate Hauler Mod DLL Injected.";
const std::string START_MESSAGE = "Version " + VERSION_NUMBER + ".\nNow working.\nNumpad 8 to enable.\nNumpad 7 to disable.\nOr middle click to enable/disable.";
const std::string STR_OUTDATED_MESSAGE = "HACK OUTDATED.\n\nIt's been patched.\n\nExiting...";
const std::string STR_UNAUTHORIZED_USER_MESSAGE = "Sorry, you are not an authorized user of this hack.\n\nThe dll will now close.\n\nExiting...";

const DWORD USER_INFO_ADDRESS = 0x39fea118;