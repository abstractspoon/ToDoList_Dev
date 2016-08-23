#include "StdAfx.h"
#include "RegUtil.h"

const static int nMaxValueNameSize = MAX_PATH;
const static int nMaxValueValueSize = 4096;
const static int nMaxClassSize = MAX_PATH;
const static int nMaxKeyNameSize = MAX_PATH;

static BOOL CopyKeys(HKEY hkFrom, HKEY hkTo) 
{
	TCHAR lpstrName[nMaxKeyNameSize];
	TCHAR lpstrClass[nMaxClassSize];

	for (int i = 0;;i++) {
		DWORD nNameSize = nMaxKeyNameSize;
		DWORD nClassSize = nMaxClassSize;
		LONG res = ::RegEnumKeyEx(hkFrom, i, lpstrName, &nNameSize, 0, lpstrClass, &nClassSize, 0);
		if (ERROR_NO_MORE_ITEMS == res) {
			break;
		}
		HKEY hkNew;
		res = ::RegCreateKeyEx(hkTo, lpstrName, 0, lpstrClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hkNew, 0); ;
		if (ERROR_SUCCESS != res) {
			return FALSE;		
		}
		::RegCloseKey(hkNew);
		BOOL bRes = CopyRegistryKey(hkFrom, lpstrName, hkTo, lpstrName);
		if (! bRes) {
			return FALSE;		
		}
	}

	return TRUE;
}

static BOOL CopyValues(HKEY hkFrom, HKEY hkTo) 
{
	TCHAR lpstrName[nMaxValueNameSize];
	BYTE pValue[nMaxValueValueSize];

	for (int i = 0;;i++) {
		DWORD nType;
		DWORD nNameSize = nMaxValueNameSize;
		DWORD nValueSize = nMaxValueValueSize;
		LONG res = ::RegEnumValue(hkFrom, i, lpstrName, &nNameSize, 0, &nType, pValue, &nValueSize);
		if (ERROR_NO_MORE_ITEMS == res) {
			break;
		}
		res = ::RegSetValueEx(hkTo, lpstrName, 0, nType, pValue, nValueSize);
		if (ERROR_SUCCESS != res) {
			return FALSE;		
		}
	}

	return TRUE;
}

BOOL CopyRegistryKey(HKEY hkRootFrom, const CString& strFromPath, HKEY hkRootTo, const CString& strToPath) 
{
	HKEY hkFrom;
	LONG res = ::RegOpenKeyEx(hkRootFrom, strFromPath, 0, KEY_READ, &hkFrom);
	if (ERROR_SUCCESS != res) {
		return FALSE;	
	}
	HKEY hkTo;
	res = ::RegCreateKeyEx(hkRootTo, strToPath, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkTo, 0);
	if (ERROR_SUCCESS != res) {
		::RegCloseKey(hkFrom);
		return FALSE;	
	}
	BOOL bRes = CopyKeys(hkFrom, hkTo) && CopyValues(hkFrom, hkTo);

	::RegCloseKey(hkFrom);
	::RegCloseKey(hkTo);

	return bRes;
}
