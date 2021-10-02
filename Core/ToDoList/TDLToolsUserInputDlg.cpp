// ToolsUserInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tdcenum.h"
#include "tdlToolsUserInputDlg.h"
#include "tdcCustomattributeDef.h"
#include "tdcStruct.h"
#include "tdcSwitch.h"

#include "..\shared\fileedit.h"
#include "..\shared\filemisc.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\datehelper.h"
#include "..\shared\misc.h"

#include "..\Interfaces\preferences.h"

/////////////////////////////////////////////////////////////////////////////

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

// Needed by CArray
CTDLToolsUserInputDlg::TUINPUTITEM& CTDLToolsUserInputDlg::TUINPUTITEM::operator=(const CTDLToolsUserInputDlg::TUINPUTITEM& tuii)
{
	nCtrlID = tuii.nCtrlID;
	sLabel = tuii.sLabel;
	sName = tuii.sName;
	sDefValue = tuii.sDefValue;
	aListValues.Copy(tuii.aListValues);
	nType = tuii.nType;
	nStyle = tuii.nStyle;
	sizeDLU = tuii.sizeDLU;
	pCtrl = tuii.pCtrl; 

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CToolsUserInputDlg dialog

CTDLToolsUserInputDlg::CTDLToolsUserInputDlg(const CCLArgArray& aArgs, const TDCAUTOLISTDATA& tdlListData, const CTDCCustomAttribDefinitionArray& aCustAttribDefs, BOOL bISODates)
	: 
	CRuntimeDlg(),
	m_tdlListData(tdlListData),
	m_aCustAttribDefs(aCustAttribDefs),
	m_rWindowOrg(0, 0, 0, 0),
	m_nDividerID(0),
	m_nHelpBtnID(0),
	m_bISODates(bISODates),
	m_btnHelp(IDD_USERTOOL_DIALOG)
{
	// process the user input items and save them off
	int nArgCount = aArgs.GetSize();
	UINT nCtrlID = 100;
	
	CPreferences prefs;

	for (int nArg = 0; nArg < nArgCount; nArg++)
	{
		const CMDLINEARG& arg = aArgs[nArg];

		// we fill in specifics as we go along
		TUINPUTITEM tuii;

		tuii.pCtrl = NULL;
		tuii.sName = arg.sName;
		tuii.sName.MakeLower();

		switch (arg.nType)
		{
			case CLAT_USERFILE:
				tuii.pCtrl = new CFileEdit;
				tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
				tuii.sizeDLU.cx = 130;
				tuii.sizeDLU.cy = 13;
				break;

			case CLAT_USERFOLDER:
				tuii.pCtrl = new CFileEdit(FES_FOLDERS);
				tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
				tuii.sizeDLU.cx = 130;
				tuii.sizeDLU.cy = 13;
				break;

			case CLAT_USERTEXT:
				if (GetArgumentListData(arg, tuii.aListValues))
				{
					tuii.pCtrl = new CComboBox;
					tuii.nStyle = CBS_DROPDOWN | CBS_SORT | WS_TABSTOP;
					tuii.sizeDLU.cx = 130;
					tuii.sizeDLU.cy = 13;
				}
				else
				{
					tuii.pCtrl = new CEdit;
					tuii.nStyle = ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP;
					tuii.sizeDLU.cx = 130;
					tuii.sizeDLU.cy = 13;
				}
				break;

			case CLAT_USERDATE:
				tuii.pCtrl = new CDateTimeCtrlEx(MCS_WEEKNUMBERS);
				tuii.nStyle = DTS_RIGHTALIGN | WS_TABSTOP;
				tuii.sizeDLU.cx = 70;
				tuii.sizeDLU.cy = 13;
				break;

			default:
				ASSERT(0);
		}

		if (tuii.pCtrl)
		{
			if (!tuii.sName.IsEmpty())
			{
				tuii.nType = arg.nType;
				tuii.nCtrlID = nCtrlID++;
				tuii.sLabel = arg.sLabel;

				tuii.sDefValue = arg.sDefValue;

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
			{
				SYSTEMTIME sysTime;

				if (GDT_VALID == tuii.pCtrl->SendMessage(DTM_GETSYSTEMTIME, 0, (LPARAM) &sysTime))
				{
					COleDateTime date(sysTime);
					sResult = CDateHelper::FormatDate(date, (m_bISODates ? DHFD_ISO : 0));
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
	// as do usertext implemented as comboboxes
	int nCtrl = m_aInputItems.GetSize();

	while (nCtrl--)
	{
		TUINPUTITEM& tuii = m_aInputItems[nCtrl];

		switch (tuii.nType)
		{
		case CLAT_USERDATE:
			if (!tuii.sDefValue.IsEmpty())
			{
				COleDateTime date;

				if (CDateHelper::DecodeDate(tuii.sDefValue, date, FALSE))
				{
					SYSTEMTIME sysTime = { 0 };
					
					if (date.GetAsSystemTime(sysTime))
						tuii.pCtrl->SendMessage(DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM) &sysTime);
				}
			}
			break;

		case CLAT_USERTEXT:
			if (tuii.pCtrl->IsKindOf(RUNTIME_CLASS(CComboBox)))
			{
				ASSERT(tuii.aListValues.GetSize());

				CComboBox* pCombo = (CComboBox*)tuii.pCtrl;
				SetComboBoxItems(*pCombo, tuii.aListValues);

				if (!tuii.sDefValue.IsEmpty())
					pCombo->SelectString(-1, tuii.sDefValue);
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

	// Save off original window
	GetWindowRect(m_rWindowOrg);
}

BOOL CTDLToolsUserInputDlg::OnHelpInfo(HELPINFO* /*lpHelpInfo*/)
{
	AfxGetApp()->WinHelp(m_btnHelp.GetHelpID());
	return TRUE;
}

int CTDLToolsUserInputDlg::GetArgumentListData(const CMDLINEARG& arg, CStringArray& aItems) const
{
	if (arg.sRelatedSwitch.IsEmpty())
		return 0;

	CString sSwitch(arg.sRelatedSwitch), sValue;
	
	if (Misc::Split(sSwitch, sValue, ' ') && (sSwitch == SWITCH_TASKCUSTOMATTRIB))
	{
		const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
		GET_DEF_RET(m_aCustAttribDefs, sValue, pDef, 0);

		if (!pDef->IsList())
			return 0;

		// else
		aItems.Copy(pDef->aAutoListData);
		Misc::AddUniqueItems(pDef->aDefaultListData, aItems);
	}
	else // built-in attribute
	{
		ASSERT(sValue.IsEmpty());

		if (sSwitch == SWITCH_TASKALLOCBY)
		{
			aItems.Copy(m_tdlListData.aAllocBy);
		}
		else if (sSwitch == SWITCH_TASKALLOCTO)
		{
			aItems.Copy(m_tdlListData.aAllocTo);
		}
		else if (sSwitch == SWITCH_TASKCATEGORY)
		{
			aItems.Copy(m_tdlListData.aCategory);
		}
		else if (sSwitch == SWITCH_TASKSTATUS)
		{
			aItems.Copy(m_tdlListData.aStatus);
		}
		else if (sSwitch == SWITCH_TASKTAGS)
		{
			aItems.Copy(m_tdlListData.aTags);
		}
		else if (sSwitch == SWITCH_TASKVERSION)
		{
			aItems.Copy(m_tdlListData.aVersion);
		}
	}

	return aItems.GetSize();
}
