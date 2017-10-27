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

	void Add(KEY key)
	{
		char val = 0;
		SetAt(key, val);
	}

	void Remove(KEY key)
	{
		CMap<KEY, ARG_KEY, char, char&>::RemoveKey(key);
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

	void Append(const CSetBase& other)
	{
		POSITION pos = other.GetStartPosition();

		while (pos)
			Add(other.GetNext(pos));
	}
	
	int CopyFrom(const CArray<KEY, KEY&>& other)
	{
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

	int CopyTo(CArray<KEY, KEY&>& other) const
	{
		CopyTo(other);
	}

protected:
	template <class ARRAY>
	int CopyFrom(const ARRAY& other)
	{
		return CopyFrom(other.GetData(), other.GetSize());
	}

	template <class ARRAY>
	int CopyTo(ARRAY& other) const
	{
		other.RemoveAll();
		POSITION pos = GetStartPosition();

		while (pos)
			other.Add(GetNext(pos));

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

class CUintSet : public CSet<UINT>
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

template <class VALUE>
class CMapStringToContainer : protected CMap<CString, LPCTSTR, VALUE*, VALUE*&>
{
public:
	CMapStringToContainer()
	{
	}

	CMapStringToContainer(UINT hashSize)
	{
		if (hashSize)
			InitHashTable(hashSize);
	}
	
	virtual ~CMapStringToContainer()
	{
		RemoveAll();
	}

	POSITION GetStartPosition() const
	{
		return CMap<CString, LPCTSTR, VALUE*, VALUE*&>::GetStartPosition();
	}

	void GetNextAssoc(POSITION& rNextPosition, CString& rKey, VALUE*& rValue) const
	{
		CMap<CString, LPCTSTR, VALUE*, VALUE*&>::GetNextAssoc(rNextPosition, rKey, rValue);
	}
	
	const VALUE* GetMapping(const CString& str) const
	{
		return const_cast<CMapStringToContainer*>(this)->GetMapping(str);
	}
	
	BOOL HasMapping(const CString& str) const
	{
		return (GetMapping(str) != NULL);
	}
	
	VALUE* GetMapping(const CString& str)
	{
		VALUE* pMapping = NULL;
		
		if (!Lookup(str, pMapping))
			return NULL;
		
		ASSERT(pMapping);
		return pMapping;
	}

	VALUE* GetAddMapping(const CString& str)
	{
		VALUE* pMapping = NULL;
		
		if (Lookup(str, pMapping))
		{
			ASSERT(pMapping);
			return pMapping;
		}

		pMapping = new VALUE;
		SetAt(str, pMapping);

		return pMapping;
	}

	BOOL IsEmpty() const
	{
		return (!GetCount());
	}

	int GetCount() const
	{
		return CMap<CString, LPCTSTR, VALUE*, VALUE*&>::GetCount();
	}

	void RemoveAll()
	{
		CString str;
		VALUE* pMapping = NULL;

		POSITION pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, str, pMapping);

			ASSERT(pMapping);
			ASSERT(!str.IsEmpty());

			delete pMapping;
		}

		CMap<CString, LPCTSTR, VALUE*, VALUE*&>::RemoveAll();
	}
	
	void RemoveKey(const CString& str)
	{
		VALUE* pMapping = NULL;
		
		if (Lookup(str, pMapping))
		{
			ASSERT(pMapping);
			delete pMapping;

			CMap<CString, LPCTSTR, VALUE*, VALUE*&>::RemoveKey(str);
		}
	}

	// overload new/delete because we've hidden the base class
	void* operator new (size_t size)
	{
		return CMap<CString, LPCTSTR, VALUE*, VALUE*&>::operator new(size);
	}

	void operator delete (void *p)
	{
		CMap<CString, LPCTSTR, VALUE*, VALUE*&>::operator delete(p);
	}

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
