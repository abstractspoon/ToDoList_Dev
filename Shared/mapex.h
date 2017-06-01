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

template <class T>
class CSet : public CMap<T, T, char, char>
{
public:
	CSet() {}

	CSet(const CSet& other)
	{
		Copy(other);
	}

	CSet(const CArray<T, T&>& other)
	{
		CopyFrom(other);
	}

	CSet(const T* pOther, int nNumOther)
	{
		CopyFrom(pOther, nNumOther);
	}

	T GetNextKey(POSITION& pos) const
	{
		char val = 0;
		T key;

		GetNextAssoc(pos, key, val);
		return key;
	}

	BOOL HasKey(T key) const
	{
		char val = 0;
		return Lookup(key, val);
	}

	void AddKey(T key)
	{
		SetAt(key, 0);
	}

	void Copy(const CSet& other)
	{
		Misc::CopyT(other, *this);
	}

	BOOL MatchAll(const CSet& other) const
	{
		Misc::MatchAllT(other, *this);
	}

	int CopyFrom(const CArray<T, T&>& other)
	{
		return CopyFrom(other.GetData(), other.GetSize());
	}

	int CopyFrom(const T* pOther, int nNumOther)
	{
		if (!pOther)
		{
			ASSERT(0);
			return 0;
		}

		RemoveAll();
		int nItem = nNumOther;

		while (nItem--)
			AddKey(pOther[nItem]);

		return GetCount();
	}

	int CopyTo(CArray<T, T&>& other) const
	{
		CopyToEx(other);
	}

protected:
	template <class S>
	int CopyFromEx(const S& other)
	{
		return CopyFrom(other.GetData(), other.GetSize());
	}

	template <class S>
	int CopyToEx(S& other) const
	{
		other.RemoveAll();
		POSITION pos = GetStartPosition();

		while (pos)
			other.Add(GetNextKey(pos));

		return other.GetSize();
	}
};

class CDWordSet : public CSet<DWORD>
{
public:
	int CopyFrom(const CDWordArray& other) { return CopyFromEx<CDWordArray>(other); }
	int CopyTo(CDWordArray& other) const { return CopyToEx<CDWordArray>(other); }
};

class CUintSet : public CSet<UINT>
{
public:
	int CopyFrom(const CUIntArray& other) {	return CopyFromEx<CUIntArray>(other); }
	int CopyTo(CUIntArray& other) const { return CopyToEx<CUIntArray>(other); }
};

class CStringSet : public CMap<CString, LPCTSTR, char, char&>
{
public:
	CStringSet() {}

	CStringSet(const CStringSet& other)
	{
		Copy(other);
	}

	CStringSet(const CStringArray& other)
	{
		CopyFrom(other);
	}

	CString GetNextKey(POSITION& pos) const
	{
		char val = 0;
		CString key;

		GetNextAssoc(pos, key, val);
		return key;
	}

	BOOL HasKey(LPCTSTR key) const
	{
		char val = 0;
		return Lookup(key, val);
	}

	void AddKey(LPCTSTR key)
	{
		char c = 0;
		SetAt(CString(key), c);
	}

	void Copy(const CStringSet& other)
	{
		Misc::CopyStrT<char>(other, *this);
	}

	BOOL MatchAll(const CStringSet& other) const
	{
		Misc::MatchAllStrT<char>(other, *this);
	}

	int CopyFrom(const CStringArray& other)
	{
		RemoveAll();
		int nItem = other.GetSize();

		while (nItem--)
			AddKey(other[nItem]);

		return GetCount();
	}

	int CopyTo(CStringArray& other) const
	{
		other.RemoveAll();
		POSITION pos = GetStartPosition();

		while (pos)
			other.Add(GetNextKey(pos));

		return other.GetSize();
	}
};

//////////////////////////////////////////////////////////////////////

template <class T>
class CMapStringToContainer : protected CMap<CString, LPCTSTR, T*, T*&>
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
		return CMap<CString, LPCTSTR, T*, T*&>::GetStartPosition();
	}

	void GetNextAssoc(POSITION& rNextPosition, CString& rKey, T*& rValue) const
	{
		CMap<CString, LPCTSTR, T*, T*&>::GetNextAssoc(rNextPosition, rKey, rValue);
	}
	
	const T* GetMapping(const CString& str) const
	{
		return const_cast<CMapStringToContainer*>(this)->GetMapping(str);
	}
	
	BOOL HasMapping(const CString& str) const
	{
		return (GetMapping(str) != NULL);
	}
	
	T* GetMapping(const CString& str)
	{
		T* pMapping = NULL;
		
		if (!Lookup(str, pMapping))
			return NULL;
		
		ASSERT(pMapping);
		return pMapping;
	}

	T* GetAddMapping(const CString& str)
	{
		T* pMapping = NULL;
		
		if (Lookup(str, pMapping))
		{
			ASSERT(pMapping);
			return pMapping;
		}

		pMapping = new T;
		SetAt(str, pMapping);

		return pMapping;
	}

	BOOL IsEmpty() const
	{
		return (!GetCount());
	}

	int GetCount() const
	{
		return CMap<CString, LPCTSTR, T*, T*&>::GetCount();
	}

	void RemoveAll()
	{
		CString str;
		T* pMapping = NULL;

		POSITION pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, str, pMapping);

			ASSERT(pMapping);
			ASSERT(!str.IsEmpty());

			delete pMapping;
		}

		CMap<CString, LPCTSTR, T*, T*&>::RemoveAll();
	}
	
	void RemoveKey(const CString& str)
	{
		T* pMapping = NULL;
		
		POSITION pos = GetStartPosition();
		
		if (Lookup(str, pMapping))
		{
			ASSERT(pMapping);
			delete pMapping;

			CMap<CString, LPCTSTR, T*, T*&>::RemoveKey(str);
		}
	}

	// overload new/delete because we've hidden the base class
	void* operator new (size_t size)
	{
		return CMap<CString, LPCTSTR, T*, T*&>::operator new(size);
	}

	void operator delete (void *p)
	{
		CMap<CString, LPCTSTR, T*, T*&>::operator delete(p);
	}

};

class CMapStringToStringArray : public CMapStringToContainer<CStringArray>
{
public:
	BOOL Map(const CString& str, const CStringArray& aItems)
	{
		CStringArray* pArray = GetAddMapping(str);

		if (!pArray)
		{
			ASSERT(0);
			return FALSE;
		}

		pArray->Copy(aItems);
		return TRUE;
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

typedef CMapStringToContainer<CMapStringToStringArray> CMapStringToStringArrayMap;

typedef CMapStringToContainer<CMapStringToString> CMapStringToStringMap;

#endif // !defined(AFX_MAPEX_H__44E4FC2A_83C2_49EE_A784_4D1584CD5339__INCLUDED_)
