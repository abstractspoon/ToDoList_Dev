
/* 
DESCRIPTION:
	CFolderDialog  - Folder Selection Dialog Class

NOTES:
	Copyright(C) Armen Hakobyan, 2002
	mailto:armenh@cit.am
	
VERSION HISTORY:
	24 Mar 2002 - First release
*/

#pragma once

#define BIF_BROWSENONETWORK		0x00008000
#define BIF_BROWSEFIXEDONLY		0x00010000
#define BIF_BROWSENOREADONLY	0x00020000
#define BIF_BROWSENOROOTDIR		0x00040000

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE		0x0040
#endif

#include <afxdlgs.h>

class CFolderDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

public:
	explicit CFolderDialog(LPCTSTR lpszTitle = NULL, 
			 LPCTSTR lpszFolderName = NULL, CWnd* pParentWnd = NULL,
			 UINT uFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE);
	virtual ~CFolderDialog();

public:
	virtual int	DoModal(void);
	
public:	
	AFX_INLINE LPCTSTR	GetFolderPath(void) const;
	AFX_INLINE LPCTSTR	GetFolderDisplayName(void) const;
	AFX_INLINE int		GetFolderImageIndex(void) const;	
	AFX_INLINE BOOL		SetSelectedFolder(LPCTSTR lpszPath);

	AFX_INLINE BROWSEINFO&		 GetBI(void);
	AFX_INLINE const BROWSEINFO& GetBI(void) const;
	
protected:	
	BROWSEINFO	m_bi;
	TCHAR		m_szFolderPath[MAX_PATH+1];
	TCHAR		m_szSelectedPath[MAX_PATH+1];
	TCHAR		m_szFolderDisplayName[MAX_PATH+1];

protected:
	DECLARE_MESSAGE_MAP()
	
	virtual void OnInitialized(void);
	virtual int	 OnValidateFailed(LPCTSTR lpstrFolderPath);
	virtual void OnSelChanged(LPITEMIDLIST /*pItemIDList*/);

protected:
	AFX_INLINE void EnableOK(BOOL bEnable = TRUE);
	AFX_INLINE void SetSelection(LPITEMIDLIST pItemIDList);
	AFX_INLINE void SetSelection(LPCTSTR lpszFolderPath);
	AFX_INLINE void SetStatusText(LPCTSTR lpszText);
	
private:
	HWND m_hWnd; // used only in the callback function
	static INT CALLBACK BrowseCallbackProc(
		HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
};

AFX_INLINE BOOL CFolderDialog::SetSelectedFolder(LPCTSTR lpszPath)
	{ ASSERT (lpszPath != NULL);
		return (lstrcpy(m_szSelectedPath, lpszPath) != NULL); }

AFX_INLINE BROWSEINFO& CFolderDialog::GetBI(void)
	{ return m_bi; }

AFX_INLINE const BROWSEINFO& CFolderDialog::GetBI(void) const
	{ return m_bi; }

// filled after a call to DoModal

AFX_INLINE LPCTSTR CFolderDialog::GetFolderPath(void) const
	{ return m_szFolderPath; }

AFX_INLINE LPCTSTR CFolderDialog::GetFolderDisplayName(void) const
	{ return m_szFolderDisplayName; }

AFX_INLINE int CFolderDialog::GetFolderImageIndex(void) const
	{ return m_bi.iImage; }

// Commands - valid to call only from handlers

AFX_INLINE void CFolderDialog::EnableOK(BOOL bEnable /*TRUE*/)
	{ ASSERT(m_hWnd != NULL); 
		::SendMessage(m_hWnd, BFFM_ENABLEOK, 0L, bEnable);}

AFX_INLINE void CFolderDialog::SetSelection(LPITEMIDLIST pItemIDList)
	{ ASSERT(m_hWnd != NULL); 
		::SendMessage(m_hWnd, BFFM_SETSELECTION, FALSE, (LPARAM)pItemIDList); }

AFX_INLINE void CFolderDialog::SetSelection(LPCTSTR lpszFolderPath)
	{ ASSERT(m_hWnd != NULL);
		::SendMessage(m_hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpszFolderPath); }

AFX_INLINE void CFolderDialog::SetStatusText(LPCTSTR lpszText)
	{ ASSERT(m_hWnd != NULL);
		::SendMessage(m_hWnd, BFFM_SETSTATUSTEXT, 0L, (LPARAM)lpszText);}