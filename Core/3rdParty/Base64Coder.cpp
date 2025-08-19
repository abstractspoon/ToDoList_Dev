// Base64Coder.cpp: implementation of the Base64Coder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Base64Coder.h"

#include <ATLCONV.H>

// Digits...
static char	Base64Digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

BOOL Base64Coder::m_Init		= FALSE;
char Base64Coder::m_DecodeTable[256];

#ifndef PAGESIZE
#define PAGESIZE					4096
#endif

#ifndef ROUNDTOPAGE
#define ROUNDTOPAGE(a)			(((a/4096)+1)*4096)
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Base64Coder::Base64Coder()
:	m_pDBuffer(NULL),
	m_pEBuffer(NULL),
	m_nDBufLen(0),
	m_nEBufLen(0)
{

}

Base64Coder::~Base64Coder()
{
	if(m_pDBuffer != NULL)
		delete [] m_pDBuffer;

	if(m_pEBuffer != NULL)
		delete [] m_pEBuffer;
}

CString Base64Coder::DecodedMessage() const 
{ 
	// CString will convert ansi to unicode as required
	//return (const char*)m_pDBuffer;
	return CString((const char*)m_pDBuffer, m_nDDataLen);
}

PBYTE Base64Coder::DecodedMessage(DWORD& nLenMessage) const
{
	nLenMessage = m_nDDataLen;

	return m_pDBuffer;
}

CString Base64Coder::EncodedMessage() const
{ 
	// CString will convert ansi to unicode as required
	//return (const char*)m_pEBuffer;
	return CString((const char*)m_pEBuffer, m_nEDataLen);
}

PBYTE Base64Coder::EncodedMessage(DWORD& nLenMessage) const
{ 
	nLenMessage = m_nEDataLen;

	return m_pEBuffer;
}

void Base64Coder::AllocEncode(DWORD nSize)
{
	if(m_nEBufLen < nSize)
	{
		if(m_pEBuffer != NULL)
			delete [] m_pEBuffer;

		m_nEBufLen = ROUNDTOPAGE(nSize);
		m_pEBuffer = new BYTE[m_nEBufLen];
	}

	::ZeroMemory(m_pEBuffer, m_nEBufLen);
	m_nEDataLen = 0;
}

void Base64Coder::AllocDecode(DWORD nSize)
{
	if(m_nDBufLen < nSize)
	{
		if(m_pDBuffer != NULL)
			delete [] m_pDBuffer;

		m_nDBufLen = ROUNDTOPAGE(nSize);
		m_pDBuffer = new BYTE[m_nDBufLen];
	}

	::ZeroMemory(m_pDBuffer, m_nDBufLen);
	m_nDDataLen = 0;
}

void Base64Coder::SetEncodeBuffer(const BYTE* pBuffer, DWORD nBufLen)
{
	DWORD	i = 0;

	AllocEncode(nBufLen);
	while(i < nBufLen)
	{
		if(!_IsBadMimeChar(pBuffer[i]))
		{
			m_pEBuffer[m_nEDataLen] = pBuffer[i];
			m_nEDataLen++;
		}

		i++;
	}
}

void Base64Coder::SetDecodeBuffer(const BYTE* pBuffer, DWORD nBufLen)
{
	AllocDecode(nBufLen);
	::CopyMemory(m_pDBuffer, pBuffer, nBufLen);
	m_nDDataLen = nBufLen;
}

void Base64Coder::Encode(const BYTE* pBuffer, DWORD nBufLen)
{
	SetDecodeBuffer(pBuffer, nBufLen);
	AllocEncode(nBufLen * 2);

	TempBucket			Raw;
	DWORD					nIndex	= 0;

	while((nIndex + 3) <= nBufLen)
	{
		Raw.Clear();
		::CopyMemory(&Raw, m_pDBuffer + nIndex, 3);
		Raw.nSize = 3;
		_EncodeToBuffer(Raw, m_pEBuffer + m_nEDataLen);
		nIndex		+= 3;
		m_nEDataLen	+= 4;
	}

	if(nBufLen > nIndex)
	{
		Raw.Clear();
		Raw.nSize = (BYTE) (nBufLen - nIndex);
		::CopyMemory(&Raw, m_pDBuffer + nIndex, nBufLen - nIndex);
		_EncodeToBuffer(Raw, m_pEBuffer + m_nEDataLen);
		m_nEDataLen += 4;
	}
}

CString Base64Coder::Encode(const CString& sMessage)
{
	Base64Coder b64;
	
	b64.Encode((const PBYTE)(LPCTSTR)sMessage, (sMessage.GetLength() * sizeof(TCHAR)));

	return b64.EncodedMessage();
}

