#include "stdafx.h"
#include "TDCFilter.h"
#include "TDCSearchParamHelper.h"
#include "TDCCustomAttributeHelper.h"

#include "..\shared\preferences.h"

/////////////////////////////////////////////////////////////////////////

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
	m_advFilter = filter.m_advFilter;
	
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

	case TDCFS_ADVANCED:
		if (filter.m_nState == TDCFS_ADVANCED)
		{
			return m_advFilter.Matches(filter.m_advFilter);
		}
		break;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
		break;

	default:
		ASSERT(0);
	}

	// all else
	return FALSE;
}

BOOL CTDCFilter::FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom, DWORD dwCustomFlags, DWORD dwIgnoreFlags) const
{
	BOOL bCustom = HasAdvancedFilter();
	BOOL bOtherCustom = (szCustom && *szCustom);

	if (bCustom && bOtherCustom)
	{
		return m_advFilter.Matches(szCustom, dwCustomFlags, dwIgnoreFlags);
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

FILTER_SHOW CTDCFilter::GetFilter(TDCFILTER& filter) const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		filter = m_filter;
		break;

	case TDCFS_ADVANCED:
	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
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

	case TDCFS_ADVANCED:
		return FS_ADVANCED;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
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

	case TDCFS_ADVANCED:
		return (m_advFilter.params.GetRuleCount() != 0);

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

BOOL CTDCFilter::SetFilter(const TDCFILTER& filter)
{
	m_filter = filter;
	m_nState = TDCFS_FILTER;

	// clear custom filter
	m_advFilter.Reset();

	return TRUE;
}

BOOL CTDCFilter::SetAdvancedFilter(const TDCADVANCEDFILTER& filter)
{
	// error checking
	ASSERT(!filter.sName.IsEmpty() && filter.params.GetRuleCount());

	if (filter.sName.IsEmpty() || !filter.params.GetRuleCount())
		return FALSE;

	m_advFilter = filter;
	m_nState = TDCFS_ADVANCED;

	// clear user filter
	m_filter.Reset();

	return TRUE;
}

CString CTDCFilter::GetAdvancedFilterName() const
{
	if (m_nState == TDCFS_ADVANCED)
		return m_advFilter.sName;

	// else
	ASSERT(0);
	return _T("");
}

BOOL CTDCFilter::HasAdvancedFilter() const
{
	return (m_nState == TDCFS_ADVANCED);
}

BOOL CTDCFilter::HasNowFilter() const
{
	TDC_ATTRIBUTE nUnused;
	return HasNowFilter(nUnused);
}

BOOL CTDCFilter::HasNowFilter(TDC_ATTRIBUTE& nAttrib) const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		return m_filter.HasNowFilter(nAttrib);

	case TDCFS_ADVANCED:
		return m_advFilter.HasNowFilter(nAttrib);

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
		break;
	}

	return FALSE;
}

BOOL CTDCFilter::HasAdvancedFilterAttribute(TDC_ATTRIBUTE nAttrib) const
{
	if (m_nState == TDCFS_ADVANCED)
		return m_advFilter.params.HasAttribute(nAttrib);

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

	case TDCFS_ADVANCED:
		return m_advFilter.dwFlags;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
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
		m_advFilter.Reset();
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

	case TDCFS_ADVANCED:
		m_nState = TDCFS_ADVANCED_TOGGLED;
		return TRUE;

	case TDCFS_FILTER_TOGGLED:
		m_nState = TDCFS_FILTER;
		return TRUE;

	case TDCFS_ADVANCED_TOGGLED:
		m_nState = TDCFS_ADVANCED;
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
	case TDCFS_ADVANCED:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
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
		BuildFilterQuery(m_filter, aCustomAttribDefs, params);
		return TRUE;

	case TDCFS_ADVANCED:
		params = m_advFilter.params;

		params.aAttribDefs.Copy(aCustomAttribDefs);
		params.bIgnoreDone = m_advFilter.HasFlag(FO_HIDEDONE);
		params.bIgnoreOverDue = m_advFilter.HasFlag(FO_HIDEOVERDUE);
		params.bWantAllSubtasks = m_advFilter.HasFlag(FO_SHOWALLSUB);
		return TRUE;

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
		return FALSE;
	}

	ASSERT(0);
	return FALSE;
}

