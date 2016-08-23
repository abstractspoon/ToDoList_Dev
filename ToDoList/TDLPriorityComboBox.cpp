// TDLPriorityComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPriorityComboBox.h"
#include "tdcenum.h"

#include "..\shared\holdredraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT IDS_TDC_SCALE[] = { IDS_TDC_SCALE0,
								IDS_TDC_SCALE1,
								IDS_TDC_SCALE2,
								IDS_TDC_SCALE3,
								IDS_TDC_SCALE4,
								IDS_TDC_SCALE5,
								IDS_TDC_SCALE6,
								IDS_TDC_SCALE7,
								IDS_TDC_SCALE8,
								IDS_TDC_SCALE9,
								IDS_TDC_SCALE10 };


const int TDC_NUMSCALES = sizeof(IDS_TDC_SCALE) / sizeof(UINT);

void AFXAPI DDX_CBPriority(CDataExchange* pDX, int nIDC, int& nPriority)
{
	if (pDX->m_bSaveAndValidate)
	{
		::DDX_CBIndex(pDX, nIDC, nPriority);

		if (nPriority == 0) // NONE
			nPriority = FM_NOPRIORITY;
		else
			nPriority--;
	}
	else
	{
		int nTemp = (nPriority == FM_NOPRIORITY) ? 0 : nPriority + 1;
		::DDX_CBIndex(pDX, nIDC, nTemp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLPriorityComboBox

CTDLPriorityComboBox::CTDLPriorityComboBox() //: m_bReverse(FALSE)
{
}

CTDLPriorityComboBox::~CTDLPriorityComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLPriorityComboBox, CColorComboBox)
	//{{AFX_MSG_MAP(CTDLPriorityComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPriorityComboBox message handlers

int CTDLPriorityComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CColorComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLPriorityComboBox::PreSubclassWindow() 
{
	CColorComboBox::PreSubclassWindow();
	
	BuildCombo();
}

int CTDLPriorityComboBox::IncrementPriority(int nAmount)
{
	int nPrevPriority = GetCurSel();
	int nPriority = nPrevPriority + nAmount;
	nPriority = max(0, min(nPriority, 11));	

	if (nPriority != nPrevPriority)
		SetCurSel(nPriority);

	return GetSelectedPriority();
}

int CTDLPriorityComboBox::GetSelectedPriority() const
{
	int nPriority = GetCurSel();

	if (nPriority == 0)
	{
		nPriority = FM_NOPRIORITY;
	}
	else if (nPriority > 0)
	{
		nPriority--; // to take account of <none>
	}

	return nPriority;
}

void CTDLPriorityComboBox::SetSelectedPriority(int nPriority) // -2 -> 10
{
	if (nPriority == FM_NOPRIORITY)
	{
		nPriority = 0;
	}
	else if (nPriority >= 0 && nPriority <= 10)
	{
		nPriority++; // to take account of <none>
	}
	else
	{
		nPriority = CB_ERR;
	}

	SetCurSel(nPriority);
}

BOOL CTDLPriorityComboBox::SetColors(const CDWordArray& aColors)
{
	if (aColors.GetSize() < 11)
		return FALSE;
	
	m_aColors.Copy(aColors);
	
	if (GetSafeHwnd())
		BuildCombo();
	
	return TRUE;
}

// not currently used
/*
void CTDLPriorityComboBox::SetReverseOrder(BOOL bReverse)
{
	if (m_bReverse != bReverse)
	{
		m_bReverse = bReverse;
		
		if (GetSafeHwnd())
			BuildCombo();
	}
}
*/
void CTDLPriorityComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	CHoldRedraw hr(*this);

	// Remove sorting else 10 will get sorted after 1
	ModifyStyle(CBS_SORT, 0);
	
	int nSel = GetCurSel(); // so we can restore it
	ResetContent();
	
	BOOL bHasColors = m_aColors.GetSize();

	// first item is 'None' and never has a colour
	AddColor(CLR_NONE, CString((LPCTSTR)IDS_TDC_NONE));
	
	for (int nLevel = 0; nLevel <= 10; nLevel++)
	{
		COLORREF color = bHasColors ? m_aColors[nLevel] : -1;
		int nPriority = /*m_bReverse ? 11 - nLevel :*/ nLevel;
		
		CString sPriority;
		sPriority.Format(_T("%d (%s)"), nPriority, CString((LPCTSTR)IDS_TDC_SCALE[nLevel]));
		AddColor(color, sPriority);
	}
	
	SetCurSel(nSel);
}

