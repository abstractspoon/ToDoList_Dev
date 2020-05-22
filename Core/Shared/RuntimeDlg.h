#if !defined(AFX_RUNTIMEDLG_H__AC34D7F7_D4E4_45E3_A746_0CC018F717F1__INCLUDED_)
#define AFX_RUNTIMEDLG_H__AC34D7F7_D4E4_45E3_A746_0CC018F717F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuntimeDlg.h : header file
//

#include "rcctrlparser.h"
#include "dialoghelper.h"

/////////////////////////////////////////////////////////////////////////////

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CRuntimeDlg window

class CRuntimeDlg : public CDialog, public CDialogHelper
{
 	DECLARE_DYNAMIC(CRuntimeDlg)

// Construction
public:
	CRuntimeDlg(CWnd* pParentWnd = NULL, IPreferences* pPrefs = NULL, LPCTSTR szSettingsKey = NULL);

	// do not use
	int DoModal() { ASSERT(0); return -1; } 
	BOOL Create(LPCTSTR /*lpszTemplateName*/, CWnd* /*pParentWnd*/ = NULL) { ASSERT(0); return FALSE; } 
	BOOL Create(UINT /*nIDTemplate*/, CWnd* /*pParentWnd*/ = NULL) { ASSERT(0); return FALSE; } 

	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void SetFont(HFONT hFont, BOOL bRedraw = TRUE);

	BOOL ShowWindow(int nCmdShow);
	BOOL DestroyWindow();

	void SavePosition(IPreferences* pPrefs, LPCTSTR szSettingsKey) const;
	void RestorePosition(const IPreferences* pPrefs, LPCTSTR szSettingsKey, BOOL bCentreFirstTime = TRUE);

// Operations
protected: // intended to be used only from a derived class

	#define RTD_DEFSTYLE (WS_VISIBLE | WS_POPUPWINDOW  | WS_CAPTION | DS_CENTER | DS_SETFONT)
	#define RTD_DEFEXSTYLE (WS_EX_DLGMODALFRAME)

	virtual int DoModal(LPCTSTR szCaption, 
						DWORD dwStyle = RTD_DEFSTYLE, 
						DWORD dwExStyle = RTD_DEFEXSTYLE, 
						const CRect& rect = rectAuto, 
						CWnd* pParentWnd = NULL, 
						UINT nID = -1);

	virtual BOOL Create(LPCTSTR szCaption, 
						DWORD dwStyle = RTD_DEFSTYLE, 
						DWORD dwExStyle = RTD_DEFEXSTYLE, 
						const CRect& rect = rectAuto, 
						CWnd* pParentWnd = NULL, 
						UINT nID = -1);

	virtual void PostCreate() {}

	// default borders are 7 DLU
	void SetBorders(int nAll) { SetBorders(nAll, nAll, nAll, nAll); }
	void SetBordersDLU(int nAll) { SetBordersDLU(nAll, nAll, nAll, nAll); }
	void SetBorders(int nLeft, int nTop, int nRight, int nBottom);
	void SetBordersDLU(int nLeft, int nTop, int nRight, int nBottom);

	// use these when you do _not_ want to subclass the control
	virtual BOOL AddControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							const CRect& rect, UINT nID); // pos in pixels
	virtual BOOL AddControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							int x, int y, int cx, int cy, UINT nID); // pos in pixels

	// use these when you _do_ want to subclass via MFC
	virtual BOOL AddControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							const CRect& rect, UINT nID); // pos in pixels
	virtual BOOL AddControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							int x, int y, int cx, int cy, UINT nID); // pos in pixels

	// individual RC controls
	virtual BOOL AddRCControl(LPCTSTR szRCType, LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, 
							DWORD dwExStyle, int x, int y, int cx, int cy, UINT nID, UINT uIconID = 0); // pos in DLUs

	virtual BOOL AddRCControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, 
							DWORD dwExStyle, int x, int y, int cx, int cy, UINT nID, UINT uIconID = 0); // pos in DLUs

	// 'doctored' text straight from an RC file
	virtual int AddRCControls(const CString& sRCControls);

	// call to size the dialog to fit the controls
	// will be called automatically during the create process if 'rectAuto' specified
	void AutoFit();

// Attributes
public:
	static const CRect rectAuto;

protected:
	CRTCtrlList m_lstControls;
	CRect m_rBorders, m_rBordersDLU;
	HICON m_hILarge, m_hISmall;
	CString m_sSettingsKey;
	HFONT m_hFont;
	CSize m_sizeInitial;

	IPreferences* m_pPrefs;

	static CMapStringToString s_mapClasses;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuntimeDlg)
	//}}AFX_VIRTUAL
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

// Implementation
public:
	virtual ~CRuntimeDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRuntimeDlg)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual HWND CreateControl(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
								int x, int y, int cx, int cy, UINT nID, BOOL bDLU, UINT nIconID = 0);

	// use this when you _do_ want to subclass
	virtual BOOL CreateControl(CWnd* pWnd, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
								int x, int y, int cx, int cy, UINT nID, BOOL bDLU, UINT nIconID = 0);

	BOOL CreateControl(const RTCONTROL& ctrl);
	void CreateControls();
	CSize GetCtrlSize(UINT nCtrlID) const { return GetCtrlRect(nCtrlID).Size(); }
	CPoint GetCtrlPos(UINT nCtrlID) const { return GetCtrlRect(nCtrlID).TopLeft(); }
	CRect GetCtrlRect(UINT nCtrlID) const;
	CRect OffsetCtrl(UINT nCtrlID, int x = 0, int y = 0);
	CRect ResizeCtrl(UINT nCtrlID, int cx = 0, int cy = 0);
	CRect MoveCtrl(UINT nCtrlID, int x, int y);
	void ShowCtrls(UINT nCtrlIDFrom, UINT nCtrlIDTo, BOOL bShow = TRUE);

	void SavePosition();
	BOOL HasInitialSize() const { return (m_sizeInitial.cx && m_sizeInitial.cy); }
	void ClearInitialSize() { m_sizeInitial.cx  = m_sizeInitial.cy = 0; }

	virtual void SetInitialPos(LPCRECT pRect, DWORD dwStyle);
	virtual CPoint GetInitialPos() const { return CPoint(-1, -1); }
	virtual CString OverrideItemText(UINT /*nIDItem*/) { return _T(""); }

	static void BuildClassMap();
	static CString GetControlClassName(CWnd* pWnd); // works up the derivation chain till it finds a 'standard' windows control class
	static HWND CreateRichEdit(LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
							            int x, int y, int cx, int cy, HWND hwndParent, UINT nID);
    static int CalcLinesRequired(const CString& sText, int nWidthDLU);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNTIMEDLG_H__AC34D7F7_D4E4_45E3_A746_0CC018F717F1__INCLUDED_)
