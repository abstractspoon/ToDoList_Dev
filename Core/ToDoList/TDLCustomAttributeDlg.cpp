// TDLCustomAttributeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCustomAttributeDlg.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\EnString.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\holdredraw.h"
#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"

#include "..\3rdparty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct TDLCAD_TYPE
{
	UINT nIDName;
	DWORD dwType;
};

static TDLCAD_TYPE DATA_TYPES[] = 
{
	{ IDS_CAD_STRING,		TDCCA_STRING },
	{ IDS_CAD_INT,			TDCCA_INTEGER },
	{ IDS_CAD_FLOAT,		TDCCA_DOUBLE },
	{ IDS_CAD_FRACTION,		TDCCA_FRACTION },
	{ IDS_CAD_DATE,			TDCCA_DATE },
	{ IDS_CAD_BOOL,			TDCCA_BOOL },
	{ IDS_CAD_ICON,			TDCCA_ICON },
	{ IDS_CAD_FILELINK,		TDCCA_FILELINK },
	{ IDS_CAD_TIMEPERIOD,	TDCCA_TIMEPERIOD },
	{ IDS_CAD_CALCULATION,	TDCCA_CALCULATION },
};
const int NUM_DATATYPES = sizeof(DATA_TYPES) / sizeof(TDLCAD_TYPE);

static CString GetDataTypeLabel(DWORD dwDataType)
{
	int nType = NUM_DATATYPES;

	while (nType--)
	{
		if (DATA_TYPES[nType].dwType == dwDataType)
			return CEnString(DATA_TYPES[nType].nIDName);
	}

	ASSERT(0);
	return _T("");
}

static TDLCAD_TYPE LIST_TYPES[] = 
{
	{ IDS_CAD_NOTLIST,			TDCCA_NOTALIST },
	{ IDS_CAD_AUTOLIST,			TDCCA_AUTOLIST },
	{ IDS_CAD_FIXEDLIST,		TDCCA_FIXEDLIST },
	{ IDS_CAD_AUTOMULTILIST,	TDCCA_AUTOMULTILIST },
	{ IDS_CAD_FIXEDMULTILIST,	TDCCA_FIXEDMULTILIST },
};

const int NUM_LISTTYPES = sizeof(LIST_TYPES) / sizeof(TDLCAD_TYPE);

enum COL_TYPE
{
	COL_ATTRIBLABEL, 
	COL_DATATYPE, 
	COL_COLUMNTITLE,
	COL_ALIGNMENT,
	COL_FEATURES, 
	COL_LISTTYPE, 
};

static UINT ALIGNMENT[3] = 
{
	IDS_CAD_LEFTALIGN,
	IDS_CAD_CENTREALIGN,
	IDS_CAD_RIGHTALIGN
};

static TCHAR SYMBOLS[] = 
{
	0x00a7,	0x00a9,	0x00ab,	0x00ae,	0x00b0,
	0x00b1,	0x00b5,	0x00bb,	0x2030,	0x20a0,
	0x20a1,	0x20a2,	0x20a3,	0x20a4,	0x20a5,
	0x20a6,	0x20a7,	0x20a8,	0x20a9,	0x20aa,
	0x20ab,	0x20ac,	0x20ad,	0x20ae,	0x20af,
	0x2105,	0x2113,	0x2116,	0x2122,	0x2126,
	0x2206,	0x2211,	0x221a,	0x221e,	0x222b,
	0x2248,	0x2264,	0x2265,	0x25a1,	0x25aa,
	0x25ca,	0x25cf,	0x25e6,	0x266f,
};
const UINT NUM_SYMBOLS = sizeof(SYMBOLS) / sizeof(TCHAR);

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#	define LVS_EX_DOUBLEBUFFER (0x00010000)
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributePageBase dialog

BOOL CCustomAttributePageBase::Create(UINT nDialogResID, CWnd* pParent)
{
	if (!CDialog::Create(nDialogResID, pParent))
		return FALSE;

	CRect rHost = CDialogHelper::GetCtrlRect(pParent, IDC_PAGEHOST);
	MoveWindow(rHost);

	// Fixup our Z order
	CWnd* pHost = pParent->GetDlgItem(IDC_PAGEHOST);
	SetWindowPos(pHost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributeListPage dialog

CCustomAttributeListPage::CCustomAttributeListPage(const CTDCImageList& ilTaskIcons)
	:
	m_btInsertSymbol(1, 0, (MBS_DOWN | MBS_RETURNCMD)),
	m_ilTaskIcons(ilTaskIcons)
{
	//{{AFX_DATA_INIT(CCustomAttributeListPage)
	//}}AFX_DATA_INIT
	m_dwListType = TDCCA_NOTALIST;
	m_dwDataType = TDCCA_STRING;
}

void CCustomAttributeListPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomAttributeListPage)
	DDX_Control(pDX, IDC_LISTTYPE, m_cbListType);
	DDX_Text(pDX, IDC_DEFAULTLISTDATA, m_sDefaultListData);
	DDX_Control(pDX, IDC_INSERTSYMBOL, m_btInsertSymbol);
	DDX_Control(pDX, IDC_BROWSEIMAGES, m_btBrowseImages);
	DDX_Control(pDX, IDC_DEFAULTLISTDATA, m_eListData);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nSel = m_cbListType.GetCurSel();
		m_dwListType = (nSel == -1) ? TDCCA_NOTALIST : m_cbListType.GetItemData(nSel);
	}
	else
	{
		CDialogHelper::SelectItemByData(m_cbListType, m_dwListType);
	}
}


