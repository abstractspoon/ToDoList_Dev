// TDLUpdateScript.h: interface for the CTDCWebUpdateScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDLUPDATESCRIPT_H__AF4E8D29_CABB_46F8_9007_B66312C66C7E__INCLUDED_)
#define AFX_TDLUPDATESCRIPT_H__AF4E8D29_CABB_46F8_9007_B66312C66C7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTDCWebUpdateScript  
{
public:
	CTDCWebUpdateScript();
	virtual ~CTDCWebUpdateScript();

	BOOL CheckForUpdates();
	BOOL CheckForPreReleaseUpdates();

	BOOL GetScriptDetails(CString& sExeVer, CString& sDisplayVer, CStringArray& aDescription) const;

	static BOOL IsPreRelease(const CString& sExeVer);

protected:
	CMapStringToString m_mapScript;

protected:
	BOOL VerifyHeader() const;
	int GetScriptVersion() const;
	BOOL GetScriptExeVersion(CString& sVer) const;
	BOOL GetScriptDisplayVersion(CString& sVer) const;
	int GetUpdateDescription(CStringArray& aDesc) const;
	LPCTSTR GetScriptUrl(BOOL bPreRelease) const;

	BOOL CheckForUpdates(BOOL bPreRelease);
	BOOL VerifyGetLine(int nLine, const CString& sKey, CString& sValue) const;
	BOOL LoadScript(const CString& sScriptUrl);
	CString GetScriptValue(const CString& sKey) const;

};

#endif // !defined(AFX_TDLUPDATESCRIPT_H__AF4E8D29_CABB_46F8_9007_B66312C66C7E__INCLUDED_)
