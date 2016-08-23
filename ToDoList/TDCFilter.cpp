#include "stdafx.h"
#include "TDCFilter.h"
#include "TDCSearchParamHelper.h"
#include "TDCCustomAttributeHelper.h"

#include "..\shared\preferences.h"

CTDCFilter::CTDCFilter() : m_nState(TDCFS_NONE)
{
}


CTDCFilter::~CTDCFilter()
{
}

CTDCFilter& CTDCFilter::operator=(const CTDCFilter& filter)
{
	m_nState = filter.m_nState;
	m_filter = filter.m_filter;
	m_custom = filter.m_custom;
	
	return *this;
}

BOOL CTDCFilter::operator==(const CTDCFilter& filter) const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		if (filter.m_nState == TDCFS_FILTER)
		{
			return m_filter.Matches(filter.m_filter);
		}
		break;

	case TDCFS_CUSTOM:
		if (filter.m_nState == TDCFS_CUSTOM)
		{
			return m_custom.Matches(filter.m_custom);
		}
		break;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		break;

	default:
		ASSERT(0);
	}

	// all else
	return FALSE;
}

BOOL CTDCFilter::FilterMatches(const FTDCFILTER& filter, LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags) const
{
	BOOL bCustom = HasCustomFilter();
	BOOL bOtherCustom = (szCustom && *szCustom);

	if (bCustom && bOtherCustom)
	{
		return m_custom.Matches(szCustom, dwCustomFlags, dwIgnoreFlags);
	}
	else if (!bCustom && !bOtherCustom)
	{
		return m_filter.Matches(filter, dwIgnoreFlags);
	}

	// One is custom but not the other
	return FALSE;
}

BOOL CTDCFilter::FlagsMatch(DWORD dwFlags1, DWORD dwFlags2, DWORD dwIgnore)
{
	return ((dwFlags1 & ~dwIgnore) == (dwFlags2 & ~dwIgnore));
}

FILTER_SHOW CTDCFilter::GetFilter(FTDCFILTER& filter) const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		filter = m_filter;
		break;

	case TDCFS_CUSTOM:
	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
	default:
		filter.Reset();
		break;
	}

	return GetFilter();
}

FILTER_SHOW CTDCFilter::GetFilter() const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		return m_filter.nShow;

	case TDCFS_CUSTOM:
		return FS_CUSTOM;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		break;

	default:
		ASSERT(0);
	}

	return FS_ALL;
}

BOOL CTDCFilter::HasAnyFilter(DWORD dwIgnoreFlags) const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		return m_filter.IsSet(dwIgnoreFlags);

	case TDCFS_CUSTOM:
		return (m_custom.params.GetRuleCount() != 0);

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCFilter::SetFilter(const FTDCFILTER& filter)
{
	m_filter = filter;
	m_nState = TDCFS_FILTER;

	// clear custom filter
	m_custom.Reset();

	return TRUE;
}

BOOL CTDCFilter::SetCustomFilter(const FTDCCUSTOMFILTER& filter)
{
	// error checking
	ASSERT(!filter.sName.IsEmpty() && filter.params.GetRuleCount());

	if (filter.sName.IsEmpty() || !filter.params.GetRuleCount())
		return FALSE;

	m_custom = filter;
	m_nState = TDCFS_CUSTOM;

	// clear user filter
	m_filter.Reset();

	return TRUE;
}

CString CTDCFilter::GetCustomFilterName() const
{
	if (m_nState == TDCFS_CUSTOM)
		return m_custom.sName;

	// else
	ASSERT(0);
	return _T("");
}

BOOL CTDCFilter::HasCustomFilter() const
{
	return (m_nState == TDCFS_CUSTOM);
}

BOOL CTDCFilter::HasCustomFilterAttribute(TDC_ATTRIBUTE nAttrib) const
{
	if (m_nState == TDCFS_CUSTOM)
		return m_custom.params.HasAttribute(nAttrib);

	// else
	ASSERT(0);
	return FALSE;
}

DWORD CTDCFilter::GetFilterFlags() const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		return m_filter.dwFlags;

	case TDCFS_CUSTOM:
		return m_custom.dwFlags;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		return 0;
	}

	ASSERT(0);
	return 0;
}

BOOL CTDCFilter::HasFilterFlag(DWORD dwFlag) const
{
	return Misc::HasFlag(GetFilterFlags(), dwFlag);
}

