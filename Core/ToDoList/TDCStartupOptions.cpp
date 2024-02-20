#include "stdafx.h"
#include "TDCStartupOptions.h"
#include "tdcenum.h"
#include "tdcswitch.h"
#include "tdcmapping.h"
#include "todoitem.h"

#include "..\Shared\EnCommandLineInfo.h"
#include "..\Shared\misc.h"
#include "..\Shared\datehelper.h"
#include "..\Shared\timehelper.h"

/////////////////////////////////////////////////////////////////////////////////////////////

TDCSTARTUPATTRIB::TDCSTARTUPATTRIB() : bSet(FALSE)
{
	szValue[0] = 0;
}

TDCSTARTUPATTRIB& TDCSTARTUPATTRIB::operator=(const TDCSTARTUPATTRIB& other)
{
	bSet = other.bSet;
	lstrcpyn(szValue, other.szValue, ATTRIBLEN);

	return *this;
}

BOOL TDCSTARTUPATTRIB::operator==(const TDCSTARTUPATTRIB& other) const
{
	return ((!bSet && !other.bSet) || (bSet && other.bSet && (_tcscmp(szValue, other.szValue) == 0)));
}

BOOL TDCSTARTUPATTRIB::operator==(const CString& sValue) const
{
	return (bSet && (sValue == szValue));
}

BOOL TDCSTARTUPATTRIB::IsEmpty() const 
{ 
	return (!IsSet() || !lstrlen(szValue)); 
}

BOOL TDCSTARTUPATTRIB::IsSet() const
{
	return bSet;
}

void TDCSTARTUPATTRIB::SetValue(const CString& sValue)
{
	bSet = TRUE;
	lstrcpyn(szValue, sValue, ATTRIBLEN);
}

BOOL TDCSTARTUPATTRIB::GetValue(CString& sValue) const
{
	if (bSet)
	{
		sValue = szValue;
		return TRUE;
	}

	return FALSE;
}

BOOL TDCSTARTUPATTRIB::GetValue(int& nValue, BOOL& bOffset) const
{
	if (bSet && Misc::IsNumber(szValue))
	{
		nValue = _ttoi(szValue);
		bOffset = IsOffset(szValue);

		return TRUE;
	}

	return FALSE;
}

BOOL TDCSTARTUPATTRIB::GetValue(double& dValue, BOOL& bOffset) const
{
	if (bSet && Misc::IsNumber(szValue))
	{
		dValue = _ttof(szValue);
		bOffset = IsOffset(szValue);

		return TRUE;
	}

	return FALSE;
}

int TDCSTARTUPATTRIB::GetValues(CStringArray& aItems, BOOL& bAppend) const 
{ 
	aItems.RemoveAll();

	if (!bSet)
		return -1;

	bAppend = FALSE;
	CString sValue(GetValue());

	if (!sValue.IsEmpty())
	{
		if (sValue[0] == '+')
		{
			bAppend = TRUE;
			sValue = sValue.Mid(1);
		}

		return Misc::Split(sValue, aItems, '|'); 
	}
	
	return 0;
}

void TDCSTARTUPATTRIB::ClearValue()
{
	bSet = FALSE;
	::ZeroMemory(szValue, (sizeof(szValue) / sizeof(szValue[0])));
}

BOOL TDCSTARTUPATTRIB::IsOffset(LPCTSTR szValue)
{
	return (szValue && ((szValue[0] == '+') || (szValue[0] == '-')));
}

