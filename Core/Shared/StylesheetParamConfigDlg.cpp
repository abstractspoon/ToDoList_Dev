// stylesheetconfigdlg.cpp : implementation file
//

#include "stdafx.h"
#include "StylesheetParamConfigDlg.h"
#include "misc.h"
#include "filemisc.h"
#include "Graphicsmisc.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT LIST_CX	= GraphicsMisc::ScaleByDPIFactor(300);
const UINT LIST_CY	= GraphicsMisc::ScaleByDPIFactor(200);
const UINT SPACING	= GraphicsMisc::ScaleByDPIFactor(7);
const UINT BORDER	= GraphicsMisc::ScaleByDPIFactor(7);
const UINT BTN_CX	= GraphicsMisc::ScaleByDPIFactor(50);
const UINT BTN_CY	= GraphicsMisc::ScaleByDPIFactor(14);

const UINT IDC_LIST	= 1000;

/////////////////////////////////////////////////////////////////////////////
// CStylesheetConfigDlg dialog

CStylesheetParamConfigDlg::CStylesheetParamConfigDlg(LPCTSTR szStylesheetPath, UINT nIDNoParamsMsg, CWnd* pParent /*=NULL*/)
	: 
	CRuntimeDlg(pParent),
	m_sStylesheetPath(szStylesheetPath),
	m_nIDNoParamsMsg(nIDNoParamsMsg)
{
}

void CStylesheetParamConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStylesheetConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_lcParams);
}


BEGIN_MESSAGE_MAP(CStylesheetParamConfigDlg, CRuntimeDlg)
	//{{AFX_MSG_MAP(CStylesheetConfigDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStylesheetConfigDlg message handlers

int CStylesheetParamConfigDlg::DoModal()
{
	if (!m_aParams.GetSize())
		return IDCANCEL;
	
	return CRuntimeDlg::DoModal(_T("CStylesheetConfigDlg"));
}

BOOL CStylesheetParamConfigDlg::OnInitDialog()
{
	ASSERT(!::PathIsRelative(m_sStylesheetPath));
	ASSERT(FileMisc::HasExtension(m_sStylesheetPath, _T("xsl")));

	// Load the stylesheet and params
	CXslFile stylesheet;

	if (!stylesheet.Load(m_sStylesheetPath))
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if (!stylesheet.GetGlobalParams(m_aParams))
	{
		AfxMessageBox(m_nIDNoParamsMsg);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// List control for editing params
	AddRCControl(&m_lcParams, NULL, LVS_REPORT | LVS_OWNERDRAWFIXED | WS_TABSTOP, WS_EX_CLIENTEDGE, BORDER, BORDER, LIST_CX, LIST_CY, IDC_LIST);

	// add ok and cancel buttons at the bottom right
	int nYPos = (BORDER + LIST_CY + SPACING);
	int nXPos = (BORDER + LIST_CX);

	AddRCControl(_T("PUSHBUTTON"), NULL, _T("OK"), 0, 0, (nXPos - (2 * BTN_CX) - SPACING), nYPos, BTN_CX, BTN_CY, IDOK);
	AddRCControl(_T("PUSHBUTTON"), NULL, _T("Cancel"), 0, 0, (nXPos - BTN_CX), nYPos, BTN_CX, BTN_CY, IDCANCEL);

	CRuntimeDlg::OnInitDialog();

	m_lcParams.AddCol(_T("Parameter Name"), (LIST_CX / 2)); // Parameter name
	m_lcParams.AddCol(_T("Parameter Value"), (LIST_CX / 2)); // Parameter value

	int nNumParams = m_aParams.GetSize();
	ASSERT(nNumParams);

	for (int nParam = 0; nParam < nNumParams; nParam++)
	{
		const XSLPARAM& param = m_aParams[nParam];

		int nItem = m_lcParams.AddRow(param.sTitle);
		m_lcParams.SetItemText(nItem, 1, param.sValue);

		// Save the original parameter index so we can sort the items
		// and find our way back at the end
		m_lcParams.SetItemData(nItem, nParam);
	}

	m_lcParams.ShowGrid(TRUE, TRUE);
	m_lcParams.DisableColumnEditing(0, TRUE);
	m_lcParams.SetSortColumn(0);
	m_lcParams.SetFocus();

	return FALSE;  
}

void CStylesheetParamConfigDlg::OnOK()
{
	// Update the parameter values from the list ctrl
	int nNumRows = m_lcParams.GetItemCount();
	ASSERT(nNumRows == m_aParams.GetSize());

	BOOL bChange = FALSE;

	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		int nParam = (int)m_lcParams.GetItemData(nRow);
		ASSERT(nParam < nNumRows);
		ASSERT(m_lcParams.GetItemText(nRow, 0) == m_aParams[nParam].sTitle);

		CString sNewValue = m_lcParams.GetItemText(nRow, 1);

		if (sNewValue != m_aParams[nParam].sValue)
		{
			m_aParams[nParam].sValue = sNewValue;
			bChange = TRUE;
		}
	}

	if (!bChange)
		CRuntimeDlg::OnCancel();
	else
		CRuntimeDlg::OnOK();
}

int CStylesheetParamConfigDlg::GetParams(CXslParamArray& aParams) const
{
	aParams.Copy(m_aParams);
	return aParams.GetSize();
}

BOOL CStylesheetParamConfigDlg::SaveChanges(LPCTSTR szStylesheetPath)
{
	if (m_aParams.GetSize() == 0)
		return FALSE;

	// Load original stylesheet and check for changes
	CXslFile stylesheet;

	if (!stylesheet.Load(m_sStylesheetPath))
		return FALSE;

	CXslParamArray aOrgParams;

	if (!stylesheet.GetGlobalParams(aOrgParams))
	{
		AfxMessageBox(m_nIDNoParamsMsg);
		return FALSE;
	}

	if (Misc::MatchAllT(m_aParams, aOrgParams, FALSE))
		return FALSE;

	if (!stylesheet.SetGlobalParams(m_aParams))
		return FALSE;

	stylesheet.Trace(FALSE);

	if (Misc::IsEmpty(szStylesheetPath))
		szStylesheetPath = m_sStylesheetPath;

	if (!stylesheet.Save(szStylesheetPath, stylesheet.GetFormat()))
		return FALSE;

#ifdef _DEBUG
	CXslFile test;
	VERIFY(test.Load(szStylesheetPath));
#endif

	return TRUE;
}
