// Please do not remove this comment.
//
// URL Encoding/Decodeing class, supports Unicode and ANSI (MFC TCHAR)
//
// Written by Daniel Cohen Gindi, (danielgindi (at) gmail.com)
//
// If you have any comments or questions, feel free to email me.
//
// You may use this class for any purpose, commercial or personal.

#include "stdafx.h"
#include "URLEncode.h"

// Put the percent (%) sign first, so it won't overwrite the converted Hex'es
LPCTSTR CURLEncode::m_lpszUnsafeString=_T(" \"<>#{}|\\^~[]`");
int CURLEncode::m_iUnsafeLen=(int)_tcslen(m_lpszUnsafeString);
LPCTSTR CURLEncode::m_lpszReservedString=_T("$&+,./:;=?@-_!*()'");
int CURLEncode::m_iReservedLen=(int)_tcslen(m_lpszReservedString);

// Convert a byte into Hexadecimal CString
CString CURLEncode::toHex(BYTE val) 
{
	CString csRet;
	csRet.Format(_T("%%%0.2X"), val);
	return csRet;
}

// Convert a Unicode or ANSI character into UTF8
WORD CURLEncode::toUTF8(TCHAR tc)
{
	WCHAR wc[2];
	CHAR mb[3];

	// If application character set is ANSI,
	// we need to convert to Unicode first
	if (sizeof(TCHAR)==1)
	{
		mb[0]=(CHAR)tc;
		mb[1]=0;
		MultiByteToWideChar(URLENCODE_DEFAULT_ANSI_CODEPAGE, 0, mb, 3, wc, 2);
	}

	if (sizeof(TCHAR)>1) wc[0]=tc;
	wc[1]=0;
	mb[0]=0;
	WideCharToMultiByte(CP_UTF8, 0, wc, 2, mb, 3, 0, 0);
	return MAKEWORD(mb[1], mb[0]);
}

// Convert a UTF8 character into Unicode or ANSI
TCHAR CURLEncode::fromUTF8(WORD w)
{
	WCHAR wc[2];
	CHAR mb[3];

	mb[0]=HIBYTE(w);
	mb[1]=LOBYTE(w);
	mb[2]=0;
	wc[0]=0;

	MultiByteToWideChar(CP_UTF8, 0, mb, 3, wc, 2);
	if (sizeof(TCHAR)==1)
	{
		mb[0]=0;
		WideCharToMultiByte(URLENCODE_DEFAULT_ANSI_CODEPAGE, 0, wc, 2, mb, 3, 0, 0);
		return mb[0];
	} else {
		return (TCHAR)wc[0];
	}
}

// strURL:			URL to encode.
// bEncodeReserved: Encode the reserved characters
//                  for example the ? character, which is used many times
//                  for arguments in URL.
//                  so if we are encoding just a string containing Keywords,
//                  we want to encode the reserved characters.
//                  but if we are encoding a simple URL, we wont.
CString CURLEncode::Encode(CString strURL, BOOL bEncodeReserved/*=FALSE*/)
{
	// First encode the % sign, because we are adding lots of it later...
	strURL.Replace(_T("%"), toHex(__toascii(_T('%'))));


	// Encdoe the reserved characters, if we choose to
	if (bEncodeReserved)
	{
		for (int i=0; i<m_iReservedLen; i++)
		{
			strURL.Replace(CString(m_lpszReservedString[i]), toHex(__toascii(m_lpszReservedString[i])));
		}
	}

	// Encode 'unsafe' characters
	// see: http://www.blooberry.com/indexdot/html/topics/urlencoding.htm
	for (int i=0; i<m_iUnsafeLen; i++)
	{
		strURL.Replace(CString(m_lpszUnsafeString[i]), toHex(__toascii(m_lpszUnsafeString[i])));
	}

	// Encode unprintable characters 0x00-0x1F, and 0x7F
	for (char c=0x00; c<=0x1F; c++)
	{
		strURL.Replace(CString(c), toHex(c));
	}
	strURL.Replace(CString((char)0x7F), toHex(0x7F));

	// Now encode all other unsafe characters
	TCHAR tc=0;
	WORD w=0;

	CString nc;
	// In this stage we do not want to convert:
	// 1. Characters A-Z, a-z, 0-9, because they are safe.
	// 2. The reserved characteres, we have already dealt with them;
	// 3. The % character...
	CString strDoNotReplace(m_lpszReservedString);
	strDoNotReplace += _T("%");

	for (int j=0; j<strURL.GetLength(); j++)
	{
		tc=strURL.GetAt(j);
		if ((tc<_T('a') || tc>_T('z')) && 
			(tc<_T('A') || tc>_T('Z')) &&
			(tc<_T('0') || tc>_T('9')) &&
			strDoNotReplace.Find(tc)<0)
		{
			w=toUTF8(tc);
			nc=toHex(HIBYTE(w));
			nc += toHex(LOBYTE(w));
			strURL.Replace(CString(tc), nc);
			// We have added 5 extra characters to the length of the string,
			// So we can ignore them.
			j+=5;
		}
	}

	return strURL;
}

