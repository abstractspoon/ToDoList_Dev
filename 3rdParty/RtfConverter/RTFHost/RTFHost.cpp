// RTFHost.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "wtypes.h"


int _tmain(int argc, _TCHAR* argv[])
{
  HMODULE hMod = LoadLibrary(L"rtf2htmlbridge.dll");

  typedef int (*PFN2)(const char*, const char*, const char*, 
					  const char*, const char*, const char*, 
					  const char*, const char*, const char*, 
					  const char*, const char*);

  PFN2 fn2 = (PFN2)GetProcAddress(hMod, "fnRtf2Html");
  int nRes = fn2("dave", "frank", "", "", "", "", "", "", "", "", "");

  return 0;
}