BEGIN_MESSAGE_MAP(CCustomAttributeListPage, CDialog)
	//{{AFX_MSG_MAP(CCustomAttributeListPage)
	ON_CBN_SELCHANGE(IDC_LISTTYPE, OnSelchangeListtype)
	ON_EN_CHANGE(IDC_DEFAULTLISTDATA, OnChangeDefaultlistdata)
	ON_BN_CLICKED(IDC_BROWSEIMAGES, OnBrowseimages)
	ON_BN_CLICKED(IDC_INSERTSYMBOL, OnInsertsymbol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg message handlers

BOOL CCustomAttributeListPage::Create(CWnd* pParent)
{
	return CCustomAttributePageBase::Create(IDD_CUSTOMATTRIBLIST_PAGE, pParent);
}

BOOL CCustomAttributeListPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// disable localization because we do it ourselves by using CEnString
	CLocalizer::EnableTranslation(m_cbListType, FALSE);

	BuildListCombo();
	UpdateListDataMask();
	EnableControls();

	// initialize buttons
	m_btBrowseImages.SetIcon(AfxGetApp()->LoadIcon(IDI_CUST_ATTRIB_ICONS));
	m_btBrowseImages.SetTooltip(CEnString(IDS_CAD_BROWSEIMAGES));

	m_btInsertSymbol.SetWindowText(0x2211);
	m_btInsertSymbol.SetTooltip(CEnString(IDS_CAD_INSERTSYMBOL));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomAttributeListPage::BuildListCombo()
{
	m_cbListType.ResetContent();

	int nNumList = NUM_LISTTYPES;

	for (int nList = 0; nList < nNumList; nList++)
	{
		DWORD dwListType = LIST_TYPES[nList].dwType;

		switch (m_dwDataType)
		{
		case TDCCA_DATE:
		case TDCCA_BOOL:
		case TDCCA_TIMEPERIOD:
		case TDCCA_CALCULATION:
			if (dwListType != TDCCA_NOTALIST)
			{
				continue;
			}
			break;

		case TDCCA_FILELINK:
			if ((dwListType != TDCCA_NOTALIST) &&
				(dwListType != TDCCA_AUTOLIST))
			{
				continue;
			}
			break;

		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
			// all list types accepted
			break;

		case TDCCA_ICON:
			// fixed list type accepted only
			if ((dwListType != TDCCA_NOTALIST) &&
				(dwListType != TDCCA_FIXEDLIST) &&
				(dwListType != TDCCA_FIXEDMULTILIST))
			{
				continue;
			}
			break;

		case TDCCA_FRACTION:
			// single selection list type accepted only
			if ((dwListType != TDCCA_NOTALIST) &&
				(dwListType != TDCCA_FIXEDLIST) &&
				(dwListType != TDCCA_AUTOLIST))
			{
				continue;
			}
			break;

		default:
			ASSERT(0);
			break;
		}

		int nIndex = m_cbListType.AddString(CEnString(LIST_TYPES[nList].nIDName));
		m_cbListType.SetItemData(nIndex, LIST_TYPES[nList].dwType);
	}

	// restore selection
	if (CDialogHelper::SelectItemByData(m_cbListType, m_dwListType) == CB_ERR)
		SetListType(TDCCA_NOTALIST);
}

BOOL CCustomAttributeListPage::SetDataType(DWORD dwDataType)
{
	if (dwDataType != m_dwDataType)
	{
		m_dwDataType = dwDataType;

		BuildListCombo();
		UpdateListDataMask();
		EnableControls();

		return TRUE;
	}

	return FALSE;
}

BOOL CCustomAttributeListPage::SetListType(DWORD dwListType)
{
	if (CDialogHelper::SelectItemByData(m_cbListType, dwListType) != CB_ERR)
	{
		m_dwListType = dwListType;

		if (dwListType == TDCCA_NOTALIST)
		{
			m_sDefaultListData.Empty();
			UpdateData(FALSE);
		}

		UpdateListDataMask();
		EnableControls();

		return TRUE;
	}

	return FALSE;
}

void CCustomAttributeListPage::SetDefaultListData(const CStringArray& aData)
{
	m_sDefaultListData = Misc::FormatArray(aData, _T("\r\n"));
	UpdateData(FALSE);
}

int CCustomAttributeListPage::GetDefaultListData(CStringArray& aData) const
{
	return Misc::Split(m_sDefaultListData, aData, _T("\r\n"));
}

void CCustomAttributeListPage::EnableControls()
{
	BOOL bEnableList = TRUE;
	BOOL bEnableListData = bEnableList;
	BOOL bEnableIconBtn = FALSE;

	switch (m_dwDataType)
	{
	case TDCCA_DATE:
	case TDCCA_BOOL:
	case TDCCA_TIMEPERIOD:
	case TDCCA_CALCULATION:
		bEnableList = bEnableListData = FALSE;
		break;

	case TDCCA_STRING:
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_FILELINK:
	case TDCCA_FRACTION:
		bEnableListData = (m_dwListType != TDCCA_NOTALIST);
		break;

	case TDCCA_ICON:
		bEnableListData = bEnableIconBtn = (m_dwListType != TDCCA_NOTALIST);
		break;

	default:
		ASSERT(0);
		break;
	}

	GetDlgItem(IDC_LISTTYPE)->EnableWindow(bEnableList);
	GetDlgItem(IDC_DEFAULTLISTDATA)->EnableWindow(bEnableListData);
	GetDlgItem(IDC_INSERTSYMBOL)->EnableWindow(bEnableListData);
	GetDlgItem(IDC_BROWSEIMAGES)->EnableWindow(bEnableIconBtn);
}

void CCustomAttributeListPage::UpdateListDataMask()
{
	switch (m_dwDataType)
	{
	case TDCCA_STRING:
	case TDCCA_DATE:
	case TDCCA_BOOL:
	case TDCCA_ICON:
	case TDCCA_FILELINK:
	case TDCCA_TIMEPERIOD:
	case TDCCA_CALCULATION:
		m_eListData.ClearMask();
		break;

	case TDCCA_INTEGER:
		m_eListData.SetMask(_T("-0123456789"));
		break;

	case TDCCA_FRACTION:
		m_eListData.SetMask(_T("-0123456789/"));
		break;

	case TDCCA_DOUBLE:
		m_eListData.SetMask(_T("-.0123456789"), ME_LOCALIZEDECIMAL);
		break;

	default:
		ASSERT(0);
		break;
	}
}

void CCustomAttributeListPage::OnSelchangeListtype()
{
	UpdateData();
	UpdateListDataMask();
	EnableControls();

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBLISTCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeListPage::OnChangeDefaultlistdata()
{
	UpdateData();

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBLISTCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeListPage::OnInsertsymbol()
{
	CMenu menu;

	// build our unicode 'symbol' menu
	if (BuildSymbolPopupMenu(menu))
	{
		UINT nID = m_btInsertSymbol.TrackPopupMenu(&menu);

		if (nID > 0)
		{
			TCHAR szItem[2] = { SYMBOLS[nID - 1], 0 };
			m_eListData.ReplaceSel(szItem, TRUE);
		}
	}
}

BOOL CCustomAttributeListPage::BuildSymbolPopupMenu(CMenu& menu)
{
	ASSERT(menu.GetSafeHmenu() == NULL);

	if (menu.GetSafeHmenu() != NULL)
		return FALSE;

	if (menu.CreatePopupMenu())
	{
		TCHAR szItem[2] = { 0 };

		for (int nSymbol = 0; nSymbol < NUM_SYMBOLS; nSymbol++)
		{
			szItem[0] = SYMBOLS[nSymbol];

			UINT nFlags = MF_STRING;

			// insert vertical separator every 10 symbols
			if (nSymbol && (nSymbol % 10) == 0)
				nFlags |= MF_MENUBREAK;

			menu.AppendMenu(nFlags, nSymbol + 1, szItem);
		}

		return TRUE;
	}

	return FALSE;
}

void CCustomAttributeListPage::OnBrowseimages()
{
	// extract icon names from list data
	CString sImage, sName;
	CStringArray aList, aImages;
	CMapStringToString mapImages;

	if (Misc::Split(m_sDefaultListData, aList, '\n'))
	{
		for (int nItem = 0; nItem < aList.GetSize(); nItem++)
		{
			const CString& sTag = aList[nItem];

			if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(sTag, sImage, sName))
			{
				mapImages[sImage] = sName;
				aImages.Add(sImage);
			}
		}
	}

	// show dialog and rebuild list
	CTDLTaskIconDlg dialog(m_ilTaskIcons, aImages/*, (CWnd*)&m_tdc*/);

	if (dialog.DoModal() == IDOK)
	{
		if (dialog.GetIconNames(aImages))
		{
			m_sDefaultListData.Empty();

			for (int nImg = 0; nImg < aImages.GetSize(); nImg++)
			{
				sImage = aImages[nImg];

				// if we already had this image use it, else new item
				sName = dialog.GetUserIconName(sImage);

				if (sName.IsEmpty())
					mapImages.Lookup(sImage, sName);

				m_sDefaultListData += TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(sImage, sName);
				m_sDefaultListData += _T("\r\n");
			}

			UpdateData(FALSE);

			// sync data
			OnChangeDefaultlistdata();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomAttributeCalcPage dialog

CCustomAttributeCalcPage::CCustomAttributeCalcPage() 
	:
	m_eSecondOperandValue(_T("-.0123456789"), ME_LOCALIZEDECIMAL),
	m_cbFirstOperand(FALSE), // don't want relative tasks
	m_cbSecondOperandAttrib(FALSE), // don't want relative tasks
	m_bSecondOperandIsValue(TRUE)
{
	//{{AFX_DATA_INIT(CCustomAttributeCalcPage)
	//}}AFX_DATA_INIT
}

void CCustomAttributeCalcPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomAttributeCalcPage)
	DDX_Control(pDX, IDC_FIRSTOPERAND, m_cbFirstOperand);
	DDX_Control(pDX, IDC_OPERATOR, m_cbOperators);
	DDX_Control(pDX, IDC_SECONDOPERANDATTRIBUTE, m_cbSecondOperandAttrib);
	DDX_Control(pDX, IDC_SECONDOPERANDVALUE, m_eSecondOperandValue);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_SECONDOPISATTRIBUTE, m_bSecondOperandIsValue);
	DDX_Text(pDX, IDC_RESULTTYPE, m_sResultType);

	DDX_Operand(pDX, m_cbFirstOperand, m_calc.opFirst.nAttribID, m_calc.opFirst.sCustAttribID);

	if (m_bSecondOperandIsValue)
		DDX_Text(pDX, IDC_SECONDOPERANDVALUE, m_calc.dSecondOperandValue);
	else
		DDX_Operand(pDX, m_cbSecondOperandAttrib, m_calc.opSecond.nAttribID, m_calc.opSecond.sCustAttribID);

	// Update calculation arguments
	if (pDX->m_bSaveAndValidate)
		m_calc.nOperator = CDialogHelper::GetSelectedItemData(m_cbOperators, TDCCAC_ADD);
	else
		CDialogHelper::SelectItemByData(m_cbOperators, m_calc.nOperator);
}

void CCustomAttributeCalcPage::DDX_Operand(CDataExchange* pDX, CTDLAttributeComboBox& cbOperand, TDC_ATTRIBUTE& nAttribID, CString& sCustAttribID)
{
	cbOperand.DDX(pDX, nAttribID, sCustAttribID);

	if (pDX->m_bSaveAndValidate)
	{
		if (!sCustAttribID.IsEmpty())
			nAttribID = TDCA_CUSTOMATTRIB;
	}
}

BEGIN_MESSAGE_MAP(CCustomAttributeCalcPage, CDialog)
	//{{AFX_MSG_MAP(CCustomAttributeCalcPage)
	ON_CBN_SELCHANGE(IDC_FIRSTOPERAND, OnSelChangeFirstOperand)
	ON_CBN_SELCHANGE(IDC_OPERATOR, OnSelChangeOperator)
	ON_CBN_SELCHANGE(IDC_SECONDOPERANDATTRIBUTE, OnSelChangeSecondOperandAttribute)
	ON_EN_CHANGE(IDC_SECONDOPERANDVALUE, OnChangeSecondOperandValue)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SECONDOPISATTRIBUTE, OnChangeSecondOperandType)
	ON_BN_CLICKED(IDC_SECONDOPISVALUE, OnChangeSecondOperandType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg message handlers

BOOL CCustomAttributeCalcPage::Create(CWnd* pParent)
{
	return CCustomAttributePageBase::Create(IDD_CUSTOMATTRIBCALC_PAGE, pParent);
}

BOOL CCustomAttributeCalcPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// disable localization because we do it ourselves by using CEnString
	CLocalizer::EnableTranslation(m_cbFirstOperand, FALSE);
	CLocalizer::EnableTranslation(m_cbOperators, FALSE);
	CLocalizer::EnableTranslation(m_cbSecondOperandAttrib, FALSE);

	BuildFirstOperandCombo();
	BuildOperatorCombo();
	BuildSecondOperandCombo();
	EnableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomAttributeCalcPage::SetAttributeDefinitions(const CTDCCustomAttribDefinitionArray& aAttribDef)
{
	m_aAttribDef.Copy(aAttribDef); // needed in OnInitDialog
	m_cbFirstOperand.SetCustomAttributes(m_aAttribDef);
	m_cbSecondOperandAttrib.SetCustomAttributes(m_aAttribDef);

	if (GetSafeHwnd())
	{
		UpdateData();

		BuildOperatorCombo();
		BuildSecondOperandCombo();
		EnableControls();
	}
}

void CCustomAttributeCalcPage::SetCalculation(const TDCCUSTOMATTRIBUTECALCULATION& calc)
{
	m_calc = calc;
	m_bSecondOperandIsValue = calc.IsSecondOperandValue();

	BuildOperatorCombo();
	BuildSecondOperandCombo();
	UpdateResultType();

	UpdateData(FALSE);
	EnableControls();
}

void CCustomAttributeCalcPage::GetCalculation(TDCCUSTOMATTRIBUTECALCULATION& calc) const
{
	calc = m_calc;
}

BOOL CCustomAttributeCalcPage::IsDate(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_CREATIONDATE:
	case TDCA_DONEDATE:
	case TDCA_DUEDATE:
	case TDCA_LASTMODDATE:
	case TDCA_STARTDATE:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			int nDef = m_aAttribDef.Find(nAttrib);
			ASSERT(nDef >= 0);

			if (nDef >= 0)
				return (m_aAttribDef[nDef].IsDataType(TDCCA_DATE));
		}
		break;
	}

	return FALSE;
}

