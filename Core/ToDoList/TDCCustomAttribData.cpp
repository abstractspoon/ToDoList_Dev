
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCCustomAttribData.h"
#include "TDCmapping.h"
#include "todoitem.h"

#include "..\shared\misc.h"
#include "..\shared\datehelper.h"
#include "..\shared\timehelper.h"

/////////////////////////////////////////////////////////////////////////////

const TCHAR NEWLINE = '\n';

/////////////////////////////////////////////////////////////////////////////

TDCCADATA::TDCCADATA()
{

}

TDCCADATA::TDCCADATA(const CString& sValue)
{
	Set(sValue);
}

TDCCADATA::TDCCADATA(LPCTSTR szValue)
{
	Set(CString(szValue));
}

TDCCADATA::TDCCADATA(double dValue) 
{ 
	Set(dValue); 
}

TDCCADATA::TDCCADATA(const TDCTIMEPERIOD& time)
{ 
	Set(time); 
}

TDCCADATA::TDCCADATA(const TDCCOST& cost)
{ 
	Set(cost); 
}

TDCCADATA::TDCCADATA(const CStringArray& aValues, BOOL bAllowEmpty) 
{ 
	Set(aValues, bAllowEmpty); 
}

TDCCADATA::TDCCADATA(const CStringArray& aValues, const CStringArray& aExtra, BOOL bAllowEmpty) 
{ 
	Set(aValues, aExtra, bAllowEmpty); 
}

TDCCADATA::TDCCADATA(int nValue) 
{ 
	Set(nValue); 
}

TDCCADATA::TDCCADATA(const COleDateTime& dtValue) 
{ 
	Set(dtValue); 
}

TDCCADATA::TDCCADATA(bool bValue) 
{ 
	Set(bValue); 
}

TDCCADATA::TDCCADATA(const TDCCADATA& data)
{
	*this = data;
}

TDCCADATA& TDCCADATA::operator=(const TDCCADATA& data)
{
	sData = data.sData;
	sExtra = data.sExtra;

	return *this;
}

BOOL TDCCADATA::operator==(const TDCCADATA& data) const 
{ 
	return ((sData == data.sData) && 
			(sExtra == data.sExtra)); 
}

BOOL TDCCADATA::operator!=(const TDCCADATA& data) const 
{ 
	return ((sData != data.sData) || 
			(sExtra != data.sExtra)); 
}

BOOL TDCCADATA::HasExtra() const
{
	return !sExtra.IsEmpty();
}

BOOL TDCCADATA::IsEmpty() const 
{ 
	return (sData.IsEmpty() && sExtra.IsEmpty()); 
}

void TDCCADATA::Clear() 
{ 
	sData.Empty(); 
	sExtra.Empty();
}

const CString& TDCCADATA::AsString() const 
{ 
	ASSERT(sExtra.IsEmpty());

	return sData; 
}

BOOL TDCCADATA::IsFraction() const
{
	int nDiv = sData.Find('/');

	return ((nDiv > 0) && (nDiv < Misc::LastIndex(sData)));
}

double TDCCADATA::AsFraction() const 
{ 
	ASSERT(sExtra.IsEmpty());

	if (sData.IsEmpty())
		return 0.0;

	int nDiv = sData.Find('/');

	if ((nDiv == -1) || (nDiv == Misc::LastIndex(sData)))
		return AsDouble();

	double dNumerator = _ttof(sData); // will stop when it hits '/'
	double dDenominator = _ttof((LPCTSTR)sData + nDiv + 1);

	if (dDenominator == 0.0)
		dDenominator = 1.0;

	return (dNumerator / dDenominator);
}

double TDCCADATA::AsDouble() const
{
	ASSERT(sExtra.IsEmpty());

	return _ttof(sData);
}

int TDCCADATA::AsInteger() const 
{ 
	ASSERT(sExtra.IsEmpty());

	return _ttoi(sData); 
} 

