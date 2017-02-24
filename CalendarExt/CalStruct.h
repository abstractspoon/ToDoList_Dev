#if !defined(AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskCalendarCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "..\shared\mapex.h"

#include "..\Interfaces\ITaskList.h"
#include "..\Interfaces\IUIExtension.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

struct TASKCALITEM
{
public:
	TASKCALITEM();
	TASKCALITEM(const TASKCALITEM& tci);
	TASKCALITEM(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates);

	TASKCALITEM& TASKCALITEM::operator=(const TASKCALITEM& tci);
	BOOL TASKCALITEM::operator==(const TASKCALITEM& tci);

	BOOL UpdateTask(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates);
	void RecalcDates(DWORD dwCalcDates);
	inline DWORD GetTaskID() const { return dwTaskID; }

	BOOL IsValid() const;
	BOOL IsDone(BOOL bIncGoodAs) const;
	
	BOOL IsStartDateSet() const;
	void SetStartDate(const COleDateTime& date);

	COleDateTime GetAnyStartDate() const;
	BOOL HasAnyStartDate() const;

	BOOL IsEndDateSet() const;
	void SetEndDate(const COleDateTime& date);

	COleDateTime GetAnyEndDate() const;
	BOOL HasAnyEndDate() const;

	COLORREF GetFillColor(BOOL bTextIsBack) const;
	COLORREF GetBorderColor(BOOL bTextIsBack) const;
	COLORREF GetTextColor(BOOL bSelected, BOOL bTextIsBack) const;
	BOOL HasColor() const;

	CString GetName() const;

public:
	COLORREF color;
	BOOL bDone, bGoodAsDone, bTopLevel;

protected:
	COleDateTime dtCreation, dtStart, dtEnd;
	COleDateTime dtStartCalc, dtEndCalc;
	CString sName, sFormattedName;
	DWORD dwTaskID;

protected:
	void UpdateTaskDates(const ITaskList14* pTasks, HTASKITEM hTask, const CSet<IUI_ATTRIBUTE>& attrib, DWORD dwCalcDates);
	void ReformatName();

	static COleDateTime GetDate(time64_t tDate);
};

/////////////////////////////////////////////////////////////////////////////

typedef CArray<TASKCALITEM*, TASKCALITEM*&> CTaskCalItemArray;
typedef CArray<const TASKCALITEM*, const TASKCALITEM*&> CTaskCalItemConstArray;
typedef CMap<DWORD, DWORD, TASKCALITEM*, TASKCALITEM*&> CTaskCalItemMap;
typedef CMap<double, double, BOOL, BOOL&> CSpecialDateMap;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALSTRUCT_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
