// RTF2HTMLConverter.h: interface for the CRTF_HTMLConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(UTIL__INCLUDED_)
#define UTIL__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//! Long->CString Conversion
CString LongToString(long newValue);
//! CString->Long Conversion.
/*! returns zero even if string is no valid number*/
long StringToLong(CString newValue);
//! Converts a ;-delimited string into its single elements
void StringToArray(const CString& strSrc, CStringArray& trgt);

#endif // !defined(UTIL__INCLUDED_)