COleDateTime TDCCADATA::AsDate() const 
{ 
	ASSERT(sExtra.IsEmpty());

	if (sData.IsEmpty())
		return CDateHelper::NullDate();

	// Try decoding the date string first
	COleDateTime date;

	if (CDateHelper::DecodeDate(sData, date, TRUE))
		return date;
	
	// else
	return _ttof(sData); 
}

bool TDCCADATA::AsBool() const 
{ 
	ASSERT(sExtra.IsEmpty());

	return !IsEmpty(); 
}

int TDCCADATA::AsArray(CStringArray& aValues) const
{
	ASSERT(sExtra.IsEmpty());

	return AsArray(sData, aValues);
}

int TDCCADATA::AsArrays(CStringArray& aValues, CStringArray& aExtra) const
{
	AsArray(sExtra, aExtra);

	return AsArray(sData, aValues);
}

int TDCCADATA::AsArray(const CString& sValue, CStringArray& aValues) 
{ 
	// Special case: 1 empty value
	if (sValue == _T("\n"))
	{
		aValues.RemoveAll();
		aValues.Add(_T(""));
		return 1;
	}
	
	// else
	return Misc::Split(sValue, aValues, NEWLINE, TRUE);
}

BOOL TDCCADATA::IsArray() const
{
	return (sData.Find(NEWLINE) > 0);
}

int TDCCADATA::GetArraySize() const
{
	int nLen = sData.GetLength();

	if (nLen < 2)
		return nLen;

	// Just count the new line characters
	int nPos = sData.Find(NEWLINE), nSize = 0;

	while (nPos != -1)
	{
		nSize++;
		nPos = sData.Find(NEWLINE, (nPos + 1));
	}

	// If we don't end in a newline then add trailing item
	if (Misc::Last(sData) != NEWLINE)
		nSize++;

	return nSize;
}

BOOL TDCCADATA::IsTimePeriod() const
{
	return (IsEmpty() || (GetTimeUnits() != TDCU_NULL));
}

BOOL TDCCADATA::AsTimePeriod(TDCTIMEPERIOD& time) const
{
	if (IsEmpty())
	{
		time.nUnits = TDCU_HOURS;
		time.dAmount = 0.0;
	}
	else
	{
		time.nUnits = GetTimeUnits();
		time.dAmount = AsDouble();
	}
	
	return (time.nUnits != TDCU_NULL);
}

BOOL TDCCADATA::AsCost(TDCCOST& cost) const
{
	return cost.Parse(sData);
}

TDC_UNITS TDCCADATA::GetTimeUnits() const
{
	if (IsEmpty())
		return TDCU_HOURS;
	
	TDC_UNITS nUnits = (TDC_UNITS)Misc::Last(sData);
	
	if (!IsValidUnits(nUnits))
		nUnits = TDCU_NULL;
	
	return nUnits;
}

void TDCCADATA::Set(double dValue) 
{ 
	sData.Format(_T("%lf"), dValue); 
}

void TDCCADATA::Set(int nValue) 
{ 
	sData.Format(_T("%d"), nValue); 
}

void TDCCADATA::Set(const COleDateTime& dtValue) 
{ 
	sData.Format(_T("%lf"), dtValue); 
}

void TDCCADATA::Set(const CString& sValue) 
{ 
	sData = sValue; 
}

void TDCCADATA::Set(const TDCTIMEPERIOD& time)
{ 
	ASSERT(IsValidUnits(time.nUnits));
	sData.Format(_T("%lf:%lc"), time.dAmount, (TCHAR)time.nUnits); 
}

void TDCCADATA::Set(const TDCCOST& cost)
{
	sData = cost.Format();
}

void TDCCADATA::Set(bool bValue, TCHAR nChar) 
{ 
	sData.Empty(); 
	
	if (bValue)
		sData.Insert(0, (nChar ? nChar : '+'));
}

void TDCCADATA::Set(const CStringArray& aValues, BOOL bAllowEmpty) 
{ 
	Set(aValues, bAllowEmpty, sData);
}

void TDCCADATA::Set(const CStringArray& aValues, const CStringArray& aExtra, BOOL bAllowEmpty) 
{ 
	Set(aValues, bAllowEmpty, sData);
	Set(aExtra, bAllowEmpty, sExtra);
}

