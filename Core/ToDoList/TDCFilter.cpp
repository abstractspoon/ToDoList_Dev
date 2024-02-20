#include "stdafx.h"
#include "TDCFilter.h"
#include "TDCStatic.h"
#include "TDCSearchParamHelper.h"

#include "..\Interfaces\Preferences.h"

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
			return m_filter.Matches(filter.m_filter);
		break;

	case TDCFS_ADVANCED:
		if (filter.m_nState == TDCFS_ADVANCED)
			return m_advFilter.Matches(filter.m_advFilter);
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

BOOL CTDCFilter::FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom, DWORD dwCustomFlags) const
{
	BOOL bOtherIsCustom = !Misc::IsEmpty(szCustom);

	switch (m_nState)
	{
	case TDCFS_FILTER:
		return (!bOtherIsCustom && m_filter.Matches(filter));

	case TDCFS_ADVANCED:
		return (bOtherIsCustom && m_advFilter.Matches(szCustom, dwCustomFlags));

	case TDCFS_NONE:
	case TDCFS_FILTER_TOGGLED:
	case TDCFS_ADVANCED_TOGGLED:
		return (!bOtherIsCustom && !filter.IsSet());
	}

	ASSERT(0);
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
		filter.Reset(FS_ADVANCED);
		break;

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

BOOL CTDCFilter::HasAnyFilter() const
{
	switch (m_nState)
	{
	case TDCFS_FILTER:
		return m_filter.IsSet();

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
	if (filter.IsAdvanced())
	{
		ASSERT(0);
		return FALSE;
	}

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
	m_filter.Reset(FS_ADVANCED);

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

BOOL CTDCFilter::HasSelectionFilter() const
{
	return ((m_nState == TDCFS_FILTER) && (m_filter.nShow == FS_SELECTED));
}

BOOL CTDCFilter::HasAdvancedFilterAttribute(TDC_ATTRIBUTE nAttrib) const
{
	if (m_nState == TDCFS_ADVANCED)
		return m_advFilter.params.HasAttribute(nAttrib);

	// else
	ASSERT(0);
	return FALSE;
}

BOOL CTDCFilter::HasFilterAttribute(TDC_ATTRIBUTE nAttrib, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const
{
	switch (nAttrib)
	{
	case TDCA_ALL:
		return TRUE; // More detailed check done later

	case TDCA_TASKNAME:
		return !m_filter.sTitle.IsEmpty();

	case TDCA_PRIORITY:
		return (m_filter.nPriority != FM_ANYPRIORITY);

	case TDCA_FLAG:
		return (m_filter.nShow == FS_FLAGGED);

	case TDCA_LOCK:
		return (m_filter.nShow == FS_LOCKED);

	case TDCA_RISK:
		return (m_filter.nRisk != FM_ANYRISK);

	case TDCA_ALLOCBY:
		return (m_filter.aAllocBy.GetSize() > 0);

	case TDCA_STATUS:
		return (m_filter.aStatus.GetSize() > 0);

	case TDCA_VERSION:
		return (m_filter.aVersions.GetSize() > 0);

	case TDCA_CATEGORY:
		return (m_filter.aCategories.GetSize() > 0);

	case TDCA_TAGS:
		return (m_filter.aTags.GetSize() > 0);

	case TDCA_ALLOCTO:
		return (m_filter.aAllocTo.GetSize() > 0);

	case TDCA_PERCENT:
		return ((m_filter.nShow == FS_DONE) || (m_filter.nShow == FS_NOTDONE));

	case TDCA_DONEDATE:
		// changing the DONE date requires refiltering if:
		return
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

	case TDCA_DUEDATE:
		// changing the DUE date requires refiltering if:
		return 
			// 1. The user wants to hide overdue tasks
			((m_filter.HasFlag(FO_HIDEOVERDUE) ||
			// 2. OR the user is filtering on priority
			(m_filter.nPriority > 0) ||
			// 3. OR a due date filter is active
			(m_filter.nDueBy != FD_ANY)) &&
			// 4. AND the user doesn't want only completed tasks
			(m_filter.nShow != FS_DONE));

	case TDCA_STARTDATE:
		// changing the START date requires refiltering if:
		return 
			// 1. A start date filter is active
			((m_filter.nStartBy != FD_ANY) &&
			// 2. AND the user doesn't want only completed tasks
			(m_filter.nShow != FS_DONE));

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			CString sAttribID = aCustomAttribDefs.GetAttributeTypeID(nAttrib);

			return m_filter.mapCustomAttrib.HasKey(sAttribID);
		}
		break;
	}

	// all else
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

	// Date filters
	CTDCSearchParamHelper::AppendDateFilter(filter.nStartBy, filter.dtUserStart, filter.nStartNextNDays, TDCA_STARTDATE, TDCA_STARTTIME, params.aRules);
	CTDCSearchParamHelper::AppendDateFilter(filter.nDueBy, filter.dtUserDue, filter.nDueNextNDays, TDCA_DUEDATE, TDCA_DUETIME, params.aRules);

	// Title text
	BOOL bWantCustomAttrib = FALSE;

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

	CTDCSearchParamHelper::AppendArrayRule(filter.aCategories, TDCA_CATEGORY, params.aRules, filter.dwFlags, FO_ANYCATEGORY);
	CTDCSearchParamHelper::AppendArrayRule(filter.aAllocTo, TDCA_ALLOCTO, params.aRules, filter.dwFlags, FO_ANYPERSON);
	CTDCSearchParamHelper::AppendArrayRule(filter.aAllocBy, TDCA_ALLOCBY, params.aRules, filter.dwFlags, FO_ANYPERSON);
	CTDCSearchParamHelper::AppendArrayRule(filter.aStatus, TDCA_STATUS, params.aRules, filter.dwFlags, FO_ANYSTATUS);
	CTDCSearchParamHelper::AppendArrayRule(filter.aVersions, TDCA_VERSION, params.aRules, filter.dwFlags, FO_ANYVERSION);
	CTDCSearchParamHelper::AppendArrayRule(filter.aTags, TDCA_TAGS, params.aRules, filter.dwFlags, FO_ANYTAG);

	CTDCSearchParamHelper::AppendPriorityRiskRule(filter.nPriority, TDCA_PRIORITY, params.aRules, FM_ANYPRIORITY, FM_NOPRIORITY);
	CTDCSearchParamHelper::AppendPriorityRiskRule(filter.nRisk, TDCA_RISK, params.aRules, FM_ANYRISK, FM_NORISK);

	if (filter.nRecurrence != TDIR_NONE)
		params.aRules.Add(SEARCHPARAM(TDCA_RECURRENCE, FOP_EQUALS, filter.nRecurrence));

	// Custom Attributes
	bWantCustomAttrib |= CTDCSearchParamHelper::AppendCustomAttributeFilterRules(filter.mapCustomAttrib, aCustomAttribDefs, params.aRules);

	// special case: no rules + ignore completed
	if (params.bIgnoreDone && (params.aRules.GetSize() == 0))
		params.aRules.Add(SEARCHPARAM(TDCA_DONEDATE, FOP_NOT_SET));

	if (bWantCustomAttrib)
		params.aAttribDefs.Copy(aCustomAttribDefs);
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
	m_nState = prefs.GetProfileEnum(sKey, _T("State"), TDCFS_NONE);

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
	filter.nShow = prefs.GetProfileEnum(sKey, _T("Show"), FS_ALL);
	filter.nStartBy = prefs.GetProfileEnum(sKey, _T("Start"), FD_ANY);
	filter.nDueBy = prefs.GetProfileEnum(sKey, _T("Due"), FD_ANY);

	filter.sTitle = prefs.GetProfileString(sKey, _T("Title"));
	filter.nTitleOption = prefs.GetProfileEnum(sKey, _T("TitleOption"), FT_FILTERONTITLEONLY);
	filter.nPriority = prefs.GetProfileInt(sKey, _T("Priority"), FM_ANYPRIORITY);
	filter.nRisk = prefs.GetProfileInt(sKey, _T("Risk"), FM_ANYRISK);
	filter.nRecurrence = prefs.GetProfileEnum(sKey, _T("Recurrence"), TDIR_NONE);
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

			filter.mapCustomAttrib[sAttribID].Set(aItems, TRUE);
		}
	}
}

DWORD CTDCFilter::LoadFlags(const CPreferences& prefs, const CString& sKey)
{
	DWORD dwFlags = 0;

	Misc::SetFlag(dwFlags, FO_ANYCATEGORY,	prefs.GetProfileInt(sKey, _T("AnyCategory"), TRUE));
	Misc::SetFlag(dwFlags, FO_ANYPERSON,	prefs.GetProfileInt(sKey, _T("AnyAllocTo"), TRUE));
	Misc::SetFlag(dwFlags, FO_ANYTAG,		prefs.GetProfileInt(sKey, _T("AnyTag"), TRUE));
	Misc::SetFlag(dwFlags, FO_ANYSTATUS,	prefs.GetProfileInt(sKey, _T("AnyStatus"), TRUE));
	Misc::SetFlag(dwFlags, FO_ANYVERSION,	prefs.GetProfileInt(sKey, _T("AnyVersion"), TRUE));
	Misc::SetFlag(dwFlags, FO_HIDEOVERDUE,	prefs.GetProfileInt(sKey, _T("HideOverDue"), FALSE));
	Misc::SetFlag(dwFlags, FO_HIDEDONE,		prefs.GetProfileInt(sKey, _T("HideDone"), FALSE));
	Misc::SetFlag(dwFlags, FO_SHOWALLSUB,	prefs.GetProfileInt(sKey, _T("ShowAllSubtasks"), FALSE));

	return dwFlags;
}

void CTDCFilter::SaveFilter(CPreferences& prefs, const CString& sKey, const TDCFILTER& filter)
{
	prefs.WriteProfileString(sKey, _T("Title"), filter.sTitle);

	prefs.WriteProfileInt(sKey, _T("TitleOption"), filter.nTitleOption);
	prefs.WriteProfileInt(sKey, _T("Show"), filter.nShow);
	prefs.WriteProfileInt(sKey, _T("Start"), filter.nStartBy);
	prefs.WriteProfileInt(sKey, _T("Due"), filter.nDueBy);
	prefs.WriteProfileInt(sKey, _T("Priority"), filter.nPriority);
	prefs.WriteProfileInt(sKey, _T("Risk"), filter.nRisk);
	prefs.WriteProfileInt(sKey, _T("Recurrence"), filter.nRecurrence);
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
	prefs.WriteProfileInt(sKey, _T("AnyAllocTo"),		Misc::HasFlag(dwFlags, FO_ANYPERSON));
	prefs.WriteProfileInt(sKey, _T("AnyCategory"),		Misc::HasFlag(dwFlags, FO_ANYCATEGORY));
	prefs.WriteProfileInt(sKey, _T("AnyTag"),			Misc::HasFlag(dwFlags, FO_ANYTAG));
	prefs.WriteProfileInt(sKey, _T("AnyStatus"),		Misc::HasFlag(dwFlags, FO_ANYSTATUS));
	prefs.WriteProfileInt(sKey, _T("AnyVersion"),		Misc::HasFlag(dwFlags, FO_ANYVERSION));
	prefs.WriteProfileInt(sKey, _T("HideOverDue"),		Misc::HasFlag(dwFlags, FO_HIDEOVERDUE));
	prefs.WriteProfileInt(sKey, _T("HideDone"),			Misc::HasFlag(dwFlags, FO_HIDEDONE));
	prefs.WriteProfileInt(sKey, _T("ShowAllSubtasks"),	Misc::HasFlag(dwFlags, FO_SHOWALLSUB));
}

BOOL CTDCFilter::ModNeedsRefilter(TDC_ATTRIBUTE nModType, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const 
{
	// we only need to refilter if the modified attribute
	// actually affects the filter
	if (m_nState == TDCFS_ADVANCED) // 'Find' filter
	{
		if (HasAdvancedFilterAttribute(nModType))
		{
			// don't refilter on Time Estimate/Spent, Cost or Comments, or 
			// similar custom attributes because the user typically hasn't 
			// finished editing when this notification is first received
			switch (nModType)
			{
			case TDCA_TIMESPENT:
			case TDCA_TIMEESTIMATE:
			case TDCA_COST:
			case TDCA_COMMENTS:
				return FALSE;

			default:
				if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nModType))
				{
					const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
					GET_DEF_RET(aCustomAttribDefs, nModType, pDef, FALSE);

					if (!pDef->IsList())
					{
						switch (pDef->GetDataType())
						{
						case TDCCA_DOUBLE:
						case TDCCA_INTEGER:
						case TDCCA_STRING:
							return FALSE;
						}
					}
				}
			}

			// all else
			return TRUE;
		}
	}
	else if (m_nState == TDCFS_FILTER) // 'Filter Bar' filter
	{
		return HasFilterAttribute(nModType, aCustomAttribDefs);
	}

	return FALSE;
}

