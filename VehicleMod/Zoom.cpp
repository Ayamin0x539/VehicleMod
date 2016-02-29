#include "stdafx.h"
#include "Zoom.h"


DWORD temp_ESI = 0;
const char *ZOOM_MASK = "xxxxxxxxxxxxxxxx";
const char *ZOOM_PATTERN = "\xf3\x0f\x10\x8e\x50\x01\x00\x00\xf3\x0f\x59\x45\xf8\x8b\x50\x0c";
DWORD ZOOM_ADDRESS = 0;
DWORD ZOOM_INSTRUCTION_ADDRESS = 0;
DWORD ZOOM_RETURN_ADDRESS = 0;
float newZoomValue = 200;

// Pattern searching functions
static bool isPattern(const BYTE *pData, const BYTE *byteArray, const char *szMask) {
	for (; *szMask; ++szMask, ++pData, ++byteArray) {
		if (*szMask == 'x' && *pData != *byteArray) {
			return false;
		}
	}
	if ((*szMask) == NULL && DEBUG) {
		Utilities::message("Found the address!");
	}
	return (*szMask) == NULL;
}

static DWORD patternSearch(DWORD dwAddress, DWORD dwLen, BYTE *byteArray, const char *szMask) {
	DWORD oldProtection;

	for (DWORD i = 0; i < dwLen; ++i) {
		if (VirtualProtect((LPVOID)dwAddress, 16, PAGE_EXECUTE_READWRITE, &oldProtection)) {
			if (isPattern((BYTE*)(dwAddress + i), byteArray, szMask)) {
				return (DWORD)(dwAddress + i);
			}
			VirtualProtect((LPVOID)dwAddress, 16, oldProtection, NULL);
		}
		else {
			if (i % 0x1000 == 0 && DEBUG) {
				std::string msg = "Failed to change protection status on the " + std::to_string(i) + " th iteration through memory";
				Utilities::message(msg);
			}
		}
	}
	return 0;
}

// Zoom utility functions, to prepare a few things...
__declspec(naked) void getAddressForZoom(void) {
	__asm {
		push edx
			mov edx, [newZoomValue]
			mov[esi + 0x150], edx
			movss xmm1, [esi + 0x150]
			pop edx
			jmp[ZOOM_RETURN_ADDRESS]
	}
}

void initializeZoomInstructionAddress() { // initializes the pointer to the instruction that writes to zoom
	ZOOM_INSTRUCTION_ADDRESS = patternSearch(START_ADDRESS, DELTA_ADDRESS, (BYTE*)ZOOM_PATTERN, ZOOM_MASK);
}

void detourZoomAndModifyZoomValue() {
	// detours the instruction that writes to zoom in order to pull esi from it
	// add 0x150 to esi and we have our zoom address
	if (DEBUG) {
		Utilities::message("Time to detour the zoom writing instruction...");
	}
	if (ZOOM_INSTRUCTION_ADDRESS) {
		ZOOM_RETURN_ADDRESS = ZOOM_INSTRUCTION_ADDRESS + 0x8; // can't think of a better way to do this...
		if (DEBUG) { 
			std::string msg = "Zoom instruction address found to be " + Utilities::toHexString(ZOOM_INSTRUCTION_ADDRESS); 
			msg += "\nWe will return execution at " + Utilities::toHexString(ZOOM_RETURN_ADDRESS);
			Utilities::message(msg); 
		}
		DWORD oldProtection;
		if (VirtualProtect((LPVOID)ZOOM_INSTRUCTION_ADDRESS, 8, PAGE_EXECUTE_READWRITE, &oldProtection)) {
			*(BYTE*)ZOOM_INSTRUCTION_ADDRESS = 0xE9; // jump to...
			*(DWORD*)(ZOOM_INSTRUCTION_ADDRESS + 1) = ((DWORD)getAddressForZoom - ZOOM_INSTRUCTION_ADDRESS - 5); // detour to code cave
			*(BYTE*)(ZOOM_INSTRUCTION_ADDRESS + 5) = 0x90; // three nops.
			*(BYTE*)(ZOOM_INSTRUCTION_ADDRESS + 6) = 0x90;
			*(BYTE*)(ZOOM_INSTRUCTION_ADDRESS + 7) = 0x90;
		}
		VirtualProtect((LPVOID)ZOOM_INSTRUCTION_ADDRESS, 8, oldProtection, NULL);
	}
	else if (DEBUG) {
		Utilities::message("Zoom Instruction Address was not initialized.");
	}
}