BOOL CCustomAttributeCalcPage::IsTimePeriod(TDC_ATTRIBUTE nAttrib) const
{
	switch (nAttrib)
	{
	case TDCA_TIMEESTIMATE:
	case TDCA_TIMESPENT:
		return TRUE;

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
		{
			int nDef = m_aAttribDef.Find(nAttrib);
			ASSERT(nDef >= 0);

			if (nDef >= 0)
				return (m_aAttribDef[nDef].IsDataType(TDCCA_TIMEPERIOD));
		}
		break;
	}

	return FALSE;
}

BOOL CCustomAttributeCalcPage::IsDate(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const
{
	return (m_aAttribDef.GetCalculationOperandDataType(op) == TDCCA_DATE);
}

BOOL CCustomAttributeCalcPage::IsTimePeriod(const TDCCUSTOMATTRIBUTECALCULATIONOPERAND& op) const
{
	return (m_aAttribDef.GetCalculationOperandDataType(op) == TDCCA_TIMEPERIOD);
}

int CCustomAttributeCalcPage::BuildFirstOperandFilter(CTDCAttributeMap& mapAttrib) const
{
	mapAttrib.RemoveAll();

	mapAttrib.Add(TDCA_COST);
	mapAttrib.Add(TDCA_CREATIONDATE);
	mapAttrib.Add(TDCA_DONEDATE);
	mapAttrib.Add(TDCA_DUEDATE);
	mapAttrib.Add(TDCA_LASTMODDATE);
	mapAttrib.Add(TDCA_PERCENT);
	mapAttrib.Add(TDCA_PRIORITY);
	mapAttrib.Add(TDCA_RISK);
	mapAttrib.Add(TDCA_STARTDATE);
	mapAttrib.Add(TDCA_TIMEESTIMATE);
	mapAttrib.Add(TDCA_TIMESPENT);

	for (int nDef = 0; nDef < m_aAttribDef.GetSize(); nDef++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDef[nDef];

		switch (attribDef.GetDataType())
		{
		case TDCCA_DATE:
		case TDCCA_DOUBLE:
		case TDCCA_TIMEPERIOD:
		case TDCCA_FRACTION:
		case TDCCA_INTEGER:
		case TDCCA_CALCULATION:
			mapAttrib.Add(attribDef.GetAttributeID());
			break;
		}
	}

	return mapAttrib.GetCount();
}

void CCustomAttributeCalcPage::BuildFirstOperandCombo()
{
	// Build attribute filter
	CTDCAttributeMap mapAttrib;
	BuildFirstOperandFilter(mapAttrib);

	m_cbFirstOperand.SetAttributeFilter(mapAttrib);

	// Restore selection
	if (m_calc.IsFirstOperandCustom())
		m_cbFirstOperand.SetSelectedAttribute(m_calc.opFirst.sCustAttribID);
	else
		m_cbFirstOperand.SetSelectedAttribute(m_calc.opFirst.nAttribID);
}

void CCustomAttributeCalcPage::BuildOperatorCombo()
{
	m_cbOperators.ResetContent();

	// Add/subtract supported by all
	CDialogHelper::AddString(m_cbOperators, CEnString(IDS_CAD_CALC_ADD), TDCCAC_ADD);
	CDialogHelper::AddString(m_cbOperators, CEnString(IDS_CAD_CALC_SUBTRACT), TDCCAC_SUBTRACT);

	// Multiply/divide NOT supported by DATES
	if (!IsDate(m_calc.opFirst.nAttribID))
	{
		CDialogHelper::AddString(m_cbOperators, CEnString(IDS_CAD_CALC_MULTIPLY), TDCCAC_MULTIPLY);
		CDialogHelper::AddString(m_cbOperators, CEnString(IDS_CAD_CALC_DIVIDE), TDCCAC_DIVIDE);
	}

	// restore selection
	if (CDialogHelper::SelectItemByData(m_cbOperators, m_calc.nOperator) == CB_ERR)
	{
		m_calc.nOperator = TDCCAC_ADD;
		UpdateData(FALSE);
	}
}

void CCustomAttributeCalcPage::BuildSecondOperandCombo()
{
	// Build attribute filter
	CTDCAttributeMap mapAttrib;
	BuildFirstOperandFilter(mapAttrib);

	// Remove invalid combinations
	//
	// First Operand  | Op  | Second Operand    | Valid
	// --------------------------------------------------
	// Numeric        | Any | Numeric           | Y
	// Numeric        | Any | Date              | N
	// Numeric        | Any | Time Period       | N
	//                |     |                   |
	// Date           | Any | Time Period       | Y
	// Date           | Any | Numeric           | Y (units are 'days')
	// Date           | +   | Date              | N
	// Date           | -   | Date              | Y
	//                |     |                   |
	// Time Period    | Any | Numeric           | Y (units are those of time period)
	// Time Period    | + - | Date              | N
	// Time Period    | + - | Time Period       | Y
	//
	BOOL bFirstIsDate = IsDate(m_calc.opFirst);
	BOOL bFirstIsTime = IsTimePeriod(m_calc.opFirst);

	POSITION pos = mapAttrib.GetStartPosition();

	while (pos)
	{
		TDC_ATTRIBUTE nSecondAttrib = mapAttrib.GetNext(pos);
		BOOL bDelete = (nSecondAttrib == m_calc.opFirst.nAttribID);

		if (!bDelete)
		{
			if (bFirstIsTime)
			{
				bDelete = IsDate(nSecondAttrib);
			}
			else if (bFirstIsDate)
			{
				bDelete = ((m_calc.nOperator == TDCCAC_ADD) && IsDate(nSecondAttrib));
			}
			else // numeric
			{
				bDelete = IsDate(nSecondAttrib) || IsTimePeriod(nSecondAttrib);
			}
		}

		if (bDelete)
			mapAttrib.Remove(nSecondAttrib);
	}

	m_cbSecondOperandAttrib.SetAttributeFilter(mapAttrib);
	ASSERT(m_cbSecondOperandAttrib.GetCount());

	// Restore selection
	if (m_calc.IsSecondOperandCustom())
		m_cbSecondOperandAttrib.SetSelectedAttribute(m_calc.opSecond.sCustAttribID);
	else
		m_cbSecondOperandAttrib.SetSelectedAttribute(m_calc.opSecond.nAttribID);
}

void CCustomAttributeCalcPage::EnableControls()
{
	BOOL bHasFirstOp = (m_calc.opFirst.nAttribID != TDCA_NONE);

	GetDlgItem(IDC_SECONDOPISATTRIBUTE)->EnableWindow(bHasFirstOp);
	GetDlgItem(IDC_SECONDOPISVALUE)->EnableWindow(bHasFirstOp);
	GetDlgItem(IDC_SECONDOPERANDATTRIBUTE)->EnableWindow(bHasFirstOp && !m_bSecondOperandIsValue);
	GetDlgItem(IDC_SECONDOPERANDVALUE)->EnableWindow(bHasFirstOp && m_bSecondOperandIsValue);
}

void CCustomAttributeCalcPage::UpdateResultType()
{
	if (!m_calc.IsValid(FALSE))
		m_sResultType.Empty();
	else
		m_sResultType = GetDataTypeLabel(m_aAttribDef.GetCalculationResultDataType(m_calc));

	UpdateData(FALSE);
}

void CCustomAttributeCalcPage::OnSelChangeFirstOperand()
{
	UpdateData();

	BuildOperatorCombo();
	BuildSecondOperandCombo();
	UpdateResultType();
	EnableControls();
	
	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBCALCCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeCalcPage::OnSelChangeOperator()
{
	UpdateData();
	BuildSecondOperandCombo();
	UpdateResultType();
	EnableControls();

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBCALCCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeCalcPage::OnSelChangeSecondOperandAttribute()
{
	UpdateData();
	UpdateResultType();

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBCALCCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeCalcPage::OnChangeSecondOperandValue()
{
	UpdateData();

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBCALCCHANGE, 0, (LPARAM)GetSafeHwnd());
}

void CCustomAttributeCalcPage::OnChangeSecondOperandType()
{
	UpdateData();
	UpdateResultType();
	EnableControls();

	if (m_bSecondOperandIsValue)
	{
		m_calc.opSecond.nAttribID = TDCA_NONE;
		m_calc.opSecond.sCustAttribID.Empty();
	}

	// Notify parent
	GetParent()->SendMessage(WM_CUSTATTRIBCALCCHANGE, 0, (LPARAM)GetSafeHwnd());
}

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg dialog

CTDLCustomAttributeDlg::CTDLCustomAttributeDlg(const CString& sTaskFile,
											   const CTDCCustomAttribDefinitionArray& aAttribDef,
											   const CTDCImageList& ilTaskIcons,
											   const CImageList& ilCheckBoxes, 
											   CWnd* pParent)
	: 
	CTDLDialog(CTDLCustomAttributeDlg::IDD, _T("CustomAttributes"), pParent), 
	m_eTaskfile(FES_NOBROWSE), 
	m_eUniqueID(_T(". \r\n\t"), ME_EXCLUDE),
	m_sTaskFile(sTaskFile),
	m_pageList(ilTaskIcons),
	m_ilCheckBoxes(ilCheckBoxes),
	m_aAttribDef(aAttribDef)
{
	//{{AFX_DATA_INIT(CTDLCustomAttributeDlg)
	m_sUniqueID = _T("");
	//}}AFX_DATA_INIT
	m_sColumnTitle = _T("");
	m_dwDataType = TDCCA_STRING;
	m_dwFeatures = TDCCAF_SORT;
	m_nAlignment = DT_LEFT;
	
#ifdef _UNICODE
	m_eColumnTitle.AddButton(1, 0x2211, CEnString(IDS_SYMBOLS), CALC_BTNWIDTH);
	m_eColumnTitle.SetDropMenuButton(1);
#endif

	m_pageCalc.SetAttributeDefinitions(aAttribDef);
}

void CTDLCustomAttributeDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLCustomAttributeDlg)
	DDX_Control(pDX, IDC_FEATURES, m_cbFeatures);
	DDX_Control(pDX, IDC_UNIQUEID, m_eUniqueID);
	DDX_Control(pDX, IDC_DATATYPE, m_cbDataType);
	DDX_Control(pDX, IDC_ALIGNMENT, m_cbAlign);
	DDX_Control(pDX, IDC_ATTRIBUTELIST, m_lcAttributes);
	DDX_Text(pDX, IDC_TASKFILE, m_sTaskFile);
	DDX_Text(pDX, IDC_COLUMNTITLE, m_sColumnTitle);
	DDX_CBIndex(pDX, IDC_ALIGNMENT, m_nAlignment);
	DDX_Text(pDX, IDC_UNIQUEID, m_sUniqueID);
	DDX_Control(pDX, IDC_COLUMNTITLE, m_eColumnTitle);
	DDX_Control(pDX, IDC_TASKFILE, m_eTaskfile);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nSel = m_cbDataType.GetCurSel();
		m_dwDataType = (nSel == -1) ? TDCCA_STRING : m_cbDataType.GetItemData(nSel);

		m_dwFeatures = m_cbFeatures.GetSelectedFeatures();
	}
	else
	{
		SelectItemByData(m_cbDataType, m_dwDataType);
	}
}


