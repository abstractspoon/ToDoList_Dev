#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

/////////////////////////////////////////////////////////////////////////////////////////////

class CEnCommandLineInfo;

/////////////////////////////////////////////////////////////////////////////////////////////

const int ATTRIBLEN		= 1024;
const int FILEPATHSLEN	= (10 * 1024);

/////////////////////////////////////////////////////////////////////////////////////////////

// NO POINTERS
class TDCSTARTUPATTRIB
{
public:
	TDCSTARTUPATTRIB();
	TDCSTARTUPATTRIB& operator=(const TDCSTARTUPATTRIB& other);

	BOOL operator==(const TDCSTARTUPATTRIB& other) const;
	BOOL operator==(const CString& sValue) const;

	BOOL IsEmpty() const;
	BOOL IsSet() const;
	void SetValue(const CString& sValue);
	BOOL GetValue(CString& sValue) const;
	BOOL GetValue(int& nValue, BOOL& bOffset) const;
	BOOL GetValue(double& nValue, BOOL& bOffset) const;
	LPCTSTR GetValue() const { return szValue; }
	void ClearValue();
	int GetValues(CStringArray& aItems, BOOL& bAppend) const;

	BOOL GetTimeOfDay(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const;
	BOOL GetTimePeriod(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const;
	BOOL GetDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const;
	BOOL IsOffset() const { return IsOffset(szValue); }
	
	static BOOL IsOffset(LPCTSTR szValue);

protected:
	TCHAR szValue[ATTRIBLEN + 1];
	BOOL bSet;
};

/////////////////////////////////////////////////////////////////////////////////////////////

// NO POINTERS
class CTDCStartupOptions
{
public:
	CTDCStartupOptions();
	CTDCStartupOptions(const CTDCStartupOptions& startup);
	CTDCStartupOptions(const CEnCommandLineInfo& cmdInfo);
	CTDCStartupOptions(const CString& sCmdInfo, int nStartArg = 0);

	CTDCStartupOptions& operator=(const CTDCStartupOptions& startup);
	BOOL operator==(const CTDCStartupOptions& startup) const;

	void Reset();
	BOOL IsEmpty(BOOL bIgnoreFlags = FALSE) const;
	BOOL HasFlag(DWORD dwFlag) const;
	BOOL ModifyFlags(DWORD dwRemove, DWORD dwAdd);

	DWORD GetTaskID() const { return m_dwIDSel; }
	DWORD GetParentTaskID() const { return m_dwParentID; }
	DWORD GetSiblingTaskID() const { return m_dwSiblingID; }
	DWORD GetFlags() const { return m_dwFlags; }

	BOOL GetSaveIntermediateAll() const;

	int GetCommandIDs(CUIntArray& aCmdIDs) const;
	BOOL HasCommandID() const { return !m_sCmdIDs.IsEmpty(); }
	CString GetTitlePrefix() const { return m_sTitlePrefix.GetValue(); }

	BOOL HasFilePath() const { return (lstrlen(m_szFilePaths) > 0); }
	int GetFilePaths(CStringArray& aFiles) const;

	BOOL WantCheckOut() const { return m_bCheckOut; }
	BOOL WantCheckIn() const { return m_bCheckIn; }

	BOOL HasNewTaskTitle() const { return !m_sNewTaskTitle.IsEmpty(); }
	BOOL GetNewTaskTitle(CString& sValue) const { return (!m_sNewTaskTitle.IsEmpty() && m_sNewTaskTitle.GetValue(sValue)); }
	BOOL GetTaskTitle(CString& sValue) const { return (!m_sTaskTitle.IsEmpty() && m_sTaskTitle.GetValue(sValue)); }
	BOOL GetTaskComments(CString& sValue) const { return m_sTaskComments.GetValue(sValue); }
	BOOL GetTaskExternalID(CString& sValue) const { return m_sTaskExternalID.GetValue(sValue); }
	BOOL GetTaskVersion(CString& sValue) const { return m_sTaskVersion.GetValue(sValue); }
	BOOL GetTaskAllocBy(CString& sValue) const { return m_sTaskAllocBy.GetValue(sValue); }
	BOOL GetTaskStatus(CString& sValue) const { return m_sTaskStatus.GetValue(sValue); }
	BOOL GetTaskFileLink(CString& sValue) const { return m_sTaskFileLink.GetValue(sValue); }
	BOOL GetTaskDependency(CString& sValue) const { return m_sTaskDepends.GetValue(sValue); }
	BOOL GetTaskIcon(CString& sValue) const { return m_sTaskIcon.GetValue(sValue); }

	BOOL GetTaskStartDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtTaskStartDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetTaskStartTime(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTaskStartTime.GetTimeOfDay(dValue, nUnits, bOffset); } 
	BOOL GetTaskDueDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtTaskDueDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetTaskDueTime(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTaskDueTime.GetTimeOfDay(dValue, nUnits, bOffset); }
	BOOL GetTaskDoneDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtTaskDoneDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetTaskDoneTime(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTaskDoneTime.GetTimeOfDay(dValue, nUnits, bOffset); } 
	BOOL GetTaskCreationDate(COleDateTime& dtValue) const;

	BOOL GetTaskPercentDone(int& nValue, BOOL& bOffset) const { return m_nTaskPercentDone.GetValue(nValue, bOffset); }
	BOOL GetTaskPriority(int& nValue, BOOL& bOffset) const;
	BOOL GetTaskRisk(int& nValue, BOOL& bOffset) const;

	BOOL GetTaskCost(double& dValue, BOOL& bOffset) const { return m_dTaskCost.GetValue(dValue, bOffset); }
	BOOL GetTaskTimeEst(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTaskTimeEst.GetTimePeriod(dValue, nUnits, bOffset); } 
	BOOL GetTaskTimeSpent(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTaskTimeSpent.GetTimePeriod(dValue, nUnits, bOffset); } 

	int GetTaskDependencies(CStringArray& aDepends, BOOL& bAppend) const { return m_sTaskDepends.GetValues(aDepends, bAppend); }
	int GetTaskCategories(CStringArray& aCats, BOOL& bAppend) const { return m_sTaskCategory.GetValues(aCats, bAppend); }
	int GetTaskAllocTo(CStringArray& aAllocTo, BOOL& bAppend) const { return m_sTaskAllocTo.GetValues(aAllocTo, bAppend); }
	int GetTaskTags(CStringArray& aTags, BOOL& bAppend) const { return m_sTaskTags.GetValues(aTags, bAppend); }
	int GetTaskFileLinks(CStringArray& aFiles, BOOL& bAppend) const { return m_sTaskFileLink.GetValues(aFiles, bAppend); }

	BOOL GetTaskCustomAttribute(CString& sCustomID, CString& sValue) const;

	BOOL GetCopyTaskAttribute(TDC_ATTRIBUTE& nFromAttrib, TDC_ATTRIBUTE& nToAttrib) const;
	BOOL GetCopyTaskAttribute(TDC_ATTRIBUTE& nFromAttrib, CString& sToCustomAttrib) const;
	BOOL GetCopyTaskAttribute(CString& sFromCustomAttrib, TDC_ATTRIBUTE& nToAttrib) const;
	BOOL GetCopyTaskAttribute(CString& sFromCustomAttrib, CString& sToCustomAttrib) const;
	
protected:
	TCHAR m_szFilePaths[FILEPATHSLEN + 1]; // tasklists to load/import
	DWORD m_dwIDSel; // task to select
	DWORD m_dwParentID; // parent task for new subtask
	DWORD m_dwSiblingID; // sibling task for new subtask

	TDCSTARTUPATTRIB m_sCmdIDs;
	TDCSTARTUPATTRIB m_sTitlePrefix;

	TDCSTARTUPATTRIB m_sNewTaskTitle, m_sTaskTitle, m_sTaskComments;
	TDCSTARTUPATTRIB m_sTaskVersion, m_sTaskExternalID, m_sTaskAllocBy, m_sTaskStatus;
	TDCSTARTUPATTRIB m_sTaskAllocTo, m_sTaskCategory, m_sTaskTags, m_sTaskDepends;
	TDCSTARTUPATTRIB m_sTaskCustomAttrib, m_sTaskIcon, m_sTaskFileLink;

	TDCSTARTUPATTRIB m_nTaskPriority, m_nTaskRisk, m_nTaskPercentDone;
	TDCSTARTUPATTRIB m_dtTaskCreateDate, m_dtTaskStartDate, m_dtTaskDueDate, m_dtTaskDoneDate;
	TDCSTARTUPATTRIB m_dTaskStartTime, m_dTaskDueTime, m_dTaskDoneTime;
	TDCSTARTUPATTRIB m_dTaskTimeEst, m_dTaskTimeSpent, m_dTaskCost;
	
	TDCSTARTUPATTRIB m_sCopyFromTaskAttrib, m_sCopyToTaskAttrib;

	DWORD m_dwFlags;
	BOOL m_bSaveIntermediateAll;
	BOOL m_bCheckOut, m_bCheckIn;

protected:
	void SetCmdInfo(const CEnCommandLineInfo& cmdInfo);

	static BOOL ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, LPTSTR szAttrib, int nLenAttrib = ATTRIBLEN);
	static BOOL ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& attrib);
	static void ParseDate(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& dtDate);
	static void ParseTime(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& dTime);

};
