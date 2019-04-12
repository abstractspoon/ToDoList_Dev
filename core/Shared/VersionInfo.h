
#ifndef _VERSIONINFO_H_
#define _VERSIONINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment (lib,"version.lib")

class CVersionInfo
{
protected:
	CVersionInfo()	{ /*don't instantiate me*/ }
	~CVersionInfo()	{ /*don't instantiate me*/ }

public:
	static CString GetVersion(const CString& strDLLFilename);
	static CString GetFileVersion(const CString& strDLLFilename);
	static CString GetProductVersion(const CString& strDLLFilename);

private:
	static CString ReformatVersionString(LPCTSTR pszFileVersion, LPCTSTR pszProductVersion);
	static BOOL GetVersion(const CString& strDLLFilename, CString& sFileVer, CString& sProductVer);
};


#endif//_VERSIONINFO_H_
