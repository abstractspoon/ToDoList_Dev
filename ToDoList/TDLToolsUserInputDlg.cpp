// ToolsUserInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tdcenum.h"
#include "tdlToolsUserInputDlg.h"

#include "..\shared\fileedit.h"
#include "..\shared\preferences.h"
#include "..\shared\filemisc.h"
#include "..\shared\datetimectrlex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT LABEL_X	= 7;
const UINT ITEM_X	= 70;
const UINT LABEL_Y	= 7; // just the start pos
const UINT SPACING	= 7; 
const UINT BTN_CX	= 50;
const UINT BTN_CY	= 14;

/////////////////////////////////////////////////////////////////////////////
// CToolsUserInputDlg dialog

CTDLToolsUserInputDlg::CTDLToolsUserInputDlg(const CTDCToolsCmdlineParser& tcp) 
	: 
	CRuntimeDlg(),
	m_rWindowOrg(0, 0, 0, 0),
	m_rClientOrg(0, 0, 0, 0),
	m_nDividerID(0),
	m_nHelpBtnID(0),
	m_btnHelp(IDD_USERTOOL_DIALOG)
{
	// process the user input items and save them off
	CCLArgArray aArgs;
	int nArgCount = tcp.GetArguments(aArgs);
	UINT nCtrlID = 100;
	
	CPreferences prefs;

	if (nArgCount)
	{
		for (int nArg = 0; nArg < nArgCount; nArg++)
		{
			TUINPUTITEM tuii; // we fill in specifics as we go along

			tuii.pCtrl = NULL;
			tuii.sName = aArgs[nArg].sName;
			tuii.sName.MakeLower();

			CLA_TYPE nType = aArgs[nArg].nType;
	
			switch (nType)
			{
			case CLAT_USERFILE:
				tuii.pCtrl = new CFileEdit;
				tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
				tuii.sizeDLU.cx = 130;
				tuii.sizeDLU.cy = 14;
				break;

			case CLAT_USERFOLDER:
				tuii.pCtrl = new CFileEdit(FES_FOLDERS);
				tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
				tuii.sizeDLU.cx = 130;
				tuii.sizeDLU.cy = 14;
				break;

			case CLAT_USERTEXT:
				tuii.pCtrl = new CEdit;
				tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
				tuii.sizeDLU.cx = 130;
				tuii.sizeDLU.cy = 14;
				break;

			case CLAT_USERDATE:
				tuii.pCtrl = new CDateTimeCtrlEx(MCS_WEEKNUMBERS);
				tuii.nStyle = DTS_RIGHTALIGN | WS_TABSTOP;
				tuii.sizeDLU.cx = 70;
				tuii.sizeDLU.cy = 13;
				break;
			}
			
			if (tuii.pCtrl)
			{
				if (!tuii.sName.IsEmpty())
				{
					tuii.nType = aArgs[nArg].nType;
					tuii.nCtrlID = nCtrlID++;
					tuii.sLabel = aArgs[nArg].sLabel;

					tuii.sDefValue = aArgs[nArg].sDefValue;

					// if sDefValue is empty see if we previously saved a value for this control 
					if (tuii.sDefValue.IsEmpty())
						tuii.sDefValue = prefs.GetProfileString(_T("Tools\\UserInput"), tuii.sLabel);

					m_aInputItems.Add(tuii);
				}
				else
				{
					delete tuii.pCtrl;
				}
			}
		}
	}
	m_nDividerID = nCtrlID++;
	m_nHelpBtnID = nCtrlID++;

	// now add the controls to CRuntimeDlg
	int nNumCtrls = m_aInputItems.GetSize();

	if (nNumCtrls)
	{
		int nMaxWidth = 0;
		int nYPos = LABEL_Y;

		for (int nCtrl = 0; nCtrl < nNumCtrls; nCtrl++)
		{
			TUINPUTITEM& tuii = m_aInputItems[nCtrl];
			
			if (AddRCControl(tuii.pCtrl, tuii.sDefValue, tuii.nStyle, WS_EX_CLIENTEDGE, ITEM_X, nYPos, tuii.sizeDLU.cx, tuii.sizeDLU.cy, tuii.nCtrlID))
			{
				// add label
				AddRCControl(_T("LTEXT"), NULL, tuii.sLabel, SS_CENTERIMAGE, 0, 0, nYPos, ITEM_X - LABEL_X - 3, tuii.sizeDLU.cy, 0);

				nYPos += (tuii.sizeDLU.cy + SPACING);

				if (nMaxWidth < (int)(ITEM_X + tuii.sizeDLU.cx))
					nMaxWidth = (ITEM_X + tuii.sizeDLU.cx);
			}
		}

		// add ok and cancel buttons at the bottom right with a preceding line
		AddRCControl(_T("CONTROL"), _T("static"), NULL, SS_ETCHEDFRAME, 0, 0, nYPos, nMaxWidth - LABEL_X, 1, m_nDividerID);
		nYPos += SPACING;

		AddRCControl(_T("PUSHBUTTON"), NULL, _T("OK"), 0, 0, nMaxWidth - 2 * BTN_CX - SPACING, nYPos, BTN_CX, BTN_CY, IDOK);
		AddRCControl(_T("PUSHBUTTON"), NULL, _T("Cancel"), 0, 0, nMaxWidth - BTN_CX, nYPos, BTN_CX, BTN_CY, IDCANCEL);
	}
}

