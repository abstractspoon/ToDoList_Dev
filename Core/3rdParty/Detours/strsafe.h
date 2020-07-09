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
#ifndef _STRSAFE_H_INCLUDED_
#define _STRSAFE_H_INCLUDED_
#if (_MSC_VER > 1000)
#pragma once
#endif

/*
#include <stdio.h>          // for _vsnprintf, _vsnwprintf, getc, getwc
#include <string.h>         // for memset
#include <stdarg.h>         // for va_start, etc.
// #include <specstrings.h>    // for _In_, etc.
//#include <winapifamily.h>   // for WINAPI_FAMILY_PARTITION()

#define STRSAFE_E_INSUFFICIENT_BUFFER           ((HRESULT)0x8007007AL)  // 0x7A = 122L = ERROR_INSUFFICIENT_BUFFER
#define STRSAFE_E_INVALID_PARAMETER             ((HRESULT)0x80070057L)  // 0x57 =  87L = ERROR_INVALID_PARAMETER
#define STRSAFE_E_END_OF_FILE                   ((HRESULT)0x80070026L)  // 0x26 =  38L = ERROR_HANDLE_EOF

#ifndef STRSAFE_MAX_CCH
#define STRSAFE_MAX_CCH     2147483647  // max buffer size, in characters, that we support (same as INT_MAX)
#endif
C_ASSERT(STRSAFE_MAX_CCH <= 2147483647);
C_ASSERT(STRSAFE_MAX_CCH > 1);

#define STRSAFE_MAX_LENGTH  (STRSAFE_MAX_CCH - 1)   // max buffer length, in characters, that we support
*/

HRESULT __stdcall StringLengthWorkerA(const char* psz, size_t cchMax, size_t* pcchLength);
HRESULT __stdcall StringLengthWorkerW(const wchar_t* psz, size_t cchMax, size_t* pcchLength);
HRESULT __stdcall StringCopyWorkerA(char* pszDest, size_t cchDest, size_t* pcchNewDestLength, const char* pszSrc, size_t cchToCopy);
HRESULT __stdcall StringCopyWorkerW(wchar_t* pszDest, size_t cchDest, size_t* pcchNewDestLength, const wchar_t* pszSrc, size_t cchToCopy);
HRESULT __stdcall StringValidateDestAndLengthA(char* pszDest, size_t cchDest, size_t* pcchDestLength, const size_t cchMax);
HRESULT __stdcall StringValidateDestAndLengthW(wchar_t* pszDest, size_t cchDest, size_t* pcchDestLength, const size_t cchMax);
HRESULT __stdcall StringVPrintfWorkerA(char* pszDest, size_t cchDest, size_t* pcchNewDestLength, const char* pszFormat, va_list argList);
HRESULT __stdcall StringCchPrintfA(char* pszDest, size_t cchDest,const char* pszFormat, ...);
HRESULT __stdcall StringVPrintfWorkerW(wchar_t* pszDest, size_t cchDest, size_t* pcchNewDestLength, const wchar_t* pszFormat, va_list argList);
HRESULT __stdcall StringCchPrintfW(wchar_t* pszDest, size_t cchDest, const wchar_t* pszFormat, ...);
HRESULT __stdcall StringCchCatW(wchar_t* pszDest, size_t cchDest, const wchar_t* pszSrc);
HRESULT __stdcall StringCchLengthA(const char* psz, size_t cchMax, size_t* pcchLength);
HRESULT __stdcall StringCchCatA(char* pszDest, size_t cchDest, const char* pszSrc);
HRESULT __stdcall StringCchCopyA(char* pszDest, size_t cchDest, const char* pszSrc);

#endif  // _STRSAFE_H_INCLUDED_
