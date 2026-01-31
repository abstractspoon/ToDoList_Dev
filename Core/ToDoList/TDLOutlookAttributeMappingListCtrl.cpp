// TDLCsvAttributeSetupListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLOutlookAttributeMappingListCtrl.h"

#include "..\Shared\AutoFlag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum { ATTRIB_ID = 5000 };

/////////////////////////////////////////////////////////////////////////////
// CTDLOutlookAttributeMappingListCtrl

CTDLOutlookAttributeMappingListCtrl::CTDLOutlookAttributeMappingListCtrl()
 : CTDLImportExportAttributeMappingListCtrl(TRUE)
{
}

CTDLOutlookAttributeMappingListCtrl::~CTDLOutlookAttributeMappingListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLOutlookAttributeMappingListCtrl, CTDLImportExportAttributeMappingListCtrl)
	//{{AFX_MSG_MAP(CTDLOutlookAttributeMappingListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(ATTRIB_ID, OnAttribEditOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLOutlookAttributeMappingListCtrl message handlers

void CTDLOutlookAttributeMappingListCtrl::OnAttribEditOK()
{
	// Turn on one-to-one mapping for everything except comments and title
	// ie. Multiple Outlook fields can be handled by the comments/title field
	// but every other field must map to a unique TDC attribute
	int nSel = m_cbAttributes.GetCurSel();

	if (nSel == CB_ERR)
		return;

	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)m_cbAttributes.GetItemData(nSel);
	BOOL bOneToOne = !((nAttribID != TDCA_COMMENTS) || (nAttribID != TDCA_TASKNAME));

	CAutoFlag af(m_bOneToOneMapping, bOneToOne);

	CTDLImportExportAttributeMappingListCtrl::OnAttribEditOK();
}

