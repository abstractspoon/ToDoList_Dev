// MenuEx.h: interface for the CTDCMainMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
#define AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\EnMenu.h"

class CTDCMainMenu : public CEnMenu  
{
public:
	CTDCMainMenu();
	virtual ~CTDCMainMenu();

	BOOL LoadMenu(HWND hwndRef, BOOL bTranslate = FALSE, BOOL bRecursiveTranslate = FALSE);
	BOOL LoadMenu(BOOL bTranslate = FALSE, BOOL bRecursiveTranslate = FALSE);

};

#endif // !defined(AFX_TDCMAINMENU_H__5AB11CC8_CCF5_4D52_ADC7_27FDC151F3FE__INCLUDED_)
