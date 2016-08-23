// VersionInfo.cpp : implementation file
//

#include "stdafx.h"
#include "VersionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct STranslation
{
	WORD wLangID;
	WORD wCharSet;
};

//_strDLLFilename should be just the filename, with no path. e.g. "blah.dll"
//returns the file-version with product-version appended
//if the file-version is in the "1, 1, 0, 2" format, returns that part in A.B.CC.DDD format
//(however skips part DDD if it's zero, and also skips part CC if both CC and DDD are zero)
//e,g if file-version is "1, 0, 3, 0" and product-version is "beta 2", returns "1.0.03 beta 2"
CString CVersionInfo::GetVersion(const CString& strDLLFilename)
{
	CString sFileVer, sProductVer;

	if (GetVersion(strDLLFilename, sFileVer, sProductVer))
		return ReformatVersionString(sFileVer, sProductVer);

	// else
	return _T("");
}

CString CVersionInfo::GetFileVersion(const CString& strDLLFilename)
{
	CString sFileVer, sProductVer;

	if (GetVersion(strDLLFilename, sFileVer, sProductVer))
		return sFileVer;

	// else
	return _T("");
}

CString CVersionInfo::GetProductVersion(const CString& strDLLFilename)
{
	CString sFileVer, sProductVer;

	if (GetVersion(strDLLFilename, sFileVer, sProductVer))
		return sProductVer;

	// else
	return _T("");
}

BOOL CVersionInfo::GetVersion(const CString& strDLLFilename, CString& sFileVer, CString& sProductVer)
{
	sFileVer.Empty();
	sProductVer.Empty();

	CString strVersion;
	DWORD dwUnused = 0;

	DWORD dwSize = GetFileVersionInfoSize((LPTSTR)(LPCTSTR)strDLLFilename, &dwUnused);

	if (dwSize > 0)
	{
		LPBYTE lpBuffer = new BYTE[dwSize];
		GetFileVersionInfo((LPTSTR)(LPCTSTR)strDLLFilename, 0, dwSize, lpBuffer);

		LPVOID lpvi;
		UINT nSize = 0;
		if (VerQueryValue(lpBuffer, _T("\\VarFileInfo\\Translation"), &lpvi, &nSize) && (nSize >= 4))
		{
			struct STranslation sTrans = *(STranslation*)lpvi;

			CString strSubBlockFileVersion;
			CString strSubBlockProductVersion;
			strSubBlockFileVersion.Format(_T("\\StringFileInfo\\%04x%04x\\FileVersion"), sTrans.wLangID, sTrans.wCharSet);
			strSubBlockProductVersion.Format(_T("\\StringFileInfo\\%04x%04x\\ProductVersion"), sTrans.wLangID, sTrans.wCharSet);
			LPTSTR pBufFileVersion;
			LPTSTR pBufProductVersion;
			if ( (::VerQueryValue(lpBuffer, (LPTSTR) ((LPCTSTR)strSubBlockFileVersion), (LPVOID*)(&pBufFileVersion), &nSize)) &&
				 (::VerQueryValue(lpBuffer, (LPTSTR) ((LPCTSTR)strSubBlockProductVersion), (LPVOID*)(&pBufProductVersion), &nSize)) )
			{
				sFileVer = pBufFileVersion;
				sProductVer = pBufProductVersion;
			}
		}
		delete[] lpBuffer;
	}
	return !sFileVer.IsEmpty();
}

CString CVersionInfo::ReformatVersionString(LPCTSTR pszFileVersion,
											LPCTSTR pszProductVersion)
{
	////////////////////
	// File Version
	//
	CString strFileVersion = pszFileVersion;
	strFileVersion.Replace(_T(" "), _T(""));	//remove spaces

	TCHAR* pszFileVersionLocal = new TCHAR[strFileVersion.GetLength()+1];
	lstrcpy(pszFileVersionLocal, strFileVersion);

	TCHAR seps[] = _T(",");

#if _MSC_VER >= 1400
	TCHAR* pNext = NULL;
	TCHAR* token = _tcstok_s(pszFileVersionLocal, seps, &pNext);
#else
	TCHAR* token = _tcstok(pszFileVersionLocal, seps);
#endif

	CStringArray arrParts;
	while (token != NULL)
	{
		arrParts.Add(token);
#if _MSC_VER >= 1400
		token = _tcstok_s(NULL, seps, &pNext);
#else
		token = _tcstok(NULL, seps);
#endif
	}

	if (arrParts.GetSize() == 4)
	{
		//if all parts are meaningful, return A.B.CC.DDD
		//if part 4 is 0, return A.B.CC
		//if part 3 is also 0, return A.B
		CString strA;
		CString strB;
		CString strC;
		CString strD;

		strA.Format(_T("%s"), arrParts[0]);
		strB.Format(_T(".%s"), arrParts[1]);

		if ((arrParts[2] != _T("0")) || (arrParts[3] != _T("0")))
		{
			strC.Format(_T(".%02s"), arrParts[2]);

			if (arrParts[3] != _T("0"))
			{
				strD.Format(_T(".%03s"), arrParts[3]);
			}
		}

		strFileVersion = strA + strB + strC + strD;
	}
	else
	{
		//not in the expected "A, B, C, D" format - return the entire string
		strFileVersion = pszFileVersion;
	}

	delete[] pszFileVersionLocal;


	////////////////////
	// Product Version
	//
	//don't try to be clever with this, just return the string
	CString strProductVersion = pszProductVersion;


	CString strReturn;
	if (strProductVersion.IsEmpty())
	{
		strReturn.Format(_T("%s"), strFileVersion);
	}
	else
	{
		strReturn.Format(_T("%s %s"), strFileVersion, strProductVersion);
	}
	return strReturn;
}
