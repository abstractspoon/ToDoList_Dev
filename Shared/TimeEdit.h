// TimeEdit.h: interface for the CTimeEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEEDIT_H__2CCFE44D_9578_4E38_B2BF_091C172C85A5__INCLUDED_)
#define AFX_TIMEEDIT_H__2CCFE44D_9578_4E38_B2BF_091C172C85A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "enedit.h"
#include "timehelper.h"

const UINT WM_TEN_UNITSCHANGE = ::RegisterWindowMessage(_T("WM_TEN_UNITSCHANGE")); // wParam == <CtrlID>, lParam = <prev units>

const int TEBTN_UNITS = 1;

class CTimeEdit : public CEnEdit, public CTimeHelper
{
public:
	CTimeEdit(TH_UNITS nUnits = THU_HOURS, int nMaxDecPlaces = 9);
	virtual ~CTimeEdit();

	double GetTime() const;
	double GetTime(TH_UNITS nUnits) const;

	void SetTime(double dTime);
	void SetTime(double dTime, TH_UNITS nUnits);

	inline TH_UNITS GetUnits() const { return m_nUnits; }
	void SetUnits(TH_UNITS nUnits);

	inline int GetMaxDecimalPlaces() const { return m_nMaxDecPlaces; }
	void SetMaxDecimalPlaces(int nMaxDecPlaces);

	CString FormatTimeHMS() const; 
	CString FormatTime(BOOL bUnits) const; 

	void EnableNegativeTimes(BOOL bEnable);

	static void SetUnits(TH_UNITS nUnits, LPCTSTR szLongUnits, LPCTSTR szAbbrevUnits);
	static void SetDefaultButtonTip(LPCTSTR szUnits);

protected:
	TH_UNITS m_nUnits;
	int m_nMaxDecPlaces;
	CMenu m_menuUnits;

	static CString s_sUnitsBtnTip;
	static BOOL s_bUnitsTranslated;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeEdit)
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow();
	virtual void OnBtnClick(UINT nID);
	virtual void OnSetReadOnly(BOOL bReadOnly);

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CTimeEdit)
	//}}AFX_MSG
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

	void CheckSetUnits(TH_UNITS nUnits, BOOL bQueryUnits);
	BOOL CreateUnitsPopupMenu(BOOL bUpdateState);
	UINT MapUnitsToMenuID() const;

	static void RemoveTrailingZeros(CString& sTime);
};

#endif // !defined(AFX_TIMEEDIT_H__2CCFE44D_9578_4E38_B2BF_091C172C85A5__INCLUDED_)
