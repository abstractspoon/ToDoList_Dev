// mapex.h: interface for the CMapEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEX_H__44E4FC2A_83C2_49EE_A784_4D1584CD5339__INCLUDED_)
#define AFX_MAPEX_H__44E4FC2A_83C2_49EE_A784_4D1584CD5339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "Misc.h"

//////////////////////////////////////////////////////////////////////

template <class KEY, class ARG_KEY>
class CSetBase : public CMap<KEY, ARG_KEY, char, char&>
{
public:
	CSetBase() {}

	CSetBase(const CSetBase& other)
	{
		Copy(other);
	}

	CSetBase(const CArray<KEY, KEY&>& other)
	{
		CopyFrom(other);
	}

	CSetBase(const KEY* pOther, int nNumOther)
	{
		CopyFrom(pOther, nNumOther);
	}

	BOOL IsEmpty() const
	{
		return (GetCount() == 0);
	}

	KEY GetNext(POSITION& pos) const
	{
		char val = 0;
		KEY key;

		GetNextAssoc(pos, key, val);
		return key;
	}

	BOOL Has(KEY key) const
	{
		char val = 0;
		return Lookup(key, val);
	}

	BOOL HasOnly(KEY key) const
	{
		return ((GetCount() == 1) && Has(key));
	}

	void Add(KEY key)
	{
		char val = 0;
		SetAt(key, val);
	}

	template <class ARRAY>
	int Add(const ARRAY& other)
	{
		if (other.GetSize() == 0)
			return 0;

		return Add(other.GetData(), other.GetSize());
	}

	int Add(const KEY* pOther, int nNumOther)
	{
		if (!pOther || !nNumOther)
		{
			ASSERT(0);
			return 0;
		}

		int nItem = nNumOther, nAdded = 0;

		while (nItem--)
		{
			if (!Has(pOther[nItem]))
			{
				Add(pOther[nItem]);
				nAdded++;
			}
		}
		
		return nAdded;
	}

	void Set(KEY key)
	{
		RemoveAll();
		Add(key);
	}

	BOOL Remove(KEY key)
	{
		return CMap<KEY, ARG_KEY, char, char&>::RemoveKey(key);
	}

	void Copy(const CSetBase& other)
	{
		RemoveAll();
		
		if (other.GetCount())
		{
			POSITION pos = other.GetStartPosition();
			
			while (pos)
				Add(other.GetNext(pos));
		}
	}

	BOOL MatchAll(const CSetBase& other) const
	{
		if (GetCount() != other.GetCount())
			return FALSE;

		POSITION pos = other.GetStartPosition();

		while (pos)
		{
			if (!Has(other.GetNext(pos)))
				return FALSE;
		}

		return TRUE;
	}

	BOOL MatchAny(const CSetBase& other) const
	{
		POSITION pos = other.GetStartPosition();

		while (pos)
		{
			if (Has(other.GetNext(pos)))
				return TRUE;
		}

		return FALSE;
	}

	int GetDifferences(const CSetBase& other, CSetBase& diffs) const
	{
		diffs.RemoveAll();

		POSITION pos = other.GetStartPosition();

		while (pos)
		{
			KEY val = other.GetNext(pos);

			if (!Has(val))
				diffs.Add(val);
		}

		pos = GetStartPosition();

		while (pos)
		{
			KEY val = GetNext(pos);

			if (!other.Has(val))
				diffs.Add(val);
		}

		return diffs.GetCount();
	}

	int GetIntersection(const CSetBase& other, CSetBase& intersection) const
	{
		intersection.RemoveAll();

		POSITION pos = GetStartPosition();

		while (pos)
		{
			KEY val = GetNext(pos);

			if (other.Has(val))
				intersection.Add(val);
		}

		return intersection.GetCount();
	}

	void Append(const CSetBase& other)
	{
		POSITION pos = other.GetStartPosition();

		while (pos)
			Add(other.GetNext(pos));
	}
	