BOOL CTDCFilter::ClearFilter()
{
	if (HasAnyFilter() || CanToggleFilter())
	{
		m_filter.Reset();
		m_custom.Reset();
		m_nState = TDCFS_NONE;

		return TRUE;
	}

	return FALSE;
}

BOOL CTDCFilter::ToggleFilter()
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		m_nState = TDCFS_FILTER_TOGGLED;
		return TRUE;

	case TDCFS_CUSTOM:
		m_nState = TDCFS_CUSTOM_TOGGLED;
		return TRUE;

	case TDCFS_FILTER_TOGGLED:
		m_nState = TDCFS_FILTER;
		return TRUE;

	case TDCFS_CUSTOM_TOGGLED:
		m_nState = TDCFS_CUSTOM;
		return TRUE;

	case TDCFS_NONE:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCFilter::CanToggleFilter() const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
	case TDCFS_CUSTOM:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		return TRUE;

	case TDCFS_NONE:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCFilter::BuildFilterQuery(SEARCHPARAMS& params, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const
{
	params.Clear();

	switch (m_nState)
	{
	case TDCFS_FILTER:
		BuildFilterQuery(m_filter, params);
		return TRUE;

	case TDCFS_CUSTOM:
		params = m_custom.params;
		params.aAttribDefs.Copy(aCustomAttribDefs);
		params.bIgnoreDone = m_custom.HasFlag(FO_HIDEDONE);
		params.bIgnoreOverDue = m_custom.HasFlag(FO_HIDEOVERDUE);
		params.bWantAllSubtasks = m_custom.HasFlag(FO_SHOWALLSUB);
		return TRUE;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_CUSTOM_TOGGLED:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

void CTDCFilter::BuildFilterQuery(const FTDCFILTER& filter, SEARCHPARAMS& params)
{
	// reset the search
	params.aRules.RemoveAll();
	params.bIgnoreDone = filter.HasFlag(FO_HIDEDONE);
	params.bWantAllSubtasks = filter.HasFlag(FO_SHOWALLSUB);

	// handle principle filter
	switch (filter.nShow)
	{
	case FS_ALL:
		break;

	case FS_DONE:
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_SET));
		params.bIgnoreDone = FALSE;
		break;

	case FS_NOTDONE:
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));
		params.bIgnoreDone = TRUE;
		break;

	case FS_FLAGGED:
		params.aRules.Add(SEARCHPARAM(TDCA_FLAG, FOP_SET));
		break;

	case FS_SELECTED:
		params.aRules.Add(SEARCHPARAM(TDCA_SELECTION, FOP_SET));
		break;

	case FS_RECENTMOD:
		params.aRules.Add(SEARCHPARAM(TDCA_RECENTMODIFIED, FOP_SET));
		break;

	default:
		ASSERT(0); // to catch unimplemented filters
		break;
	}

	// handle start date filters
	COleDateTime dateStart;

	if (InitFilterDate(filter.nStartBy, filter.dtUserStart, filter.nStartNextNDays, dateStart))
	{
		// special case: FD_NOW
		if (filter.nStartBy == FD_NOW)
			params.aRules.Add(SEARCHPARAM(TDCA_STARTTIME, FOP_ON_OR_BEFORE, dateStart));
		else
			params.aRules.Add(SEARCHPARAM(TDCA_STARTDATE, FOP_ON_OR_BEFORE, dateStart));

		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));
	}
	else if (filter.nStartBy == FD_NONE)
	{
		params.aRules.Add(SEARCHPARAM(TDCA_STARTDATE, FOP_NOT_SET));
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));
	}

	// handle due date filters
	COleDateTime dateDue;

	if (InitFilterDate(filter.nDueBy, filter.dtUserDue, filter.nDueNextNDays, dateDue))
	{
		// special case: FD_NOW
		if (filter.nDueBy == FD_NOW)
			params.aRules.Add(SEARCHPARAM(TDCA_DUETIME, FOP_ON_OR_BEFORE, dateDue));
		else
			params.aRules.Add(SEARCHPARAM(TDCA_DUEDATE, FOP_ON_OR_BEFORE, dateDue));

		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));

		// this flag only applies to due filters
		params.bIgnoreOverDue = filter.HasFlag(FO_HIDEOVERDUE);
	}
	else if (filter.nDueBy == FD_NONE)
	{
		params.aRules.Add(SEARCHPARAM(TDCA_DUEDATE, FOP_NOT_SET));
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));
	}

	// handle other attributes
	AddNonDateFilterQueryRules(filter, params);
}

