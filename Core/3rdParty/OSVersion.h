// OSVersion.h: interface for the COSVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSVERSION_H__2D4F4362_09C2_48B2_91EB_C9671C4E08C4__INCLUDED_)
#define AFX_OSVERSION_H__2D4F4362_09C2_48B2_91EB_C9671C4E08C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum OSVERSION
{
	OSV_LINUX	= -2,
	OSV_UNKNOWN = -1,
	OSV_95,
	OSV_98,
	OSV_ME,
	OSV_NT4,
	OSV_2K,
	OSV_XP,
	OSV_XPP,
	OSV_VISTA,
	OSV_WIN7, 
	OSV_WIN8,
	OSV_WIN81,
	OSV_WIN10,
};


class COSVersion  
{
public:
	COSVersion();
	virtual ~COSVersion();

	CString FormatOSVersion();
	operator OSVERSION() { return GetOSVersion(); }

protected:
	OSVERSION GetOSVersion();
};

#endif // !defined(AFX_OSVERSION_H__2D4F4362_09C2_48B2_91EB_C9671C4E08C4__INCLUDED_)
