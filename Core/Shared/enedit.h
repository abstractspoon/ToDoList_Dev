#if !defined(AFX_ENEDIT_H__65D418F0_0107_431F_95B2_E31BF25FF286__INCLUDED_)
#define AFX_ENEDIT_H__65D418F0_0107_431F_95B2_E31BF25FF286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// enedit.h : header file
//

#include "maskedit.h"
#include "hottracker.h"
#include "tooltipctrlex.h"
#include "enimagelist.h"
#include "GraphicsMisc.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CEnEdit window


const int EE_BTNWIDTH_DEFAULT	= ::GetSystemMetrics(SM_CXHTHUMB);
const int EE_BTNWIDTH_ICON		= (GraphicsMisc::ScaleByDPIFactor(16 + 2));
const int EE_BTNWIDTH_CALCULATE = -1;

/////////////////////////////////////////////////////////////////////////////

const UINT WM_EE_BTNCLICK = ::RegisterWindowMessage(_T("WM_EE_BTNCLICK")); // wParam = CtrlID, lParam = BtnID

/////////////////////////////////////////////////////////////////////////////

enum // TrackPopupMenu
{
	EETPM_BELOW		= 0,
	EETPM_RIGHT		= 1,
	EETPM_RETURNCMD = 2,
};

/////////////////////////////////////////////////////////////////////////////

class CEnEdit : public CMaskEdit
{
	DECLARE_DYNAMIC(CEnEdit)

// Construction
public:
	CEnEdit(LPCTSTR szMask = NULL, DWORD dwMaskFlags = 0);

	BOOL AddButton(UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT, LPCTSTR szFont = NULL);
	BOOL AddButton(UINT nID, UINT nChar, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT, LPCTSTR szFont = NULL);
	BOOL AddButton(UINT nID, HICON hIcon, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT);
	BOOL InsertButton(int nPos, UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT, LPCTSTR szFont = NULL);
	BOOL InsertButton(int nPos, UINT nID, UINT nChar, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT, LPCTSTR szFont = NULL);
	BOOL InsertButton(int nPos, UINT nID, HICON hIcon, LPCTSTR szTip, int nWidth = EE_BTNWIDTH_DEFAULT );

	BOOL DeleteButton(UINT nID);
	void DeleteAllButtons();

	BOOL EnableButton(UINT nID, BOOL bEnable = TRUE);
	BOOL CheckButton(UINT nID, BOOL bChecked = TRUE);
	BOOL ClickButton(UINT nID);
	BOOL SetButtonTip(UINT nID, LPCTSTR szTip);
	BOOL SetButtonCaption(UINT nID, LPCTSTR szCaption);
	BOOL SetButtonWidth(UINT nID, int nWidth);
	BOOL SetButtonWidthDLU(UINT nID, int nDLU);
	void SetDefaultButton(UINT nID);

	CRect GetButtonRect(UINT nID) const;
	CString GetButtonTip(UINT nID) const;
	int GetButtonCount() const;
	UINT GetButtonID(int nPos) const;

	BOOL SetDropMenuButton(UINT nID, BOOL bDropMenu = TRUE);
	UINT TrackPopupMenu(UINT nID, CMenu* pMenu, DWORD dwFlags = EETPM_BELOW);

	void EnableButtonPadding(BOOL bEnable = TRUE);
	void FilterToolTipMessage(MSG* pMsg);

// Attributes
protected:
	struct EDITBTN
	{
		EDITBTN();

		UINT nID;
		CString sCaption;
		CString sTip;
		int nWidth;
		BOOL bEnabled;
		HFONT hFont;
		BOOL bChecked;
		BOOL bDropMenu;
		BOOL bSymbol;
		HICON hIcon;
		int iImage;
	};

	CArray<EDITBTN, EDITBTN&> m_aButtons;
	CHotTracker m_hotTrack;
	int m_nBtnPadding;
	CEnImageList m_ilBtns, m_ilDisabledBtns;
	CToolTipCtrlEx m_tooltip;
	int m_nDefaultBtn;

	BOOL m_bFirstShow;
	BOOL m_nButtonDown;
	BOOL m_bParentIsCombo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnEdit)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual void OnSetReadOnly(BOOL /*bReadOnly*/) {}
	virtual void OnBtnClick(UINT /*nID*/) {}
	virtual void RecalcBtnHotRects();
	virtual void NcPaint(CDC* pDC, const CRect& rWindow);

// Implementation
public:
	virtual ~CEnEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnEdit)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg LRESULT OnSetReadOnly(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnHotChange(WPARAM wp, LPARAM lp);
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnToolHitTest(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	CRect GetButtonRectByIndex(int nBtn) const; // screen coords
	virtual void DrawButton(CDC* pDC, const CRect& rWindow, int nBtn, const CPoint& ptCursor);
	int ButtonHitTest(CPoint ptScreen) const;
	int ButtonHitTest(UINT nID) const;
	int GetButtonWidthByIndex(int nBtn) const;
	CRect GetButtonsRect() const;
	int GetButtonsWidth() const;
	HBRUSH PrepareColors(CDC* pDC) const;
	BOOL InsertButton(int nPos, UINT nID, LPCTSTR szCaption, LPCTSTR szTip, int nWidth, 
						LPCTSTR szFont, BOOL bSymbolFont);

	void RedrawButtonByIndex(int nBtn);
	void ForceRedraw();
	void FreeButtonResources();
	void FreeButtonResources(int nBtn);
	void InitializeHotTracking();

	virtual BOOL InitializeTooltips();
	virtual BOOL InitializeImageLists();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	static void DrawText(CDC* pDC, const CPoint& ptTopLeft, const CString& sText, 
						const CRect& rect, BOOL bSymbol = FALSE);
	static void DrawEnabledText(CDC* pDC, const CPoint& ptTopLeft, const CString& sText, 
						const CRect& rect, BOOL bEnabled = TRUE, BOOL bSymbol = FALSE);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENEDIT_H__65D418F0_0107_431F_95B2_E31BF25FF286__INCLUDED_)
