// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"

#include "..\Interfaces\ITasklist.h"
#include "..\Interfaces\ITransText.h"
#include "..\Interfaces\IPreferences.h"

using namespace PluginHelpers;
using namespace System;
using namespace System::Runtime::InteropServices;

////////////////////////////////////////////////////////////////////////////////////////////////

// converts System::String to LPCWSTR, and frees memory on exit
class CMarshallString
{
public:
   CMarshallString(String^ str) : m_wszGlobal(NULL)
   {
      m_wszGlobal = (LPCWSTR)Marshal::StringToHGlobalUni(str).ToPointer();
   }

   ~CMarshallString()
   {
      Marshal::FreeHGlobal((IntPtr)(void*)m_wszGlobal);
   }

   operator LPCWSTR() { return m_wszGlobal; }

private:
   LPCWSTR m_wszGlobal;
};

#define MS(str) CMarshallString(str)

////////////////////////////////////////////////////////////////////////////////////////////////

CPreferences::CPreferences(IPreferences* pPrefs) : m_pPrefs(pPrefs) 
{
   int breakpoint = 0;
} 

// private constructor
CPreferences::CPreferences() : m_pPrefs(NULL) 
{

}

// ---------------------------------------------------------

#define GETPREF(fn, defval) \
   (m_pPrefs ? m_pPrefs->fn(MS(sSection), MS(sEntry), defval) : defval)

#define SETPREF(fn, val) \
   (m_pPrefs ? (m_pPrefs->fn(MS(sSection), MS(sEntry), val) != FALSE) : false)

// ---------------------------------------------------------

int CPreferences::GetProfileInt(String^ sSection, String^ sEntry, int nDefault)
{
   return GETPREF(GetProfileInt, nDefault);
}

bool CPreferences::WriteProfileInt(String^ sSection, String^ sEntry, int nValue)
{
   return SETPREF(WriteProfileInt, nValue);
}

String^ CPreferences::GetProfileString(String^ sSection, String^ sEntry, String^ sDefault)
{
   return gcnew String(GETPREF(GetProfileString, MS(sDefault)));
}

bool CPreferences::WriteProfileString(String^ sSection, String^ sEntry, String^ sValue)
{
   return SETPREF(WriteProfileString, MS(sValue));
}

double CPreferences::GetProfileDouble(String^ sSection, String^ sEntry, double dDefault)
{
   return GETPREF(GetProfileDouble, dDefault);
}

