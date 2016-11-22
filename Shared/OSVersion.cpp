// OSVersion.cpp: implementation of the COSVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OSVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COSVersion::COSVersion()
{

}

COSVersion::~COSVersion()
{

}

CString COSVersion::FormatOSVersion()
{
	CString sVersion;

	if (GetOSVersion() == OSV_LINUX)
	{
		sVersion = _T("Linux (Wine)");
	}
	else
	{
		OSVERSIONINFO vinfo = { sizeof(OSVERSIONINFO) };
		
		if (GetVersionEx(&vinfo))
			sVersion.Format(_T("%lu.%lu"), vinfo.dwMajorVersion, vinfo.dwMinorVersion);
	}
	
	return sVersion;
}

OSVERSION COSVersion::GetOSVersion()
{
	static OSVERSION nVersion = OSV_UNKNOWN;

	if (nVersion == OSV_UNKNOWN) // first time only
	{
		// special: Running on Linux under Wine
		HMODULE hMod = GetModuleHandle(_T("ntdll.dll"));
			
		if (hMod && (GetProcAddress(hMod, "wine_get_version") != NULL))
		{
			nVersion = OSV_LINUX;
			return nVersion;
		}

		// Running natively under Windows
		OSVERSIONINFO vinfo;
		vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		
		BOOL rslt = GetVersionEx(&vinfo);
		
		if (rslt)
		{
			switch (vinfo.dwPlatformId)
			{
			case VER_PLATFORM_WIN32_NT:
				{
					switch (vinfo.dwMajorVersion)
					{
					case 3: // nt351
						ASSERT (0); // not supported
						break;
						
					case 4: // nt4
						nVersion = OSV_NT4;
						break;
						
					case 5: // >= w2k
						{
							switch (vinfo.dwMinorVersion)
							{
							case 0: // w2k
								nVersion = OSV_2K;
								break;
								
							case 1: // xp
								nVersion = OSV_XP;
								break;
								
							case 2: // XP 64-Bit, Server 2003, Server 2003 R2
							default:
								nVersion = OSV_XPP;
								break;
							}
						}
						break;
						
					case 6: // > XP
						{
							switch (vinfo.dwMinorVersion)
							{
							case 0: // Vista, Server 2008
								nVersion = OSV_VISTA;
								break;
								
							case 1: // W7, Server 2008 R2
								nVersion = OSV_WIN7;
								break;
								
							case 2: // W8, Server 2012
								nVersion = OSV_WIN8;
								break;
								
							case 3: // W8.1, Server 2012 R2, W10
								{
									// Check registry for Windows 10
									HKEY hKey = NULL;
									LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
																_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 
																REG_OPTION_NON_VOLATILE, 
																KEY_READ, 
																&hKey);

									if (lResult == ERROR_SUCCESS)
									{
										DWORD dwType = 0;
										DWORD dwSize = sizeof(DWORD);
										DWORD dwMajorVer = 0;
											
										lResult = RegQueryValueEx(hKey, 
																	_T("CurrentMajorVersionNumber"), 
																	NULL, 
																	&dwType, 
																	(BYTE*)&dwMajorVer, 
																	&dwSize);
										
										if ((lResult = ERROR_SUCCESS) && (dwMajorVer >= 10))
										{
											nVersion = OSV_WIN10;
											break;
										}
									}
								}
								// else
								nVersion = OSV_WIN81;
								break;
								
							default: // > w8
								nVersion = OSV_WIN81;
								break;
							}
						}
						break;
						
					default: // > w8
						nVersion = OSV_WIN81;
						break;
					}
				}
				break;
				
			case VER_PLATFORM_WIN32_WINDOWS:
				{
					ASSERT (vinfo.dwMajorVersion == 4);
					
					switch (vinfo.dwMinorVersion)
					{
					case 0: 
						nVersion = OSV_95;
						break;
						
					case 10:
						nVersion = OSV_98;
						break;
						
					case 90:
						nVersion = OSV_ME;
						break;
						
					default:
						ASSERT (0);
						break;
					}

				}
				break;
				
			default:
				ASSERT (0);
				break;
			}
		}
	}

	return nVersion;
}
