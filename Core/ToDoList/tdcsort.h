#if !defined(AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdlutil.h : header file
//

#include "tdcenum.h"

#include "..\shared\misc.h"
#include "..\shared\mapex.h"
#include "..\shared\enstring.h"

#include "..\Interfaces\preferences.h"

/////////////////////////////////////////////////////////////////////////////////////////////

class CTDCAttributeMap;
class CTDCStyleMap;
class CTDCCustomAttribDefinitionArray;

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDSORTCOLUMN
{
	TDSORTCOLUMN(TDC_COLUMN nSortBy = TDCC_NONE, BOOL bSortAscending = -1);

	BOOL IsSorting() const;
	BOOL IsSortingBy(TDC_COLUMN nSortBy) const;
	BOOL IsSortingByCustom() const;
	BOOL SetSortBy(TDC_COLUMN nSortBy, BOOL bSortAscending = -1);

	BOOL Verify() const;
	void Clear();
	BOOL Validate();

	BOOL Matches(const CTDCAttributeMap& mapAttribIDs,
				 const CTDCStyleMap& styles,
				 const CTDCCustomAttribDefinitionArray& aCustAttribs) const;

	static BOOL Verify(TDC_COLUMN nSortBy);

	static BOOL Matches(TDC_ATTRIBUTE nAttribID,
						TDC_COLUMN nSortBy,
						const CTDCStyleMap& styles,
						const CTDCCustomAttribDefinitionArray& aCustAttribs);

	TDC_COLUMN nBy;
	BOOL bAscending;
};

struct TDSORTCOLUMNS
{
	TDSORTCOLUMNS();
	TDSORTCOLUMNS(TDC_COLUMN nBy, BOOL bAscending = -1);

	BOOL IsSorting() const;
	BOOL IsSorting(int nCol) const;
	BOOL IsSortingBy(TDC_COLUMN nSortBy) const;
	TDC_COLUMN GetSortBy(int nCol) const;
	
	BOOL SetSortBy(const TDSORTCOLUMNS& sortCols);
	BOOL SetSortBy(int nCol, const TDSORTCOLUMN& col);
	BOOL SetSortBy(int nCol, TDC_COLUMN nBy, BOOL bAscending = -1);

	const TDSORTCOLUMN* Cols() const;

	BOOL Verify() const;
	void Clear();
	BOOL Validate();

	BOOL Matches(const CTDCAttributeMap& mapAttribIDs,
				 const CTDCStyleMap& styles,
				 const CTDCCustomAttribDefinitionArray& aCustAttribs) const;

protected:
	TDSORTCOLUMN cols[3];
};

struct TDSORT
{
	TDSORT(TDC_COLUMN nBy = TDCC_NONE, BOOL bAscending = -1);

	BOOL IsSorting() const;
	BOOL IsSortingBy(TDC_COLUMN nBy, BOOL bCheckMulti) const;

	BOOL SetSortBy(TDC_COLUMN nBy, BOOL bAscending);
	BOOL SetSortBy(const TDSORTCOLUMN& col);
	BOOL SetSortBy(const TDSORTCOLUMNS& cols);

	BOOL Verify() const;
	BOOL Validate();

	BOOL Matches(const CTDCAttributeMap& mapAttribIDs,
				 const CTDCStyleMap& styles,
				 const CTDCCustomAttribDefinitionArray& aCustAttribs) const;

	void SaveState(IPreferences* pPrefs, const CString& sKey) const;
	BOOL LoadState(const IPreferences* pPrefs, const CString& sKey);

	TDSORTCOLUMN single;
	TDSORTCOLUMNS multi;

	BOOL bMulti;
	BOOL bModSinceLastSort;
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