	template <class ARRAY>
	int CopyFrom(const ARRAY& other)
	{
		if (other.GetSize() == 0)
			return 0;

		return CopyFrom(other.GetData(), other.GetSize());
	}

	int CopyFrom(const KEY* pOther, int nNumOther)
	{
		if (!pOther)
		{
			ASSERT(0);
			return 0;
		}

		RemoveAll();
		int nItem = nNumOther;

		while (nItem--)
			Add(pOther[nItem]);

		return GetCount();
	}

	template <class ARRAY>
	int CopyTo(ARRAY& other) const
	{
		other.RemoveAll();
		POSITION pos = GetStartPosition();

		while (pos)
		{
			KEY val = GetNext(pos);
			other.Add(val);
		}

		return other.GetSize();
	}
};

//////////////////////////////////////////////////////////////////////

template <class KEY>
class CSet : public CSetBase<KEY, KEY>
{
public:
	CSet() {}
	CSet(const CSet& other) : CSetBase<KEY, KEY>(other) {}
	CSet(const CArray<KEY, KEY&>& other) : CSetBase<KEY, KEY>(other) {}
	CSet(const KEY* pOther, int nNumOther) : CSetBase<KEY, KEY>(pOther, nNumOther) {}

};

//////////////////////////////////////////////////////////////////////

class CDWordSet : public CSet<DWORD>
{
public:
	int CopyFrom(const CDWordArray& other) { return CSet<DWORD>::CopyFrom(other); }
	int CopyTo(CDWordArray& other) const { return CSet<DWORD>::CopyTo(other); }
};

class CUIntSet : public CSet<UINT>
{
public:
	int CopyFrom(const CUIntArray& other) {	return CSet<UINT>::CopyFrom(other); }
	int CopyTo(CUIntArray& other) const { return CSet<UINT>::CopyTo(other); }
};

class CStringSet : public CSetBase<CString, LPCTSTR>
{
public:
	int CopyFrom(const CStringArray& other) { return CSetBase<CString, LPCTSTR>::CopyFrom(other); }
	int CopyTo(CStringArray& other) const { return CSetBase<CString, LPCTSTR>::CopyTo(other); }

	BOOL MatchAll(const CStringSet& other, BOOL bCaseSensitive = FALSE) const
	{
		if (GetCount() != other.GetCount())
			return FALSE;

		// Try case sensitive
		BOOL bMatch = CSetBase<CString, LPCTSTR>::MatchAll(other);

		if (bMatch || bCaseSensitive)
			return bMatch;

		// Compare item by item case-insensitively
		POSITION pos = GetStartPosition();

		while (pos)
		{
			CString sItem = GetNext(pos);

			// Compare item by item case sensitively
			BOOL bFound = FALSE;
			POSITION posOther = other.GetStartPosition();

			while (posOther && !bFound)
			{
				CString sOtherItem = other.GetNext(posOther);
				bFound = (sItem.CompareNoCase(sOtherItem) == 0);
			}

			if (!bFound)
				return FALSE;
		}

		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////////

template <class ARG, class ARG_KEY, class VALUE>
class CContainerMap : protected CMap<ARG, ARG_KEY, VALUE*, VALUE*&>
{
	// To be derived from only
protected:
	CContainerMap()
	{
	}

public:
	CContainerMap(UINT hashSize)
	{
		if (hashSize)
			InitHashTable(hashSize);
	}

	virtual ~CContainerMap()
	{
		RemoveAll();
	}

	POSITION GetStartPosition() const
	{
		return CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::GetStartPosition();
	}

	void GetNextAssoc(POSITION& rNextPosition, ARG& rKey, VALUE*& rValue) const
	{
		CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::GetNextAssoc(rNextPosition, rKey, rValue);
	}

	const VALUE* GetMapping(const ARG& rKey) const
	{
		return const_cast<CContainerMap*>(this)->GetMapping(rKey);
	}

	BOOL HasMapping(const ARG& rKey) const
	{
		return (GetMapping(rKey) != NULL);
	}

	VALUE* GetMapping(const ARG& rKey)
	{
		VALUE* pMapping = NULL;

		if (!Lookup(rKey, pMapping))
			return NULL;

		ASSERT(pMapping);
		return pMapping;
	}

	VALUE* GetAddMapping(const ARG& rKey)
	{
		VALUE* pMapping = NULL;

		if (Lookup(rKey, pMapping))
		{
			ASSERT(pMapping);
			return pMapping;
		}

		pMapping = new VALUE;
		SetAt(rKey, pMapping);

		return pMapping;
	}

	BOOL IsEmpty() const
	{
		return (!GetCount());
	}

	int GetCount() const
	{
		return CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::GetCount();
	}

	void RemoveAll()
	{
		ARG rKey;
		VALUE* pMapping = NULL;

		POSITION pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, rKey, pMapping);

			ASSERT(pMapping);
			ASSERT(!rKey.IsEmpty());

			delete pMapping;
		}

		CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::RemoveAll();
	}