CTDLToolsUserInputDlg::~CTDLToolsUserInputDlg()
{
	// cleanup dynamically allocated items
	int nCtrl = m_aInputItems.GetSize();

	while (nCtrl--)
		delete m_aInputItems[nCtrl].pCtrl;
}

void CTDLToolsUserInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolsUserInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLToolsUserInputDlg, CRuntimeDlg)
	//{{AFX_MSG_MAP(CToolsUserInputDlg)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolsUserInputDlg message handlers

int CTDLToolsUserInputDlg::DoModal(LPCTSTR szTitle) 
{ 
	// Make it resizable
	return CRuntimeDlg::DoModal(szTitle, (RTD_DEFSTYLE | WS_THICKFRAME)); 
}

void CTDLToolsUserInputDlg::OnOK()
{
	CRuntimeDlg::OnOK();

	// now we dynamically extract the window text of the items and map to name
	CPreferences prefs;
	int nCtrl = m_aInputItems.GetSize();

	while (nCtrl--)
	{
		TUINPUTITEM& tuii = m_aInputItems[nCtrl];

		CString sResult;
		tuii.pCtrl->GetWindowText(sResult);

		// save to registry
		prefs.WriteProfileString(_T("Tools\\UserInput"), tuii.sLabel, sResult);

		switch (tuii.nType)
		{
		case CLAT_USERFOLDER:
			// make sure folders are terminated
			sResult.TrimRight();
			FileMisc::TerminatePath(sResult);
			break;

		case CLAT_USERDATE:
			// make sure dates are formatted to ISO standards ie yyyy-mm-dd
			{
				SYSTEMTIME sysTime;

				if (GDT_VALID == tuii.pCtrl->SendMessage(DTM_GETSYSTEMTIME, 0, (LPARAM) &sysTime))
				{
					COleDateTime date(sysTime);
					sResult = date.Format(_T("%Y-%m-%d"));
				}
				else
					ASSERT(0);
			}
			break;
		}

		m_mapResults[tuii.sName] = sResult;
	}
}

BOOL CTDLToolsUserInputDlg::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	// userdate default values need a bit more work
	int nCtrl = m_aInputItems.GetSize();

	while (nCtrl--)
	{
		TUINPUTITEM& tuii = m_aInputItems[nCtrl];

		switch (tuii.nType)
		{
		case CLAT_USERDATE:
			if (!tuii.sDefValue.IsEmpty())
			{
				// parse the date to ISO standards ie yyyy-mm-dd
				SYSTEMTIME sysTime;
				ZeroMemory(&sysTime, sizeof(sysTime));
//fabio_2005
#if _MSC_VER >= 1400
				int nRes = _stscanf_s(tuii.sDefValue, _T("%d-%d-%d"), &sysTime.wYear, &sysTime.wMonth, &sysTime.wDay);
#else
				int nRes = _stscanf(tuii.sDefValue, _T("%d-%d-%d"), &sysTime.wYear, &sysTime.wMonth, &sysTime.wDay);
#endif

				if (nRes == 3)
					tuii.pCtrl->SendMessage(DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM) &sysTime);
			}
			break;
		}
	}

	// Help button in default position
	m_btnHelp.Create(m_nHelpBtnID, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CTDLToolsUserInputDlg::GetResult(LPCTSTR szItemName)
{
	CString sItem(szItemName);
	sItem.MakeLower();

	CString sResult;
	m_mapResults.Lookup(sItem, sResult);

	return sResult;
}

void CTDLToolsUserInputDlg::OnSize(UINT nType, int cx, int cy)
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	// Resize all controls to the RHS 
	int nCtrl = m_aInputItems.GetSize();
	
	while (nCtrl--)
	{
		const TUINPUTITEM& tuii = m_aInputItems[nCtrl];

		CWnd* pCtrl = GetDlgItem(tuii.nCtrlID);

		if (!pCtrl) // not yet created
			return;

		CRect rCtrl(GetChildRect(pCtrl));
		rCtrl.right = (cx - m_rBorders.right);

		pCtrl->MoveWindow(rCtrl);
	}

	// Repos help btn
	m_btnHelp.UpdatePosition();

	// Resize divider
	ASSERT(m_nDividerID);
	CRect rDivider(GetCtrlRect(m_nDividerID));
	rDivider.right = (cx - m_rBorders.right);

	GetDlgItem(m_nDividerID)->MoveWindow(rDivider);

	// Repos OK/Cancel btns
	int nOffset = (cx - m_rBorders.right - GetCtrlRect(IDCANCEL).right);

	OffsetCtrl(IDCANCEL, nOffset, 0);
	OffsetCtrl(IDOK, nOffset, 0);

	Invalidate(FALSE);
}

void CTDLToolsUserInputDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CRuntimeDlg::OnGetMinMaxInfo(lpMMI);

	if (!m_rWindowOrg.IsRectNull())
	{
		// Limit minimum size
		lpMMI->ptMinTrackSize.x = m_rWindowOrg.Width();
		lpMMI->ptMinTrackSize.y = m_rWindowOrg.Height();

		// Prevent height changing
		lpMMI->ptMaxTrackSize.y = m_rWindowOrg.Height();
	}
}

void CTDLToolsUserInputDlg::SetInitialPos(LPCRECT pRect, DWORD dwStyle)
{
	CRuntimeDlg::SetInitialPos(pRect, dwStyle);

	// Save off original window and client rects
	GetWindowRect(m_rWindowOrg);
	GetClientRect(m_rClientOrg);
}

BOOL CTDLToolsUserInputDlg::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(m_btnHelp.GetHelpID());
	return TRUE;
}