void CTDCFilter::BuildFilterQuery(const TDCFILTER& filter, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, SEARCHPARAMS& params)
{
	// reset the search
	params.aRules.RemoveAll();
	params.bIgnoreDone = filter.WantHideCompletedTasks();
	params.bWantAllSubtasks = filter.HasFlag(FO_SHOWALLSUB);

	// handle principle filter
	switch (filter.nShow)
	{
	case FS_ALL:
		break;

	case FS_DONE:
		ASSERT(!params.bIgnoreDone);
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_SET));
		break;

	case FS_NOTDONE:
		ASSERT(params.bIgnoreDone);
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

	case FS_LOCKED:
		params.aRules.Add(SEARCHPARAM(TDCA_LOCK, FOP_SET));
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
	}
	else if (filter.nStartBy == FD_NONE)
	{
		params.aRules.Add(SEARCHPARAM(TDCA_STARTDATE, FOP_NOT_SET));
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

		// this flag only applies to due filters
		params.bIgnoreOverDue = filter.HasFlag(FO_HIDEOVERDUE);
	}
	else if (filter.nDueBy == FD_NONE)
	{
		params.aRules.Add(SEARCHPARAM(TDCA_DUEDATE, FOP_NOT_SET));
	}

	// handle other attributes
	AddNonDateFilterQueryRules(filter, aCustomAttribDefs, params);
}

void CTDCFilter::AddNonDateFilterQueryRules(const TDCFILTER& filter, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, SEARCHPARAMS& params)
{
	BOOL bWantCustomAttrib = FALSE;

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
			bWantCustomAttrib = TRUE;
			break;

		case FT_FILTERONTITLEONLY:
		default:
			params.aRules.Add(SEARCHPARAM(TDCA_TASKNAME, FOP_INCLUDES, filter.sTitle));
			break;
		}
	}

	// note: these are all 'AND' 
	AppendArrayRule(filter.aCategories, TDCA_CATEGORY, params.aRules, filter.dwFlags, FO_ANYCATEGORY);
	AppendArrayRule(filter.aAllocTo, TDCA_ALLOCTO, params.aRules, filter.dwFlags, FO_ANYALLOCTO);
	AppendArrayRule(filter.aAllocBy, TDCA_ALLOCBY, params.aRules);
	AppendArrayRule(filter.aStatus, TDCA_STATUS, params.aRules);
	AppendArrayRule(filter.aVersions, TDCA_VERSION, params.aRules);
	AppendArrayRule(filter.aTags, TDCA_TAGS, params.aRules);

	AppendPriorityRiskRule(filter.nPriority, TDCA_PRIORITY, params.aRules, FM_ANYPRIORITY, FM_NOPRIORITY);
	AppendPriorityRiskRule(filter.nRisk, TDCA_RISK, params.aRules, FM_ANYRISK, FM_NORISK);

	// Custom Attributes
	bWantCustomAttrib = CTDCCustomAttributeHelper::AppendFilterRules(filter.mapCustomAttrib, aCustomAttribDefs, params.aRules);

	// special case: no rules + ignore completed
	if (params.bIgnoreDone && (params.aRules.GetSize() == 0))
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));

	if (bWantCustomAttrib)
		params.aAttribDefs.Copy(aCustomAttribDefs);
}

void CTDCFilter::AppendArrayRule(const CStringArray& aValues, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules, 
								DWORD dwFlags, DWORD dwIncludeMask)
{
	if (aValues.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(aValues);

		if ((aValues.GetSize() == 1) && sMatchBy.IsEmpty())
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_NOT_SET));
		}
		else if (dwFlags && dwIncludeMask)
		{
			if (dwFlags & dwIncludeMask)
				aRules.Add(SEARCHPARAM(nAttrib, FOP_INCLUDES, sMatchBy));
			else
				aRules.Add(SEARCHPARAM(nAttrib, FOP_EQUALS, sMatchBy));
		}
		else // includes
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_INCLUDES, sMatchBy));
		}
	}
}

void CTDCFilter::AppendPriorityRiskRule(int nValue, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules,
										int nAnyValue, int nNoValue)
{
	ASSERT((nAttrib == TDCA_PRIORITY) || (nAttrib == TDCA_RISK));

	if (nValue != nAnyValue)
	{
		if (nValue == nNoValue)
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_NOT_SET));
		}
		else if (nValue != nAnyValue)
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_GREATER_OR_EQUAL, nValue));
		}
	}
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
		SaveFilter(prefs, sKey, m_filter);
		break;

	case TDCFS_ADVANCED:
	case TDCFS_ADVANCED_TOGGLED:
		SaveAdvancedFilter(prefs, sKey, m_advFilter);
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
		LoadFilter(prefs, sKey, m_filter);
		break;

	case TDCFS_ADVANCED:
	case TDCFS_ADVANCED_TOGGLED:
		LoadAdvancedFilter(prefs, sKey, aCustomAttribDefs, m_advFilter);
		break;

	case TDCFS_NONE:
		break;

	default:
		ASSERT(0);
	}
}

