#pragma once

#include "TDCEnum.h"

#include "..\shared\EnString.h"
#include "..\shared\StatusBarACTEx.h"
#include "..\shared\StatusBarProgress.h"

/////////////////////////////////////////////////////////////////////////////

class CUIThemeFile;
class CFilteredToDoCtrl;
class CTDCAttributeMap;

struct TODOITEM;
struct TDCTIMEPERIOD;

/////////////////////////////////////////////////////////////////////////////

class CTDLStatusBar;

class CTDLStatusBarProgressProxy : protected CStatusBarProgressProxy
{
public:
	CTDLStatusBarProgressProxy(CTDLStatusBar& pStatusBar, const CString& sPrompt);
	virtual ~CTDLStatusBarProgressProxy();
};

/////////////////////////////////////////////////////////////////////////////
// CTDLStatusBar window

class CTDLStatusBar : public CStatusBarACTEx
{
	friend class CTDLStatusBarProgressProxy; // for access to progress bar

public:
	CTDLStatusBar(const TODOITEM& tdiDefault);
	virtual ~CTDLStatusBar();

	void SetUITheme(const CUIThemeFile& theme);
	void UpdateTasks(const CFilteredToDoCtrl& tdc, const  CTDCAttributeMap& mapAttrib);
	void UpdateFocusedControl(const CString& sFocus);

	BOOL BeginProgress(const CString& sPrompt) { return m_progress.BeginProgress(*this, sPrompt); }
	void EndProgress() { m_progress.EndProgress(); }

protected:
	HIMAGELIST m_hilTaskIcons;
	int m_iSelTaskIcon;
	CStatusBarProgress m_progress;

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
	static BOOL WantUpdateAttribute(TDC_ATTRIBUTE nAttribID, const  CTDCAttributeMap& mapAttrib);
};

