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

	BOOL SetFilter(const FTDCFILTER& filter);
	BOOL SetCustomFilter(const FTDCCUSTOMFILTER& filter);
	BOOL FilterMatches(const FTDCFILTER& filter, LPCTSTR szCustom = NULL, DWORD dwCustomFlags = 0, DWORD dwIgnoreFlags = 0) const;
	BOOL ClearFilter();
	BOOL ToggleFilter();

	BOOL HasAnyFilter(DWORD dwIgnoreFlags = 0) const;
	CString GetCustomFilterName() const;
	BOOL HasCustomFilter() const;
	BOOL HasCustomFilterAttribute(TDC_ATTRIBUTE nAttrib) const;
	DWORD GetFilterFlags() const;
	BOOL HasFilterFlag(DWORD dwFlag) const;
	BOOL CanToggleFilter() const;
	BOOL BuildFilterQuery(SEARCHPARAMS& params, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	BOOL ModNeedsRefilter(TDC_ATTRIBUTE nModType, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs) const;
	FILTER_SHOW GetFilter(FTDCFILTER& filter) const;
	FILTER_SHOW GetFilter() const;

	void LoadFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs);
	void SaveFilter(CPreferences& prefs, const CString& sKey) const;

	CTDCFilter& operator=(const CTDCFilter& filter);
	BOOL operator==(const CTDCFilter& filter) const;

protected:
	FTDCFILTER m_filter;
	FTDCCUSTOMFILTER m_custom;

	enum TDCFILTER_STATE
	{
		TDCFS_NONE,
		TDCFS_FILTER,
		TDCFS_CUSTOM,
		TDCFS_FILTER_TOGGLED,
		TDCFS_CUSTOM_TOGGLED,
	};
	TDCFILTER_STATE m_nState;
	
protected:
	void LoadUserFilter(const CPreferences& prefs, const CString& sKey);
	void SaveUserFilter(CPreferences& prefs, const CString& sKey) const;
	void LoadCustomFilter(const CPreferences& prefs, const CString& sKey, const CTDCCustomAttribDefinitionArray& aCustomAttribDefs);
	void SaveCustomFilter(CPreferences& prefs, const CString& sKey) const;
	
	static void BuildFilterQuery(const FTDCFILTER& filter, SEARCHPARAMS& params);
	static void AddNonDateFilterQueryRules(const FTDCFILTER& filter, SEARCHPARAMS& params);
	static BOOL InitFilterDate(FILTER_DATE nDate, const COleDateTime& dateUser, int nNextNDays, COleDateTime& dateDue);
	static DWORD LoadFlags(const CPreferences& prefs, const CString& sKey);
	static void SaveFlags(DWORD dwFlags, CPreferences& prefs, const CString& sKey);
	static BOOL FlagsMatch(DWORD dwFlags1, DWORD dwFlags2, DWORD dwIgnore);
};