BEGIN_MESSAGE_MAP(CTDLCustomAttributeDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLCustomAttributeDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ATTRIBUTELIST, OnItemchangedAttriblist)
	ON_NOTIFY(NM_DBLCLK, IDC_ATTRIBUTELIST, OnDoubleClickItem)
	ON_CBN_SELCHANGE(IDC_DATATYPE, OnSelchangeDatatype)
	ON_CBN_SELCHANGE(IDC_ALIGNMENT, OnSelchangeAlignment)
	ON_EN_CHANGE(IDC_COLUMNTITLE, OnChangeColumntitle)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_ATTRIBUTELIST, OnEndlabeleditAttributelist)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_ATTRIBUTELIST, OnBeginlabeleditAttributelist)
	ON_EN_CHANGE(IDC_UNIQUEID, OnChangeUniqueid)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_NOTIFY(NM_CLICK, IDC_ATTRIBUTELIST, OnClickAttributelist)
	ON_COMMAND(ID_CUSTATTRIB_NEW, OnNewAttribute)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_NEW, OnUpdateNewAttribute)
	ON_COMMAND(ID_CUSTATTRIB_DELETE, OnDeleteAttribute)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_DELETE, OnUpdateDeleteAttribute)
	ON_COMMAND(ID_CUSTATTRIB_MOVEDOWN, OnMoveAttributeDown)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_MOVEDOWN, OnUpdateMoveAttributeDown)
	ON_COMMAND(ID_CUSTATTRIB_MOVEUP, OnMoveAttributeUp)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_MOVEUP, OnUpdateMoveAttributeUp)
	ON_CBN_CLOSEUP(IDC_FEATURES, OnChangeFeatures)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CUSTATTRIB_EDIT, OnEditAttribute)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_EDIT, OnUpdateEditAttribute)

	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEClick)
	ON_MESSAGE(WM_CUSTATTRIBLISTCHANGE, OnChangeListAttributes)
	ON_MESSAGE(WM_CUSTATTRIBCALCCHANGE, OnChangeCalculationAttributes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg message handlers

BOOL CTDLCustomAttributeDlg::OnInitDialog()
{
	CTDLDialog::OnInitDialog();

	// disable localization because we do it ourselves by using CEnString
	CLocalizer::EnableTranslation(m_cbDataType, FALSE);
	CLocalizer::EnableTranslation(ListView_GetHeader(m_lcAttributes), FALSE);

	VERIFY(m_pageList.Create(this));
	VERIFY(m_pageCalc.Create(this));

	VERIFY(InitializeToolbar());

	BuildDataTypeCombo();
	
	// build list ctrl from attribute list
	CRect rList;
	m_lcAttributes.GetClientRect(rList);
	m_lcAttributes.AllowOffItemClickDeselection(FALSE);
	m_lcAttributes.ModifyStyle(0, WS_CLIPCHILDREN);

	m_lcAttributes.InsertColumn(COL_ATTRIBLABEL, CEnString(IDS_CAD_COLATTRIBLABEL), LVCFMT_LEFT, 125);
	m_lcAttributes.InsertColumn(COL_DATATYPE, CEnString(IDS_CAD_COLDATATYPE), LVCFMT_LEFT, 125);
	m_lcAttributes.InsertColumn(COL_COLUMNTITLE, CEnString(IDS_CAD_COLCOLTITLE), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_ALIGNMENT, CEnString(IDS_CAD_COLALIGNMENT), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_FEATURES, CEnString(IDS_CAD_COLFEATURES), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_LISTTYPE, CEnString(IDS_CAD_COLLISTTYPE), LVCFMT_LEFT, rList.Width() - 475);

	ListView_SetExtendedListViewStyleEx(m_lcAttributes, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcAttributes, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	for (int nAttrib = 0; nAttrib < m_aAttribDef.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nAttrib];
		VERIFY (AddAttributeToListCtrl(attrib, FALSE) >= 0);
	}

	m_lcAttributes.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	OnItemchangedAttriblist(NULL, NULL);

	ListView_SetImageList(m_lcAttributes, m_ilCheckBoxes, LVSIL_SMALL);
	CThemed::SetWindowTheme(&m_lcAttributes, _T("Explorer"));

	m_mgrPrompts.SetComboPrompt(m_cbFeatures, IDS_TDC_NONE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLCustomAttributeDlg::InitializeToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;

	if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return FALSE;

	VERIFY(m_toolbar.LoadToolBar(IDR_CUSTATTRIB_TOOLBAR, IDB_CUSTATTRIB_TOOLBAR_STD, colorMagenta));
	VERIFY(m_tbHelper.Initialize(&m_toolbar, this));
	
	// very important - turn OFF all the auto positioning and sizing
	// by default have no borders
	UINT nStyle = m_toolbar.GetBarStyle();
	nStyle &= ~(CCS_NORESIZE | CCS_NOPARENTALIGN | CBRS_BORDER_ANY);
	nStyle |= (CBRS_SIZE_FIXED | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_toolbar.SetBarStyle(nStyle);
	
	CRect rToolbar;
	GetDlgItem(IDC_TOOLBAR)->GetWindowRect(rToolbar);
	ScreenToClient(rToolbar);
	m_toolbar.MoveWindow(rToolbar);

	return TRUE;
}

int CTDLCustomAttributeDlg::AddAttributeToListCtrl(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, BOOL bNew, int nPos)
{
	if (bNew || nPos == -1)
		nPos = m_lcAttributes.GetItemCount();

	int nIndex = m_lcAttributes.InsertItem(nPos, attrib.sLabel, attrib.bEnabled ? 2 : 1);

	m_lcAttributes.SetItemData(nIndex, bNew);
	m_lcAttributes.SetItemText(nIndex, COL_COLUMNTITLE, attrib.sColumnTitle);
	
	// interpret attrib.dwDataType
	CString sDataType, sListType;
	GetTypeStrings(attrib, sDataType, sListType);

	m_lcAttributes.SetItemText(nIndex, COL_DATATYPE, sDataType);
	m_lcAttributes.SetItemText(nIndex, COL_LISTTYPE, sListType);
	m_lcAttributes.SetItemText(nIndex, COL_ALIGNMENT, CEnString(ALIGNMENT[attrib.nTextAlignment]));
	m_lcAttributes.SetItemText(nIndex, COL_FEATURES, FormatFeatureList(attrib.dwFeatures));
	
	return nIndex;
}

CString CTDLCustomAttributeDlg::FormatFeatureList(DWORD dwFeatures)
{
	return CTDLCustomAttribFeatureComboBox::FormatFeatureList(dwFeatures);
}

int CTDLCustomAttributeDlg::GetAttributeDefinitions(CTDCCustomAttribDefinitionArray& aAttribDef) const
{
	aAttribDef.Copy(m_aAttribDef);
	return aAttribDef.GetSize();
}

void CTDLCustomAttributeDlg::OnOK()
{
	// check for duplicate unique IDs
	for (int nAttrib = 0; nAttrib < m_aAttribDef.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nAttrib];

		if (UniqueIDExists(attrib.sUniqueID, nAttrib))
		{
			CString sID(attrib.sUniqueID);
			sID.MakeLower();

			AfxMessageBox(CEnString(IDS_MULTIPLEUNIQUEIDSEXIST, sID), MB_OK | MB_ICONERROR);
			return;
		}
	}
	
	// all good
	CTDLDialog::OnOK();
}

void CTDLCustomAttributeDlg::GetTypeStrings(const TDCCUSTOMATTRIBUTEDEFINITION& attrib, CString& sDataType, CString& sListType)
{
	DWORD dwDataType = attrib.GetDataType();
	int nData = NUM_DATATYPES;

	while (nData--)
	{
		if (DATA_TYPES[nData].dwType == dwDataType)
		{
			sDataType = CEnString(DATA_TYPES[nData].nIDName);
			break;
		}
	}

	DWORD dwListType = attrib.GetListType();
	int nList = NUM_LISTTYPES;

	while (nList--)
	{
		if (LIST_TYPES[nList].dwType == dwListType)
		{
			sListType = CEnString(LIST_TYPES[nList].nIDName);
			break;
		}
	}
}

void CTDLCustomAttributeDlg::BuildDataTypeCombo()
{
	int nNumData = NUM_DATATYPES;

	for (int nData = 0; nData < nNumData; nData++)
	{
		int nIndex = m_cbDataType.AddString(CEnString(DATA_TYPES[nData].nIDName));
		m_cbDataType.SetItemData(nIndex, DATA_TYPES[nData].dwType);
	}
}

void CTDLCustomAttributeDlg::OnDoubleClickItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint ptCursor(GetMessagePos());
	m_lcAttributes.ScreenToClient(&ptCursor);

	int nItem = m_lcAttributes.HitTest(ptCursor);

	if (nItem != -1)
		m_lcAttributes.EditLabel(nItem);
}

void CTDLCustomAttributeDlg::OnItemchangedAttriblist(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
	int nSel = GetCurSel();

	// Changing a single selection listctrl always cycles
	// through and 'no selection' state which causes flicker
	// so ignore it if there are items in the list
	if ((nSel == -1) && m_lcAttributes.GetItemCount())
		return;

	if (nSel >= 0)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];

		m_sColumnTitle = attrib.sColumnTitle;
		m_dwFeatures = attrib.dwFeatures;
		m_nAlignment = attrib.nTextAlignment;
		m_dwDataType = attrib.GetDataType();

		// unique ID is special
		m_sUniqueID = attrib.sUniqueID;
		m_sUniqueID.MakeLower();

		m_pageList.SetDataType(m_dwDataType);
		m_pageList.SetDefaultListData(attrib.aDefaultListData);
		m_pageCalc.SetCalculation(attrib.Calculation());

		if (attrib.IsDataType(TDCCA_CALCULATION))
		{
			DWORD dwDataType = m_aAttribDef.GetCalculationResultDataType(attrib.Calculation());
			m_cbFeatures.SetAttributeDefinition(dwDataType, TDCCA_NOTALIST, attrib.dwFeatures);
		}
		else
		{
			m_cbFeatures.SetAttributeDefinition(attrib);
		}
	}
	else
	{
		m_sColumnTitle.Empty();
		m_dwFeatures = TDCCAF_SORT;
		m_nAlignment = DT_LEFT;
		m_dwDataType = TDCCA_STRING;
		m_sUniqueID.Empty();

		m_pageList.SetListType(TDCCA_NOTALIST);
		m_pageCalc.SetCalculation(TDCCUSTOMATTRIBUTECALCULATION());

		m_cbFeatures.ResetContent();
	}

	EnableControls();
	UpdateData(FALSE);
}

