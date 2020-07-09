/******************************************************************
*                                                                 *
*  strsafe.h -- This module defines safer C library string        *
*               routine replacements. These are meant to make C   *
*               a bit more safe in reference to security and      *
*               robustness                                        *
*                                                                 *
*  Copyright (c) Microsoft Corp.  All rights reserved.            *
*                                                                 *
******************************************************************/

#include <stdio.h>          // for _vsnprintf, _vsnwprintf, getc, getwc
#include <string.h>         // for memset
#include <stdarg.h>         // for va_start, etc.
// #include <specstrings.h>    // for _In_, etc.
#include <wtypes.h>   // for WINAPI_FAMILY_PARTITION()

#define STRSAFE_E_INSUFFICIENT_BUFFER           ((HRESULT)0x8007007AL)  // 0x7A = 122L = ERROR_INSUFFICIENT_BUFFER
#define STRSAFE_E_INVALID_PARAMETER             ((HRESULT)0x80070057L)  // 0x57 =  87L = ERROR_INVALID_PARAMETER
#define STRSAFE_E_END_OF_FILE                   ((HRESULT)0x80070026L)  // 0x26 =  38L = ERROR_HANDLE_EOF

#ifndef STRSAFE_MAX_CCH
#define STRSAFE_MAX_CCH     2147483647  // max buffer size, in characters, that we support (same as INT_MAX)
#endif
// C_ASSERT(STRSAFE_MAX_CCH <= 2147483647);
// C_ASSERT(STRSAFE_MAX_CCH > 1);

#define STRSAFE_MAX_LENGTH  (STRSAFE_MAX_CCH - 1)   // max buffer length, in characters, that we support

