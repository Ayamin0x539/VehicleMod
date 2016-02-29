// VehicleMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "constants.h"
#include "Utilities.h"


class HaulerMod {
private:
	DWORD x2gameAddress, 
		climbAngleAddress, velocityAddress,
		redClimbAngleAddress, redVelocityAddress;

	// Regular
	void initClimbAngle() {
		if (!x2gameAddress) {
			MessageBoxA(NULL, "x2gameAddress not initialized yet.", "ERROR", MB_OK);
			return;
		}
		climbAngleAddress = 0;
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
		velocityAddress = 0;
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
		redClimbAngleAddress = 0;
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
		redVelocityAddress = 0;
		unsigned int i = 0;
		for (; i < RED_VELOCITY_OFFSETS.size() - 1; ++i) {
			redVelocityAddress = *(DWORD*)(redVelocityAddress + RED_VELOCITY_OFFSETS[i]);
		}
		redVelocityAddress += RED_VELOCITY_OFFSETS[i];
	}



public:
	HaulerMod(DWORD x2gamehandle) {
		this->x2gameAddress = x2gamehandle;
	}

	void updateAddresses() {
		// Regular
		initClimbAngle();
		initVelocity();

		// Red
		initRedClimbAngle();
		initRedVelocity();
	}

	void modClimbAngle(DWORD address) { // sets the float at the address to 0.0
		Utilities::writeStringToMemory(address, 4, HAULER_CLIMB_ANGLE_MOD_VALUE);
	}
	void disableClimbAngle(DWORD address) { // sets the float at the address to 45.0
		Utilities::writeStringToMemory(address, 4, HAULER_CLIMB_ANGLE_RESET_VALUE);
	}
	void modVelocity(DWORD address) { // sets the float at the address to 13.0
		Utilities::writeStringToMemory(address, 4, HAULER_VELOCITY_MOD_VALUE);
	}
	void disableVelocity(DWORD address) { // sets the float at the address to 5.0
		Utilities::writeStringToMemory(address, 4, HAULER_VELOCITY_RESET_VALUE);
	}
	void enableHauler() {


		modVelocity(velocityAddress);
		modVelocity(redVelocityAddress);

		modClimbAngle(climbAngleAddress);
		modClimbAngle(redClimbAngleAddress);

		if (DEBUG || PUBLIC_DEBUG)
			MessageBoxA(NULL, "Hauler hacks enabled.", "[ENABLED]", MB_OK);
	}

	void disableHauler() {
		disableVelocity(velocityAddress);
		disableVelocity(redVelocityAddress);

		disableClimbAngle(climbAngleAddress);
		disableClimbAngle(redClimbAngleAddress);

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
		if (redVelocityAddress)
			msg += "\nred velocity address: " + std::to_string(redVelocityAddress);
		if (redClimbAngleAddress)
			msg += "\nred climb angle address " + std::to_string(redClimbAngleAddress);
		MessageBoxA(NULL, msg.c_str(), "Address Info", MB_OK);
	}
};