bool CPreferences::WriteProfileDouble(String^ sSection, String^ sEntry, double dValue)
{
   return SETPREF(WriteProfileDouble, dValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// CTaskList 
CTaskList::CTaskList(ITaskList14* pTaskList) : m_pTaskList(pTaskList), m_pConstTaskList(NULL) 
{
   int breakpoint = 0;
} 

CTaskList::CTaskList(const ITaskList14* pTaskList) : m_pTaskList(NULL), m_pConstTaskList(pTaskList) 
{
   int breakpoint = 0;
} 

// private constructor
CTaskList::CTaskList() : m_pTaskList(NULL), m_pConstTaskList(NULL)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////

#define TASK(t) ((HTASKITEM)t.ToPointer())

////////////////////////////////////////////////////////////////////////////////////////////////
// GETTERS

#define GETVAL(fn, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : errret))

#define GETVAL_ARG(fn, arg, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : errret))

String^ CTaskList::GetReportTitle()
{
   return gcnew String(GETVAL(GetReportTitle, L""));
}

String^ CTaskList::GetReportDate()
{
   return gcnew String(GETVAL(GetReportDate, L""));
}

UInt32 CTaskList::GetCustomAttributeCount()
{
   return GETVAL(GetCustomAttributeCount, 0);
}

String^ CTaskList::GetCustomAttributeLabel(int nIndex)
{
   return gcnew String(GETVAL_ARG(GetCustomAttributeLabel, nIndex, L""));
}

String^ CTaskList::GetCustomAttributeID(int nIndex)
{
   return gcnew String(GETVAL_ARG(GetCustomAttributeID, nIndex, L""));
}

String^ CTaskList::GetCustomAttributeValue(int nIndex, String^ sItem)
{
    LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : 
                      (m_pTaskList ? m_pTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : L""));
   
    return gcnew String(szValue);
}

UInt32 CTaskList::GetTaskCount()
{
   return GETVAL(GetTaskCount, 0);
}

IntPtr CTaskList::FindTask(UInt32 dwTaskID)
{
   return IntPtr(GETVAL_ARG(FindTask, dwTaskID, NULL));
}

// ---------------------------------------------------------

#define GETTASKVAL(fn, htask, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(htask) : (m_pTaskList ? m_pTaskList->fn(htask) : errret))

#define GETTASKVAL_ARG(fn, htask, arg, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(htask, arg) : (m_pTaskList ? m_pTaskList->fn(htask, arg) : errret))

#define IMPL_GETTASKSTRFUNC(fn) \
   String^ CTaskList::fn(IntPtr hTask) { \
   return gcnew String(GETTASKVAL(fn, TASK(hTask), NULL)); }

#define IMPL_GETTASKSTRFUNC_IDX(fn) \
   String^ CTaskList::fn(IntPtr hTask, int nIndex) { \
   return gcnew String(GETTASKVAL_ARG(fn, TASK(hTask), nIndex, NULL)); }

#define IMPL_GETTASKSTRFUNC_ARG(fn, arg) \
   String^ CTaskList::fn(IntPtr hTask) { \
   return gcnew String(GETTASKVAL_ARG(fn, TASK(hTask), arg, NULL)); }

#define IMPL_GETTASKVALFUNC(fn, t, errret) \
   t CTaskList::fn(IntPtr hTask) { \
   return t(GETTASKVAL(fn, TASK(hTask), errret)); }

#define IMPL_GETTASKVALFUNC_ARG(fn, t, arg, errret) \
   t CTaskList::fn(IntPtr hTask) { \
   return t(GETTASKVAL_ARG(fn, TASK(hTask), arg, errret)); }

// ---------------------------------------------------------

IntPtr CTaskList::GetFirstTask(IntPtr hParent)
{
   return IntPtr(GETTASKVAL(GetFirstTask, TASK(hParent), NULL));
}

IntPtr CTaskList::GetNextTask(IntPtr hTask)
{
   return IntPtr(GETTASKVAL(GetNextTask, TASK(hTask), NULL));
}

IMPL_GETTASKSTRFUNC(GetTaskTitle)
IMPL_GETTASKSTRFUNC(GetTaskComments)
IMPL_GETTASKSTRFUNC(GetTaskAllocatedBy)
IMPL_GETTASKSTRFUNC(GetTaskStatus)
IMPL_GETTASKSTRFUNC(GetTaskWebColor)
IMPL_GETTASKSTRFUNC(GetTaskPriorityWebColor)
IMPL_GETTASKSTRFUNC(GetTaskVersion)
IMPL_GETTASKSTRFUNC(GetTaskExternalID)
IMPL_GETTASKSTRFUNC(GetTaskCreatedBy)
IMPL_GETTASKSTRFUNC(GetTaskIcon)

IMPL_GETTASKSTRFUNC(GetTaskPositionString)
IMPL_GETTASKSTRFUNC(GetTaskDoneDateString)
IMPL_GETTASKSTRFUNC(GetTaskCreationDateString)

IMPL_GETTASKSTRFUNC_IDX(GetTaskAllocatedTo)
IMPL_GETTASKSTRFUNC_IDX(GetTaskCategory)
IMPL_GETTASKSTRFUNC_IDX(GetTaskTag)
IMPL_GETTASKSTRFUNC_IDX(GetTaskDependency)
IMPL_GETTASKSTRFUNC_IDX(GetTaskFileReference)

IMPL_GETTASKVALFUNC(GetTaskAllocatedToCount,	UInt32, 0)
IMPL_GETTASKVALFUNC(GetTaskCategoryCount,		UInt32, 0)
IMPL_GETTASKVALFUNC(GetTaskTagCount,			UInt32, 0)
IMPL_GETTASKVALFUNC(GetTaskDependencyCount,		UInt32, 0)
IMPL_GETTASKVALFUNC(GetTaskFileReferenceCount,	UInt32, 0)

IMPL_GETTASKSTRFUNC_ARG(GetTaskDueDateString,   false)
IMPL_GETTASKSTRFUNC_ARG(GetTaskStartDateString, false)

// other types
IMPL_GETTASKVALFUNC(GetTaskParent,        IntPtr,  NULL)
IMPL_GETTASKVALFUNC(GetTaskID,            UInt32,  0)
IMPL_GETTASKVALFUNC(GetTaskColor,         UInt32,  0)
IMPL_GETTASKVALFUNC(GetTaskTextColor,     UInt32,  0)
IMPL_GETTASKVALFUNC(GetTaskPriorityColor, UInt32,  0)
IMPL_GETTASKVALFUNC(GetTaskPosition,      UInt32,  0)

IMPL_GETTASKVALFUNC(GetTaskLastModified,  Int64,   0)
IMPL_GETTASKVALFUNC(GetTaskDoneDate,      Int64,   0)
IMPL_GETTASKVALFUNC(GetTaskCreationDate,  Int64,   0)

IMPL_GETTASKVALFUNC(IsTaskDone,           Boolean, false)
IMPL_GETTASKVALFUNC(IsTaskDue,            Boolean, false)
IMPL_GETTASKVALFUNC(IsTaskGoodAsDone,     Boolean, false)
IMPL_GETTASKVALFUNC(IsTaskFlagged,        Boolean, false)

IMPL_GETTASKVALFUNC_ARG(GetTaskDueDate,      Int64,   FALSE, 0)
IMPL_GETTASKVALFUNC_ARG(GetTaskStartDate,    Int64,   FALSE, 0)
IMPL_GETTASKVALFUNC_ARG(GetTaskPriority,     UInt32,  FALSE, 0)
IMPL_GETTASKVALFUNC_ARG(GetTaskPercentDone,  Byte,    FALSE, 0)
IMPL_GETTASKVALFUNC_ARG(GetTaskRisk,	     UInt32,  FALSE, 0)
IMPL_GETTASKVALFUNC_ARG(GetTaskCost,	     double,  FALSE, 0.0)

// ---------------------------------------------------------

double CTaskList::GetTaskTimeEstimate(IntPtr hTask, Char% cUnits)
{
   WCHAR nUnits = 0;
   double dTime = 0.0;

   if (m_pConstTaskList)
      dTime = m_pConstTaskList->GetTaskTimeEstimate(TASK(hTask), nUnits, FALSE);
   else 
      dTime = m_pTaskList->GetTaskTimeEstimate(TASK(hTask), nUnits, FALSE);

   cUnits = nUnits;
   return dTime;
}

double CTaskList::GetTaskTimeSpent(IntPtr hTask, Char% cUnits)
{
   WCHAR nUnits = 0;
   double dTime = 0.0;

   if (m_pConstTaskList)
      dTime = m_pConstTaskList->GetTaskTimeSpent(TASK(hTask), nUnits, FALSE);
   else 
      dTime = m_pTaskList->GetTaskTimeSpent(TASK(hTask), nUnits, FALSE);

   cUnits = nUnits;
   return dTime;
}

Boolean CTaskList::GetTaskRecurrence(IntPtr hTask)
{
	// TODO
	return false;
}

Boolean CTaskList::TaskHasAttribute(IntPtr hTask, String^ sAttrib)
{
   return (m_pConstTaskList ? m_pConstTaskList->TaskHasAttribute(TASK(hTask), MS(sAttrib)) : 
          (m_pTaskList ? m_pTaskList->TaskHasAttribute(TASK(hTask), MS(sAttrib)) : false));
}

String^ CTaskList::GetTaskAttribute(IntPtr hTask, String^ sAttrib)
{
   LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskAttribute(TASK(hTask), MS(sAttrib)) : 
                     (m_pTaskList ? m_pTaskList->GetTaskAttribute(TASK(hTask), MS(sAttrib)) : L""));

   return gcnew String(szValue);
}

