#pragma once

//////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

#include "..\Shared\mapex.h"

#include "..\Interfaces\IEnums.h"
#include "..\Interfaces\Preferences.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class IPreferences;

//////////////////////////////////////////////////////////////////////

template <class T> 
class CTDCBaseEnumSet : public CSet<T> 
{
public:
	~CTDCBaseEnumSet() {}

	void Save(IPreferences* pPrefs, LPCTSTR szSection, BOOL bPreDeleteSection = TRUE) const
	{
		if (bPreDeleteSection)
			pPrefs->DeleteProfileSection(szSection);

		if (IsEmpty())
		{
			// Have to write something else we've no way of
			// detecting a successful read
			pPrefs->WriteProfileString(szSection, _T("Items"), _T("|"));
		}
		else
		{
			CArray<T, T&> aEnums;
			CopyTo(aEnums);

			CString sItems = Misc::FormatArrayT(aEnums, _T("%d"), '|');
			pPrefs->WriteProfileString(szSection, _T("Items"), sItems);
		}
	}

	BOOL Load(const IPreferences* pPrefs, LPCTSTR szSection) 
	{
		RemoveAll();

		CString sItems = pPrefs->GetProfileString(szSection, _T("Items"));

		if (sItems.IsEmpty())
			return FALSE;

		CDWordArray aEnums;
		int nItem = Misc::Split(sItems, aEnums, '|');

		while (nItem--)
			Add((T)aEnums[nItem]);

		return TRUE;
	}

	BOOL LegacyLoad(const IPreferences* pPrefs, LPCTSTR szSection, LPCTSTR szValueKeyFmt)
	{
		RemoveAll();

		int nItem = pPrefs->GetProfileInt(szSection, _T("Count"), -1);

		if (nItem == -1)
			return FALSE;

		T tDef = GetDefaultValue();

		while (nItem--)
		{
			CString sValueKey = Misc::MakeKey(szValueKeyFmt, nItem);
			Add((T)pPrefs->GetProfileInt(szSection, sValueKey, tDef));
		}

		return TRUE;
	}

protected:
	CTDCBaseEnumSet() {}

protected:
	virtual T GetDefaultValue() = 0;
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeMap : public CTDCBaseEnumSet<TDC_ATTRIBUTE>
{
public:
	CTDCAttributeMap();
	CTDCAttributeMap(TDC_ATTRIBUTE nAttrib);
	CTDCAttributeMap(const CTDCAttributeMap& mapOther);
	~CTDCAttributeMap();

	BOOL Add(TDC_ATTRIBUTE nAttrib);
	int Append(const CTDCAttributeMap& other);

	static BOOL IsTaskAttribute(TDC_ATTRIBUTE nAttrib);

protected:
	TDC_ATTRIBUTE GetDefaultValue() { return TDCA_NONE; }

	BOOL CanAdd(TDC_ATTRIBUTE nAttrib) const;
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeArray : public CArray<TDC_ATTRIBUTE, TDC_ATTRIBUTE>
{
public:
	CTDCAttributeArray();
	CTDCAttributeArray(const CTDCAttributeArray& aOther);
	~CTDCAttributeArray();
	
	BOOL Has(TDC_ATTRIBUTE nAttrib) const;
	BOOL AddUnique(TDC_ATTRIBUTE nAttrib);
	void Remove(TDC_ATTRIBUTE nAttrib);

	BOOL MatchAll(const CTDCAttributeArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDMap : public CTDCBaseEnumSet<TDC_COLUMN> 
{
public:
	CTDCColumnIDMap();
	CTDCColumnIDMap(TDC_COLUMN nColID);
	CTDCColumnIDMap(const CTDCColumnIDMap& mapOther);
	~CTDCColumnIDMap();

	BOOL Has(TDC_COLUMN nColID) const;
	TDC_COLUMN GetFirst() const;

protected:
	TDC_COLUMN GetDefaultValue() { return TDCC_NONE; }
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDArray : public CArray<TDC_COLUMN, TDC_COLUMN>
{
public:
	CTDCColumnIDArray();
	CTDCColumnIDArray(const CTDCColumnIDArray& aOther);
	~CTDCColumnIDArray();

	BOOL Has(TDC_COLUMN nColID) const;
	BOOL AddUnique(TDC_COLUMN nColID);
	void Remove(TDC_COLUMN nColID);

	BOOL MatchAll(const CTDCColumnIDArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

