#include "stdafx.h"
#include "TDCStartupOptions.h"
#include "tdcenum.h"
#include "tdcswitch.h"
#include "tdcmapping.h"

#include "..\Shared\EnCommandLineInfo.h"
#include "..\Shared\misc.h"
#include "..\Shared\datehelper.h"
#include "..\Shared\timehelper.h"

/////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1400                        
#define COPYTEXT(DEST, SRC, LEN)		\
	int len = min(LEN, lstrlen(SRC));\
	_tcsncpy_s(DEST, LEN, SRC, len);	\
	DEST[len] = 0;
#else                                       
#define COPYTEXT(DEST, SRC, LEN)		\
	int len = min(LEN, lstrlen(SRC));\
	_tcsncpy(DEST, SRC, len);			\
	DEST[len] = 0;
#endif                                      

/////////////////////////////////////////////////////////////////////////////////////////////

TDCSTARTUPATTRIB::TDCSTARTUPATTRIB() : bSet(FALSE)
{
	szValue[0] = 0;
}

TDCSTARTUPATTRIB& TDCSTARTUPATTRIB::operator=(const TDCSTARTUPATTRIB& other)
{
	bSet = other.bSet;
	COPYTEXT(szValue, other.szValue, ATTRIBLEN);

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
	return (!bSet || !lstrlen(szValue)); 
}