	void RemoveKey(const ARG& rKey)
	{
		VALUE* pMapping = NULL;

		if (Lookup(rKey, pMapping))
		{
			ASSERT(pMapping);
			delete pMapping;

			CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::RemoveKey(rKey);
		}
	}

	// overload new/delete because we've hidden the base class
	void* operator new (size_t size)
	{
		return CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::operator new(size);
	}

	void operator delete (void *p)
	{
		CMap<ARG, ARG_KEY, VALUE*, VALUE*&>::operator delete(p);
	}

};

//////////////////////////////////////////////////////////////////////

template <class VALUE>
class CMapStringToContainer : public CContainerMap<CString, LPCTSTR, VALUE>
{
};

//////////////////////////////////////////////////////////////////////

template <class VALUE>
class CMapDWordToContainer : public CContainerMap<DWORD, DWORD, VALUE>
{
};

//////////////////////////////////////////////////////////////////////

class CMapStringToStringArray : public CMapStringToContainer<CStringArray>
{
public:
	BOOL Set(const CString& sKey, const CStringArray& aItems)
	{
		CStringArray* pArray = GetAddMapping(sKey);

		if (!pArray)
		{
			ASSERT(0);
			return FALSE;
		}

		pArray->RemoveAll();
		Misc::AddUniqueItems(aItems, *pArray);

		return TRUE;
	}

	BOOL Set(const CString& sKey, const CString& sValue)
	{
		CStringArray* pArray = GetAddMapping(sKey);

		if (!pArray)
		{
			ASSERT(0);
			return FALSE;
		}

		pArray->RemoveAll();
		Misc::AddUniqueItem(sValue, *pArray);

		return TRUE;
	}

	BOOL Add(const CString& sKey, const CString& sValue)
	{
		CStringArray* pArray = GetAddMapping(sKey);

		if (!pArray)
		{
			ASSERT(0);
			return FALSE;
		}

		return (Misc::AddUniqueItem(sValue, *pArray) != -1);
	}

	void Remove(const CString& sKey, const CString& sValue, BOOL bRemoveKeyWhenEmpty)
	{
		CStringArray* pArray = GetMapping(sKey);

		if (pArray == NULL)
			return; // not an error

		Misc::RemoveItem(sValue, *pArray);

		if (bRemoveKeyWhenEmpty && (pArray->GetSize() == 0))
			RemoveKey(sKey);
	}

	void Copy(const CMapStringToStringArray& other)
	{
		RemoveAll();

		POSITION pos = other.GetStartPosition();
		CString sKey;
		CStringArray* pOtherArray = NULL;

		while (pos)
		{
			other.GetNextAssoc(pos, sKey, pOtherArray);
			ASSERT(pOtherArray);

			CStringArray* pArray = GetAddMapping(sKey);
			ASSERT(pArray);

			if (pArray && pOtherArray)
				pArray->Copy(*pOtherArray);
		}	
	}