void CTDCFilter::AddNonDateFilterQueryRules(const FTDCFILTER& filter, SEARCHPARAMS& params)
{
	// title text
	if (!filter.sTitle.IsEmpty())
	{
		switch (filter.nTitleOption)
		{
		case FT_FILTERONTITLECOMMENTS:
			params.aRules.Add(SEARCHPARAM(TDCA_TASKNAMEORCOMMENTS, FOP_INCLUDES, filter.sTitle));
			break;

		case FT_FILTERONANYTEXT:
			params.aRules.Add(SEARCHPARAM(TDCA_ANYTEXTATTRIBUTE, FOP_INCLUDES, filter.sTitle));
			break;

		case FT_FILTERONTITLEONLY:
		default:
			params.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_INCLUDES, filter.sTitle));
			break;
		}
	}

	// note: these are all 'AND' 
	// category
	if (filter.aCategories.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aCategories);

		if (filter.aCategories.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_CATEGORY, FOP_NOT_SET));
		}
		else if (filter.dwFlags & FO_ANYCATEGORY)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_CATEGORY, FOP_INCLUDES, sMatchBy));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_CATEGORY, FOP_EQUALS, sMatchBy));
		}
	}

	// allocated to
	if (filter.aAllocTo.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aAllocTo);

		if (filter.aAllocTo.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_ALLOCTO, FOP_NOT_SET));
		}
		else if (filter.dwFlags & FO_ANYALLOCTO)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_ALLOCTO, FOP_INCLUDES, sMatchBy));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_ALLOCTO, FOP_EQUALS, sMatchBy));
		}
	}

	// allocated by
	if (filter.aAllocBy.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aAllocBy);

		if (filter.aAllocBy.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_ALLOCBY, FOP_NOT_SET));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_ALLOCBY, FOP_INCLUDES, sMatchBy));
		}
	}

	// status
	if (filter.aStatus.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aStatus);

		if (filter.aStatus.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_STATUS, FOP_NOT_SET));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_STATUS, FOP_INCLUDES, sMatchBy));
		}
	}

	// version
	if (filter.aVersions.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aVersions);

		if (filter.aVersions.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_VERSION, FOP_NOT_SET));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_VERSION, FOP_INCLUDES, sMatchBy));
		}
	}

	// tags
	if (filter.aTags.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(filter.aTags);

		if (filter.aTags.GetSize() == 1 && sMatchBy.IsEmpty())
		{
			params.aRules.Add(SEARCHPARAM(TDCA_TAGS, FOP_NOT_SET));
		}
		else
		{
			params.aRules.Add(SEARCHPARAM(TDCA_TAGS, FOP_INCLUDES, sMatchBy));
		}
	}

	// priority
	if (filter.nPriority != FM_ANYPRIORITY)
	{
		if (filter.nPriority == FM_NOPRIORITY)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_PRIORITY, FOP_NOT_SET));
		}
		else if (filter.nPriority != FM_ANYPRIORITY)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_PRIORITY, FOP_GREATER_OR_EQUAL, filter.nPriority));
		}
	}

	// risk
	if (filter.nRisk != FM_ANYRISK)
	{
		if (filter.nRisk == FM_NORISK)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_NOT_SET));
		}
		else if (filter.nRisk != FM_ANYRISK)
		{
			params.aRules.Add(SEARCHPARAM(TDCA_RISK, FOP_GREATER_OR_EQUAL, filter.nRisk));
		}
	}

	// special case: no aRules + ignore completed
	if ((params.bIgnoreDone) && params.aRules.GetSize() == 0)
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));
}