String^ CTaskList::GetTaskCustomAttributeData(IntPtr hTask, String^ sID)
{
   LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskCustomAttributeData(TASK(hTask), MS(sID)) : 
                     (m_pTaskList ? m_pTaskList->GetTaskCustomAttributeData(TASK(hTask), MS(sID)) : L""));

   return gcnew String(szValue);
}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////
// SETTERS

IntPtr CTaskList::NewTask(String^ sTitle, IntPtr hParent)
{
   return IntPtr(m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), TASK(hParent), 0) : NULL);
}

// ---------------------------------------------------------

#define SETTASKVAL(fn, htask, val) \
   (m_pTaskList ? m_pTaskList->fn(htask, val) : false)

#define IMPL_SETTASKSTRFUNC(fn) \
   Boolean CTaskList::fn(IntPtr hTask, String^ value) { \
   return SETTASKVAL(fn, TASK(hTask), MS(value)); }

#define IMPL_SETTASKVALFUNC(fn, t) \
   Boolean CTaskList::fn(IntPtr hTask, t value) { \
   return SETTASKVAL(fn, TASK(hTask), value); }

// ---------------------------------------------------------

IMPL_SETTASKSTRFUNC(SetTaskTitle);
IMPL_SETTASKSTRFUNC(SetTaskComments);
IMPL_SETTASKSTRFUNC(SetTaskAllocatedBy);
IMPL_SETTASKSTRFUNC(SetTaskStatus);
IMPL_SETTASKSTRFUNC(SetTaskVersion);
IMPL_SETTASKSTRFUNC(SetTaskExternalID);
IMPL_SETTASKSTRFUNC(SetTaskCreatedBy);
IMPL_SETTASKSTRFUNC(SetTaskPosition);
IMPL_SETTASKSTRFUNC(SetTaskIcon);