	BOOL MatchAll(const CMapStringToStringArray& other, 
				  BOOL bOrderSensitive = FALSE, BOOL bCaseSensitive = FALSE) const
	{
		if (GetCount() != other.GetCount())
			return FALSE;

		POSITION pos = GetStartPosition();
		CString sKey;
		CStringArray* pArray = NULL;

		while (pos)
		{
			GetNextAssoc(pos, sKey, pArray);
			ASSERT(pArray);

			const CStringArray* pOtherArray = other.GetMapping(sKey);

			if (pOtherArray == NULL)
				return FALSE;
			
			if (!Misc::MatchAll(*pArray, *pOtherArray, bOrderSensitive, bCaseSensitive))
				return FALSE;
		}	

		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////////

typedef CMapStringToContainer<CMapStringToStringArray> CMapStringToStringArrayMap;

typedef CMapStringToContainer<CMapStringToString> CMapStringToStringMap;

//////////////////////////////////////////////////////////////////////

class CMapStringToStringSet : public CMapStringToContainer<CStringSet>
{
public:
	BOOL Set(const CString& sKey, const CStringSet& set)
	{
		CStringSet* pSet = GetAddMapping(sKey);

		if (!pSet)
		{
			ASSERT(0);
			return FALSE;
		}

		pSet->RemoveAll();
		pSet->Append(set);

		return TRUE;
	}

	BOOL Set(const CString& sKey, const CString& sValue)
	{
		CStringSet* pSet = GetAddMapping(sKey);

		if (!pSet)
		{
			ASSERT(0);
			return FALSE;
		}

		pSet->RemoveAll();
		pSet->Add(sValue);

		return TRUE;
	}
	
	BOOL Set(const CString& sKey, const CStringArray& aValues)
	{
		CStringSet* pSet = GetAddMapping(sKey);
		
		if (!pSet)
		{
			ASSERT(0);
			return FALSE;
		}
		
		pSet->RemoveAll();
		pSet->CopyFrom(aValues);
		
		return TRUE;
	}

	BOOL Add(const CString& sKey, const CString& sValue)
	{
		CStringSet* pSet = GetAddMapping(sKey);

		if (!pSet)
		{
			ASSERT(0);
			return FALSE;
		}

		if (pSet->Has(sValue))
			return FALSE;

		pSet->Add(sValue);
		return TRUE;
	}

	void Remove(const CString& sKey, const CString& sValue, BOOL bRemoveKeyWhenEmpty)
	{
		CStringSet* pSet = GetMapping(sKey);

		if (pSet == NULL)
			return; // not an error

		pSet->Remove(sValue);

		if (bRemoveKeyWhenEmpty && (pSet->GetCount() == 0))
			RemoveKey(sKey);
	}

	void Copy(const CMapStringToStringSet& other)
	{
		RemoveAll();

		POSITION pos = other.GetStartPosition();
		CString sKey;
		CStringSet* pOtherSet = NULL;

		while (pos)
		{
			other.GetNextAssoc(pos, sKey, pOtherSet);
			ASSERT(pOtherSet);

			CStringSet* pSet = GetAddMapping(sKey);
			ASSERT(pSet);

			if (pSet && pOtherSet)
				pSet->Copy(*pOtherSet);
		}	
	}

	BOOL MatchAll(const CMapStringToStringSet& other, BOOL bCaseSensitive = FALSE) const
	{
		if (GetCount() != other.GetCount())
			return FALSE;

		POSITION pos = GetStartPosition();
		CString sKey;
		CStringSet* pSet = NULL;

		while (pos)
		{
			GetNextAssoc(pos, sKey, pSet);
			ASSERT(pSet);

			const CStringSet* pOtherSet = other.GetMapping(sKey);

			if (pOtherSet == NULL)
				return FALSE;
			
			if (!pSet->MatchAll(*pOtherSet, bCaseSensitive))
				return FALSE;
		}	

		return TRUE;
	}
};


#endif // !defined(AFX_MAPEX_H__44E4FC2A_83C2_49EE_A784_4D1584CD5339__INCLUDED_)