BOOL CTDCFilter::InitFilterDate(FILTER_DATE nDate, const COleDateTime& dateUser, int nNextNDays, COleDateTime& date) 
{
	switch (nDate)
	{
	case FD_TODAY:
		date = CDateHelper::GetDate(DHD_TODAY);
		break;

	case FD_YESTERDAY:
		date = CDateHelper::GetDate(DHD_YESTERDAY);
		break;
		
	case FD_TOMORROW:
		date = CDateHelper::GetDate(DHD_TOMORROW);
		break;

	case FD_ENDTHISWEEK:
		date = CDateHelper::GetDate(DHD_ENDTHISWEEK);
		break;

	case FD_ENDNEXTWEEK: 
		date = CDateHelper::GetDate(DHD_ENDNEXTWEEK);
		break;

	case FD_ENDTHISMONTH:
		date = CDateHelper::GetDate(DHD_ENDTHISMONTH);
		break;

	case FD_ENDNEXTMONTH:
		date = CDateHelper::GetDate(DHD_ENDNEXTMONTH);
		break;

	case FD_ENDTHISYEAR:
		date = CDateHelper::GetDate(DHD_ENDTHISYEAR);
		break;

	case FD_ENDNEXTYEAR:
		date = CDateHelper::GetDate(DHD_ENDNEXTYEAR);
		break;

	case FD_NEXTNDAYS:
		date = (CDateHelper::GetDate(DHD_TODAY) + nNextNDays - 1); // -1 because filter is FOP_ON_OR_BEFORE
		break;

	case FD_NOW:
		date = COleDateTime::GetCurrentTime();
		break;

	case FD_USER:
		ASSERT(CDateHelper::IsDateSet(dateUser));

		date = CDateHelper::GetDateOnly(dateUser);
		break;

	case FD_ANY:
	case FD_NONE:
		break;

	default:
		ASSERT(0);
		break;
	}

	return CDateHelper::IsDateSet(date);
}

void CTDCFilter::SaveFilter(CPreferences& prefs, const CString& sKey) const
{
	// remove existing key
	prefs.DeleteProfileSection(sKey, TRUE);

	prefs.WriteProfileInt(sKey, _T("State"), m_nState);

	switch (m_nState)
	{
	case TDCFS_FILTER:
	case TDCFS_FILTER_TOGGLED:
		SaveUserFilter(prefs, sKey);
		break;

	case TDCFS_CUSTOM:
	case TDCFS_CUSTOM_TOGGLED:
		SaveCustomFilter(prefs, sKey);
		break;

	case TDCFS_NONE:
		break;

	default:
		ASSERT(0);
	}
}

void CTDCFilter::LoadFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs)
{
	m_nState = (TDCFILTER_STATE)prefs.GetProfileInt(sKey, _T("State"), TDCFS_NONE);

	switch (m_nState)
	{
	case TDCFS_FILTER:
	case TDCFS_FILTER_TOGGLED:
		LoadUserFilter(prefs, sKey);
		break;

	case TDCFS_CUSTOM:
	case TDCFS_CUSTOM_TOGGLED:
		LoadCustomFilter(prefs, sKey, aCustomAttribDefs);
		break;

	case TDCFS_NONE:
		break;

	default:
		ASSERT(0);
	}
}

void CTDCFilter::LoadCustomFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs)
{
	CString sSubKey = (sKey + _T("\\Custom"));

	m_custom.sName = prefs.GetProfileString(sSubKey, _T("Name"));
	m_custom.dwFlags = LoadFlags(prefs, sSubKey);

	// rules
	m_custom.params.bIgnoreDone = prefs.GetProfileInt(sSubKey, _T("IgnoreDone"), FALSE);
	m_custom.params.bIgnoreOverDue = prefs.GetProfileInt(sSubKey, _T("IgnoreOverDue"), FALSE);
	m_custom.params.bWantAllSubtasks = prefs.GetProfileInt(sSubKey, _T("WantAllSubtasks"), FALSE);

	int nNumRules = prefs.GetProfileInt(sSubKey, _T("NumRules"), 0), nNumDupes = 0;
	SEARCHPARAM rule, rulePrev;

	for (int nRule = 0; nRule < nNumRules; nRule++)
	{
		CString sRule = Misc::MakeKey(_T("Rule%d"), nRule, sSubKey);

		// stop loading if we meet an invalid rule
		if (!CTDCSearchParamHelper::LoadRule(prefs, sRule, aCustomAttribDefs, rule))
			break;

		// stop loading if we get more than 2 duplicates in a row.
		// this handles a bug resulting in an uncontrolled 
		// proliferation of duplicates that I cannot reproduce
		if (rule == rulePrev)
		{
			nNumDupes++;

			if (nNumDupes > 2)
				break;
		}
		else
		{
			nNumDupes = 0; // reset
			rulePrev = rule; // new 'previous rule
		}

		m_custom.params.aRules.Add(rule);
	}
}

