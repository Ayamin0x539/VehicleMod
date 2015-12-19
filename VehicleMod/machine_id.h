#pragma comment(lib, "iphlpapi.lib")
#include "stdafx.h"

u16 hashMacAddress(PIP_ADAPTER_INFO info);
void getMacHash(u16& mac, u16& mac2);
u16 getVolumeHash();
u16 getCpuHash();
TCHAR *getMachineName();