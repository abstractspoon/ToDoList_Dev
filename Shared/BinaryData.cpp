// BinaryData.cpp: implementation of the CBinaryData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BinaryData.h"
#include "misc.h"

#include "..\3rdParty\Base64Coder.h"

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

BOOL CBinaryData::Base64Encode(CString& sEncoded) const
{
	Base64Coder b64;
	b64.Encode((const PBYTE)Get(), GetLength());
	
	sEncoded = b64.EncodedMessage();
	return !sEncoded.IsEmpty();
}

BOOL CBinaryData::Base64Decode(const CString& sEncoded)
{
	Base64Coder b64;
	
#ifdef _UNICODE
	// if text is unicode then we need to convert it back to multibyte
	// to read the binary stream as unsigned chars
	int nLen = sEncoded.GetLength();
	unsigned char* pBinary = (unsigned char*)Misc::WideToMultiByte((LPCTSTR)sEncoded, nLen);
	b64.Decode(pBinary, nLen);
	delete [] pBinary;
#else
	b64.Decode(sEncoded);
#endif
	
	unsigned long nLenContent;
	PBYTE pContent = b64.DecodedMessage(nLenContent);
	
	PBYTE szContent = (PBYTE)GetBuffer(nLenContent);
	CopyMemory(szContent, pContent, nLenContent);
	ReleaseBuffer(nLenContent);
	
	return (nLenContent > 0);
}
