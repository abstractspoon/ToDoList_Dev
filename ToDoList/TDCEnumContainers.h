#pragma once

//////////////////////////////////////////////////////////////////////

#include "tdcenum.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class IPreferences;

//////////////////////////////////////////////////////////////////////

template <class T> 
class CTDCBaseEnumMap : public CMap<T, T, char, char&> 
{
public:
	~CTDCBaseEnumMap() {}

	BOOL IsEmpty() const
	{
		return (GetCount() == 0);
	}
	
protected:
	CTDCBaseEnumMap() {}
	CTDCBaseEnumMap(const CTDCBaseEnumMap& mapOther)
	{
		Copy(mapOther);
	}

	BOOL Has(T t) const
	{
		char cUnused;
		return Lookup(t, cUnused);
	}

	BOOL Add(T t)
	{
		const char UNUSED = 0;
		(*this)[t] = UNUSED;

		return TRUE;
	}

	void Remove(T t)
	{
		RemoveKey(t);
	}

	BOOL MatchAll(const CTDCBaseEnumMap& mapOther) const
	{
		if (GetCount() != mapOther.GetCount())
			return FALSE;

		POSITION pos = GetStartPosition();

		while (pos)
		{
			if (!mapOther.Has(GetNext(pos)))
				return FALSE;
		}

		return TRUE;
	}

	void Copy(const CTDCBaseEnumMap& mapOther)
	{
		RemoveAll();
		Append(mapOther);
	}
	
	void Append(const CTDCBaseEnumMap& mapOther)
	{
		POSITION pos = mapOther.GetStartPosition();

		while (pos)
			Add(mapOther.GetNext(pos));
	}

	T GetNext(POSITION& pos) const
	{
		ASSERT(pos);

		char cUnused;
		T t;

		GetNextAssoc(pos, t, cUnused);
		return t;
	}

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
			Add((T)pPrefs->GetProfileInt(szKey, sValueKey, tNone));
		}
	}
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeMap : public CTDCBaseEnumMap<TDC_ATTRIBUTE>
{
public:
	CTDCAttributeMap();
	CTDCAttributeMap(const CTDCAttributeMap& mapOther);
	~CTDCAttributeMap();

	BOOL HasAttribute(TDC_ATTRIBUTE nAttrib) const;
	BOOL AddAttribute(TDC_ATTRIBUTE nAttrib);
	void RemoveAttribute(TDC_ATTRIBUTE nAttrib);

	TDC_ATTRIBUTE GetNextAttribute(POSITION& pos) const;

	BOOL MatchAllAttributes(const CTDCAttributeMap& mapOther) const;
	void CopyAttributes(const CTDCAttributeMap& mapOther);
	void AppendAttributes(const CTDCAttributeMap& mapOther);

	void SaveAttributes(IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt) const;
	void LoadAttributes(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt);

protected:
	static BOOL CanAddAttribute(TDC_ATTRIBUTE nAttrib);
};

//////////////////////////////////////////////////////////////////////

class CTDCAttributeArray : public CArray<TDC_ATTRIBUTE, TDC_ATTRIBUTE>
{
public:
	CTDCAttributeArray();
	CTDCAttributeArray(const CTDCAttributeArray& aOther);
	~CTDCAttributeArray();
	
	BOOL HasAttribute(TDC_ATTRIBUTE nAttrib) const;
	int AddAttribute(TDC_ATTRIBUTE nAttrib);
	void RemoveAttribute(TDC_ATTRIBUTE nAttrib);

	BOOL MatchAll(const CTDCAttributeArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDMap : public CTDCBaseEnumMap<TDC_COLUMN> 
{
public:
	CTDCColumnIDMap();
	CTDCColumnIDMap(const CTDCColumnIDMap& mapOther);
	~CTDCColumnIDMap();

	BOOL HasColumn(TDC_COLUMN nColID) const;
	void AddColumn(TDC_COLUMN nColID);
	void RemoveColumn(TDC_COLUMN nColID);

	TDC_COLUMN GetNextColumn(POSITION& pos) const;

	BOOL MatchAllColumns(const CTDCColumnIDMap& mapOther) const;
	void CopyColumns(const CTDCColumnIDMap& mapOther);
	void AppendColumns(const CTDCColumnIDMap& mapOther);

	void SaveColumns(IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt) const;
	void LoadColumns(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt);
};

//////////////////////////////////////////////////////////////////////

class CTDCColumnIDArray : public CArray<TDC_COLUMN, TDC_COLUMN>
{
public:
	CTDCColumnIDArray();
	CTDCColumnIDArray(const CTDCColumnIDArray& aOther);
	~CTDCColumnIDArray();

	BOOL HasColumn(TDC_COLUMN nColID) const;
	int AddColumn(TDC_COLUMN nColID);
	void RemoveColumn(TDC_COLUMN nColID);

	BOOL MatchAll(const CTDCColumnIDArray& aOther) const;
};

//////////////////////////////////////////////////////////////////////

