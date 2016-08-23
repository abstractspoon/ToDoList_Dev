#if !defined(AFX_ENFILEDIALOG_H__79203A03_61CE_4234_AC53_0A23B1FD2204__INCLUDED_)
#define AFX_ENFILEDIALOG_H__79203A03_61CE_4234_AC53_0A23B1FD2204__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// filedialogex.h : header file
//

/////////////////////////////////////////////////////////////////////////////

const DWORD EOFN_DEFAULTOPEN = (OFN_FILEMUSTEXIST);
const DWORD EOFN_DEFAULTSAVE = (OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CEnFileDialog dialog

class CEnFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CEnFileDialog)

public:
	CEnFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
					LPCTSTR lpszTitle = NULL,
					LPCTSTR lpszDefExt = NULL,
					LPCTSTR lpszFileName = NULL,
					DWORD dwFlags = 0,
					LPCTSTR lpszFilter = NULL,
					CWnd* pParentWnd = NULL);

	void SetTitle(LPCTSTR szTitle);
	int DoModal(IPreferences* pPrefs = NULL); 
	CString GetLastFolder() const { return m_sLastFolder; }
	int GetPathNames(CStringArray& aPaths) const; // only for open file

protected:
	CString m_sTitle, m_sLastFolder, m_sOrgFilters;

protected:
	//{{AFX_MSG(CEnFileDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL HasInitialFileName() const;
	BOOL HasInitialFolder() const;
	CString GetPreferenceKey() const;
};

class CFileOpenDialog : public CEnFileDialog
{
	DECLARE_DYNAMIC(CFileOpenDialog)

public:
	CFileOpenDialog(LPCTSTR lpszTitle,
					LPCTSTR lpszDefExt = NULL,
					LPCTSTR lpszFileName = NULL,
					DWORD dwFlags = EOFN_DEFAULTOPEN,
					LPCTSTR lpszFilter = NULL,
					CWnd* pParentWnd = NULL);

	CFileOpenDialog(UINT nIDTitle,
					LPCTSTR lpszDefExt = NULL,
					LPCTSTR lpszFileName = NULL,
					DWORD dwFlags = EOFN_DEFAULTOPEN,
					LPCTSTR lpszFilter = NULL,
					CWnd* pParentWnd = NULL);

};

class CFileSaveDialog : public CEnFileDialog
{
	DECLARE_DYNAMIC(CFileSaveDialog)

public:
	CFileSaveDialog(LPCTSTR lpszTitle,
					LPCTSTR lpszDefExt = NULL,
					LPCTSTR lpszFileName = NULL,
					DWORD dwFlags = EOFN_DEFAULTSAVE,
					LPCTSTR lpszFilter = NULL,
					CWnd* pParentWnd = NULL);

	CFileSaveDialog(UINT nIDTitle,
					LPCTSTR lpszDefExt = NULL,
					LPCTSTR lpszFileName = NULL,
					DWORD dwFlags = EOFN_DEFAULTSAVE,
					LPCTSTR lpszFilter = NULL,
					CWnd* pParentWnd = NULL);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENFILEDIALOG_H__79203A03_61CE_4234_AC53_0A23B1FD2204__INCLUDED_)
