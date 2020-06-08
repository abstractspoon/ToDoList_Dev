// EDialog.h: interface for the EDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCDIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
#define AFX_TDCDIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////

#include "..\Shared\DialogHelper.h"

//////////////////////////////////////////////////////////////////////

class CTimeEdit;

struct TDCCOST;
struct TDCTIMEPERIOD;

//////////////////////////////////////////////////////////////////////

class CTDCDialogHelper : public CDialogHelper
{
public:
	// helpers for updating only a single item
	static BOOL UpdateDataEx(CWnd* pWnd, int nIDC, TDCCOST& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);
	static BOOL UpdateDataEx(CWnd* pWnd, CTimeEdit& ctrl, TDCTIMEPERIOD& value, BOOL bSaveAndValidate = TRUE, int nDecimals = -1);

	static void DDX_Text(CDataExchange* pDX, int nIDC, TDCCOST& value, int nDecimals = -1);
	static void DDX_Text(CDataExchange* pDX, int nIDC, int& value, CSpinButtonCtrl& spin, int nMin = 0, int nMax = 100);

	static void DDX_Text(CDataExchange* pDX, CTimeEdit& ctrl, TDCTIMEPERIOD& value, int nDecimals = -1);
};

#endif // !defined(AFX_TDCDIALOGHELPER_H__8543A453_171B_11D4_AE08_0000E8425C3E__INCLUDED_)