void CTDLCustomAttributeDlg::EnableControls()
{
	int nSel = GetCurSel();
	BOOL bIsCalculation = FALSE;

	m_toolbar.RefreshButtonStates();

	GetDlgItem(IDC_COLUMNTITLE)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_DATATYPE)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_ALIGNMENT)->EnableWindow(nSel >= 0);
	GetDlgItem(IDC_FEATURES)->EnableWindow(nSel >= 0);

	// unique ID is special
	if (nSel >= 0)
	{
		m_eUniqueID.EnableWindow(TRUE);
		m_eUniqueID.SetReadOnly(m_lcAttributes.GetItemData(nSel) == 0);

		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
		bIsCalculation = (attrib.GetDataType() == TDCCA_CALCULATION);
	}
	else
	{
		m_eUniqueID.EnableWindow(FALSE);
	}
	
	m_pageList.EnableWindow(!bIsCalculation);
	m_pageList.ShowWindow(bIsCalculation ? SW_HIDE : SW_SHOW);

	m_pageCalc.EnableWindow(bIsCalculation);
	m_pageCalc.ShowWindow(bIsCalculation ? SW_SHOW : SW_HIDE);
}

int CTDLCustomAttributeDlg::GetCurSel()
{
	int nSel = -1;
	POSITION pos = m_lcAttributes.GetFirstSelectedItemPosition();

	if (pos)
		nSel = m_lcAttributes.GetNextSelectedItem(pos);

	return nSel;
}

