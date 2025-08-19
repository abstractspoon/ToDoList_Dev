#if !defined(AFX_CALENDARMSG_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
#define AFX_CALENDARMSG_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

#include "CalEnum.h"

/////////////////////////////////////////////////////////////////////////////

// WPARAM = TCINOTIFYDATECHANGE*, LPARAM = Task ID
const UINT WM_CALENDAR_DATECHANGE = ::RegisterWindowMessage(_T("WM_CALENDAR_DATECHANGE"));

struct TCINOTIFYDATECHANGE
{
	TCC_HITTEST nHit;
	CString sCustAttribID;
};

// WPARAM = 0, LPARAM = Task ID
const UINT WM_CALENDAR_SELCHANGE			= ::RegisterWindowMessage(_T("WM_CALENDAR_SELCHANGE"));

// WPARAM = Drag Mode, LPARAM = Task ID
const UINT WM_CALENDAR_DRAGCHANGE			= ::RegisterWindowMessage(_T("WM_CALENDAR_DRAGCHANGE"));

// WPARAM = , LPARAM = 
const UINT WM_CALENDAR_PREFSHELP			= ::RegisterWindowMessage(_T("WM_CALENDAR_PREFSHELP"));

// WPARAM = LPCTSTR (Task Icon), LPARAM = int* (imageIndex), return HIMAGELIST
const UINT WM_CALENDAR_GETTASKICON			= ::RegisterWindowMessage(_T("WM_CALENDAR_GETTASKICON"));

// WPARAM = , LPARAM = 
const UINT WM_CALENDAR_EDITTASKICON			= ::RegisterWindowMessage(_T("WM_CALENDAR_EDITTASKICON"));

// WPARAM = Task ID, LPARAM = TASKCALFUTUREDATES*
const UINT WM_CALENDAR_GETTASKFUTUREDATES	= ::RegisterWindowMessage(_T("WM_CALENDAR_GETTASKFUTUREDATES"));

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CALENDARMSG_H__09FB7C3D_BBA8_43B3_A7B3_1D95C946892B__INCLUDED_)
