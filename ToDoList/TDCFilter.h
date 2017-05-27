#pragma once

//////////////////////////////////////////////////////////////////////////

#include "tdcstruct.h"

//////////////////////////////////////////////////////////////////////////

class CPreferences;

//////////////////////////////////////////////////////////////////////////

class CTDCFilter
{
public:
	CTDCFilter();
	virtual ~CTDCFilter();

	BOOL SetFilter(const TDCFILTER& filter);
	BOOL SetAdvancedFilter(const TDCADVANCEDFILTER& filter);
	BOOL FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom = NULL, DWORD dwCustomFlags = 0, DWORD dwIgnoreFlags = 0) const;
	BOOL ClearFilter();
	BOOL ToggleFilter();

	BOOL HasAnyFilter(DWORD dwIgnoreFlags = 0) const;
	BOOL HasAdvancedFilter() const;
	BOOL HasAdvancedFilterAttribute(TDC_ATTRIBUTE nAttrib) const;
	BOOL HasFilterFlag(DWORD dwFlag) const;
	BOOL HasNowFilter() const;
	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const;

	CString GetAdvancedFilterName() const;
	DWORD GetFilterFlags() const;
	BOOL CanToggleFilter() const;
	BOOL BuildFilterQuery(SEARCHPARAMS& params, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	BOOL ModNeedsRefilter(TDC_ATTRIBUTE nModType, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	FILTER_SHOW GetFilter(TDCFILTER& filter) const;
	FILTER_SHOW GetFilter() const;

	void LoadFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs);
	void SaveFilter(CPreferences& prefs, const CString& sKey) const;

	CTDCFilter& operator=(const CTDCFilter& filter);
	BOOL operator==(const CTDCFilter& filter) const;

protected:
	TDCFILTER m_filter;
	TDCADVANCEDFILTER m_advFilter;

	enum TDCFILTER_STATE
	{
		TDCFS_NONE,
		TDCFS_FILTER,
		TDCFS_ADVANCED,
		TDCFS_FILTER_TOGGLED,
		TDCFS_ADVANCED_TOGGLED,
	};
	TDCFILTER_STATE m_nState;
	
protected:
	static void LoadFilter(const CPreferences& prefs, const CString& sKey, TDCFILTER& filter);
	static void SaveFilter(CPreferences& prefs, const CString& sKey, const TDCFILTER& filter);
	static void LoadAdvancedFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, TDCADVANCEDFILTER& filter);
	static void SaveAdvancedFilter(CPreferences& prefs, const CString& sKey, const TDCADVANCEDFILTER& filter);
	
	static void BuildFilterQuery(const TDCFILTER& filter, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, SEARCHPARAMS& params);
	static void AddNonDateFilterQueryRules(const TDCFILTER& filter, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, SEARCHPARAMS& params);
	static BOOL InitFilterDate(FILTER_DATE nDate, const COleDateTime& dateUser, int nNextNDays, COleDateTime& dateDue);
	static void AppendArrayRule(const CStringArray& aValues, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules, 
								DWORD dwFlags = 0, DWORD dwMask = 0);
	static void AppendPriorityRiskRule(int nValue, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules,
										int nAnyValue, int nNoValue);

	static DWORD LoadFlags(const CPreferences& prefs, const CString& sKey);
	static void SaveFlags(DWORD dwFlags, CPreferences& prefs, const CString& sKey);
	static BOOL FlagsMatch(DWORD dwFlags1, DWORD dwFlags2, DWORD dwIgnore);

};