void CTDCFilter::SaveCustomFilter(CPreferences& prefs, const CString& sKey) const
{
	// check anything to save
	if (m_custom.params.aRules.GetSize() == 0)
		return;

	CString sSubKey = (sKey + _T("\\Custom"));

	prefs.WriteProfileString(sSubKey, _T("Name"), m_custom.sName);

	prefs.WriteProfileInt(sSubKey, _T("IgnoreDone"), m_custom.params.bIgnoreDone);
	prefs.WriteProfileInt(sSubKey, _T("IgnoreOverDue"), m_custom.params.bIgnoreOverDue);
	prefs.WriteProfileInt(sSubKey, _T("WantAllSubtasks"), m_custom.params.bWantAllSubtasks);

	int nNumRules = 0, nNumDupes = 0;
	SEARCHPARAM rulePrev;

	for (int nRule = 0; nRule < m_custom.params.aRules.GetSize(); nRule++)
	{
		CString sRule = Misc::MakeKey(_T("Rule%d"), nRule, sSubKey);
		const SEARCHPARAM& rule = m_custom.params.aRules[nRule];

		// stop saving if we meet an invalid rule
		if (!CTDCSearchParamHelper::SaveRule(prefs, sRule, rule))
			break;

		// stop saving if we get more than 2 duplicates in a row.
		// this handles a bug resulting in an uncontrolled 
		// proliferation of duplicates that I cannot reproduce
		if (rule == rulePrev)
		{
			nNumDupes++;

			if (nNumDupes > 2)
				break;
		}
		else
		{
			nNumDupes = 0; // reset
			rulePrev = rule; // new 'previous rule
		}

		nNumRules++;
	}
	prefs.WriteProfileInt(sSubKey, _T("NumRules"), nNumRules);

	// options
	SaveFlags(m_custom.dwFlags, prefs, sSubKey);
}

void CTDCFilter::LoadUserFilter(const CPreferences& prefs, const CString& sKey)
{
	m_filter.nShow = (FILTER_SHOW)prefs.GetProfileInt(sKey, _T("Show"), FS_ALL);
	m_filter.nStartBy = (FILTER_DATE)prefs.GetProfileInt(sKey, _T("Start"), FD_ANY);
	m_filter.nDueBy = (FILTER_DATE)prefs.GetProfileInt(sKey, _T("Due"), FD_ANY);

	m_filter.sTitle = prefs.GetProfileString(sKey, _T("Title"));
	m_filter.nPriority = prefs.GetProfileInt(sKey, _T("Priority"), FM_ANYPRIORITY);
	m_filter.nRisk = prefs.GetProfileInt(sKey, _T("Risk"), FM_ANYRISK);
	m_filter.nStartNextNDays = prefs.GetProfileInt(sKey, _T("StartNextNDays"), 7);
	m_filter.nDueNextNDays = prefs.GetProfileInt(sKey, _T("DueNextNDays"), 7);

	// dates
	if (m_filter.nStartBy == FD_USER)
		m_filter.dtUserStart = prefs.GetProfileDouble(sKey, _T("UserStart"), COleDateTime::GetCurrentTime());
	else
		m_filter.dtUserStart = COleDateTime::GetCurrentTime();

	if (m_filter.nDueBy == FD_USER)
		m_filter.dtUserDue = prefs.GetProfileDouble(sKey, _T("UserDue"), COleDateTime::GetCurrentTime());
	else
		m_filter.dtUserDue = COleDateTime::GetCurrentTime();

	// arrays
	prefs.GetProfileArray(sKey + _T("\\Category"), m_filter.aCategories, TRUE);
	prefs.GetProfileArray(sKey + _T("\\AllocTo"), m_filter.aAllocTo, TRUE);
	prefs.GetProfileArray(sKey + _T("\\AllocBy"), m_filter.aAllocBy, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Status"), m_filter.aStatus, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Version"), m_filter.aVersions, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Tags"), m_filter.aTags, TRUE);

	// options
	m_filter.dwFlags = LoadFlags(prefs, sKey);
}