HRESULT __stdcall
StringLengthWorkerA(const char* psz, size_t cchMax, size_t* pcchLength)
{
    HRESULT hr = S_OK;
    size_t cchOriginalMax = cchMax;
	
    while (cchMax && (*psz != '\0'))
    {
        psz++;
        cchMax--;
    }
	
    if (cchMax == 0)
    {
        // the string is longer than cchMax
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (pcchLength)
    {
        if (SUCCEEDED(hr))
        {
            *pcchLength = cchOriginalMax - cchMax;
        }
        else
        {
            *pcchLength = 0;
        }
    }
	
    return hr;
}

HRESULT __stdcall
StringLengthWorkerW(const wchar_t* psz, size_t cchMax, size_t* pcchLength)
{
    HRESULT hr = S_OK;
    size_t cchOriginalMax = cchMax;
	
    while (cchMax && (*psz != '\0'))
    {
        psz++;
        cchMax--;
    }
	
    if (cchMax == 0)
    {
        // the string is longer than cchMax
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (pcchLength)
    {
        if (SUCCEEDED(hr))
        {
            *pcchLength = cchOriginalMax - cchMax;
        }
        else
        {
            *pcchLength = 0;
        }
    }
	
    return hr;
}

HRESULT __stdcall
StringCopyWorkerA(char* pszDest, size_t cchDest, 
				  size_t* pcchNewDestLength, 
				  const char* pszSrc, size_t cchToCopy)
{
    HRESULT hr = S_OK;
    size_t cchNewDestLength = 0;
	
    // ASSERT(cchDest != 0);
	
    while (cchDest && cchToCopy && (*pszSrc != '\0'))
    {
        *pszDest++ = *pszSrc++;
        cchDest--;
        cchToCopy--;
		
        cchNewDestLength++;
    }
	
    if (cchDest == 0)
    {
        // we are going to truncate pszDest
        pszDest--;
        cchNewDestLength--;
		
        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
	
    *pszDest = '\0';
	
    if (pcchNewDestLength)
    {
        *pcchNewDestLength = cchNewDestLength;
    }
	
    return hr;
}

HRESULT __stdcall
StringCopyWorkerW(wchar_t* pszDest, size_t cchDest, 
				  size_t* pcchNewDestLength, 
				  const wchar_t* pszSrc, size_t cchToCopy)
{
    HRESULT hr = S_OK;
    size_t cchNewDestLength = 0;
	
    // ASSERT(cchDest != 0);
	
    while (cchDest && cchToCopy && (*pszSrc != '\0'))
    {
        *pszDest++ = *pszSrc++;
        cchDest--;
        cchToCopy--;
		
        cchNewDestLength++;
    }
	
    if (cchDest == 0)
    {
        // we are going to truncate pszDest
        pszDest--;
        cchNewDestLength--;
		
        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
	
    *pszDest = '\0';
	
    if (pcchNewDestLength)
    {
        *pcchNewDestLength = cchNewDestLength;
    }
	
    return hr;
}

HRESULT __stdcall
StringValidateDestAndLengthA(char* pszDest, size_t cchDest, size_t* pcchDestLength, const size_t /*cchMax*/)
{
    HRESULT hr = S_OK;
	
    if ((cchDest == 0) || (cchDest > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (SUCCEEDED(hr))
    {
        hr = StringLengthWorkerA(pszDest, cchDest, pcchDestLength);
    }
    else
    {
        *pcchDestLength = 0;
    }
	
    return hr;
}

HRESULT __stdcall
StringValidateDestAndLengthW(wchar_t* pszDest, size_t cchDest, size_t* pcchDestLength, const size_t /*cchMax*/)
{
    HRESULT hr = S_OK;
	
    if ((cchDest == 0) || (cchDest > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (SUCCEEDED(hr))
    {
        hr = StringLengthWorkerW(pszDest, cchDest, pcchDestLength);
    }
    else
    {
        *pcchDestLength = 0;
    }
	
    return hr;
}

HRESULT __stdcall
StringVPrintfWorkerA(char* pszDest, size_t cchDest, size_t* pcchNewDestLength, const char* pszFormat, va_list argList)
{
    HRESULT hr = S_OK;
    int iRet;
    size_t cchMax;
    size_t cchNewDestLength = 0;
	
    // leave the last space for the null terminator
    cchMax = cchDest - 1;
	
    iRet = _vsnprintf(pszDest, cchMax, pszFormat, argList);
    // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));
	
    if ((iRet < 0) || (((size_t)iRet) > cchMax))
    {
        // need to null terminate the string
        pszDest += cchMax;
        *pszDest = '\0';
		
        cchNewDestLength = cchMax;
		
        // we have truncated pszDest
        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
    else if (((size_t)iRet) == cchMax)
    {
        // need to null terminate the string
        pszDest += cchMax;
        *pszDest = '\0';
		
        cchNewDestLength = cchMax;
    }
    else
    {
        cchNewDestLength = (size_t)iRet;
    }
	
    if (pcchNewDestLength)
    {
        *pcchNewDestLength = cchNewDestLength;
    }
	
    return hr;
}

HRESULT __stdcall
StringCchPrintfA(char* pszDest, size_t cchDest,const char* pszFormat, ...)
{
    HRESULT hr = S_OK;
	
    if ((cchDest == 0) || (cchDest > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (SUCCEEDED(hr))
    {
        va_list argList;
		
        va_start(argList, pszFormat);
		
        hr = StringVPrintfWorkerA(pszDest,
			cchDest,
			NULL,
			pszFormat,
			argList);
		
        va_end(argList);
    }
    else if (cchDest > 0)
    {
        *pszDest = '\0';
    }
	
    return hr;
}

HRESULT __stdcall
StringVPrintfWorkerW(wchar_t* pszDest, size_t cchDest, size_t* pcchNewDestLength, const wchar_t* pszFormat, va_list argList)
{
    HRESULT hr = S_OK;
    int iRet;
    size_t cchMax;
    size_t cchNewDestLength = 0;
	
    // leave the last space for the null terminator
    cchMax = cchDest - 1;
	
    iRet = _vsnwprintf(pszDest, cchMax, pszFormat, argList);
    // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));
	
    if ((iRet < 0) || (((size_t)iRet) > cchMax))
    {
        // need to null terminate the string
        pszDest += cchMax;
        *pszDest = L'\0';
		
        cchNewDestLength = cchMax;
		
        // we have truncated pszDest
        hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
    else if (((size_t)iRet) == cchMax)
    {
        // need to null terminate the string
        pszDest += cchMax;
        *pszDest = L'\0';
		
        cchNewDestLength = cchMax;
    }
    else
    {
        cchNewDestLength = (size_t)iRet;
    }
	
    if (pcchNewDestLength)
    {
        *pcchNewDestLength = cchNewDestLength;
    }
	
    return hr;
}

HRESULT __stdcall
StringCchPrintfW(wchar_t* pszDest, size_t cchDest, const wchar_t* pszFormat, ...)
{
    HRESULT hr = S_OK;
	
    if ((cchDest == 0) || (cchDest > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
	
    if (SUCCEEDED(hr))
    {
        va_list argList;
		
        va_start(argList, pszFormat);
		
        hr = StringVPrintfWorkerW(pszDest,
			cchDest,
			NULL,
			pszFormat,
			argList);
		
        va_end(argList);
    }
    else if (cchDest > 0)
    {
        *pszDest = '\0';
    }
	
    return hr;
}

HRESULT __stdcall
StringCchCatW(wchar_t* pszDest, size_t cchDest, const wchar_t* pszSrc)
{
    HRESULT hr;
    size_t cchDestLength;
	
    hr = StringValidateDestAndLengthW(pszDest,
		cchDest,
		&cchDestLength,
		STRSAFE_MAX_CCH);
	
    if (SUCCEEDED(hr))
    {
        hr = StringCopyWorkerW(pszDest + cchDestLength,
			cchDest - cchDestLength,
			NULL,
			pszSrc,
			STRSAFE_MAX_LENGTH);
    }
	
    return hr;
}

HRESULT __stdcall
StringCchLengthA(const char* psz, size_t cchMax, size_t* pcchLength)
{
    HRESULT hr;
	
    if ((psz == NULL) || (cchMax > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        hr = StringLengthWorkerA(psz, cchMax, pcchLength);
    }
	
    if (FAILED(hr) && pcchLength)
    {
        *pcchLength = 0;
    }
	
    return hr;
}

HRESULT __stdcall
	StringCchCatA(char* pszDest, size_t cchDest, const char* pszSrc)
{
    HRESULT hr;
    size_t cchDestLength;
	
    hr = StringValidateDestAndLengthA(pszDest,
		cchDest,
		&cchDestLength,
		STRSAFE_MAX_CCH);
	
    if (SUCCEEDED(hr))
    {
        hr = StringCopyWorkerA(pszDest + cchDestLength,
			cchDest - cchDestLength,
			NULL,
			pszSrc,
			STRSAFE_MAX_LENGTH);
    }
	
    return hr;
}


HRESULT __stdcall
    StringCchCopyA(char* pszDest, size_t cchDest, const char* pszSrc)
{
    HRESULT hr = S_OK;
	
    if ((cchDest == 0) || (cchDest > STRSAFE_MAX_CCH))
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }

    if (SUCCEEDED(hr))
    {
        hr = StringCopyWorkerA(pszDest,
                cchDest,
                NULL,
                pszSrc,
                STRSAFE_MAX_LENGTH);
    }
    else if (cchDest > 0)
    {
        *pszDest = '\0';
    }

    return hr;
}