void TDCSTARTUPATTRIB::SetValue(const CString& sValue)
{
	bSet = TRUE;
	COPYTEXT(szValue, sValue, ATTRIBLEN);
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

CString TDCSTARTUPATTRIB::GetValue() const
{
	CString sValue;

	if (bSet)
	{
		sValue = szValue;
		Misc::Trim(sValue);
	}
	
	return sValue;
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

BOOL TDCSTARTUPATTRIB::GetTime(double& dValue, TDC_UNITS& nUnits, BOOL& bOffset) const
{
	if (!bSet)
		return FALSE;

	TH_UNITS nTHUnits = THU_NULL;

	if (IsOffset(szValue) && CTimeHelper::DecodeOffset(szValue, dValue, nTHUnits, TRUE))
	{
		bOffset = TRUE;
	}
	else if (CTimeHelper::DecodeOffset(szValue, dValue, nTHUnits, FALSE))
	{
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

	if (IsOffset(szValue) && CDateHelper::DecodeOffset(szValue, dValue, nDHUnits, TRUE))
	{
		bOffset = TRUE;
		nUnits = TDC::MapDHUnitsToUnits(nDHUnits);
	}
	else if (CDateHelper::DecodeOffset(szValue, dValue, nDHUnits, FALSE))
	{
		bOffset = FALSE;
	}
	else
	{
		bOffset = -1; // error
		return FALSE;
	}

	return TRUE;
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
	Reset();

	// insert default path at front
	COPYTEXT(m_szFilePaths, cmdInfo.m_strFileName, FILEPATHSLEN);

	// then multiple others
	if (cmdInfo.HasOption(SWITCH_TASKFILE))
	{
		if (!cmdInfo.m_strFileName.IsEmpty())
		{
#if _MSC_VER >= 1400                        
			_tcscat_s(m_szFilePaths, FILEPATHSLEN, _T("|")); // add delimiter
#else                                       
			_tcscat(m_szFilePaths, _T("|")); // add delimiter
#endif                                      
		}

		int nLen = lstrlen(m_szFilePaths);
		ExtractAttribute(cmdInfo, SWITCH_TASKFILE, m_szFilePaths + nLen, FILEPATHSLEN - nLen);
	}

	CString sValue;
	COleDateTime date;

	// new task
	if (ExtractAttribute(cmdInfo, SWITCH_NEWTASK, m_sNewTask))
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
		ParseDate(cmdInfo, SWITCH_TASKCREATEDATE, m_dtCreateDate);

		// We don't support date offsets
		if (m_dtCreateDate.IsOffset())
		{
			m_dtCreateDate.ClearValue();
		}
		else
		{
			// time overrides
			TDCSTARTUPATTRIB time;
			ParseDate(cmdInfo, SWITCH_TASKCREATETIME, time);

			double dTime = 0.0;
			BOOL bOffset = FALSE;

			// We don't support time offsets either
			if (time.GetValue(dTime, bOffset) && !bOffset)
			{
				double dDateTime = 0.0;

				if (m_dtCreateDate.GetValue(dDateTime, bOffset))
					dDateTime = CDateHelper::GetDateOnly(dDateTime).m_dt;
				else
					dDateTime = CDateHelper::GetDate(DHD_TODAY);

				m_dtCreateDate.SetValue(Misc::Format(dDateTime + dTime));
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
	else if (cmdInfo.GetOption(SWITCH_COMMANDID, sValue))
	{
		m_sCmdIDs.SetValue(sValue);
	}

	// other task attribs
	ExtractAttribute(cmdInfo, SWITCH_TASKEXTID, m_sExternalID);	
	ExtractAttribute(cmdInfo, SWITCH_TASKCATEGORY, m_sCategory);	
	ExtractAttribute(cmdInfo, SWITCH_TASKSTATUS, m_sStatus);	
	ExtractAttribute(cmdInfo, SWITCH_TASKALLOCBY, m_sAllocBy);	
	ExtractAttribute(cmdInfo, SWITCH_TASKALLOCTO, m_sAllocTo);	
	ExtractAttribute(cmdInfo, SWITCH_TASKVERSION, m_sVersion);	
	ExtractAttribute(cmdInfo, SWITCH_TASKTAGS, m_sTags);	
	ExtractAttribute(cmdInfo, SWITCH_TASKDEPENDENCY, m_sDepends);	
	ExtractAttribute(cmdInfo, SWITCH_TASKFILEREF, m_sFileRef);	
	ExtractAttribute(cmdInfo, SWITCH_TASKPRIORITY, m_nPriority);
	ExtractAttribute(cmdInfo, SWITCH_TASKRISK, m_nRisk);

	// % completion
	if (cmdInfo.GetOption(SWITCH_TASKPERCENT, sValue))
		m_nPercentDone.SetValue(sValue);	

	// dates
	ParseDate(cmdInfo, SWITCH_TASKSTARTDATE, m_dtStartDate);
	ParseTime(cmdInfo, SWITCH_TASKSTARTTIME, m_dStartTime);

	ParseDate(cmdInfo, SWITCH_TASKDUEDATE, m_dtDueDate);
	ParseTime(cmdInfo, SWITCH_TASKDUETIME, m_dDueTime);

	ParseDate(cmdInfo, SWITCH_TASKDONEDATE, m_dtDoneDate);
	ParseTime(cmdInfo, SWITCH_TASKDONETIME, m_dDoneTime);

	// times and cost
	if (cmdInfo.GetOption(SWITCH_TASKCOST, sValue))
		m_dCost.SetValue(sValue);	

	if (cmdInfo.GetOption(SWITCH_TASKTIMEEST, sValue))
		m_dTimeEst.SetValue(sValue);	

	if (cmdInfo.GetOption(SWITCH_TASKTIMESPENT, sValue))
		m_dTimeSpent.SetValue(sValue);	

	// comments replace [\][n] with [\n]
	if (cmdInfo.GetOption(SWITCH_TASKCOMMENTS, sValue))
	{
		sValue.Replace(_T("\\n"), _T("\n"));
		m_sComments.SetValue(sValue);
	}

	// Custom attribute
	CStringArray aValues;

	if (cmdInfo.GetOptions(SWITCH_TASKCUSTOMATTRIB, aValues) && (aValues.GetSize() == 2))
		m_sCustomAttrib.SetValue(Misc::FormatArray(aValues, '|'));

	// App-level flags
	if (cmdInfo.HasOption(SWITCH_FORCEVISIBLE))
		m_dwFlags |= TLD_FORCEVISIBLE;

	if (cmdInfo.HasOption(SWITCH_NOPWORDPROMPT))
		m_dwFlags &= ~TLD_PASSWORDPROMPTING;

	if (cmdInfo.HasOption(SWITCH_LOGGING))
		m_dwFlags |= TLD_LOGGING;

	if (cmdInfo.HasOption(SWITCH_STARTEMPTY))
		m_dwFlags |= TLD_STARTEMPTY;

	if (cmdInfo.HasOption(SWITCH_UPGRADED))
		m_dwFlags |= TLD_UPGRADED;

	if (cmdInfo.HasOption(SWITCH_SAVEUIVISINTASKLIST))
		m_dwFlags |= TLD_SAVEUIVISINTASKLIST;

	if (cmdInfo.GetOption(SWITCH_SAVEINTERMEDIATE, sValue))
	{
		m_dwFlags |= TLD_SAVEINTERMEDIATE;

		sValue.MakeUpper();
		m_bSaveIntermediateAll = (sValue == _T("ALL"));
	}

	if (cmdInfo.HasOption(SWITCH_TASKLINK))
	{
		if (!HasFilePath())
		{
			CString sLink(cmdInfo.GetOption(SWITCH_TASKLINK));
			COPYTEXT(m_szFilePaths, sLink, FILEPATHSLEN);
		}

		m_dwFlags |= TLD_TASKLINK;
	}
}

BOOL CTDCStartupOptions::GetSaveIntermediateAll() const
{
	return (HasFlag(TLD_SAVEINTERMEDIATE) ? m_bSaveIntermediateAll : FALSE);
}

CTDCStartupOptions& CTDCStartupOptions::operator=(const CTDCStartupOptions& startup)
{
#if _MSC_VER >= 1400
	_tcscpy_s(m_szFilePaths, FILEPATHSLEN, startup.m_szFilePaths); 
#else
	_tcscpy(m_szFilePaths, startup.m_szFilePaths); 
#endif

	m_sNewTask = startup.m_sNewTask; 
	m_sComments = startup.m_sComments; 
	m_sExternalID = startup.m_sExternalID; 
	m_sVersion = startup.m_sVersion;
	m_sAllocTo = startup.m_sAllocTo; 
	m_sAllocBy = startup.m_sAllocBy; 
	m_sCategory = startup.m_sCategory; 
	m_sTags = startup.m_sTags; 
	m_sStatus = startup.m_sStatus; 
	m_sDepends = startup.m_sDepends; 
	m_sFileRef = startup.m_sFileRef; 
	m_sCustomAttrib = startup.m_sCustomAttrib; 

	m_dtCreateDate = startup.m_dtCreateDate;
	m_dtStartDate = startup.m_dtStartDate;
	m_dtDueDate = startup.m_dtDueDate;
	m_dtDoneDate = startup.m_dtDoneDate;
	m_dStartTime = startup.m_dStartTime;
	m_dDueTime = startup.m_dDueTime;
	m_dDoneTime = startup.m_dDoneTime;
	m_nPriority = startup.m_nPriority; 
	m_nRisk = startup.m_nRisk; 
	m_dCost = startup.m_dCost;
	m_dTimeEst = startup.m_dTimeEst;
	m_dTimeSpent = startup.m_dTimeSpent;
	m_nPercentDone = startup.m_nPercentDone;

	m_dwIDSel = startup.m_dwIDSel;
	m_dwParentID = startup.m_dwParentID; 
	m_dwSiblingID = startup.m_dwSiblingID; 
	m_dwFlags = startup.m_dwFlags;
	m_bSaveIntermediateAll = startup.m_bSaveIntermediateAll;

	return *this;
}

BOOL CTDCStartupOptions::operator==(const CTDCStartupOptions& startup) const
{
	return 
		(
		(_tcscmp(m_szFilePaths, startup.m_szFilePaths) == 0) &&

		(m_sNewTask == startup.m_sNewTask) &&
		(m_sComments == startup.m_sComments) &&
		(m_sExternalID == startup.m_sExternalID) &&
		(m_sVersion == startup.m_sVersion) &&
		(m_sAllocTo == startup.m_sAllocTo) &&
		(m_sAllocBy == startup.m_sAllocBy) &&
		(m_sCategory == startup.m_sCategory) &&
		(m_sTags == startup.m_sTags) &&
		(m_sStatus == startup.m_sStatus) &&
		(m_sDepends == startup.m_sDepends) &&
		(m_sFileRef == startup.m_sFileRef) &&
		(m_sCustomAttrib == startup.m_sCustomAttrib) &&

		(m_dtCreateDate == startup.m_dtCreateDate) &&
		(m_dtStartDate == startup.m_dtStartDate) &&
		(m_dtDueDate == startup.m_dtDueDate) &&
		(m_dtDoneDate == startup.m_dtDoneDate) &&
		(m_dStartTime == startup.m_dStartTime) &&
		(m_dDueTime == startup.m_dDueTime) &&
		(m_dDoneTime == startup.m_dDoneTime) &&
		(m_nPriority == startup.m_nPriority) &&
		(m_nRisk == startup.m_nRisk) &&
		(m_dCost == startup.m_dCost) &&
		(m_dTimeEst == startup.m_dTimeEst) &&
		(m_dTimeSpent == startup.m_dTimeSpent) &&
		(m_nPercentDone == startup.m_nPercentDone) &&

		(m_dwIDSel == startup.m_dwIDSel) &&
		(m_dwParentID == startup.m_dwParentID) &&
		(m_dwSiblingID == startup.m_dwSiblingID) &&
		(m_dwFlags == startup.m_dwFlags) &&
		(m_bSaveIntermediateAll == startup.m_bSaveIntermediateAll) &&
		(m_sCmdIDs == startup.m_sCmdIDs)
		);
}

BOOL CTDCStartupOptions::IsEmpty(BOOL bIgnoreFlags) const
{
	CTDCStartupOptions empty;

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
		COPYTEXT(szAttrib, Misc::FormatArray(aSrc, '|'), nLenAttrib);
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
		else // actual date
		{ 
			COleDateTime date;

			if (CDateHelper::DecodeRelativeDate(sValue, date, FALSE) ||
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
			dTime.SetValue(sValue); // in hours
		}
		else // actual time
		{ 
			COleDateTime dtTime;

			if (dtTime.ParseDateTime(sValue, VAR_TIMEVALUEONLY))
				dTime.SetValue(Misc::Format(dtTime.m_dt));
		}
	}
}

void CTDCStartupOptions::Reset() 
{ 
	m_szFilePaths[0] = 0; 

	m_sNewTask.ClearValue();
	m_sComments.ClearValue();
	m_sExternalID.ClearValue();
	m_sVersion.ClearValue();
	m_sAllocTo.ClearValue();
	m_sAllocBy.ClearValue();
	m_sCategory.ClearValue();
	m_sStatus.ClearValue();
	m_sTags.ClearValue(); 
	m_sFileRef.ClearValue();
	m_sDepends.ClearValue();
	m_sCustomAttrib.ClearValue();
	m_sCmdIDs.ClearValue();

	m_dtCreateDate.ClearValue();
	m_dtStartDate.ClearValue();
	m_dtDueDate.ClearValue();
	m_dtDoneDate.ClearValue();
	m_dStartTime.ClearValue();
	m_dDueTime.ClearValue();
	m_dDoneTime.ClearValue();
	m_dTimeEst.ClearValue();
	m_dTimeSpent.ClearValue();
	m_dCost.ClearValue();

	m_nPercentDone.ClearValue();
	m_nPriority.ClearValue();
	m_nRisk.ClearValue(); 

	m_dwIDSel = 0;
	m_dwParentID = 0; 
	m_dwSiblingID = 0; 
	m_dwFlags = TLD_PASSWORDPROMPTING;
	m_bSaveIntermediateAll = FALSE;
}

BOOL CTDCStartupOptions::GetCreationDate(COleDateTime& dtValue) const
{
	BOOL bOffset = FALSE;
	double dValue = 0.0;
	TDC_UNITS nUnits = TDCU_NULL;
	
	if (m_dtCreateDate.GetDate(dValue, nUnits, bOffset) && !bOffset)
	{
		dtValue = COleDateTime(dValue);
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CTDCStartupOptions::GetPriority(int& nValue, BOOL& bOffset) const 
{ 
	// handle 'n' for none
	if (m_nPriority == _T("n"))
	{
		nValue = -2;
		bOffset = FALSE;

		return TRUE;
	}

	// else
	return m_nPriority.GetValue(nValue, bOffset); 
}

BOOL CTDCStartupOptions::GetRisk(int& nValue, BOOL& bOffset) const 
{ 
	// handle 'n' for none
	if (m_nRisk == _T("n"))
	{
		nValue = -2;
		bOffset = FALSE;

		return TRUE;
	}

	return m_nRisk.GetValue(nValue, bOffset); 
}

BOOL CTDCStartupOptions::GetCustomAttribute(CString& sCustomID, CString& sValue) const
{
	if (!m_sCustomAttrib.IsEmpty())
	{
		CStringArray aValues;
		
		if (Misc::Split(m_sCustomAttrib.GetValue(), aValues, '|') == 2)
		{
			sCustomID = aValues[0];
			sValue = aValues[1];
			
			return !(sCustomID.IsEmpty() || sValue.IsEmpty());
		}
	}

	sCustomID.Empty();
	sValue.Empty();

	return FALSE;
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
