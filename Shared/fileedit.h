#if !defined(AFX_FILEEDIT_H__7A50F411_3AAE_4BC2_989A_53D44291B643__INCLUDED_)
#define AFX_FILEEDIT_H__7A50F411_3AAE_4BC2_989A_53D44291B643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fileedit.h : header file
//

#include "enedit.h"
#include "sysimagelist.h"
#include "stringres.h"

/////////////////////////////////////////////////////////////////////////////

const UINT WM_FE_GETFILEICON = ::RegisterWindowMessage(_T("WM_FE_GETFILEICON")); // lParam == <filepath>
const UINT WM_FE_DISPLAYFILE = ::RegisterWindowMessage(_T("WM_FE_DISPLAYFILE")); // lParam == <filepath>
const UINT WM_FEN_BROWSECHANGE = ::RegisterWindowMessage(_T("WM_FEN_BROWSEFILE")); // lParam == <filepath>

/////////////////////////////////////////////////////////////////////////////

enum
{
	FES_NOBROWSE			= 0x0001,
	FES_FOLDERS				= 0x0002,
	FES_COMBOSTYLEBTN		= 0x0004, // draws the browse button like a combo box rather than a button
	FES_GOBUTTON			= 0x0008,
	FES_ALLOWURL			= 0x0010, // supports ? in the filename
	FES_SAVEAS				= 0x0020, // else open file dialog
	FES_RELATIVEPATHS		= 0x0040,
	FES_DISPLAYSIMAGES		= 0x0080,
	FES_NOPROMPTOVERWRITE	= 0x0100, // exclude OFN_OVERWRITEPROMPT
};

/////////////////////////////////////////////////////////////////////////////

// button types
enum { FEBTN_NONE, FEBTN_BROWSE, FEBTN_GO, FE_BTNLAST };

//////////////////////////////////////////////////////////////////////////

class CEnFileDialog;

/////////////////////////////////////////////////////////////////////////////
// CFileEdit window

class CFileEdit : public CEnEdit
{
	friend class CFileComboBox;

	DECLARE_DYNAMIC(CFileEdit)

// Construction
public:
	CFileEdit(int nStyle = (FES_COMBOSTYLEBTN | FES_DISPLAYSIMAGES), LPCTSTR szFilter = FILTER_ALLFILES);
	virtual ~CFileEdit();

	void EnableStyle(int nStyle, BOOL bEnable = TRUE);
	inline BOOL HasStyle(int nStyle) const { return (m_nStyle & nStyle); }

	void SetFilter(LPCTSTR szFilter) { m_sFilter = szFilter; }
	void SetCurrentFolder(LPCTSTR szFolder);
	CString GetCurrentFolder() const { return m_sCurFolder; }
	void SetBrowseTitle(LPCTSTR szTitle) { m_sBrowseTitle = szTitle; }

	static void SetDefaultButtonTips(LPCTSTR szBrowse, LPCTSTR szGo);
	static void SetDefaultBrowseTitles(LPCTSTR szBrowseFiles, LPCTSTR szBrowseFolders);
	static void SetDefaultButtonImages(HICON hBrowse, HICON hGo);

	static int GotoFile(HWND hWnd, LPCTSTR szPath, BOOL bHandleError = TRUE);
	static int GotoFile(HWND hWnd, LPCTSTR szPath, LPCTSTR szFolder, BOOL bHandleError = TRUE);

// Attributes
protected:
	BOOL m_bTipNeeded;
	CSysImageList m_ilSys;
	CString m_sFilter;
	int m_nStyle;
	CString m_sCurFolder;
	CString m_sBrowseTitle;
	BOOL m_bParentIsCombo;
	CImageList m_ilImageIcon;

	static CString s_sBrowseBtnTip, s_sGoBtnTip;
	static CString s_sBrowseFilesTitle, s_sBrowseFoldersTitle;
	static HICON s_hBrowseImage, s_hGoImage;
	
	const UINT ICON_WIDTH;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileEdit)
	protected:
	virtual void PreSubclassWindow();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CFileEdit)
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnChange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnPaste(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	virtual void OnBtnClick(UINT nID);
	virtual void NcPaint(CDC* pDC, const CRect& rWindow);
	virtual void OnSetReadOnly(BOOL bReadOnly);
	virtual void HandleBrowseForFile(CEnFileDialog& dlg);

protected:
	CRect GetIconRect() const; // screen coords
	void DrawFileIcon(CDC* pDC, const CString& sFilePath, const CRect& rIcon);
	CString GetBrowseTitle(BOOL bFolder) const;
	void ClearImageIcon();
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEEDIT_H__7A50F411_3AAE_4BC2_989A_53D44291B643__INCLUDED_)
