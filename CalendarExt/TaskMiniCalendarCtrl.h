#if !defined(AFX_TASKMINICALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_TASKMINICALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskMinCalendarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "CalStruct.h"
#include "CalEnum.h"

#include "..\shared\mapex.h"
#include "..\shared\tooltipctrlex.h"
#include "..\shared\fontcache.h"

#include "..\Interfaces\IUIExtension.h"
#include "..\Interfaces\ITaskList.h"

#include "..\3rdParty\FPSMiniCalendarCtrl.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CTaskMiniCalendarCtrl : public CFPSMiniCalendarCtrl
{
// Construction
public:
	CTaskMiniCalendarCtrl(const CTaskCalItemMap& mapData);
	virtual ~CTaskMiniCalendarCtrl();

	void RecalcSpecialDates();
	void SetOptions(DWORD dwOptions);

protected:
	const CTaskCalItemMap& m_mapData;

	CSpecialDateSet m_setSpecialDates;
	DWORD m_dwOptions;

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

	// helpers
	virtual BOOL IsSpecialDate(const COleDateTime& dt) const;
	virtual COLORREF GetDateBkgndColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;
	virtual COLORREF GetDateTextColor(const COleDateTime& dt, BOOL bSelected, BOOL bSpecial, BOOL bActiveMonth) const;

	BOOL HasOption(DWORD dwOption) const { return ((m_dwOptions & dwOption) == dwOption); }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKCALENDARCTRL_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