void TDCCADATA::Set(const CStringArray& aValues, BOOL bAllowEmpty, CString& sValue)
{
	ASSERT(bAllowEmpty || !Misc::HasEmpty(aValues));

	// Special case: 1 empty value
	if (bAllowEmpty && (aValues.GetSize() == 1) && aValues[0].IsEmpty())
		sValue = NEWLINE;
	else	
		sValue = Misc::FormatArray(aValues, NEWLINE, bAllowEmpty);
}

CString TDCCADATA::FormatAsArray(TCHAR cSep) const
{
	if (cSep == NEWLINE)
		return sData;
	
	CString sArray(sData);
	
	if (cSep != 0)
		sArray.Replace(NEWLINE, cSep);
	else
		sArray.Replace(_T("\n"), (Misc::GetListSeparator() + ' '));
	
	return sArray;
}

CString TDCCADATA::FormatAsDate(BOOL bISO, BOOL bWithTime) const
{
	if (IsEmpty())
		return sData;

	// else
	COleDateTime date(AsDate());
	DWORD dwFlags = DHFD_NOSEC;
	
	if (bWithTime && CDateHelper::DateHasTime(date))
		dwFlags |= DHFD_TIME;
	
	if (bISO)
		dwFlags |= DHFD_ISO;
	
	return CDateHelper::FormatDate(date, dwFlags);
}

CString TDCCADATA::FormatAsTimePeriod(int nDecimalPlaces) const
{
	TDCTIMEPERIOD time;
	
	if (!AsTimePeriod(time))
		return _T("");

	// else
	return time.Format(nDecimalPlaces);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CTDCCustomAttributeDataMap::Lookup(LPCTSTR key, TDCCADATA& rValue) const
{
	return CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>::Lookup(Misc::ToUpper(key), rValue);
}

BOOL CTDCCustomAttributeDataMap::HasKey(LPCTSTR key) const
{
	TDCCADATA dataUnused;
	return Lookup(key, dataUnused);
}

TDCCADATA& CTDCCustomAttributeDataMap::operator[](LPCTSTR key)
{
	return CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>::operator[](Misc::ToUpper(key));
}

void CTDCCustomAttributeDataMap::SetAt(LPCTSTR key, const TDCCADATA& newValue)
{ 
	(*this)[key] = newValue; 
}

void CTDCCustomAttributeDataMap::Copy(const CTDCCustomAttributeDataMap& mapData)
{
	Misc::CopyStrT<TDCCADATA>(mapData, *this);
}

BOOL CTDCCustomAttributeDataMap::MatchAll(const CTDCCustomAttributeDataMap& mapData) const
{
	return Misc::MatchAllStrT<TDCCADATA>(*this, mapData);
}

void CTDCCustomAttributeDataMap::RemoveKey(LPCTSTR key)
{
	CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>::RemoveKey(Misc::ToUpper(key));
}

/////////////////////////////////////////////////////////////////////////////

BOOL CTDCMetaDataMap::Lookup(LPCTSTR key, CString& rValue) const
{
	return CMapStringToString::Lookup(Misc::ToUpper(key), rValue);
}

CString& CTDCMetaDataMap::operator[](LPCTSTR key)
{
	return CMapStringToString::operator[](Misc::ToUpper(key));
}

void CTDCMetaDataMap::SetAt(LPCTSTR key, const CString& newValue)
{ 
	(*this)[key] = newValue; 
}

void CTDCMetaDataMap::Copy(const CTDCMetaDataMap& mapData)
{
	Misc::Copy(mapData, *this);
}

BOOL CTDCMetaDataMap::MatchAll(const CTDCMetaDataMap& mapData) const
{
	return Misc::MatchAll(*this, mapData);
}

void CTDCMetaDataMap::RemoveKey(LPCTSTR key)
{
	CMapStringToString::RemoveKey(Misc::ToUpper(key));
}

/////////////////////////////////////////////////////////////////////////////

