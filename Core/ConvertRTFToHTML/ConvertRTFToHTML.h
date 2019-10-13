
// ConvertRTFToHTML.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CConvertRTFToHTMLApp:
// See ConvertRTFToHTML.cpp for the implementation of this class
//

class CConvertRTFToHTMLApp : public CWinApp
{
public:
	CConvertRTFToHTMLApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CConvertRTFToHTMLApp theApp;