void CTDLCustomAttributeDlg::OnSelchangeDatatype() 
{
	UpdateData();

	// update data type
	int nSel = GetCurSel();

	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
	attrib.SetDataType(m_dwDataType);

	CString sDataType, sUnused;
	GetTypeStrings(attrib, sDataType, sUnused);

	m_lcAttributes.SetItemText(nSel, COL_DATATYPE, sDataType);

	// update feature combo
	m_cbFeatures.SetAttributeDefinition(attrib);

	// And features in case they changed
	m_dwFeatures = m_cbFeatures.GetSelectedFeatures();

	if (attrib.dwFeatures != m_dwFeatures)
	{
		attrib.dwFeatures = m_dwFeatures;
		m_lcAttributes.SetItemText(nSel, COL_FEATURES, FormatFeatureList(m_dwFeatures));
	}
	
	// Update alignment if it changed
	if (m_nAlignment != (int)attrib.nTextAlignment)
	{
		m_nAlignment = (int)attrib.nTextAlignment;
		UpdateData(FALSE);

		m_lcAttributes.SetItemText(nSel, COL_ALIGNMENT, CEnString(ALIGNMENT[m_nAlignment]));
	}

	// update list type in case it has changed
	m_pageList.SetDataType(m_dwDataType);

	EnableControls();
}

