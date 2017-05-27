#if !defined(AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdlutil.h : header file
//

#include "..\interfaces\ITaskList.h"

#include "..\shared\misc.h"

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDCCADATA
{
	TDCCADATA(const CString& sValue = _T(""), TCHAR cSep = 0) { Set(sValue, cSep); }
	TDCCADATA(LPCTSTR szValue, TCHAR cSep = 0) { Set(CString(szValue), cSep); }
	TDCCADATA(double dValue) { Set(dValue); }
	TDCCADATA(double dValue, TDC_UNITS nUnits) { Set(dValue, nUnits); }
	TDCCADATA(const CStringArray& aValues) { Set(aValues); }
	TDCCADATA(int nValue) { Set(nValue); }
	TDCCADATA(const COleDateTime& dtValue) { Set(dtValue); }
	TDCCADATA(bool bValue) { Set(bValue); }

	TDCCADATA(const TDCCADATA& data)
	{
		*this = data;
	}

	TDCCADATA& operator=(const TDCCADATA& data)
	{
		sData = data.sData;
		return *this;
	}

	BOOL operator==(const TDCCADATA& data) const { return sData == data.sData; }
	BOOL operator!=(const TDCCADATA& data) const { return sData != data.sData; }

	BOOL IsEmpty() const { return sData.IsEmpty(); }
	void Clear() { sData.Empty(); }

	const CString& AsString() const { return sData; }
	double AsDouble() const { return _ttof(sData); }
	int AsInteger() const { return _ttoi(sData); } 
	COleDateTime AsDate() const { return _ttof(sData); }
	bool AsBool() const { return !IsEmpty(); }

	int AsArray(CStringArray& aValues) const 
	{ 
		// Special case: 1 empty value
		if (sData == _T("\n"))
		{
			aValues.RemoveAll();
			aValues.Add(_T(""));
			return 1;
		}
		
		// else
		return Misc::Split(sData, aValues, '\n', TRUE); 
	}

	double AsTimePeriod(TDC_UNITS& nUnits) const
	{
		if (IsEmpty())
		{
			nUnits = TDCU_HOURS;
			return 0.0;
		}

		// else
		nUnits = (TDC_UNITS)Misc::Last(sData);
		ASSERT(IsValidUnits(nUnits));

		return _ttof(sData);
	}

	void Set(double dValue) { sData.Format(_T("%lf"), dValue); }
	void Set(int nValue) { sData.Format(_T("%d"), nValue); }
	void Set(const COleDateTime& dtValue) { sData.Format(_T("%lf"), dtValue); }

	void Set(const CString& sValue, TCHAR cSep = 0) 
	{ 
		if (cSep != 0)
		{
			CStringArray aItems;

			if (Misc::Split(sValue, aItems, cSep, TRUE))
			{
				Set(aItems);
				return;
			}
		}

		// all else
		sData = sValue; 
	}

	void Set(double dValue, TDC_UNITS nUnits) 
	{ 
		ASSERT(IsValidUnits(nUnits));
		sData.Format(_T("%lf:%lc"), dValue, (TCHAR)nUnits); }

	void Set(bool bValue, TCHAR nChar = 0) 
	{ 
		sData.Empty(); 

		if (bValue)
		{
			if (nChar > 0)
				sData.Insert(0, nChar);
			else
				sData.Insert(0, '+');
		}
	}

	void Set(const CStringArray& aValues) 
	{ 
		// Special case: 1 empty value
		if ((aValues.GetSize() == 1) && aValues[0].IsEmpty())
			sData = '\n';
		else
			sData = Misc::FormatArray(aValues, '\n'); 
	}

	CString FormatAsArray(TCHAR cSep = 0) const
	{
		if (cSep == '\n')
			return sData;

		CString sArray(sData);

		if (cSep != 0)
			sArray.Replace('\n', cSep);
		else
			sArray.Replace(_T("\n"), Misc::GetListSeparator());

		return sArray;
	}

	CString FormatAsDate() const
	{
		return AsDate().Format(VAR_DATEVALUEONLY);
	}

protected:
	CString sData;
};

/////////////////////////////////////////////////////////////////////////////

class CTDCCustomAttributeDataMap : public CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>
{
public:
	BOOL Lookup(LPCTSTR key, TDCCADATA& rValue) const
	{
		return CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>::Lookup(Misc::ToUpper(key), rValue);
	}

	TDCCADATA& operator[](LPCTSTR key)
	{
		return CMap<CString, LPCTSTR, TDCCADATA, TDCCADATA&>::operator[](Misc::ToUpper(key));
	}

	void SetAt(LPCTSTR key, TDCCADATA& newValue)
	{ 
		(*this)[key] = newValue; 
	}

};

/////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCCDATA_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_