DWORD CTDCFilter::LoadFlags(const CPreferences& prefs, const CString& sKey)
{
	DWORD dwFlags = 0;

	if (prefs.GetProfileInt(sKey, _T("AnyCategory"), TRUE))
		dwFlags |= FO_ANYCATEGORY;

	if (prefs.GetProfileInt(sKey, _T("AnyAllocTo"), TRUE))
		dwFlags |= FO_ANYALLOCTO;

	if (prefs.GetProfileInt(sKey, _T("AnyTag"), TRUE))
		dwFlags |= FO_ANYTAG;

	if (prefs.GetProfileInt(sKey, _T("HideParents"), FALSE))
		dwFlags |= FO_HIDEPARENTS;

	if (prefs.GetProfileInt(sKey, _T("HideOverDue"), FALSE))
		dwFlags |= FO_HIDEOVERDUE;

	if (prefs.GetProfileInt(sKey, _T("HideDone"), FALSE))
		dwFlags |= FO_HIDEDONE;

	if (prefs.GetProfileInt(sKey, _T("HideCollapsed"), FALSE))
		dwFlags |= FO_HIDECOLLAPSED;

	if (prefs.GetProfileInt(sKey, _T("ShowAllSubtasks"), FALSE))
		dwFlags |= FO_SHOWALLSUB;

	return dwFlags;
}

void CTDCFilter::SaveFlags(DWORD dwFlags, CPreferences& prefs, const CString& sKey)
{
	prefs.WriteProfileInt(sKey, _T("AnyAllocTo"), Misc::HasFlag(dwFlags, FO_ANYALLOCTO));
	prefs.WriteProfileInt(sKey, _T("AnyCategory"), Misc::HasFlag(dwFlags, FO_ANYCATEGORY));
	prefs.WriteProfileInt(sKey, _T("AnyTag"), Misc::HasFlag(dwFlags, FO_ANYTAG));
	prefs.WriteProfileInt(sKey, _T("HideParents"), Misc::HasFlag(dwFlags, FO_HIDEPARENTS));
	prefs.WriteProfileInt(sKey, _T("HideOverDue"), Misc::HasFlag(dwFlags, FO_HIDEOVERDUE));
	prefs.WriteProfileInt(sKey, _T("HideDone"), Misc::HasFlag(dwFlags, FO_HIDEDONE));
	prefs.WriteProfileInt(sKey, _T("HideCollapsed"), Misc::HasFlag(dwFlags, FO_HIDECOLLAPSED));
	prefs.WriteProfileInt(sKey, _T("ShowAllSubtasks"), Misc::HasFlag(dwFlags, FO_SHOWALLSUB));
}

void CTDCFilter::SaveUserFilter(CPreferences& prefs, const CString& sKey) const
{
	prefs.WriteProfileString(sKey, _T("Title"), m_filter.sTitle);

	prefs.WriteProfileInt(sKey, _T("Show"), m_filter.nShow);
	prefs.WriteProfileInt(sKey, _T("Start"), m_filter.nStartBy);
	prefs.WriteProfileInt(sKey, _T("Due"), m_filter.nDueBy);
	prefs.WriteProfileInt(sKey, _T("Priority"), m_filter.nPriority);
	prefs.WriteProfileInt(sKey, _T("Risk"), m_filter.nRisk);
	prefs.WriteProfileInt(sKey, _T("StartNextNDays"), m_filter.nStartNextNDays);
	prefs.WriteProfileInt(sKey, _T("DueNextNDays"), m_filter.nDueNextNDays);

	// dates
	prefs.WriteProfileDouble(sKey, _T("UserStart"), m_filter.dtUserStart);
	prefs.WriteProfileDouble(sKey, _T("UserDue"), m_filter.dtUserDue);

	// arrays
	prefs.WriteProfileArray(sKey + _T("\\AllocBy"), m_filter.aAllocBy);
	prefs.WriteProfileArray(sKey + _T("\\Status"), m_filter.aStatus);
	prefs.WriteProfileArray(sKey + _T("\\Version"), m_filter.aVersions);
	prefs.WriteProfileArray(sKey + _T("\\AllocTo"), m_filter.aAllocTo);
	prefs.WriteProfileArray(sKey + _T("\\Category"), m_filter.aCategories);
	prefs.WriteProfileArray(sKey + _T("\\Tags"), m_filter.aTags);

	// options
	SaveFlags(m_filter.dwFlags, prefs, sKey);
}

