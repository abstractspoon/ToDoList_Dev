// DarkMode.h: interface for the CDarkMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_)
#define AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDarkMode  
{
public:
	static void Enable(BOOL bEnable = TRUE);
	static BOOL IsEnabled() { return s_bDarkMode; }

protected:
	CDarkMode();
	virtual ~CDarkMode();

protected:
	static BOOL s_bDarkMode;


};

#endif // !defined(AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_)
