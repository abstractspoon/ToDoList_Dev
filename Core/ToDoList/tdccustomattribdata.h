#if !defined(AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////////////////

#include "..\interfaces\ITaskList.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCOST;
struct TDCTIMEPERIOD;

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCADATA
{
	TDCCADATA(const CString& sValue = _T(""), TCHAR cSep = 0);
	TDCCADATA(LPCTSTR szValue, TCHAR cSep = 0);
	TDCCADATA(double dValue);
	TDCCADATA(const TDCCOST& cost);
	TDCCADATA(const TDCTIMEPERIOD& time);
	TDCCADATA(const CStringArray& aValues);
	TDCCADATA(const CStringArray& aValues, const CStringArray& aExtra);
	TDCCADATA(int nValue);
	TDCCADATA(const COleDateTime& dtValue);
	TDCCADATA(bool bValue);
	TDCCADATA(const TDCCADATA& data);

	TDCCADATA& operator=(const TDCCADATA& data);

	BOOL operator==(const TDCCADATA& data) const;
	BOOL operator!=(const TDCCADATA& data) const;

	BOOL HasExtra() const;
	BOOL IsEmpty() const;
	void Clear();

	const CString& AsString() const;
	double AsDouble() const;
	double AsFraction() const;
	int AsInteger() const;
	COleDateTime AsDate() const;
	bool AsBool() const;
	int AsArray(CStringArray& aValues) const;
	int AsArrays(CStringArray& aValues, CStringArray& aExtra) const;
	BOOL AsTimePeriod(TDCTIMEPERIOD& time) const;
	BOOL AsCost(TDCCOST& cost) const;
	
	BOOL IsFraction() const;
	BOOL IsTimePeriod() const;
	
	TDC_UNITS GetTimeUnits() const;

	void Set(double dValue);
	void Set(int nValue);
	void Set(const COleDateTime& dtValue);
	void Set(const CString& sValue, TCHAR cSep = 0);
	void Set(const TDCTIMEPERIOD& time);
	void Set(const TDCCOST& cost);
	void Set(bool bValue, TCHAR nChar = 0);
	void Set(const CStringArray& aValues);
	void Set(const CStringArray& aValues, const CStringArray& aExtra);

	CString FormatAsArray(TCHAR cSep = 0) const;
	CString FormatAsDate(BOOL bISO) const;
	CString FormatAsTimePeriod(int nDecimalPlaces = 2) const;

protected:
	CString sData, sExtra;

protected:
	static void Set(const CStringArray& aValues, CString& sValue);
	static int AsArray(const CString& sValue, CStringArray& aValues);

};

/////////////////////////////////////////////////////////////////////////////

class CTDCCustomAttributeDataMap : public CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>
{
public:
	BOOL Lookup(LPCTSTR key, TDCCADATA& rValue) const;
	void SetAt(LPCTSTR key, const TDCCADATA& newValue);
	void Copy(const CTDCCustomAttributeDataMap& mapData);
	BOOL MatchAll(const CTDCCustomAttributeDataMap& mapData) const;
	void RemoveKey(LPCTSTR key);

	TDCCADATA& operator[](LPCTSTR key);
};

/////////////////////////////////////////////////////////////////////////////

class CTDCMetaDataMap : public CMapStringToString
{
public:
	BOOL Lookup(LPCTSTR key, CString& rValue) const;
	void SetAt(LPCTSTR key, const CString& newValue);
	void Copy(const CTDCMetaDataMap& mapData);
	BOOL MatchAll(const CTDCMetaDataMap& mapData) const;
	void RemoveKey(LPCTSTR key);

	CString& operator[](LPCTSTR key);
};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
