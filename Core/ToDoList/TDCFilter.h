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
	BOOL FilterMatches(const TDCFILTER& filter, LPCTSTR szCustom = NULL, DWORD dwCustomFlags = 0) const;
	BOOL ClearFilter();
	BOOL ToggleFilter();

	BOOL HasAnyFilter() const;
	BOOL HasAdvancedFilter() const;
	BOOL HasAdvancedFilterAttribute(TDC_ATTRIBUTE nAttrib) const;
	BOOL HasFilterAttribute(TDC_ATTRIBUTE nAttrib, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	BOOL HasFilterFlag(DWORD dwFlag) const;
	BOOL HasNowFilter() const;
	BOOL HasNowFilter(TDC_ATTRIBUTE& nAttrib) const;
	BOOL HasSelectionFilter() const;

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

	static const CStringArray& GetDefaultFilterNames();
	static CString GetDefaultFilterName(FILTER_SHOW nFilter);
	static int BuildAdvancedFilterMenuItemMapping(const CStringArray& aFromNames, const CStringArray& aToNames, CMap<UINT, UINT, UINT, UINT&>& mapMenuIDs);

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

	static DWORD LoadFlags(const CPreferences& prefs, const CString& sKey);
	static void SaveFlags(DWORD dwFlags, CPreferences& prefs, const CString& sKey);
	static BOOL FlagsMatch(DWORD dwFlags1, DWORD dwFlags2, DWORD dwIgnore);

};