void CTDLCustomAttributeDlg::OnSelchangeAlignment() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
	attrib.nTextAlignment = m_nAlignment;

	// and list
	m_lcAttributes.SetItemText(nSel, COL_ALIGNMENT, CEnString(ALIGNMENT[m_nAlignment]));
}

LRESULT CTDLCustomAttributeDlg::OnChangeListAttributes(WPARAM wp, LPARAM lp)
{
	ASSERT(lp == (LPARAM)m_pageList.GetSafeHwnd());

	// update attribute
	int nSel = GetCurSel();
	ASSERT(nSel >= 0);

	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];

	DWORD dwListType = m_pageList.GetListType();

	if (dwListType != attrib.GetListType())
	{
		BOOL bWasList = attrib.IsList();

		attrib.SetListType(dwListType);

		// If we've switched from non-list to list
		// then automatically add 'filterable'
		if (!bWasList && attrib.IsList())
			attrib.dwFeatures |= TDCCAF_FILTER;

		// update list type in case it has changed
		m_pageList.SetListType(attrib.GetListType());

		// update feature combo
		m_cbFeatures.SetAttributeDefinition(attrib);

		// And features in case they changed
		m_dwFeatures = m_cbFeatures.GetSelectedFeatures();
	
		if ((attrib.dwFeatures != m_dwFeatures) || (!bWasList && attrib.IsList()))
		{
			attrib.dwFeatures = m_dwFeatures;
			m_lcAttributes.SetItemText(nSel, COL_FEATURES, FormatFeatureList(m_dwFeatures));
		}

		// and list
		CString sDummy, sListType;
		GetTypeStrings(attrib, sDummy, sListType);

		m_lcAttributes.SetItemText(nSel, COL_LISTTYPE, sListType);

		EnableControls();
	}
	else
	{
		m_pageList.GetDefaultListData(attrib.aDefaultListData);
	}

	return 0L;
}

LRESULT CTDLCustomAttributeDlg::OnChangeCalculationAttributes(WPARAM wp, LPARAM lp)
{
	ASSERT(lp == (LPARAM)m_pageCalc.GetSafeHwnd());

	// update attribute
	TDCCUSTOMATTRIBUTECALCULATION calc;
	m_pageCalc.GetCalculation(calc);

	int nSel = GetCurSel();
	ASSERT(nSel >= 0);

	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
	attrib.SetCalculation(calc);

	// Update feature combo
	DWORD dwDataType = m_aAttribDef.GetCalculationResultDataType(attrib.Calculation());
	m_cbFeatures.SetAttributeDefinition(dwDataType, TDCCA_NOTALIST, attrib.dwFeatures);
	
	return 0L;
}

void CTDLCustomAttributeDlg::OnChangeColumntitle() 
{
	UpdateData();

	// update attribute
	int nSel = GetCurSel();
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];

	attrib.sColumnTitle = m_sColumnTitle;

	// and list
	m_lcAttributes.SetItemText(nSel, COL_COLUMNTITLE, m_sColumnTitle);
}

CString CTDLCustomAttributeDlg::MakeID(const CString& sLabel)
{
	CString sID = Misc::ToLower(_T("CUST_") + sLabel);

	// remove invalid chars
	sID.Remove(' ');
	sID.Remove('.');
	sID.Remove('\t');
	sID.Remove('\n');
	sID.Remove('\r');

	return sID;
}

void CTDLCustomAttributeDlg::MakeUniqueID(CString& sID, int nIgnore) const
{
	if (UniqueIDExists(sID))
	{
		CString sUniqueID = sID;
		int nTry = 1;

		// test for uniqueness
		while (UniqueIDExists(sUniqueID, nIgnore))
			sUniqueID.Format(_T("%s%d"), sID, nTry++);

		sID = sUniqueID;
	}
}

BOOL CTDLCustomAttributeDlg::UniqueIDExists(const CString& sID, int nIgnore) const
{
	return (m_aAttribDef.Find(sID, nIgnore) != -1);
}

void CTDLCustomAttributeDlg::OnBeginlabeleditAttributelist(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
	EnableControls();
}

void CTDLCustomAttributeDlg::OnEndlabeleditAttributelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	BOOL bCancelled = (pDispInfo->item.pszText == NULL);
	
	int nSel = pDispInfo->item.iItem;
	ASSERT(nSel != -1);

	if (nSel >= 0)
	{
		CString sItem(pDispInfo->item.pszText);
		Misc::Trim(sItem);
		
		if (bCancelled)
		{
			sItem = m_lcAttributes.GetItemText(nSel, 0);
		}
		else if (sItem.IsEmpty())
		{
			// disallow empty strings
			*pResult = FALSE;
			return;
		}
		
		TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
		
		// if this is the first edit then use the text to create a UNIQUE ID
		// for the attrib
		if (attrib.sUniqueID.IsEmpty())
		{
			CString sUniqueID = MakeID(sItem);
			MakeUniqueID(m_eUniqueID.Validate(sUniqueID), nSel);
			
			m_sUniqueID = attrib.sUniqueID = sUniqueID;		
		}
		
		// update attrib long name
		attrib.sLabel = sItem;
		*pResult = TRUE;
	}

	UpdateData(FALSE); // update unique ID field
	EnableControls();

	m_pageCalc.SetAttributeDefinitions(m_aAttribDef);

	// Strangely, it seems the act of auto-editing the label
	// interferes with the update of the toolbar button states
	// so we post a little prompt
	m_toolbar.RefreshButtonStates(FALSE);
}

void CTDLCustomAttributeDlg::OnChangeUniqueid() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
	attrib.sUniqueID = m_sUniqueID;
}

void CTDLCustomAttributeDlg::OnImport() 
{
	// browse to a tasklist from which to import attributes
	CPreferences prefs;
	CFileOpenDialog dialog(IDS_OPENTASKLIST_TITLE, 
							NULL, 
							NULL, 
							EOFN_DEFAULTOPEN, 
							CEnString(IDS_TDLFILEFILTER), 
							this);

	if (dialog.DoModal(prefs) == IDOK)
	{
		CTaskFile tasks;

		if (tasks.Load(dialog.GetPathName()))
		{
			CTDCCustomAttribDefinitionArray aAttribDefs;
			int nNumDefs = tasks.GetCustomAttributeDefs(aAttribDefs);
			int nNumImported = 0;

			for (int nDef = 0; nDef < nNumDefs; nDef++)
			{
				TDCCUSTOMATTRIBUTEDEFINITION attribDef = aAttribDefs[nDef];

				if (UniqueIDExists(attribDef.sUniqueID))
				{
					if (IDYES == AfxMessageBox(CEnString(IDS_IMPORTCUSTATTR_IDCOLLISION), MB_YESNO))
					{
						MakeUniqueID(attribDef.sUniqueID);
					}
					else 
						continue; // skip item
				}

				m_aAttribDef.Add(attribDef);
				AddAttributeToListCtrl(attribDef, FALSE);
				nNumImported++;
			}

			// error handling
			if (nNumDefs == 0)
				AfxMessageBox(CEnString(IDS_NOCUSTOMDEFS));

			else if (nNumImported == 0)
				AfxMessageBox(CEnString(IDS_NOCUSTOMDEFSIMPORTED));
		}
	}
}