void CTDCFilter::LoadAdvancedFilter(const CPreferences& prefs, const CString& sKey, 
									const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, 
									TDCADVANCEDFILTER& filter)
{
	CString sSubKey = (sKey + _T("\\Advanced"));

	// Backward compatibility
	if (!prefs.HasProfileSection(sSubKey))
		sSubKey = (sKey + _T("\\Custom"));

	filter.sName = prefs.GetProfileString(sSubKey, _T("Name"));
	filter.dwFlags = LoadFlags(prefs, sSubKey);

	// rules
	filter.params.bIgnoreDone = prefs.GetProfileInt(sSubKey, _T("IgnoreDone"), FALSE);
	filter.params.bIgnoreOverDue = prefs.GetProfileInt(sSubKey, _T("IgnoreOverDue"), FALSE);
	filter.params.bWantAllSubtasks = prefs.GetProfileInt(sSubKey, _T("WantAllSubtasks"), FALSE);

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

		filter.params.aRules.Add(rule);
	}
}

void CTDCFilter::SaveAdvancedFilter(CPreferences& prefs, const CString& sKey, const TDCADVANCEDFILTER& filter)
{
	// Delete old key(s)
	prefs.DeleteProfileSection(sKey + _T("\\Custom"));

	CString sSubKey = (sKey + _T("\\Advanced"));
	prefs.DeleteProfileSection(sSubKey);

	// check anything to save
	if (filter.params.aRules.GetSize() == 0)
		return;

	prefs.WriteProfileString(sSubKey, _T("Name"), filter.sName);

	prefs.WriteProfileInt(sSubKey, _T("IgnoreDone"), filter.params.bIgnoreDone);
	prefs.WriteProfileInt(sSubKey, _T("IgnoreOverDue"), filter.params.bIgnoreOverDue);
	prefs.WriteProfileInt(sSubKey, _T("WantAllSubtasks"), filter.params.bWantAllSubtasks);

	int nNumRules = 0, nNumDupes = 0;
	SEARCHPARAM rulePrev;

	for (int nRule = 0; nRule < filter.params.aRules.GetSize(); nRule++)
	{
		CString sRule = Misc::MakeKey(_T("Rule%d"), nRule, sSubKey);
		const SEARCHPARAM& rule = filter.params.aRules[nRule];

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
	SaveFlags(filter.dwFlags, prefs, sSubKey);
}

void CTDCFilter::LoadFilter(const CPreferences& prefs, const CString& sKey, TDCFILTER& filter)
{
	filter.nShow = (FILTER_SHOW)prefs.GetProfileInt(sKey, _T("Show"), FS_ALL);
	filter.nStartBy = (FILTER_DATE)prefs.GetProfileInt(sKey, _T("Start"), FD_ANY);
	filter.nDueBy = (FILTER_DATE)prefs.GetProfileInt(sKey, _T("Due"), FD_ANY);

	filter.sTitle = prefs.GetProfileString(sKey, _T("Title"));
	filter.nPriority = prefs.GetProfileInt(sKey, _T("Priority"), FM_ANYPRIORITY);
	filter.nRisk = prefs.GetProfileInt(sKey, _T("Risk"), FM_ANYRISK);
	filter.nStartNextNDays = prefs.GetProfileInt(sKey, _T("StartNextNDays"), 7);
	filter.nDueNextNDays = prefs.GetProfileInt(sKey, _T("DueNextNDays"), 7);

	// dates
	if (filter.nStartBy == FD_USER)
		filter.dtUserStart = prefs.GetProfileDouble(sKey, _T("UserStart"), COleDateTime::GetCurrentTime());
	else
		filter.dtUserStart = COleDateTime::GetCurrentTime();

	if (filter.nDueBy == FD_USER)
		filter.dtUserDue = prefs.GetProfileDouble(sKey, _T("UserDue"), COleDateTime::GetCurrentTime());
	else
		filter.dtUserDue = COleDateTime::GetCurrentTime();

	// arrays
	prefs.GetProfileArray(sKey + _T("\\Category"), filter.aCategories, TRUE);
	prefs.GetProfileArray(sKey + _T("\\AllocTo"), filter.aAllocTo, TRUE);
	prefs.GetProfileArray(sKey + _T("\\AllocBy"), filter.aAllocBy, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Status"), filter.aStatus, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Version"), filter.aVersions, TRUE);
	prefs.GetProfileArray(sKey + _T("\\Tags"), filter.aTags, TRUE);

	// options
	filter.dwFlags = LoadFlags(prefs, sKey);

	// Custom Attributes
	int nItem = prefs.GetProfileInt(sKey + _T("\\Custom"), _T("ItemCount"));

	while (nItem--)
	{
		CString sItemKey(Misc::MakeKey(_T("Custom\\Custom%d"), nItem, sKey));
		CStringArray aItems;

		if (prefs.GetProfileArray(sItemKey, aItems, TRUE))
		{
			CString sAttribID = prefs.GetProfileString(sItemKey, _T("AttributeID"));
			ASSERT(!sAttribID.IsEmpty());

			filter.mapCustomAttrib[sAttribID] = TDCCADATA(aItems).AsString();
		}
	}
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

void CTDCFilter::SaveFilter(CPreferences& prefs, const CString& sKey, const TDCFILTER& filter)
{
	prefs.WriteProfileString(sKey, _T("Title"), filter.sTitle);

	prefs.WriteProfileInt(sKey, _T("Show"), filter.nShow);
	prefs.WriteProfileInt(sKey, _T("Start"), filter.nStartBy);
	prefs.WriteProfileInt(sKey, _T("Due"), filter.nDueBy);
	prefs.WriteProfileInt(sKey, _T("Priority"), filter.nPriority);
	prefs.WriteProfileInt(sKey, _T("Risk"), filter.nRisk);
	prefs.WriteProfileInt(sKey, _T("StartNextNDays"), filter.nStartNextNDays);
	prefs.WriteProfileInt(sKey, _T("DueNextNDays"), filter.nDueNextNDays);

	// dates
	prefs.WriteProfileDouble(sKey, _T("UserStart"), filter.dtUserStart);
	prefs.WriteProfileDouble(sKey, _T("UserDue"), filter.dtUserDue);

	// arrays
	prefs.WriteProfileArray(sKey + _T("\\AllocBy"), filter.aAllocBy);
	prefs.WriteProfileArray(sKey + _T("\\Status"), filter.aStatus);
	prefs.WriteProfileArray(sKey + _T("\\Version"), filter.aVersions);
	prefs.WriteProfileArray(sKey + _T("\\AllocTo"), filter.aAllocTo);
	prefs.WriteProfileArray(sKey + _T("\\Category"), filter.aCategories);
	prefs.WriteProfileArray(sKey + _T("\\Tags"), filter.aTags);

	// options
	SaveFlags(filter.dwFlags, prefs, sKey);

	// Custom Attributes
	POSITION pos = filter.mapCustomAttrib.GetStartPosition();
	CString sAttribID;
	TDCCADATA data;
	int nItem = 0;

	while (pos)
	{
		filter.mapCustomAttrib.GetNextAssoc(pos, sAttribID, data);
		ASSERT(!sAttribID.IsEmpty());

		CStringArray aItems;

		if (data.AsArray(aItems))
		{
			CString sItemKey(Misc::MakeKey(_T("Custom\\Custom%d"), nItem++, sKey));

			// Write ID after array because array will pre-delete the key
			prefs.WriteProfileArray(sItemKey, aItems);
			prefs.WriteProfileString(sItemKey, _T("AttributeID"), sAttribID);
		}
	}	
	
	prefs.WriteProfileInt(sKey + _T("\\Custom"), _T("ItemCount"), nItem);
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

BOOL CTDCFilter::ModNeedsRefilter(TDC_ATTRIBUTE nModType, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const 
{
	// we only need to refilter if the modified attribute
	// actually affects the filter
	BOOL bNeedRefilter = FALSE;

	if (m_nState == TDCFS_ADVANCED) // 'Find' filter
	{
		bNeedRefilter = m_advFilter.params.HasAttribute(nModType); 

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

		case TDCA_LOCK:		
			bNeedRefilter = (m_filter.nShow == FS_LOCKED); 
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