#define isHex(c) (((c)>=_T('A') && (c)<=_T('F')) || \
				 ((c)>=_T('a') && (c)<=_T('f')) || \
				 ((c)>=_T('0') && (c)<=_T('9')))
#define hexToDec2(c) (((c)>=_T('A') && (c)<=_T('F')) ? c-_T('A')+10 : \
					(((c)>=_T('a') && (c)<=_T('f')) ? c-_T('a')+10 : c-_T('0')))

#define hexToDec(high, low) (hexToDec2(high)<<4 | hexToDec2(low))

// strURL: URL to decode.
CString CURLEncode::Decode(CString strURL)
{
	int i=strURL.Find(_T('%'));
	TCHAR tc1=0, tc2=0;
	BYTE b=0;
	BOOL bFound=FALSE;
	while (i>-1)
	{
		tc1=strURL.GetAt(i+1);
		tc2=strURL.GetAt(i+2);

		if (isHex(tc1) && isHex(tc2))
		{
			b=hexToDec(tc1, tc2);

			// first deal with 1-byte unprintable characters
			if (b<0x1F || b==0x7F) {
				strURL.SetAt(i, b);
				strURL.Delete(i+1, 2);
			} else {
				// Then deal with 1-byte unsafe/reserved characters
				// We are reading for those static LPCTSTR strings,
				// so we have nice support for Unicode
				bFound=FALSE;
				for (int ii=0; ii<m_iUnsafeLen && !bFound; ii++)
				{
					if (__toascii(m_lpszUnsafeString[ii])==b)
					{
						strURL.SetAt(i, m_lpszUnsafeString[ii]);
						strURL.Delete(i+1, 2);
						bFound=TRUE;
					}
				}
				for (int jj=0; jj<m_iReservedLen && !bFound; jj++)
				{
					if (__toascii(m_lpszReservedString[jj])==b)
					{
						strURL.SetAt(i, m_lpszReservedString[jj]);
						strURL.Delete(i+1, 2);
						bFound=TRUE;
					}
				}
				// Then deal with UTF-8 (2-bytes) characters
				if (!bFound)
				{
					// We need to have 2 bytes for decoding
					if (strURL.GetAt(i+3)==_T('%'))
					{
						tc1=strURL.GetAt(i+4);
						tc2=strURL.GetAt(i+5);

						if (isHex(tc1) && isHex(tc2))
						{
							BYTE b2=hexToDec(tc1, tc2);
							strURL.SetAt(i, fromUTF8(MAKEWORD(b2, b)));
							strURL.Delete(i+1, 5);
						}
					}
				}
			}
		}

		i=strURL.Find(_T('%'), i+1);
	}

	return strURL;
}
