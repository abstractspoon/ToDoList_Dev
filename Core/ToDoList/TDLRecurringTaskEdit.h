// RecurrenceEdit.h: interface for the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECURRENCEEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_)
#define AFX_RECURRENCEEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToDoCtrlData.h"
#include "TDLDialog.h"

#include "..\shared\enedit.h"
#include "..\shared\propertypagehost.h"
#include "..\shared\DayOfWeekcombobox.h"
#include "..\shared\monthcheckcombobox.h"
#include "..\Shared\DayOfWeekCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////

class CTDLRecurringTaskEdit : public CEnEdit  
{
public:
	CTDLRecurringTaskEdit();
	virtual ~CTDLRecurringTaskEdit();

	void GetRecurrenceOptions(TDCRECURRENCE& tr) const;
	void SetRecurrenceOptions(const TDCRECURRENCE& tr);

	void SetDefaultDate(const COleDateTime& date) { m_dtDefault = date; }

	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL DoEdit(BOOL bForce = FALSE);

	static int CalcMaxRegularityWidth(CDC* pDC, BOOL bIncOnce = TRUE);
	static void SetDefaultButtonTip(LPCTSTR szOption);

	void DDX(CDataExchange* pDX, TDCRECURRENCE& value);

protected:
	TDCRECURRENCE m_tr;
	BOOL m_bReadOnly;
	COleDateTime m_dtDefault;
	BOOL m_bInOnSetReadOnly;

	static CString s_sOptionsBtnTip;

protected:
	virtual void OnBtnClick(UINT nID);
	virtual void PreSubclassWindow();

protected:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg LRESULT OnSetReadOnly(WPARAM wp, LPARAM lp);
	afx_msg void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTDLRecurringTaskOnceOptionPage dialog

class CTDLRecurringTaskOnceOptionPage : public CPropertyPage
{
public:
	CTDLRecurringTaskOnceOptionPage();
	~CTDLRecurringTaskOnceOptionPage() {}
};

/////////////////////////////////////////////////////////////////////////////
// CTDLRecurringTaskDailyOptionPage dialog

class CTDLRecurringTaskDailyOptionPage : public CCmdNotifyPropertyPage
{
// Construction
public:
	CTDLRecurringTaskDailyOptionPage(const TDCRECURRENCE& tr, const COleDateTime& dtDefault);
	~CTDLRecurringTaskDailyOptionPage();

	void GetRecurrenceOptions(TDCRECURRENCE& tr) const;

protected:
	int		m_nEveryNumDays;
	int		m_nEveryNumWeekdays;
	int		m_nDailyOption;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeDailyValues();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTDLRecurringTaskWeeklyOptionPage dialog

class CTDLRecurringTaskWeeklyOptionPage : public CCmdNotifyPropertyPage
{
// Construction
public:
	CTDLRecurringTaskWeeklyOptionPage(const TDCRECURRENCE& tr, const COleDateTime& dtDefault);
	~CTDLRecurringTaskWeeklyOptionPage();

	void GetRecurrenceOptions(TDCRECURRENCE& tr) const;

protected:
	int		m_nWeeklyOption;
	int		m_nEveryNumWeeks;
	int		m_nRecreateAfterNumWeeks;
 	DWORD m_dwWeekdays;
	CDayOfWeekCheckListBox m_lbWeekdays;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeWeeklyValues();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTDLRecurringTaskMonthlyOptionPage dialog

class CTDLRecurringTaskMonthlyOptionPage : public CCmdNotifyPropertyPage
{
// Construction
public:
	CTDLRecurringTaskMonthlyOptionPage(const TDCRECURRENCE& tr, const COleDateTime& dtDefault);
	~CTDLRecurringTaskMonthlyOptionPage();

	void GetRecurrenceOptions(TDCRECURRENCE& tr) const;

protected:
	int		m_nEveryDayOfMonth;
	int		m_nEveryNumMonthsDay;
	int		m_nSpecificNumber;
	int		m_nSpecificDayOfWeek;
	int		m_nSpecificNumMonths;
	int		m_nEveryNumMonths;
	int		m_nMonthlyOption;

	CDayOfWeekComboBox	m_cbDaysOfWeek;
	CComboBox			m_cbSpecificWeek;
	CStringArray		m_aSpecificWeek;
	BOOL				m_bFirstLastWeekday;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeMonthlyValues();
	afx_msg void OnChangeSpecificWeek();
	afx_msg void OnChangeWeekday();
	DECLARE_MESSAGE_MAP()

protected:
	void FixupWeekdayCombo(int iWeek, int iPrevWeek);
	void FixupSpecificWeekCombo(int iWeekday, int iPrevWeekday);
};

/////////////////////////////////////////////////////////////////////////////
// CTDLRecurringTaskYearlyOptionPage dialog

class CTDLRecurringTaskYearlyOptionPage : public CCmdNotifyPropertyPage
{
public:
	CTDLRecurringTaskYearlyOptionPage(const TDCRECURRENCE& tr, const COleDateTime& dtDefault);
	~CTDLRecurringTaskYearlyOptionPage();

	void GetRecurrenceOptions(TDCRECURRENCE& tr) const;

protected:
	int		m_nYearlyOption;
	int		m_nEveryDayOfMonth;
	int		m_nSpecificNumber;
	int		m_nEveryNumYears;
	DWORD	m_dwSpecificMonths;
	int		m_nSpecificDayOfWeek;
	DWORD	m_dwEveryMonths;

	CMonthCheckComboBox	m_cbSpecificMonthList;
	CMonthCheckComboBox	m_cbEveryMonthList;
	CDayOfWeekComboBox	m_cbDaysOfWeek;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeYearlyValues();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CRecurringTaskOptionDlg dialog

class CTDLRecurringTaskOptionDlg : public CTDLDialog
{
// Construction
public:
	CTDLRecurringTaskOptionDlg(const TDCRECURRENCE& tr, const COleDateTime& dtDefault, CWnd* pParent = NULL);   // standard constructor

	TDC_REGULARITY GetRecurrenceOptions(TDCRECURRENCE& tr) const;

protected:
	int		m_nRecalcFrom;
	int		m_nReuse;
	int		m_nNumRecur;
	BOOL	m_bEndAfterNumRecur;
	BOOL	m_bPreserveComments;
	
	CTDLRecurringTaskOnceOptionPage m_pageOnce;
	CTDLRecurringTaskDailyOptionPage m_pageDaily;
	CTDLRecurringTaskWeeklyOptionPage m_pageWeekly;
	CTDLRecurringTaskMonthlyOptionPage m_pageMonthly;
	CTDLRecurringTaskYearlyOptionPage m_pageYearly;
	CPropertyPageHost m_ppHost;
	TDC_REGULARITY m_nFrequency;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnChangeFrequency();
	afx_msg void OnEndAfterNumRecur();
	DECLARE_MESSAGE_MAP()
};

#endif // AFX_RECURRENCEEDIT_H__4EE655E3_F4B1_44EA_8AAA_39DD459AD8A8__INCLUDED_
