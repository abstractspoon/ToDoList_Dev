// CDialogHelper.cpp: implementation of the CDialogHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCDialogHelper.h"
#include "ToDoItem.h"

#include "..\Shared\TimeEdit.h"
#include "..\Shared\autoflag.h"

// #include "winclasses.h"
// #include "wclassdefines.h"
// #include "misc.h"
// #include "Graphicsmisc.h"
// #include "runtimedlg.h"
// #include "enstring.h"
// #include "AcceleratorString.h"

#include <afxpriv.h>
#include <afxtempl.h>

#include <float.h>
#include <locale.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int FLOATBUFLEN = 400;

//////////////////////////////////////////////////////////////////////

void CTDCDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, TDCCOST& value, int nDecimals)
{
	if (pDX->m_bSaveAndValidate)
	{
		CString sCost;
		CDialogHelper::DDX_Text(pDX, nIDC, sCost);

		value.Parse(sCost);
	}
	else
	{
		HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
		TCHAR szBuffer[FLOATBUFLEN + 1] = { 0 }; // +1 for '@'
		LPTSTR szNumStart = szBuffer;

		if (value.bIsRate)
		{
			szBuffer[0] = '@';
			szNumStart++;
		}

		TextFloatFormat(FALSE, &value.dAmount, value.dAmount, DBL_DIG, nDecimals, szNumStart, FLOATBUFLEN);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

void CTDCDialogHelper::DDX_Text(CDataExchange* pDX, CTimeEdit& ctrl, TDCTIMEPERIOD& value, int nDecimals)
{
	TH_UNITS nUnits;
	
	if (pDX->m_bSaveAndValidate)
	{
		ctrl.DDX(pDX, value.dAmount, nUnits, nDecimals);
		value.SetTHUnits(nUnits, FALSE);
	}
	else
	{
		nUnits = value.GetTHUnits();
		ctrl.DDX(pDX, value.dAmount, nUnits, nDecimals);
	}
}

void CTDCDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, int& value, CSpinButtonCtrl& spin, int nMin, int nMax)
{
	if (pDX->m_bSaveAndValidate)
	{
		CDialogHelper::DDX_Text(pDX, nIDC, value);
		value = max(nMin, min(nMax, value));
	}
	else
	{
		value = max(nMin, min(nMax, value));
		CDialogHelper::DDX_Text(pDX, nIDC, value);

		spin.SetRange32(nMin, nMax);
		spin.SetPos(value);
	}
}

////////////////////////////////////////////////////////////////////////////////

BOOL CTDCDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, TDCCOST& value, BOOL bSaveAndValidate, int nDecimals)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CTDCDialogHelper::UpdateDataEx(CWnd* pWnd, CTimeEdit& ctrl, TDCTIMEPERIOD& value, BOOL bSaveAndValidate, int nDecimals)
{
	CLockNotify lock(*pWnd);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, ctrl, value, nDecimals);

	return TRUE;
}

