// TDLRiskComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLRiskComboBox.h"
#include "resource.h"
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

void AFXAPI DDX_CBRisk(CDataExchange* pDX, int nIDC, int& nRisk)
{
	if (pDX->m_bSaveAndValidate)
	{
		::DDX_CBIndex(pDX, nIDC, nRisk);

		if (nRisk == 0) // NONE
			nRisk = FM_NORISK;
		else
			nRisk--;
	}
	else
	{
		int nTemp = (nRisk == FM_NORISK) ? 0 : nRisk + 1;
		::DDX_CBIndex(pDX, nIDC, nTemp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox

CTDLRiskComboBox::CTDLRiskComboBox()
{
}

CTDLRiskComboBox::~CTDLRiskComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLRiskComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLRiskComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLRiskComboBox message handlers

int CTDLRiskComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLRiskComboBox::PreSubclassWindow() 
{
	COwnerdrawComboBoxBase::PreSubclassWindow();

 	BuildCombo();
}

int CTDLRiskComboBox::GetSelectedRisk() const
{
	int nRisk = GetCurSel();

	if (nRisk == 0)
	{
		nRisk = FM_NORISK;
	}
	else if (nRisk > 0)
	{
		nRisk--; // to take account of <none>
	}

	return nRisk;
}

void CTDLRiskComboBox::SetSelectedRisk(int nRisk) // -2 -> 10
{
   if (nRisk == FM_NORISK)
   {
	   nRisk = 0;
   }
   else if (nRisk >= 0 && nRisk <= 10)
   {
	   nRisk++; // to take account of <none>
   }
   else
   {
	   nRisk = CB_ERR;
   }

   SetCurSel(nRisk);
}

void CTDLRiskComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	CHoldRedraw hr(*this);
	
	int nSel = GetCurSel(); // so we can restore it
	ResetContent();
	
	// first item is 'None' 
	AddString(CString((LPCTSTR)IDS_TDC_NONE));
	
	for (int nLevel = 0; nLevel <= 10; nLevel++)
	{
		CString sRisk;
		sRisk.Format(_T("%d (%s)"), nLevel, CString((LPCTSTR)IDS_TDC_SCALE[nLevel]));
		AddString(sRisk);
	}
	
	SetCurSel(nSel);
}

