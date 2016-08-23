#if !defined(AFX_REGKEY_H__E451D194_D4C3_11D1_AB17_0000E8425C3E__INCLUDED_)
#define AFX_REGKEY_H__E451D194_D4C3_11D1_AB17_0000E8425C3E__INCLUDED_

#include <winreg.h>
#include <afxtempl.h>

class CRegKey2
{

// Construction
public:
	CRegKey2();
	CRegKey2(CRegKey2& regKey);
	virtual ~CRegKey2();

	CRegKey2& operator=(CRegKey2 regKey); 

// Operations
public:
	// note: calling open on an already open key will assert
	// you must call Close() first if you want to re-use CRegKey objects
	LONG Open(HKEY hKeyRoot, LPCTSTR pszPath, BOOL bReadOnly); 
	void Close();
	CString GetPath() const { return m_sPath; }
	HKEY GetKey() const { return m_hKey; }
	HKEY GetKeyRoot() const { return m_hKeyRoot; }
	
	LONG Write(LPCTSTR pszItem, DWORD dwVal);
	LONG Write(LPCTSTR pszItem, LPCTSTR pszVal);
	LONG Write(LPCTSTR pszItem, const BYTE* pData, DWORD dwLength);
	
	LONG Read(LPCTSTR pszItem, DWORD& dwVal) const;
	LONG Read(LPCTSTR pszItem, CString& sVal) const;
	LONG Read(LPCTSTR pszItem, BYTE* pData, DWORD& dwLength) const;

	LONG DeleteValue(LPCTSTR pszItem);
	
	int GetSubkeyNames(CStringArray& aNames) const;
	int GetValueNames(CStringArray& aNames) const;
	
	BOOL HasValues() const;
	DWORD GetValueType(LPCTSTR pszItem) const;
	
	BOOL ExportToIni(LPCTSTR szIniPath, BOOL bUnicode) const;
	BOOL ImportFromIni(LPCTSTR szIniPath);
	
	static BOOL CopyKey(HKEY hkRootFrom, const CString& sFromPath, HKEY hkRootTo, const CString& sToPath);
	static LONG DeleteKey(HKEY hKeyRoot, LPCTSTR pszPath);
	static BOOL KeyExists(HKEY hKeyRoot, LPCTSTR pszPath);

#ifndef _NOT_USING_MFC_
	static CString GetAppRegPath(LPCTSTR szAppName = NULL);
#endif

protected:
	HKEY 	m_hKey;
	HKEY 	m_hKeyRoot;
	CString m_sPath;
	BOOL	m_bReadOnly;

protected:
	static LONG RecurseDeleteKey(HKEY key, LPCTSTR lpszKey);
	BOOL ExportKeyToIni(LPCTSTR pszKey, CStdioFile& file) const;
	BOOL ExportValueToIni(DWORD nIndex, CStdioFile& file) const;
	BOOL ImportSectionFromIni(const CString& sSection, CStdioFile& file, CString& sNextSection);
	int GetNames(CStringArray& aNames, BOOL bSubKey) const;
};

#endif
