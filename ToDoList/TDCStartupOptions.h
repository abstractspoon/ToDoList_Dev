#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////

#include "..\Interfaces\ITaskList.h"

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
	void SetValue(const CString& sValue);
	BOOL GetValue(CString& sValue) const;
	BOOL GetValue(int& nValue, BOOL& bOffset) const;
	BOOL GetValue(double& nValue, BOOL& bOffset) const;
	CString GetValue() const;
	void ClearValue();
	int GetValues(CStringArray& aItems, BOOL& bAppend) const;

	BOOL GetTime(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const;
	BOOL GetDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const;
	BOOL IsOffset() const { return IsOffset(szValue); }
	
	static BOOL IsOffset(LPCTSTR szValue);

protected:
	TCHAR szValue[ATTRIBLEN+1];
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

	UINT GetCommandID() const { return m_nCmdID; }
	BOOL HasCommandID() const { return (m_nCmdID != 0); }

	BOOL HasFilePath() const { return (lstrlen(m_szFilePaths) > 0); }
	int GetFilePaths(CStringArray& aFiles) const;

	BOOL HasNewTaskName() const { return !m_sNewTask.IsEmpty(); }
	BOOL GetNewTaskName(CString& sValue) const { return (!m_sNewTask.IsEmpty() && m_sNewTask.GetValue(sValue)); }
	BOOL GetComments(CString& sValue) const { return m_sComments.GetValue(sValue); }
	BOOL GetExternalID(CString& sValue) const { return m_sExternalID.GetValue(sValue); }
	BOOL GetVersion(CString& sValue) const { return m_sVersion.GetValue(sValue); }
	BOOL GetAllocBy(CString& sValue) const { return m_sAllocBy.GetValue(sValue); }
	BOOL GetStatus(CString& sValue) const { return m_sStatus.GetValue(sValue); }
	BOOL GetFileRef(CString& sValue) const { return m_sFileRef.GetValue(sValue); }
	BOOL GetDependency(CString& sValue) const { return m_sDepends.GetValue(sValue); }

	BOOL GetStartDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtStartDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetStartTime(double& dValue, BOOL& bOffset) const { return m_dStartTime.GetValue(dValue, bOffset); } 
	BOOL GetDueDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtDueDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetDueTime(double& dValue, BOOL& bOffset) const { return m_dDueTime.GetValue(dValue, bOffset); }
	BOOL GetDoneDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dtDoneDate.GetDate(dValue, nUnits, bOffset); } 
	BOOL GetDoneTime(double& dValue, BOOL& bOffset) const { return m_dDoneTime.GetValue(dValue, bOffset); } 
	BOOL GetCreationDate(COleDateTime& dtValue) const;

	BOOL GetPercentDone(int& nValue, BOOL& bOffset) const { return m_nPercentDone.GetValue(nValue, bOffset); }
	BOOL GetPriority(int& nValue, BOOL& bOffset) const;
	BOOL GetRisk(int& nValue, BOOL& bOffset) const;

	BOOL GetCost(double& dValue, BOOL& bOffset) const { return m_dCost.GetValue(dValue, bOffset); }
	BOOL GetTimeEst(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTimeEst.GetTime(dValue, nUnits, bOffset); } 
	BOOL GetTimeSpent(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const { return m_dTimeSpent.GetTime(dValue, nUnits, bOffset); } 

	int GetDependencies(CStringArray& aDepends, BOOL& bAppend) const { return m_sDepends.GetValues(aDepends, bAppend); }
	int GetCategories(CStringArray& aCats, BOOL& bAppend) const { return m_sCategory.GetValues(aCats, bAppend); }
	int GetAllocTo(CStringArray& aAllocTo, BOOL& bAppend) const { return m_sAllocTo.GetValues(aAllocTo, bAppend); }
	int GetTags(CStringArray& aTags, BOOL& bAppend) const { return m_sTags.GetValues(aTags, bAppend); }
	int GetFileRefs(CStringArray& aFiles, BOOL& bAppend) const { return m_sFileRef.GetValues(aFiles, bAppend); }

	BOOL GetCustomAttribute(CString& sCustomID, CString& sValue) const;
	
protected:
	TCHAR m_szFilePaths[FILEPATHSLEN+1]; // tasklists to load/import
	DWORD m_dwIDSel; // task to select
	DWORD m_dwParentID; // parent task for new subtask
	DWORD m_dwSiblingID; // sibling task for new subtask

	TDCSTARTUPATTRIB m_sNewTask, m_sComments;
	TDCSTARTUPATTRIB m_sVersion, m_sExternalID, m_sAllocBy, m_sStatus;
	TDCSTARTUPATTRIB m_sAllocTo, m_sCategory, m_sTags, m_sDepends, m_sFileRef;
	TDCSTARTUPATTRIB m_sCustomAttrib;

	TDCSTARTUPATTRIB m_nPriority, m_nRisk, m_nPercentDone;
	TDCSTARTUPATTRIB m_dtCreateDate, m_dtStartDate, m_dtDueDate, m_dtDoneDate;
	TDCSTARTUPATTRIB m_dStartTime, m_dDueTime, m_dDoneTime;
	TDCSTARTUPATTRIB m_dTimeEst, m_dTimeSpent, m_dCost;

	DWORD m_dwFlags;
	UINT m_nCmdID;
	BOOL m_bSaveIntermediateAll;

protected:
	static BOOL ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, LPTSTR szAttrib, int nLenAttrib = ATTRIBLEN);
	static BOOL ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& attrib);
	static void ParseDate(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& dtDate);
	static void ParseTime(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& dTime);

};