void Base64Coder::Decode(const BYTE* pBuffer, DWORD dwBufLen)
{
	if(!Base64Coder::m_Init)
		_Init();

	SetEncodeBuffer(pBuffer, dwBufLen);

	AllocDecode(dwBufLen);

	TempBucket			Raw;

	DWORD		nIndex = 0;

	while((nIndex + 4) <= m_nEDataLen)
	{
		Raw.Clear();
		Raw.nData[0] = Base64Coder::m_DecodeTable[m_pEBuffer[nIndex]];
		Raw.nData[1] = Base64Coder::m_DecodeTable[m_pEBuffer[nIndex + 1]];
		Raw.nData[2] = Base64Coder::m_DecodeTable[m_pEBuffer[nIndex + 2]];
		Raw.nData[3] = Base64Coder::m_DecodeTable[m_pEBuffer[nIndex + 3]];

		if(Raw.nData[2] == 255)
			Raw.nData[2] = 0;
		if(Raw.nData[3] == 255)
			Raw.nData[3] = 0;
		
		Raw.nSize = 4;
		_DecodeToBuffer(Raw, m_pDBuffer + m_nDDataLen);
		nIndex += 4;
		m_nDDataLen += 3;
	}
	
   // If nIndex < m_nEDataLen, then we got a decode message without padding.
   // We may want to throw some kind of warning here, but we are still required
   // to handle the decoding as if it was properly padded.
	if(nIndex < m_nEDataLen)
	{
		Raw.Clear();
		for(DWORD i = nIndex; i < m_nEDataLen; i++)
		{
			Raw.nData[i - nIndex] = Base64Coder::m_DecodeTable[m_pEBuffer[i]];
			Raw.nSize++;
			if(Raw.nData[i - nIndex] == 255)
				Raw.nData[i - nIndex] = 0;
		}

		_DecodeToBuffer(Raw, m_pDBuffer + m_nDDataLen);
		m_nDDataLen += (m_nEDataLen - nIndex);
	}
}

CString Base64Coder::Decode(const CString& sMessage)
{
	DWORD dwLen = sMessage.GetLength();
	Base64Coder b64;
	
	USES_CONVERSION;
	b64.Decode((const PBYTE)W2A(sMessage), dwLen);

	return (LPCTSTR)b64.DecodedMessage(dwLen);
}

DWORD Base64Coder::_DecodeToBuffer(const TempBucket &Decode, PBYTE pBuffer)
{
	TempBucket	Data;
	DWORD			nCount = 0;

	_DecodeRaw(Data, Decode);

	for(int i = 0; i < 3; i++)
	{
		pBuffer[i] = Data.nData[i];
		if(pBuffer[i] != 255)
			nCount++;
	}

	return nCount;
}


void Base64Coder::_EncodeToBuffer(const TempBucket &Decode, PBYTE pBuffer)
{
	TempBucket	Data;

	_EncodeRaw(Data, Decode);

	for(int i = 0; i < 4; i++)
		pBuffer[i] = Base64Digits[Data.nData[i]];

	switch(Decode.nSize)
	{
	case 1:
		pBuffer[2] = '=';
	case 2:
		pBuffer[3] = '=';
	}
}

void Base64Coder::_DecodeRaw(TempBucket &Data, const TempBucket &Decode)
{
	BYTE		nTemp;

	Data.nData[0] = Decode.nData[0];
	Data.nData[0] <<= 2;

	nTemp = Decode.nData[1];
	nTemp >>= 4;
	nTemp &= 0x03;
	Data.nData[0] |= nTemp;

	Data.nData[1] = Decode.nData[1];
	Data.nData[1] <<= 4;

	nTemp = Decode.nData[2];
	nTemp >>= 2;
	nTemp &= 0x0F;
	Data.nData[1] |= nTemp;

	Data.nData[2] = Decode.nData[2];
	Data.nData[2] <<= 6;
	nTemp = Decode.nData[3];
	nTemp &= 0x3F;
	Data.nData[2] |= nTemp;
}

void Base64Coder::_EncodeRaw(TempBucket &Data, const TempBucket &Decode)
{
	BYTE		nTemp;

	Data.nData[0] = Decode.nData[0];
	Data.nData[0] >>= 2;
	
	Data.nData[1] = Decode.nData[0];
	Data.nData[1] <<= 4;
	nTemp = Decode.nData[1];
	nTemp >>= 4;
	Data.nData[1] |= nTemp;
	Data.nData[1] &= 0x3F;

	Data.nData[2] = Decode.nData[1];
	Data.nData[2] <<= 2;

	nTemp = Decode.nData[2];
	nTemp >>= 6;

	Data.nData[2] |= nTemp;
	Data.nData[2] &= 0x3F;

	Data.nData[3] = Decode.nData[2];
	Data.nData[3] &= 0x3F;
}

BOOL Base64Coder::_IsBadMimeChar(BYTE nData)
{
	switch(nData)
	{
		case '\r': case '\n': case '\t': case ' ' :
		case '\b': case '\a': case '\f': case '\v':
			return TRUE;
		default:
			return FALSE;
	}
}

void Base64Coder::_Init()
{  // Initialize Decoding table.

	int	i;

	for(i = 0; i < 256; i++)
		Base64Coder::m_DecodeTable[i] = -2;

	for(i = 0; i < 64; i++)
	{
		Base64Coder::m_DecodeTable[Base64Digits[i]]			= (char)i;
		Base64Coder::m_DecodeTable[Base64Digits[i]|0x80]	= (char)i;
	}

	Base64Coder::m_DecodeTable['=']				= (char)-1;
	Base64Coder::m_DecodeTable['='|0x80]		= (char)-1;

	Base64Coder::m_Init = TRUE;
}

