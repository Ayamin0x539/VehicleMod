#pragma once

#include "stdafx.h"
#include "constants.h"
#include "Utilities.h"

// Pattern searching functions
static bool isPattern(const BYTE *pData, const BYTE *byteArray, const char *szMask);
static DWORD patternSearch(DWORD dwAddress, DWORD dwLen, BYTE *byteArray, const char *szMask);
// Zoom utility functions, to prepare a few things...
static void getAddressForZoom(void);
void initializeZoomInstructionAddress();
void detourZoomAndModifyZoomValue();