BOOL CTDCFilter::ModNeedsRefilter(TDC_ATTRIBUTE nModType, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const 
{
	// we only need to refilter if the modified attribute
	// actually affects the filter
	BOOL bNeedRefilter = FALSE;

	if (m_nState == TDCFS_CUSTOM) // 'Find' filter
	{
		bNeedRefilter = m_custom.params.HasAttribute(nModType); 

		if (bNeedRefilter)
		{
			// don't refilter on Time Estimate/Spent, Cost or Comments, or 
			// similar custom attributes  because the user typically hasn't 
			// finished editing when this notification is first received
			switch (nModType)
			{
			case TDCA_TIMESPENT:
			case TDCA_TIMEEST:
			case TDCA_COST:
			case TDCA_COMMENTS:
				bNeedRefilter = FALSE;
				break;

			default:
				if (CTDCCustomAttributeHelper::IsCustomAttribute(nModType))
				{
					TDCCUSTOMATTRIBUTEDEFINITION attribDef;
					VERIFY(CTDCCustomAttributeHelper::GetAttributeDef(nModType, aCustomAttribDefs, attribDef));

					if (!attribDef.IsList())
					{
						switch (attribDef.GetDataType())
						{
						case TDCCA_DOUBLE:
						case TDCCA_INTEGER:
						case TDCCA_STRING:
							bNeedRefilter = FALSE;
							break;
						}
					}
				}
			}
		}
	}
	else if (m_nState == TDCFS_FILTER) // 'Filter Bar' filter
	{
		switch (nModType)
		{
		case TDCA_TASKNAME:		
			bNeedRefilter = !m_filter.sTitle.IsEmpty(); 
			break;

		case TDCA_PRIORITY:		
			bNeedRefilter = (m_filter.nPriority != -1); 
			break;

		case TDCA_FLAG:		
			bNeedRefilter = (m_filter.nShow == FS_FLAGGED); 
			break;

		case TDCA_RISK:			
			bNeedRefilter = (m_filter.nRisk != -1);
			break;

		case TDCA_ALLOCBY:		
			bNeedRefilter = (m_filter.aAllocBy.GetSize() > 0);
			break;

		case TDCA_STATUS:		
			bNeedRefilter = (m_filter.aStatus.GetSize() > 0);
			break;

		case TDCA_VERSION:		
			bNeedRefilter = (m_filter.aVersions.GetSize() > 0);
			break;

		case TDCA_CATEGORY:		
			bNeedRefilter = (m_filter.aCategories.GetSize() > 0);
			break;

		case TDCA_TAGS:		
			bNeedRefilter = (m_filter.aTags.GetSize() > 0);
			break;

		case TDCA_ALLOCTO:		
			bNeedRefilter = (m_filter.aAllocTo.GetSize() > 0);
			break;

		case TDCA_PERCENT:
			bNeedRefilter = ((m_filter.nShow == FS_DONE) || (m_filter.nShow == FS_NOTDONE));
			break;

		case TDCA_DONEDATE:		
			// changing the DONE date requires refiltering if:
			bNeedRefilter = 
				// 1. The user wants to hide completed tasks
				(m_filter.HasFlag(FO_HIDEDONE) ||
				// 2. OR the user wants only completed tasks
				(m_filter.nShow == FS_DONE) || 
				// 3. OR the user wants only incomplete tasks
				(m_filter.nShow == FS_NOTDONE) ||
				// 4. OR a due date filter is active
				(m_filter.nDueBy != FD_ANY) ||
				// 5. OR a start date filter is active
				(m_filter.nStartBy != FD_ANY) ||
				// 6. OR the user is filtering on priority
				(m_filter.nPriority > 0));
			break;

		case TDCA_DUEDATE:		
			// changing the DUE date requires refiltering if:
			bNeedRefilter = 
				// 1. The user wants to hide overdue tasks
				((m_filter.HasFlag(FO_HIDEOVERDUE) ||
				// 2. OR the user is filtering on priority
				(m_filter.nPriority > 0) ||
				// 3. OR a due date filter is active
				(m_filter.nDueBy != FD_ANY)) &&
				// 3. AND the user doesn't want only completed tasks
				(m_filter.nShow != FS_DONE));
			break;

		case TDCA_STARTDATE:		
			// changing the START date requires refiltering if:
			bNeedRefilter = 
				// 1. A start date filter is active
				((m_filter.nStartBy != FD_ANY) &&
				// 2. AND the user doesn't want only completed tasks
				(m_filter.nShow != FS_DONE));
			break;
		}
	}
	return bNeedRefilter;
}
