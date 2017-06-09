#pragma once

//////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

#include "..\Shared\mapex.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class IPreferences;

//////////////////////////////////////////////////////////////////////

template <class T> 
class CTDCBaseEnumSet : public CSet<T> 
{
public:
	~CTDCBaseEnumSet() {}

protected:
	CTDCBaseEnumSet() {}

public:
	void Save(IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt) const
	{
		pPrefs->WriteProfileInt(szKey, _T("Count"), GetCount());

		POSITION pos = GetStartPosition();
		int nItem = 0;

		while (pos)
		{
			CString sValueKey = Misc::MakeKey(szValueKeyFmt, nItem++);
			T t = GetNext(pos);

			pPrefs->WriteProfileInt(szKey, sValueKey, t);
		}
	}

	void Load(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt, T tNone) 
	{
		RemoveAll();

		int nItem = pPrefs->GetProfileInt(szKey, _T("Count"));

		while (nItem--)
		{
			CString sValueKey = Misc::MakeKey(szValueKeyFmt, nItem);
			T t = (T)pPrefs->GetProfileInt(szKey, sValueKey, tNone);

			Add(t);
		}
	}
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeMap : public CTDCBaseEnumSet<TDC_ATTRIBUTE>
{
public:
	CTDCAttributeMap();
	CTDCAttributeMap(const CTDCAttributeMap& mapOther);
	~CTDCAttributeMap();

	BOOL Add(TDC_ATTRIBUTE nAttrib);
	int Append(const CTDCAttributeMap& other);

	void Load(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt);

protected:
	static BOOL CanAdd(TDC_ATTRIBUTE nAttrib);
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeArray : public CArray<TDC_ATTRIBUTE, TDC_ATTRIBUTE>
{
public:
	CTDCAttributeArray();
	CTDCAttributeArray(const CTDCAttributeArray& aOther);
	~CTDCAttributeArray();
	
	BOOL Has(TDC_ATTRIBUTE nAttrib) const;
	int Add(TDC_ATTRIBUTE nAttrib);
	void Remove(TDC_ATTRIBUTE nAttrib);

	BOOL MatchAll(const CTDCAttributeArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDMap : public CTDCBaseEnumSet<TDC_COLUMN> 
{
public:
	CTDCColumnIDMap();
	CTDCColumnIDMap(const CTDCColumnIDMap& mapOther);
	~CTDCColumnIDMap();

	void Load(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt);
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDArray : public CArray<TDC_COLUMN, TDC_COLUMN>
{
public:
	CTDCColumnIDArray();
	CTDCColumnIDArray(const CTDCColumnIDArray& aOther);
	~CTDCColumnIDArray();

	BOOL Has(TDC_COLUMN nColID) const;
	int Add(TDC_COLUMN nColID);
	void Remove(TDC_COLUMN nColID);

	BOOL MatchAll(const CTDCColumnIDArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

