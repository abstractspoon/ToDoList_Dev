#if !defined(AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_)
#define AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalendarCtrlEx.h : header file
//

#include "..\3rdParty\CalendarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrlEx window

class CCalendarCtrlEx : public CCalendarCtrl
{
public:
	CCalendarCtrlEx();
	virtual ~CCalendarCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawHeader(CDC* pDC);
	virtual CString GetMonthName(const COleDateTime& date, BOOL bShort = FALSE) const;
	virtual CString GetDayOfWeekName(const COleDateTime& date, BOOL bShort = FALSE) const;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_)
