// AcceleratorString.h: interface for the CAcceleratorString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCELERATORSTRING_H__9860B9E9_144B_47B1_978C_BE00250CDCFE__INCLUDED_)
#define AFX_ACCELERATORSTRING_H__9860B9E9_144B_47B1_978C_BE00250CDCFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAcceleratorString : public CString  
{
public:
	CAcceleratorString();
	CAcceleratorString(const CString& sOther, BOOL bRemoveAccelerator = FALSE);
	virtual ~CAcceleratorString();

	TCHAR GetAccelerator(BOOL bMakeLowercase = TRUE) const;
	int FindAccelerator() const;
	BOOL HasAccelerator() const;
	BOOL HasAccelerator(TCHAR cAccel, BOOL bCaseSensitive = FALSE) const;

	BOOL RemoveAccelerator();
	TCHAR EnsureUniqueAccelerator(const CString& sExclude);
	TCHAR SetAcceleratorPos(int nPos);

	static TCHAR GetAccelerator(const CString& sText, BOOL bMakeLowercase = TRUE);
	static TCHAR SetAcceleratorPos(CString& sText, int nPos);
	static BOOL RemoveAccelerator(CString& sText);
	static int FindAccelerator(const CString& sText);
	static BOOL HasAccelerator(const CString& sText);
	static BOOL HasAccelerator(const CString& sText, TCHAR cAccel, BOOL bCaseSensitive = FALSE);
	static TCHAR EnsureUniqueAccelerator(CString& sText, const CString& sExclude);
	static BOOL IsValidAccelerator(TCHAR cAccel, const CString& sExclude);
	static BOOL IsValidAccelerator(TCHAR cAccel);
	static BOOL EnsureUniqueAccelerators(CStringArray& aText);
	static CString GetAccelerators(const CStringArray& aText, BOOL bMakeLowercase = TRUE);
	
};

#endif // !defined(AFX_ACCELERATORSTRING_H__9860B9E9_144B_47B1_978C_BE00250CDCFE__INCLUDED_)
