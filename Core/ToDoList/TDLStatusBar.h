#pragma once

#include "TDCEnum.h"

#include "..\shared\StatusBarACTEx.h"

/////////////////////////////////////////////////////////////////////////////

class CUIThemeFile;
class CFilteredToDoCtrl;

struct TODOITEM;
struct TDCTIMEPERIOD;

/////////////////////////////////////////////////////////////////////////////
// CTDLStatusBar window

class CTDLStatusBar : public CStatusBarACTEx
{
public:
	CTDLStatusBar(const TODOITEM& tdiDefault);
	virtual ~CTDLStatusBar();

	void SetUITheme(const CUIThemeFile& theme);
	void UpdateTaskTotals(const CFilteredToDoCtrl& tdc);
	void UpdateTaskSelection(const CFilteredToDoCtrl& tdc);

protected:
	HIMAGELIST m_hilTaskIcons;
	int m_iSelTaskIcon;

	const TODOITEM& m_tdiDefault;

protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLStatusBar)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL DrawPaneText(CDC* pDC, int nPane, int nOffset);

	void SetPaneTextAndTooltip(UINT nIDPane, UINT nIDTextFormat, const CString& sTextValue, UINT nIDTipFormat, LPCTSTR szTipValue = NULL);
	
	static CString FormatTime(double dAmount, TDC_UNITS nUnits, BOOL bHMS);
};

