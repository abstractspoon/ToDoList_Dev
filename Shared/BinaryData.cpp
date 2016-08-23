// BinaryData.cpp: implementation of the CBinaryData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BinaryData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBinaryData::CBinaryData(LPCTSTR szData) 
{
    if (szData)
		Set((const unsigned char*)szData, lstrlen(szData) * sizeof(TCHAR));
}

CBinaryData::CBinaryData(const unsigned char* pData, int nByteLength)
{
    Set(pData, nByteLength);
}

CBinaryData::CBinaryData(const CString& sContent)
{
    Set((const unsigned char*)(LPCTSTR)sContent, sContent.GetLength() * sizeof(TCHAR));
}

CBinaryData::CBinaryData(const CBinaryData& data)
{
    Set(data, data.GetLength());
}

unsigned char* CBinaryData::GetBuffer(int nByteLength)
{
#ifdef _UNICODE
	
    // need to copy into wide buffer
    // must make the length multiple of 2
    int nFixedLen = nByteLength + (nByteLength % 2);
    LPWSTR pBuf = CString::GetBuffer(nFixedLen / 2);
	
    ZeroMemory(pBuf, nFixedLen);
	
#else
	
    // simple copy
    LPSTR pBuf = CString::GetBuffer(nByteLength);
    ZeroMemory(pBuf, nByteLength);
	
#endif
	
    return (unsigned char*)pBuf;
}

void CBinaryData::ReleaseBuffer(int nByteLength)
{
#ifdef _UNICODE

    // must make the length multiple of 2
    int nFixedLen = nByteLength + (nByteLength % 2);
	CString::ReleaseBuffer(nFixedLen / 2);

#else

    // simple release
	CString::ReleaseBuffer(nByteLength);

#endif
}


const unsigned char* CBinaryData::Get(int& nByteLength) const
{
    nByteLength = GetLength();
    return Get();
}

const unsigned char* CBinaryData::Get() const
{
    return *this;
}

void CBinaryData::Empty()
{
    CString::Empty();
}

BOOL CBinaryData::IsEmpty() const
{
    return CString::IsEmpty();
}

int CBinaryData::Get(CString& sContent) const
{
    int nStrLength = GetByteLength() / sizeof(TCHAR);
    sContent = CString((LPCTSTR)Get(), nStrLength);

	return GetLength();
}

bool CBinaryData::operator == (const CBinaryData& data) const
{
    return (this == &data || (GetLength() == data.GetLength() && memcmp(Get(), data.Get(), GetLength()) == 0));
}

bool CBinaryData::operator != (const CBinaryData& data) const
{
    return !(*this == data);
}

int CBinaryData::GetLength() const 
{ 
	return GetByteLength(); 
}

int CBinaryData::GetByteLength() const
{
	int nByteLength = CString::GetLength() * sizeof(TCHAR);
	return nByteLength;
}

void CBinaryData::Set(const unsigned char* pData, int nByteLength)
{
	unsigned char* pBuf = CBinaryData::GetBuffer(nByteLength);
	CopyMemory(pBuf, pData, nByteLength);
	CBinaryData::ReleaseBuffer(nByteLength);
}

CBinaryData::operator const unsigned char*() const
{
	return (const unsigned char*)(LPCTSTR)(*this);
}
