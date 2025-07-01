// DarkMode.h: interface for the CDarkMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_)
#define AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const COLORREF DM_3DFACE = RGB(119, 120, 121);
const COLORREF DM_WINDOW = RGB(63, 64, 65);

class CDarkMode  
{
public:
	static void Enable(BOOL bEnable = TRUE);
	static void PrepareForIEPrintOrPreview();

	static BOOL IsSupported();
	static BOOL IsEnabled()
	{
		return ((GetSysColor(COLOR_3DFACE) == DM_3DFACE) && (GetSysColor(COLOR_WINDOW) == DM_WINDOW));
	}
	
protected:
	CDarkMode();
	virtual ~CDarkMode();

};

#endif // !defined(AFX_DARKMODE_H__06C3DFE9_33B9_4124_A294_C86007AAB225__INCLUDED_)