void CTDLCustomAttributeDlg::OnClickAttributelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// if user clicked on the checkbox then toggle enabled state
	LPNMITEMACTIVATE pNMIA = (LPNMITEMACTIVATE)pNMHDR;

	if (pNMIA->iItem != -1 && pNMIA->iSubItem == 0) // validate item
	{
		CRect rCheck;
								
		if (m_lcAttributes.GetItemRect(pNMIA->iItem, rCheck, LVIR_ICON) &&
			rCheck.PtInRect(pNMIA->ptAction))
		{
			TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[pNMIA->iItem];
			
			attrib.bEnabled = !attrib.bEnabled;
			m_lcAttributes.SetItem(pNMIA->iItem, 0, LVIF_IMAGE, NULL, attrib.bEnabled ? 2 : 1, 0, 0, 0);
		}
	}

	*pResult = 0;
}

BOOL CTDLCustomAttributeDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nSel = GetCurSel();
		
		if (nSel != -1)
		{
			// if the focus is on the attrib list and the user hits the spacebar
			// then toggle the enabled state
			switch (pMsg->wParam)
			{
			case VK_SPACE:
				if (pMsg->hwnd == m_lcAttributes)
				{
					TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];
					
					attrib.bEnabled = !attrib.bEnabled;
					m_lcAttributes.SetItem(nSel, 0, LVIF_IMAGE, NULL, attrib.bEnabled ? 2 : 1, 0, 0, 0);
				}
				break;

			case VK_F2:
				m_lcAttributes.EditLabel(nSel);
				break;

			case VK_DELETE:
				if (pMsg->hwnd == m_lcAttributes)
					OnDeleteAttribute();
				break;

			case VK_UP:
				if (pMsg->hwnd == m_lcAttributes && Misc::ModKeysArePressed(MKS_CTRL))
				{
					OnMoveAttributeUp();
					return TRUE; // eat it
				}
				break;

			case VK_DOWN:
				if (pMsg->hwnd == m_lcAttributes && Misc::ModKeysArePressed(MKS_CTRL))
				{
					OnMoveAttributeDown();
					return TRUE; // eat it
				}
				break;
			}
		}
	}
	
	return CTDLDialog::PreTranslateMessage(pMsg);
}

LRESULT CTDLCustomAttributeDlg::OnEEClick(WPARAM /*wp*/, LPARAM lp)
{
	CMenu menu;

	// build our unicode 'symbol' menu
	if (CCustomAttributeListPage::BuildSymbolPopupMenu(menu))
	{
		UINT nID = m_eColumnTitle.TrackPopupMenu(lp, &menu, EETPM_RETURNCMD);

		if (nID > 0)
		{
			TCHAR szItem[2] = { SYMBOLS[nID - 1], 0 };
			m_eColumnTitle.ReplaceSel(szItem, TRUE);
		}
	}

	return 0L;
}

void CTDLCustomAttributeDlg::OnNewAttribute() 
{
	// add to attrib array
	TDCCUSTOMATTRIBUTEDEFINITION attrib(CEnString(IDS_CAD_NEWATTRIB));

	m_aAttribDef.Add(attrib);
	int nIndex = AddAttributeToListCtrl(attrib, TRUE);

	// select
	m_lcAttributes.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	OnItemchangedAttriblist(NULL, NULL);

	m_lcAttributes.SetFocus();
	m_lcAttributes.EditLabel(nIndex);

	EnableControls();
}

void CTDLCustomAttributeDlg::OnUpdateNewAttribute(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_lcAttributes.GetItemCount() < 64);
}

void CTDLCustomAttributeDlg::OnDeleteAttribute() 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		m_lcAttributes.DeleteItem(nSel);
		m_aAttribDef.RemoveAt(nSel);

		// Move selection to next attribute
		if (nSel == m_lcAttributes.GetItemCount())
			nSel--;
		
		if (nSel >= 0)
			m_lcAttributes.SetItemState(nSel, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));
		
		OnItemchangedAttriblist(NULL, NULL);
	}
}

void CTDLCustomAttributeDlg::OnUpdateDeleteAttribute(CCmdUI* pCmdUI) 
{
	int nSel = GetCurSel();
	pCmdUI->Enable(nSel != -1);
}

void CTDLCustomAttributeDlg::OnMoveAttributeDown() 
{
	MoveAttribute(1);
}

void CTDLCustomAttributeDlg::OnUpdateMoveAttributeDown(CCmdUI* pCmdUI) 
{
	int nSel = GetCurSel();
	pCmdUI->Enable(nSel >= 0 && nSel < (m_lcAttributes.GetItemCount() - 1) &&
					m_lcAttributes.GetEditControl() == NULL);
}

void CTDLCustomAttributeDlg::OnMoveAttributeUp() 
{
	MoveAttribute(-1);
}

void CTDLCustomAttributeDlg::OnUpdateMoveAttributeUp(CCmdUI* pCmdUI) 
{
	int nSel = GetCurSel();
	pCmdUI->Enable(nSel > 0 && m_lcAttributes.GetEditControl() == NULL);
}

void CTDLCustomAttributeDlg::OnEditAttribute() 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
		m_lcAttributes.EditLabel(nSel);
}

void CTDLCustomAttributeDlg::OnUpdateEditAttribute(CCmdUI* pCmdUI) 
{
	int nSel = GetCurSel();
	pCmdUI->Enable(nSel != -1);
}

void CTDLCustomAttributeDlg::MoveAttribute(int nRows)
{
	int nRow = GetCurSel();

	if (nRow == -1)
		return;

	if ((nRow + nRows) < 0 || (nRow + nRows) > (m_lcAttributes.GetItemCount() - 1))
		return;

	// save off attribute
	CHoldRedraw hr(m_lcAttributes, NCR_PAINT);
	TDCCUSTOMATTRIBUTEDEFINITION attrib = m_aAttribDef[nRow];

	// delete attrib
	m_aAttribDef.RemoveAt(nRow);
	m_lcAttributes.DeleteItem(nRow);

	// reinsert attribute
	nRow += nRows;

	m_aAttribDef.InsertAt(nRow, attrib);
	nRow = AddAttributeToListCtrl(attrib, FALSE, nRow);

	// restore selection
	m_lcAttributes.SetItemState(nRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CTDLCustomAttributeDlg::OnChangeFeatures() 
{
	UpdateData();

	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttribDef[nSel];

	if (attrib.dwFeatures != m_dwFeatures)
	{
		attrib.dwFeatures = m_dwFeatures;
		m_lcAttributes.SetItemText(nSel, COL_FEATURES, FormatFeatureList(m_dwFeatures));
	}
}
