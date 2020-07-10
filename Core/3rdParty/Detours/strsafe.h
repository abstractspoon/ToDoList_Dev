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
