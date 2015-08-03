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

#define GETSTR(fn) \
   gcnew String(m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : L""))

#define GETVAL_ARG(fn, arg, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : errret))

#define GETSTR_ARG(fn, arg) \
   gcnew String(m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : L""))

// -------------------------------------------------------------

String^ CTaskList::GetReportTitle()
{
   return GETSTR(GetReportTitle);
}

String^ CTaskList::GetReportDate()
{
   return GETSTR(GetReportDate);
}

String^ CTaskList::GetMetaData(String^ sKey)
{
   return GETSTR_ARG(GetMetaData, MS(sKey));
}

UInt32 CTaskList::GetCustomAttributeCount()
{
   return GETVAL(GetCustomAttributeCount, 0);
}

String^ CTaskList::GetCustomAttributeLabel(int nIndex)
{
   return GETSTR_ARG(GetCustomAttributeLabel, nIndex);
}

String^ CTaskList::GetCustomAttributeID(int nIndex)
{
   return GETSTR_ARG(GetCustomAttributeID, nIndex);
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

#define GETTASKDATE(fn, htask, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(htask, date) : (m_pTaskList ? m_pTaskList->fn(htask, date) : errret))

#define GETTASKSTR(fn, htask) \
   gcnew String(GETTASKVAL(fn, htask, L""))

#define GETTASKVAL_ARG(fn, htask, arg, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(htask, arg) : (m_pTaskList ? m_pTaskList->fn(htask, arg) : errret))

#define GETTASKSTR_ARG(fn, htask, arg) \
   gcnew String(GETTASKVAL_ARG(fn, htask, arg, L""))

#define GETTASKDATE_ARG(fn, htask, arg, errret) \
   (m_pConstTaskList ? m_pConstTaskList->fn(htask, arg, date) : (m_pTaskList ? m_pTaskList->fn(htask, arg, date) : errret))

// ---------------------------------------------------------

IntPtr CTaskList::GetFirstTask(IntPtr hParent)
{
   return IntPtr(GETTASKVAL(GetFirstTask, TASK(hParent), NULL));
}

IntPtr CTaskList::GetNextTask(IntPtr hTask)
{
   return IntPtr(GETTASKVAL(GetNextTask, TASK(hTask), NULL));
}

IntPtr CTaskList::GetTaskParent(IntPtr hTask)
{
   return IntPtr(GETTASKVAL(GetTaskParent, TASK(hTask), NULL));
}

String^ CTaskList::GetTaskTitle(IntPtr hTask)
{
   return GETTASKSTR(GetTaskTitle, TASK(hTask));
}

String^ CTaskList::GetTaskComments(IntPtr hTask)
{
   return GETTASKSTR(GetTaskComments, TASK(hTask));
}

String^ CTaskList::GetTaskAllocatedBy(IntPtr hTask)
{
   return GETTASKSTR(GetTaskAllocatedBy, TASK(hTask));
}

String^ CTaskList::GetTaskStatus(IntPtr hTask)
{
   return GETTASKSTR(GetTaskStatus, TASK(hTask));
}

String^ CTaskList::GetTaskWebColor(IntPtr hTask)
{
   return GETTASKSTR(GetTaskWebColor, TASK(hTask));
}

String^ CTaskList::GetTaskPriorityWebColor(IntPtr hTask)
{
   return GETTASKSTR(GetTaskPriorityWebColor, TASK(hTask));
}

String^ CTaskList::GetTaskVersion(IntPtr hTask)
{
   return GETTASKSTR(GetTaskVersion, TASK(hTask));
}

String^ CTaskList::GetTaskExternalID(IntPtr hTask)
{
   return GETTASKSTR(GetTaskExternalID, TASK(hTask));
}

String^ CTaskList::GetTaskCreatedBy(IntPtr hTask)
{
   return GETTASKSTR(GetTaskCreatedBy, TASK(hTask));
}

String^ CTaskList::GetTaskPositionString(IntPtr hTask)
{
   return GETTASKSTR(GetTaskPositionString, TASK(hTask));
}

String^ CTaskList::GetTaskIcon(IntPtr hTask)
{
   return GETTASKSTR(GetTaskIcon, TASK(hTask));
}

UInt32 CTaskList::GetTaskID(IntPtr hTask)
{
   return GETTASKVAL(GetTaskID, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskColor(IntPtr hTask)
{
   return GETTASKVAL(GetTaskColor, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskTextColor(IntPtr hTask)
{
   return GETTASKVAL(GetTaskTextColor, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskPriorityColor(IntPtr hTask)
{
   return GETTASKVAL(GetTaskPriorityColor, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskPosition(IntPtr hTask)
{
   return GETTASKVAL(GetTaskPosition, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskPriority(IntPtr hTask)
{
   return GETTASKVAL_ARG(GetTaskPriority, TASK(hTask), FALSE, 0);
}

UInt32 CTaskList::GetTaskRisk(IntPtr hTask)
{
   return GETTASKVAL_ARG(GetTaskRisk, TASK(hTask), FALSE, 0);
}

UInt32 CTaskList::GetTaskCategoryCount(IntPtr hTask)
{
   return GETTASKVAL(GetTaskCategoryCount, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskAllocatedToCount(IntPtr hTask)
{
   return GETTASKVAL(GetTaskAllocatedToCount, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskTagCount(IntPtr hTask)
{
   return GETTASKVAL(GetTaskTagCount, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskDependencyCount(IntPtr hTask)
{
   return GETTASKVAL(GetTaskDependencyCount, TASK(hTask), 0);
}

UInt32 CTaskList::GetTaskFileReferenceCount(IntPtr hTask)
{
   return GETTASKVAL(GetTaskFileReferenceCount, TASK(hTask), 0);
}

String^ CTaskList::GetTaskAllocatedTo(IntPtr hTask, int nIndex)
{
   return GETTASKSTR_ARG(GetTaskAllocatedTo, TASK(hTask), nIndex);
}

String^ CTaskList::GetTaskCategory(IntPtr hTask, int nIndex)
{
   return GETTASKSTR_ARG(GetTaskCategory, TASK(hTask), nIndex);
}

String^ CTaskList::GetTaskTag(IntPtr hTask, int nIndex)
{
   return GETTASKSTR_ARG(GetTaskTag, TASK(hTask), nIndex);
}

String^ CTaskList::GetTaskDependency(IntPtr hTask, int nIndex)
{
   return GETTASKSTR_ARG(GetTaskDependency, TASK(hTask), nIndex);
}

String^ CTaskList::GetTaskFileReference(IntPtr hTask, int nIndex)
{
   return GETTASKSTR_ARG(GetTaskFileReference, TASK(hTask), nIndex);
}

Byte CTaskList::GetTaskPercentDone(IntPtr hTask)
{
   return GETTASKVAL_ARG(GetTaskPercentDone, TASK(hTask), FALSE, 0);
}

double CTaskList::GetTaskCost(IntPtr hTask)
{
   return GETTASKVAL_ARG(GetTaskCost, TASK(hTask), FALSE, 0);
}

Int64 CTaskList::GetTaskLastModified(IntPtr hTask)
{
   __int64 date = 0;
   GETTASKDATE(GetTaskLastModified64, TASK(hTask), 0);

   return date;
}

Int64 CTaskList::GetTaskDoneDate(IntPtr hTask)
{
   __int64 date = 0;
   GETTASKDATE(GetTaskDoneDate64, TASK(hTask), 0);

   return date;
}

Int64 CTaskList::GetTaskDueDate(IntPtr hTask)
{
   __int64 date = 0;
   GETTASKDATE_ARG(GetTaskDueDate64, TASK(hTask), FALSE, 0);

   return date;
}

Int64 CTaskList::GetTaskStartDate(IntPtr hTask)
{
   __int64 date = 0;
   GETTASKDATE_ARG(GetTaskStartDate64, TASK(hTask), FALSE, 0);

   return date;
}

Int64 CTaskList::GetTaskCreationDate(IntPtr hTask)
{
   __int64 date = 0;
   GETTASKDATE(GetTaskCreationDate64, TASK(hTask), 0);

   return date;
}

String^ CTaskList::GetTaskDoneDateString(IntPtr hTask)
{
   return GETTASKSTR(GetTaskDoneDateString, TASK(hTask));
}

String^ CTaskList::GetTaskDueDateString(IntPtr hTask)
{
   return GETTASKSTR_ARG(GetTaskDueDateString, TASK(hTask), FALSE);
}

String^ CTaskList::GetTaskStartDateString(IntPtr hTask)
{
   return GETTASKSTR_ARG(GetTaskStartDateString, TASK(hTask), FALSE);
}

String^ CTaskList::GetTaskCreationDateString(IntPtr hTask)
{
   return GETTASKSTR(GetTaskCreationDateString, TASK(hTask));
}

Boolean CTaskList::IsTaskDone(IntPtr hTask)
{
   return GETTASKVAL(IsTaskDone, TASK(hTask), false);
}

Boolean CTaskList::IsTaskDue(IntPtr hTask)
{
   return GETTASKVAL(IsTaskDue, TASK(hTask), false);
}

Boolean CTaskList::IsTaskGoodAsDone(IntPtr hTask)
{
   return GETTASKVAL(IsTaskGoodAsDone, TASK(hTask), false);
}

Boolean CTaskList::IsTaskFlagged(IntPtr hTask)
{
   return GETTASKVAL(IsTaskFlagged, TASK(hTask), false);
}

// ---------------------------------------------------------

double CTaskList::GetTaskTimeEstimate(IntPtr hTask, Char% cUnits)
{
   WCHAR nUnits = 0;

   double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeEstimate(TASK(hTask), nUnits, FALSE) :
                  (m_pTaskList ? m_pTaskList->GetTaskTimeEstimate(TASK(hTask), nUnits, FALSE) : 0.0));

   cUnits = nUnits;
   return dTime;
}

double CTaskList::GetTaskTimeSpent(IntPtr hTask, Char% cUnits)
{
   WCHAR nUnits = 0;

   double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeSpent(TASK(hTask), nUnits, FALSE) :
                  (m_pTaskList ? m_pTaskList->GetTaskTimeSpent(TASK(hTask), nUnits, FALSE) : 0.0));

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

String^ CTaskList::GetTaskMetaData(IntPtr hTask, String^ sKey)
{
   LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskMetaData(TASK(hTask), MS(sKey)) : 
                     (m_pTaskList ? m_pTaskList->GetTaskMetaData(TASK(hTask), MS(sKey)) : L""));

   return gcnew String(szValue);
}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////
// SETTERS

IntPtr CTaskList::NewTask(String^ sTitle, IntPtr hParent)
{
   return IntPtr(m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), TASK(hParent), 0) : NULL);
}

Boolean CTaskList::AddCustomAttribute(String^ sID, String^ sLabel)
{
   return (m_pTaskList ? m_pTaskList->AddCustomAttribute(MS(sID), MS(sLabel)) : false);
}

Boolean CTaskList::SetMetaData(String^ sKey, String^ sValue)
{
   return (m_pTaskList ? m_pTaskList->SetMetaData(MS(sKey), MS(sValue)) : false);
}

Boolean CTaskList::ClearMetaData(String^ sKey)
{
   return (m_pTaskList ? m_pTaskList->ClearMetaData(MS(sKey)) : false);
}

// ---------------------------------------------------------

#define SETTASKVAL(fn, val) \
   (m_pTaskList ? m_pTaskList->fn(TASK(hTask), val) : false)

#define SETTASKSTR(fn, str) \
   (m_pTaskList ? m_pTaskList->fn(TASK(hTask), MS(str)) : false)

// ---------------------------------------------------------

Boolean CTaskList::SetTaskTitle(IntPtr hTask, String^ sTitle)
{
   return SETTASKSTR(SetTaskTitle, sTitle);
}

Boolean CTaskList::SetTaskComments(IntPtr hTask, String^ sComments)
{
   return SETTASKSTR(SetTaskComments, sComments);
}

Boolean CTaskList::SetTaskAllocatedBy(IntPtr hTask, String^ sAllocBy)
{
   return SETTASKSTR(SetTaskAllocatedBy, sAllocBy);
}

Boolean CTaskList::SetTaskStatus(IntPtr hTask, String^ sStatus)
{
   return SETTASKSTR(SetTaskStatus, sStatus);
}

Boolean CTaskList::SetTaskVersion(IntPtr hTask, String^ sVersion)
{
   return SETTASKSTR(SetTaskVersion, sVersion);
}

Boolean CTaskList::SetTaskExternalID(IntPtr hTask, String^ sExternalID)
{
   return SETTASKSTR(SetTaskExternalID, sExternalID);
}

Boolean CTaskList::SetTaskCreatedBy(IntPtr hTask, String^ sCreatedBy)
{
   return SETTASKSTR(SetTaskCreatedBy, sCreatedBy);
}

Boolean CTaskList::SetTaskPosition(IntPtr hTask, String^ sPosition)
{
   return SETTASKSTR(SetTaskPosition, sPosition);
}

Boolean CTaskList::SetTaskIcon(IntPtr hTask, String^ sIcon)
{
   return SETTASKSTR(SetTaskIcon, sIcon);
}

Boolean CTaskList::AddTaskAllocatedTo(IntPtr hTask, String^ sAllocTo)
{
   return SETTASKSTR(AddTaskAllocatedTo, sAllocTo);
}

Boolean CTaskList::AddTaskCategory(IntPtr hTask, String^ sCategory)
{
   return SETTASKSTR(AddTaskCategory, sCategory);
}

Boolean CTaskList::AddTaskTag(IntPtr hTask, String^ sTag)
{
   return SETTASKSTR(AddTaskTag, sTag);
}

Boolean CTaskList::AddTaskDependency(IntPtr hTask, String^ sDependency)
{
   return SETTASKSTR(AddTaskDependency, sDependency);
}

Boolean CTaskList::AddTaskFileReference(IntPtr hTask, String^ sFileLink)
{
   return SETTASKSTR(AddTaskFileReference, sFileLink);
}

Boolean CTaskList::SetTaskColor(IntPtr hTask, UINT32 color)
{
   return SETTASKVAL(SetTaskColor, color);
}

Boolean CTaskList::SetTaskPriority(IntPtr hTask, Byte nPriority)
{
   return SETTASKVAL(SetTaskPriority, nPriority);
}

Boolean CTaskList::SetTaskRisk(IntPtr hTask, Byte Risk)
{
   return SETTASKVAL(SetTaskRisk, Risk);
}

Boolean CTaskList::SetTaskPercentDone(IntPtr hTask, Byte nPercent)
{
   return SETTASKVAL(SetTaskPercentDone, nPercent);
}

Boolean CTaskList::SetTaskCost(IntPtr hTask, double dCost)
{
   return SETTASKVAL(SetTaskCost, dCost);
}

Boolean CTaskList::SetTaskFlag(IntPtr hTask, Boolean bFlag)
{
   return SETTASKVAL(SetTaskFlag, bFlag);
}

Boolean CTaskList::SetTaskLastModified(IntPtr hTask, Int64 dtLastMod)
{
   return SETTASKVAL(SetTaskLastModified, dtLastMod);
}

Boolean CTaskList::SetTaskDoneDate(IntPtr hTask, Int64 dtCompletion)
{
   return SETTASKVAL(SetTaskDoneDate, dtCompletion);
}

Boolean CTaskList::SetTaskDueDate(IntPtr hTask, Int64 dtDue)
{
   return SETTASKVAL(SetTaskDueDate, dtDue);
}

Boolean CTaskList::SetTaskStartDate(IntPtr hTask, Int64 dtStart)
{
   return SETTASKVAL(SetTaskStartDate, dtStart);
}

Boolean CTaskList::SetTaskCreationDate(IntPtr hTask, Int64 dtCreation)
{
   return SETTASKVAL(SetTaskCreationDate, dtCreation);
}

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

Boolean CTaskList::SetTaskMetaData(IntPtr hTask, String^ sKey, String^ sValue)
{
   return (m_pTaskList ? m_pTaskList->SetTaskMetaData(TASK(hTask), MS(sKey), MS(sValue)) : false);
}

Boolean CTaskList::ClearTaskMetaData(IntPtr hTask, String^ sKey)
{
   return (m_pTaskList ? m_pTaskList->ClearTaskMetaData(TASK(hTask), MS(sKey)) : false);
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
