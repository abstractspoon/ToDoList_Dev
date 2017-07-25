#if !defined(AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////

#include "..\interfaces\ITaskList.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCADATA
{
	TDCCADATA(const CString& sValue = _T(""), TCHAR cSep = 0);
	TDCCADATA(LPCTSTR szValue, TCHAR cSep = 0);
	TDCCADATA(double dValue);
	TDCCADATA(double dValue, TDC_UNITS nUnits);
	TDCCADATA(const CStringArray& aValues);
	TDCCADATA(int nValue);
	TDCCADATA(const COleDateTime& dtValue);
	TDCCADATA(bool bValue);
	TDCCADATA(const TDCCADATA& data);

	TDCCADATA& operator=(const TDCCADATA& data);

	BOOL operator==(const TDCCADATA& data) const;
	BOOL operator!=(const TDCCADATA& data) const;

	BOOL IsEmpty() const;
	void Clear();

	const CString& AsString() const;
	double AsDouble() const;
	int AsInteger() const;
	COleDateTime AsDate() const;
	bool AsBool() const;
	int AsArray(CStringArray& aValues) const;
	double AsTimePeriod(TDC_UNITS& nUnits) const;

	TDC_UNITS GetTimeUnits() const;

	void Set(double dValue);
	void Set(int nValue);
	void Set(const COleDateTime& dtValue);
	void Set(const CString& sValue, TCHAR cSep = 0);
	void Set(double dValue, TDC_UNITS nUnits);
	void Set(bool bValue, TCHAR nChar = 0);
	void Set(const CStringArray& aValues);

	CString FormatAsArray(TCHAR cSep = 0) const;
	CString FormatAsDate(BOOL bISO = FALSE) const;
	CString FormatAsTimePeriod(int nDecimalPlaces = 2) const;

protected:
	CString sData;
};

/////////////////////////////////////////////////////////////////////////////

class CTDCCustomAttributeDataMap : public CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>
{
public:
	BOOL Lookup(LPCTSTR key, TDCCADATA& rValue) const;
	void SetAt(LPCTSTR key, TDCCADATA& newValue);
	void Copy(const CTDCCustomAttributeDataMap& mapData);
	BOOL MatchAll(const CTDCCustomAttributeDataMap& mapData) const;

	TDCCADATA& operator[](LPCTSTR key);
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