IMPL_SETTASKSTRFUNC(AddTaskAllocatedTo);
IMPL_SETTASKSTRFUNC(AddTaskCategory);
IMPL_SETTASKSTRFUNC(AddTaskTag);
IMPL_SETTASKSTRFUNC(AddTaskDependency);
IMPL_SETTASKSTRFUNC(AddTaskFileReference);

IMPL_SETTASKVALFUNC(SetTaskColor,              UInt32);
IMPL_SETTASKVALFUNC(SetTaskPriority,           UInt32);
IMPL_SETTASKVALFUNC(SetTaskRisk,               UInt32);

IMPL_SETTASKVALFUNC(SetTaskPercentDone,        Byte);
IMPL_SETTASKVALFUNC(SetTaskCost,               double);
IMPL_SETTASKVALFUNC(SetTaskFlag,               Boolean);

IMPL_SETTASKVALFUNC(SetTaskLastModified,       Int64);
IMPL_SETTASKVALFUNC(SetTaskDoneDate,           Int64);
IMPL_SETTASKVALFUNC(SetTaskDueDate,            Int64);
IMPL_SETTASKVALFUNC(SetTaskStartDate,          Int64);
IMPL_SETTASKVALFUNC(SetTaskCreationDate,       Int64);

// ---------------------------------------------------------

Boolean CTaskList::SetTaskTimeEstimate(IntPtr hTask, double dTime, Char cUnits)
{
   return (m_pTaskList ? m_pTaskList->SetTaskTimeEstimate(TASK(hTask), dTime, cUnits) : false);
}

Boolean CTaskList::SetTaskTimeSpent(IntPtr hTask, double dTime, Char cUnits)
{
   return (m_pTaskList ? m_pTaskList->SetTaskTimeSpent(TASK(hTask), dTime, cUnits) : false);
}

Boolean CTaskList::SetTaskCustomAttributeData(IntPtr hTask, String^ sID, String^ sValue)
{
   return (m_pTaskList ? m_pTaskList->SetTaskCustomAttributeData(TASK(hTask), MS(sID), MS(sValue)) : false);
}

Boolean CTaskList::ClearTaskCustomAttributeData(IntPtr hTask, String^ sID)
{
   return (m_pTaskList ? m_pTaskList->ClearTaskCustomAttributeData(TASK(hTask), MS(sID)) : false);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CTransText::CTransText(ITransText* pTransText) : m_pTransText(pTransText) 
{
   int breakpoint = 0;
} 

// private constructor
CTransText::CTransText() : m_pTransText(NULL)
{

}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////
