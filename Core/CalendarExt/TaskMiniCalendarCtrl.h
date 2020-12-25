#if !defined(AFX_TASKMINICALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TASKMINICALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskMinCalendarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "CalStruct.h"

#include "..\shared\tooltipctrlex.h"
#include "..\shared\MiniCalendarCtrl.h"

#include "..\Interfaces\IUIExtension.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CTaskMiniCalendarCtrl : public CMiniCalendarCtrl
{
// Construction
public:
	CTaskMiniCalendarCtrl(const CTaskCalItemMap& mapData);
	virtual ~CTaskMiniCalendarCtrl();

	void EnableHeatMap(const CDWordArray& aPalette, TDC_ATTRIBUTE nAttrib);
	void DisableHeatMap();

	void SetOptions(DWORD dwOptions);
	void SetBorderColor(COLORREF crBorder);
	void SetUITheme(const UITHEME& theme);

	void OnUpdateTasks();
	void FilterToolTipMessage(MSG* pMsg);

protected:
	const CTaskCalItemMap& m_mapData;

	CSpecialDateSet m_setSpecialDates;
	CHeatMap m_mapHeatMap;
	CToolTipCtrlEx m_tooltip;
	
	TDC_ATTRIBUTE m_nHeatMapAttribute;
	DWORD m_dwOptions;
	COLORREF m_crBorder, m_crWeekend, m_crToday, m_crSelected;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskMiniCalendarCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskMiniCalendarCtrl)
	//}}AFX_VIRTUAL
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual void Draw(CDC& dc, const CRect& rDraw);

	// helpers
	virtual BOOL IsSpecialDate(const COleDateTime& dt) const;
	virtual COLORREF GetCellBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;
	virtual COLORREF GetCellTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;
	virtual void DrawCellBkgnd(CDC& dc, const CRect& rCell, const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth);

	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) == dwOption); }

	void RecalcHeatMap();
	void RecalcSpecialDates();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