const CStringArray& CTDCFilter::GetDefaultFilterNames()
{
	static CStringArray aFilters;

	if (aFilters.GetSize() == 0)
	{
		for (int nFilter = 0; nFilter < NUM_SHOWFILTER; nFilter++)
			aFilters.Add(CEnString(SHOW_FILTERS[nFilter][0]));
	}

	return aFilters;
}

CString CTDCFilter::GetDefaultFilterName(FILTER_SHOW nShow)
{
	int nFilter = NUM_SHOWFILTER;

	while (nFilter--)
	{
		if (SHOW_FILTERS[nFilter][1] == (UINT)nShow)
			return CEnString(SHOW_FILTERS[nFilter][0]);
	}

	ASSERT(0);
	return _T("");
}

int CTDCFilter::BuildAdvancedFilterMenuItemMapping(const CStringArray& aFromNames, 
													const CStringArray& aToNames, 
													CMap<UINT, UINT, UINT, UINT&>& mapMenuIDs)
{
	mapMenuIDs.RemoveAll();

	int nNumFrom = aFromNames.GetSize();

	for (int nFrom = 0; nFrom < nNumFrom; nFrom++)
	{
		// Find this filter's name in the new list
		int nTo = Misc::Find(aFromNames[nFrom], aToNames, FALSE, TRUE);

		UINT nFromCmdID = (nFrom + ID_VIEW_ACTIVATEADVANCEDFILTER1);
		UINT nToCmdID = ((nTo == -1) ? 0 : (nTo + ID_VIEW_ACTIVATEADVANCEDFILTER1));

		if (nToCmdID != nFromCmdID)
			mapMenuIDs[nFromCmdID] = nToCmdID;
	}

	return mapMenuIDs.GetCount();
}
