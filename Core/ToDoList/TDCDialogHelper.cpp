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
		TCHAR szBuffer[FLOATBUFLEN+1] = { 0 };
		LPTSTR szNumStart = szBuffer;

		if (value.bIsRate)
		{
			szBuffer[0] = '@';
			szNumStart++;
		}

		TextFloatFormat(FALSE, &value.dAmount, value.dAmount, DBL_DIG, nDecimals, szNumStart);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}

void CTDCDialogHelper::DDX_Text(CDataExchange* pDX, int nIDC, TDCTIMEPERIOD& value, int nDecimals)
{
	CTimeEdit* pTime = reinterpret_cast<CTimeEdit*>(CWnd::FromHandle(pDX->PrepareCtrl(nIDC)));

	if (!pTime)
	{
		ASSERT(0);
		return;
	}

	CDialogHelper::DDX_Text(pDX, nIDC, value.dAmount, nDecimals);
	
	if (pDX->m_bSaveAndValidate)
		value.SetTHUnits(pTime->GetUnits());
	else
		pTime->SetUnits(value.GetTHUnits());
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
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

BOOL CTDCDialogHelper::UpdateDataEx(CWnd* pWnd, int nIDC, TDCTIMEPERIOD& value, BOOL bSaveAndValidate, int nDecimals)
{
	CAutoFlag af(m_bInUpdateEx, TRUE);
	CDataExchange dx(pWnd, bSaveAndValidate);

	DDX_Text(&dx, nIDC, value, nDecimals);

	return TRUE;
}