BOOL TDCSTARTUPATTRIB::GetTimePeriod(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const
{
	if (!bSet)
		return FALSE;

	TH_UNITS nTHUnits = THU_NULL;

	if (IsOffset(szValue) && CTimeHelper::DecodeOffset(szValue, dValue, nTHUnits, TRUE))
	{
		bOffset = TRUE;
	}
	else if (CTimeHelper::DecodeOffset(szValue, dValue, nTHUnits, FALSE)) // Decode as plain number
	{
		bOffset = FALSE;
	}
	else if (Misc::IsEmpty(szValue))
	{
		dValue = 0;
		nTHUnits = THU_HOURS;
		bOffset = FALSE;
	}
	else
	{
		bOffset = -1; // error
		return FALSE;
	}

	nUnits = TDC::MapTHUnitsToUnits(nTHUnits);
	return TRUE;
}

BOOL TDCSTARTUPATTRIB::GetDate(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const
{
	if (!bSet)
		return FALSE;
	
	DH_UNITS nDHUnits = DHU_NULL;
	int nOffset = 0; // We only support integer offsets

	if (IsOffset(szValue) && CDateHelper::DecodeOffset(szValue, nOffset, nDHUnits, TRUE))
	{
		dValue = nOffset;
		bOffset = TRUE;
	}
	else if (CDateHelper::DecodeOffset(szValue, nOffset, nDHUnits, FALSE)) // Decode as plain number
	{
		dValue = _ttof(szValue);
		bOffset = FALSE;
	}
	else if (Misc::IsEmpty(szValue))
	{
		dValue = 0.0;
		nDHUnits = DHU_DAYS;
		bOffset = FALSE;
	}
	else
	{
		bOffset = -1; // error
		return FALSE;
	}

	nUnits = TDC::MapDHUnitsToUnits(nDHUnits);
	return TRUE;
}

BOOL TDCSTARTUPATTRIB::GetTimeOfDay(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const
{
	if (!bSet)
		return FALSE;

	// We support offsets in hours and minutes only
	if (GetTimePeriod(dValue, nUnits, bOffset) && bOffset)
	{
		switch (nUnits)
		{
		case TDCU_MINS:
		case TDCU_HOURS:
			return TRUE;
		}

		ASSERT(0);
		bOffset = FALSE;
		return FALSE;
	}

	// else
	nUnits = TDCU_DAYS;
	return GetValue(dValue, bOffset); 
}

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCNullDate : public COleDateTime
{
public:
	CTDCNullDate()
	{
		CDateHelper::ClearDate(*this);
	}
};

static const CTDCNullDate NULLDATE;

/////////////////////////////////////////////////////////////////////////////////////////////

CTDCStartupOptions::CTDCStartupOptions() 
{
	Reset();
}

CTDCStartupOptions::CTDCStartupOptions(const CTDCStartupOptions& startup)
{
	*this = startup;
}

CTDCStartupOptions::CTDCStartupOptions(const CEnCommandLineInfo& cmdInfo)
{
	SetCmdInfo(cmdInfo);
}

CTDCStartupOptions::CTDCStartupOptions(const CString& sCmdInfo, int nFirstArg)
{
	Reset();

	CEnCommandLineInfo cmdInfo(_T(".tdl;.xml"));

	if (cmdInfo.SetCommandLine(sCmdInfo, nFirstArg) || !cmdInfo.m_strFileName.IsEmpty())
		SetCmdInfo(cmdInfo);
}

void CTDCStartupOptions::SetCmdInfo(const CEnCommandLineInfo& cmdInfo)
{
	Reset();

	// insert default path at front
	lstrcpyn(m_szFilePaths, cmdInfo.m_strFileName, FILEPATHSLEN);

	// then multiple others
	if (cmdInfo.HasOption(SWITCH_TASKFILE))
	{
		if (!cmdInfo.m_strFileName.IsEmpty())
			lstrcat(m_szFilePaths, _T("|")); // add delimiter

		int nLen = lstrlen(m_szFilePaths);
		ExtractAttribute(cmdInfo, SWITCH_TASKFILE, m_szFilePaths + nLen, FILEPATHSLEN - nLen);
	}

	CString sValue;
	COleDateTime date;

	// new task
	if (ExtractAttribute(cmdInfo, SWITCH_NEWTASK, m_sNewTaskTitle))
	{
		m_dwFlags |= TLD_NEWTASK;

		// user can specify parentID else 0
		if (cmdInfo.GetOption(SWITCH_PARENTID, sValue))
		{
			m_dwParentID = _ttoi(sValue);
		}
		else if (cmdInfo.GetOption(SWITCH_SIBLINGID, sValue))
		{
			m_dwSiblingID = _ttoi(sValue);
		}

		// creation date for new tasks only
		ParseDate(cmdInfo, SWITCH_TASKCREATEDATE, m_dtTaskCreateDate);

		// We don't support date offsets
		if (m_dtTaskCreateDate.IsOffset())
		{
			m_dtTaskCreateDate.ClearValue();
		}
		else
		{
			// time overrides
			TDCSTARTUPATTRIB time;
			ParseTime(cmdInfo, SWITCH_TASKCREATETIME, time);

			double dTime = 0.0;
			BOOL bOffset = FALSE;

			// We don't support time offsets either
			if (time.GetValue(dTime, bOffset) && !bOffset)
			{
				double dDateTime = 0.0;

				if (m_dtTaskCreateDate.GetValue(dDateTime, bOffset))
					dDateTime = CDateHelper::GetDateOnly(dDateTime).m_dt;
				else
					dDateTime = CDateHelper::GetDate(DHD_TODAY);

				m_dtTaskCreateDate.SetValue(Misc::Format(dDateTime + dTime));
			}
		}
	}
	// select task
	else if (cmdInfo.GetOption(SWITCH_SELECTTASKID, sValue))
	{
		m_dwIDSel = _ttoi(sValue);
	}
	// or merge/import
	else if (ExtractAttribute(cmdInfo, SWITCH_IMPORT, m_szFilePaths, FILEPATHSLEN))
	{
		m_dwFlags |= TLD_IMPORTFILE;
	}

	// Other tasklist attributes
	ExtractAttribute(cmdInfo, SWITCH_COMMANDID, m_sCmdIDs);
	ExtractAttribute(cmdInfo, SWITCH_TITLEPREFIX, m_sTitlePrefix);

	// Task attributes
	ExtractAttribute(cmdInfo, SWITCH_TASKTITLE, m_sTaskTitle);	
	ExtractAttribute(cmdInfo, SWITCH_TASKEXTID, m_sTaskExternalID);	
	ExtractAttribute(cmdInfo, SWITCH_TASKCATEGORY, m_sTaskCategory);	
	ExtractAttribute(cmdInfo, SWITCH_TASKSTATUS, m_sTaskStatus);	
	ExtractAttribute(cmdInfo, SWITCH_TASKALLOCBY, m_sTaskAllocBy);	
	ExtractAttribute(cmdInfo, SWITCH_TASKALLOCTO, m_sTaskAllocTo);	
	ExtractAttribute(cmdInfo, SWITCH_TASKVERSION, m_sTaskVersion);	
	ExtractAttribute(cmdInfo, SWITCH_TASKTAGS, m_sTaskTags);	
	ExtractAttribute(cmdInfo, SWITCH_TASKDEPENDENCY, m_sTaskDepends);	
	ExtractAttribute(cmdInfo, SWITCH_TASKFILELINK, m_sTaskFileLink);	
	ExtractAttribute(cmdInfo, SWITCH_TASKICON, m_sTaskIcon);

	ExtractAttribute(cmdInfo, SWITCH_TASKPRIORITY, m_nTaskPriority);
	ExtractAttribute(cmdInfo, SWITCH_TASKRISK, m_nTaskRisk);

	// % completion
	// WHY IS THIS NOT HANDLED AS ABOVE?
	if (cmdInfo.GetOption(SWITCH_TASKPERCENT, sValue))
		m_nTaskPercentDone.SetValue(sValue);	

	// dates
	ParseDate(cmdInfo, SWITCH_TASKSTARTDATE, m_dtTaskStartDate);
	ParseTime(cmdInfo, SWITCH_TASKSTARTTIME, m_dTaskStartTime);

	ParseDate(cmdInfo, SWITCH_TASKDUEDATE, m_dtTaskDueDate);
	ParseTime(cmdInfo, SWITCH_TASKDUETIME, m_dTaskDueTime);

	ParseDate(cmdInfo, SWITCH_TASKDONEDATE, m_dtTaskDoneDate);
	ParseTime(cmdInfo, SWITCH_TASKDONETIME, m_dTaskDoneTime);

	// times and cost
	if (cmdInfo.GetOption(SWITCH_TASKCOST, sValue))
		m_dTaskCost.SetValue(sValue);	

	if (cmdInfo.GetOption(SWITCH_TASKTIMEEST, sValue))
		m_dTaskTimeEst.SetValue(sValue);	

	if (cmdInfo.GetOption(SWITCH_TASKTIMESPENT, sValue))
		m_dTaskTimeSpent.SetValue(sValue);	

	// comments replace [\][n] with [\n]
	if (cmdInfo.GetOption(SWITCH_TASKCOMMENTS, sValue))
	{
		sValue.Replace(_T("\\n"), _T("\n"));
		m_sTaskComments.SetValue(sValue);
	}

	// Custom attribute
	CStringArray aValues;

	if (cmdInfo.GetOptions(SWITCH_TASKCUSTOMATTRIB, aValues))
	{
		switch (aValues.GetSize())
		{
		case 0:
		default:
			break;

		case 1:
			m_sTaskCustomAttrib.SetValue(aValues[0] + '|');
			break;

		case 2:
			m_sTaskCustomAttrib.SetValue(Misc::FormatArray(aValues, '|'));
			break;
		}
	}

	// Copying task attributes
	if (cmdInfo.GetOptions(SWITCH_COPYTASKATTRIB, aValues) && (aValues.GetSize() == 2))
	{
		m_sCopyFromTaskAttrib.SetValue(aValues[0]);
		m_sCopyToTaskAttrib.SetValue(aValues[1]);
	}

	// App-level flags
	if (cmdInfo.HasOption(SWITCH_FORCEVISIBLE))
		m_dwFlags |= TLD_FORCEVISIBLE;

	if (cmdInfo.HasOption(SWITCH_NOPASSWORDPROMPT))
	{
		m_dwFlags &= ~TLD_PASSWORDPROMPTING;

		if (cmdInfo.HasOption(SWITCH_MASTERPASSWORD))
			m_dwFlags |= TLD_MASTERPASSWORDENABLED;
	}

	if (cmdInfo.HasOption(SWITCH_LOGGING))
	{
		m_dwFlags |= TLD_LOGGING;

		CStringArray aOptions;

		if (cmdInfo.GetOptions(SWITCH_LOGGING, aOptions))
		{
			if (Misc::Find(_T("m"), aOptions) != -1)
				m_dwFlags |= TLD_LOGMODULES;
			
			if (Misc::Find(_T("c"), aOptions) != -1)
				m_dwFlags |= TLD_LOGCOMMANDS;
		}
	}

	if (cmdInfo.HasOption(SWITCH_STARTEMPTY))
		m_dwFlags |= TLD_STARTEMPTY;

	if (cmdInfo.HasOption(SWITCH_UPGRADED))
		m_dwFlags |= TLD_UPGRADED;

	if (cmdInfo.HasOption(SWITCH_SAVEUIVISINTASKLIST))
		m_dwFlags |= TLD_SAVEUIVISINTASKLIST;

	if (cmdInfo.HasOption(SWITCH_ALLOWFORCEDCHECKOUT))
		m_dwFlags |= TLD_ALLOWFORCEDCHECKOUT;

	if (cmdInfo.GetOption(SWITCH_SAVEINTERMEDIATE, sValue))
	{
		m_dwFlags |= TLD_SAVEINTERMEDIATE;

		sValue.MakeUpper();
		m_bSaveIntermediateAll = (sValue == _T("ALL"));
	}

	if (cmdInfo.GetOption(SWITCH_SIMPLESOURCECONTROL, sValue))
	{
		if (sValue.CompareNoCase(_T("out")) == 0)
		{
			m_bCheckOut = TRUE;
		}
		else if (sValue.CompareNoCase(_T("in")) == 0)
		{
			m_bCheckIn = TRUE;
		}
		else
		{
			m_bCheckOut = TRUE;
			m_bCheckIn = TRUE;
		}
	}

	if (cmdInfo.HasOption(SWITCH_TASKLINK))
	{
		m_dwFlags |= TLD_TASKLINK;

		if (!HasFilePath())
		{
			CString sPath, sLink(cmdInfo.GetOption(SWITCH_TASKLINK));
			DWORD dwTaskID = 0;

			if (TDCTASKLINK::Parse(sLink, FALSE, _T(""), dwTaskID, sPath))
			{
				lstrcpyn(m_szFilePaths, sPath, FILEPATHSLEN);
				m_dwIDSel = dwTaskID;

				// We converted it
				m_dwFlags &= ~TLD_TASKLINK;
			}
		}
	}
}

BOOL CTDCStartupOptions::GetSaveIntermediateAll() const
{
	return (HasFlag(TLD_SAVEINTERMEDIATE) ? m_bSaveIntermediateAll : FALSE);
}

CTDCStartupOptions& CTDCStartupOptions::operator=(const CTDCStartupOptions& startup)
{
	lstrcpy(m_szFilePaths, startup.m_szFilePaths); 

	m_sCmdIDs = startup.m_sCmdIDs;
	m_sTitlePrefix = startup.m_sTitlePrefix;

	m_sNewTaskTitle = startup.m_sNewTaskTitle; 
	m_sTaskTitle = startup.m_sTaskTitle;
	m_sTaskComments = startup.m_sTaskComments; 
	m_sTaskExternalID = startup.m_sTaskExternalID; 
	m_sTaskVersion = startup.m_sTaskVersion;
	m_sTaskAllocTo = startup.m_sTaskAllocTo; 
	m_sTaskAllocBy = startup.m_sTaskAllocBy; 
	m_sTaskCategory = startup.m_sTaskCategory; 
	m_sTaskTags = startup.m_sTaskTags; 
	m_sTaskStatus = startup.m_sTaskStatus; 
	m_sTaskDepends = startup.m_sTaskDepends; 
	m_sTaskFileLink = startup.m_sTaskFileLink; 
	m_sTaskCustomAttrib = startup.m_sTaskCustomAttrib; 
	m_sTaskIcon = startup.m_sTaskIcon;

	m_dtTaskCreateDate = startup.m_dtTaskCreateDate;
	m_dtTaskStartDate = startup.m_dtTaskStartDate;
	m_dtTaskDueDate = startup.m_dtTaskDueDate;
	m_dtTaskDoneDate = startup.m_dtTaskDoneDate;
	m_dTaskStartTime = startup.m_dTaskStartTime;
	m_dTaskDueTime = startup.m_dTaskDueTime;
	m_dTaskDoneTime = startup.m_dTaskDoneTime;
	m_nTaskPriority = startup.m_nTaskPriority; 
	m_nTaskRisk = startup.m_nTaskRisk; 
	m_dTaskCost = startup.m_dTaskCost;
	m_dTaskTimeEst = startup.m_dTaskTimeEst;
	m_dTaskTimeSpent = startup.m_dTaskTimeSpent;
	m_nTaskPercentDone = startup.m_nTaskPercentDone;

	m_dwIDSel = startup.m_dwIDSel;
	m_dwParentID = startup.m_dwParentID; 
	m_dwSiblingID = startup.m_dwSiblingID; 
	m_dwFlags = startup.m_dwFlags;

	m_bSaveIntermediateAll = startup.m_bSaveIntermediateAll;
	m_bCheckOut = startup.m_bCheckOut;
	m_bCheckIn = startup.m_bCheckIn;

	m_sCopyFromTaskAttrib = startup.m_sCopyFromTaskAttrib;
	m_sCopyToTaskAttrib = startup.m_sCopyToTaskAttrib;

	return *this;
}

BOOL CTDCStartupOptions::operator==(const CTDCStartupOptions& startup) const
{
	return 
		(
		(_tcscmp(m_szFilePaths, startup.m_szFilePaths) == 0) &&

		(m_sCmdIDs == startup.m_sCmdIDs) &&
		(m_sTitlePrefix == startup.m_sTitlePrefix) &&

		(m_sNewTaskTitle == startup.m_sNewTaskTitle) &&
		(m_sTaskTitle == startup.m_sTaskTitle) &&
		(m_sTaskComments == startup.m_sTaskComments) &&
		(m_sTaskExternalID == startup.m_sTaskExternalID) &&
		(m_sTaskVersion == startup.m_sTaskVersion) &&
		(m_sTaskAllocTo == startup.m_sTaskAllocTo) &&
		(m_sTaskAllocBy == startup.m_sTaskAllocBy) &&
		(m_sTaskCategory == startup.m_sTaskCategory) &&
		(m_sTaskTags == startup.m_sTaskTags) &&
		(m_sTaskStatus == startup.m_sTaskStatus) &&
		(m_sTaskDepends == startup.m_sTaskDepends) &&
		(m_sTaskFileLink == startup.m_sTaskFileLink) &&
		(m_sTaskCustomAttrib == startup.m_sTaskCustomAttrib) &&
		(m_sTaskIcon == startup.m_sTaskIcon) &&

		(m_dtTaskCreateDate == startup.m_dtTaskCreateDate) &&
		(m_dtTaskStartDate == startup.m_dtTaskStartDate) &&
		(m_dtTaskDueDate == startup.m_dtTaskDueDate) &&
		(m_dtTaskDoneDate == startup.m_dtTaskDoneDate) &&
		(m_dTaskStartTime == startup.m_dTaskStartTime) &&
		(m_dTaskDueTime == startup.m_dTaskDueTime) &&
		(m_dTaskDoneTime == startup.m_dTaskDoneTime) &&
		(m_nTaskPriority == startup.m_nTaskPriority) &&
		(m_nTaskRisk == startup.m_nTaskRisk) &&
		(m_dTaskCost == startup.m_dTaskCost) &&
		(m_dTaskTimeEst == startup.m_dTaskTimeEst) &&
		(m_dTaskTimeSpent == startup.m_dTaskTimeSpent) &&
		(m_nTaskPercentDone == startup.m_nTaskPercentDone) &&

		(m_dwIDSel == startup.m_dwIDSel) &&
		(m_dwParentID == startup.m_dwParentID) &&
		(m_dwSiblingID == startup.m_dwSiblingID) &&
		(m_dwFlags == startup.m_dwFlags) &&

		(m_bSaveIntermediateAll == startup.m_bSaveIntermediateAll) &&
		(m_bCheckOut == startup.m_bCheckOut) &&
		(m_bCheckIn == startup.m_bCheckIn) &&

		(m_sCopyFromTaskAttrib == startup.m_sCopyFromTaskAttrib) && 
		(m_sCopyToTaskAttrib == startup.m_sCopyToTaskAttrib)
		);
}

BOOL CTDCStartupOptions::IsEmpty(BOOL bIgnoreFlags) const
{
	static CTDCStartupOptions empty;

	if (bIgnoreFlags)
		empty.m_dwFlags = m_dwFlags;

	return (*this == empty);
}

BOOL CTDCStartupOptions::HasFlag(DWORD dwFlag) const 
{ 
	return Misc::HasFlag(m_dwFlags, dwFlag); 
}

BOOL CTDCStartupOptions::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
	return Misc::ModifyFlags(m_dwFlags, dwRemove, dwAdd); 
}

int CTDCStartupOptions::GetFilePaths(CStringArray& aFiles) const 
{ 
	return Misc::Split(m_szFilePaths, aFiles, '|'); 
}

BOOL CTDCStartupOptions::ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, LPTSTR szAttrib, int nLenAttrib)
{
	CStringArray aSrc;

	if (cmdInfo.GetOptions(szSwitch, aSrc))                                
	{                                                                   
		lstrcpyn(szAttrib, Misc::FormatArray(aSrc, '|'), nLenAttrib);
		return TRUE;
	}

	return FALSE;
}

BOOL CTDCStartupOptions::ExtractAttribute(const CEnCommandLineInfo& cmdInfo, LPCTSTR szSwitch, TDCSTARTUPATTRIB& attrib)
{
	CStringArray aSrc;

	if (cmdInfo.GetOptions(szSwitch, aSrc))                                
	{              
		attrib.SetValue(Misc::FormatArray(aSrc, '|'));
		return TRUE;
	}

	return FALSE;
}

void CTDCStartupOptions::ParseDate(const CEnCommandLineInfo& cmdInfo, 
									LPCTSTR szSwitch, TDCSTARTUPATTRIB& dtDate)
{
	CString sValue;
	
	if (cmdInfo.GetOption(szSwitch, sValue))
	{
		if (TDCSTARTUPATTRIB::IsOffset(sValue))
		{
			dtDate.SetValue(sValue);
		}
		else if (sValue.IsEmpty())
		{
			// Clear date
			dtDate.SetValue(sValue);
		}
		else // actual date
		{ 
			CTwentyFourSevenWeek week;
			CDateHelper dh(week);
			COleDateTime date;

			if (dh.DecodeRelativeDate(sValue, date) ||
				CDateHelper::DecodeDate(sValue, date, TRUE))
			{
				dtDate.SetValue(Misc::Format(date.m_dt));
			}
		}
	}
}

void CTDCStartupOptions::ParseTime(const CEnCommandLineInfo& cmdInfo, 
									LPCTSTR szSwitch, TDCSTARTUPATTRIB& dTime)
{
	CString sValue;
	
	if (cmdInfo.GetOption(szSwitch, sValue))
	{
		if (TDCSTARTUPATTRIB::IsOffset(sValue))
		{
			dTime.SetValue(sValue);
		}
		else if (sValue.IsEmpty())
		{
			// Clear time
			dTime.SetValue(sValue);
		}
		else // actual time
		{ 
			COleDateTime dtTime;

			if (dtTime.ParseDateTime(sValue, VAR_TIMEVALUEONLY))
				dTime.SetValue(Misc::Format(dtTime.m_dt)); // fraction of a day
		}
	}
}

void CTDCStartupOptions::Reset() 
{ 
	m_szFilePaths[0] = 0; 

	m_sCmdIDs.ClearValue();

	m_sNewTaskTitle.ClearValue();
	m_sTaskTitle.ClearValue();
	m_sTaskComments.ClearValue();
	m_sTaskExternalID.ClearValue();
	m_sTaskVersion.ClearValue();
	m_sTaskAllocTo.ClearValue();
	m_sTaskAllocBy.ClearValue();
	m_sTaskCategory.ClearValue();
	m_sTaskStatus.ClearValue();
	m_sTaskTags.ClearValue(); 
	m_sTaskFileLink.ClearValue();
	m_sTaskDepends.ClearValue();
	m_sTaskCustomAttrib.ClearValue();
	m_sTaskIcon.ClearValue();

	m_dtTaskCreateDate.ClearValue();
	m_dtTaskStartDate.ClearValue();
	m_dtTaskDueDate.ClearValue();
	m_dtTaskDoneDate.ClearValue();
	m_dTaskStartTime.ClearValue();
	m_dTaskDueTime.ClearValue();
	m_dTaskDoneTime.ClearValue();
	m_dTaskTimeEst.ClearValue();
	m_dTaskTimeSpent.ClearValue();
	m_dTaskCost.ClearValue();

	m_nTaskPercentDone.ClearValue();
	m_nTaskPriority.ClearValue();
	m_nTaskRisk.ClearValue(); 

	m_dwIDSel = 0;
	m_dwParentID = 0; 
	m_dwSiblingID = 0; 
	m_dwFlags = TLD_PASSWORDPROMPTING;

	m_bSaveIntermediateAll = FALSE;
	m_bCheckOut = FALSE;
	m_bCheckIn = FALSE;

	m_sCopyFromTaskAttrib.ClearValue();
	m_sCopyToTaskAttrib.ClearValue();
}

BOOL CTDCStartupOptions::GetTaskCreationDate(COleDateTime& dtValue) const
{
	BOOL bOffset = FALSE;
	double dValue = 0.0;
	TDC_UNITS nUnits = TDCU_NULL;
	
	if (m_dtTaskCreateDate.GetDate(dValue, nUnits, bOffset) && !bOffset)
	{
		dtValue = COleDateTime(dValue);
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CTDCStartupOptions::GetTaskPriority(int& nValue, BOOL& bOffset) const 
{ 
	if (m_nTaskPriority.IsSet() && m_nTaskPriority.IsEmpty())
	{
		nValue = -2;
		bOffset = FALSE;

		return TRUE;
	}

	// else
	return m_nTaskPriority.GetValue(nValue, bOffset); 
}

BOOL CTDCStartupOptions::GetTaskRisk(int& nValue, BOOL& bOffset) const 
{ 
	if (m_nTaskRisk.IsSet() && m_nTaskRisk.IsEmpty())
	{
		nValue = -2;
		bOffset = FALSE;

		return TRUE;
	}

	return m_nTaskRisk.GetValue(nValue, bOffset); 
}

BOOL CTDCStartupOptions::GetTaskCustomAttribute(CString& sCustomID, CString& sValue) const
{
	sCustomID = m_sTaskCustomAttrib.GetValue();

	return Misc::Split(sCustomID, sValue, '|');
}

int CTDCStartupOptions::GetCommandIDs(CUIntArray& aCmdIDs) const
{
	CStringArray aCommands;
	int nNumCmds = Misc::Split(m_sCmdIDs.GetValue(), aCommands, '|');

	aCmdIDs.RemoveAll();

	for (int nCmd = 0; nCmd < nNumCmds; nCmd++)
	{
		UINT nCmdID = _ttoi(aCommands[nCmd]);

		if (nCmdID != 0)
			aCmdIDs.Add(nCmdID);
	}

	return aCmdIDs.GetSize();
}

BOOL CTDCStartupOptions::GetCopyTaskAttribute(TDC_ATTRIBUTE& nFromAttrib, TDC_ATTRIBUTE& nToAttrib) const
{
	TDC_ATTRIBUTE nFrom = TDC::MapCommandLineSwitchToAttribute(m_sCopyFromTaskAttrib.GetValue());

	if (nFrom == TDCA_NONE)
		return FALSE;

	TDC_ATTRIBUTE nTo = TDC::MapCommandLineSwitchToAttribute(m_sCopyToTaskAttrib.GetValue());

	if (nTo == TDCA_NONE)
		return FALSE;

	// else
	nFromAttrib = nFrom;
	nToAttrib = nTo;

	return TRUE;
}

BOOL CTDCStartupOptions::GetCopyTaskAttribute(TDC_ATTRIBUTE& nFromAttrib, CString& sToCustomAttrib) const
{
	TDC_ATTRIBUTE nFrom = TDC::MapCommandLineSwitchToAttribute(m_sCopyFromTaskAttrib.GetValue());

	if (nFrom == TDCA_NONE)
		return FALSE;

	TDC_ATTRIBUTE nTo = TDC::MapCommandLineSwitchToAttribute(m_sCopyToTaskAttrib.GetValue());

	if (nTo != TDCA_NONE)
		return FALSE;

	// else
	nFromAttrib = nFrom;
	sToCustomAttrib = m_sCopyToTaskAttrib.GetValue();

	return TRUE;
}

BOOL CTDCStartupOptions::GetCopyTaskAttribute(CString& sFromCustomAttrib, TDC_ATTRIBUTE& nToAttrib) const
{
	TDC_ATTRIBUTE nFrom = TDC::MapCommandLineSwitchToAttribute(m_sCopyFromTaskAttrib.GetValue());

	if (nFrom != TDCA_NONE)
		return FALSE;

	TDC_ATTRIBUTE nTo = TDC::MapCommandLineSwitchToAttribute(m_sCopyToTaskAttrib.GetValue());

	if (nTo == TDCA_NONE)
		return FALSE;

	// else
	sFromCustomAttrib = m_sCopyFromTaskAttrib.GetValue();
	nToAttrib = nTo;

	return TRUE;
}

BOOL CTDCStartupOptions::GetCopyTaskAttribute(CString& sFromCustomAttrib, CString& sToCustomAttrib) const
{
	if (m_sCopyFromTaskAttrib.IsEmpty() || m_sCopyToTaskAttrib.IsEmpty())
		return FALSE;

	TDC_ATTRIBUTE nFrom = TDC::MapCommandLineSwitchToAttribute(m_sCopyFromTaskAttrib.GetValue());

	if (nFrom != TDCA_NONE)
		return FALSE;

	TDC_ATTRIBUTE nTo = TDC::MapCommandLineSwitchToAttribute(m_sCopyToTaskAttrib.GetValue());

	if (nTo != TDCA_NONE)
		return FALSE;

	// else
	sFromCustomAttrib = m_sCopyFromTaskAttrib.GetValue();
	sToCustomAttrib = m_sCopyToTaskAttrib.GetValue();

	return TRUE;
}
