// TDLCustomAttributeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCustomAttributeDlg.h"
#include "TDCCustomAttributeHelper.h"
#include "TDLTaskIconDlg.h"

#include "..\shared\EnString.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\enfiledialog.h"
#include "..\shared\holdredraw.h"
#include "..\shared\misc.h"
#include "..\shared\preferences.h"
#include "..\shared\localizer.h"
#include "..\shared\themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct TDLCAD_TYPE
{
	UINT nIDName;
	DWORD dwType;
};

static TDLCAD_TYPE DATA_TYPES[] = 
{
	{ IDS_CAD_STRING, TDCCA_STRING },
	{ IDS_CAD_INT, TDCCA_INTEGER },
	{ IDS_CAD_FLOAT, TDCCA_DOUBLE },
	{ IDS_CAD_DATE, TDCCA_DATE },
	{ IDS_CAD_BOOL, TDCCA_BOOL },
	{ IDS_CAD_ICON, TDCCA_ICON },
};
const int NUM_DATATYPES = sizeof(DATA_TYPES) / sizeof(TDLCAD_TYPE);

static TDLCAD_TYPE LIST_TYPES[] = 
{
	{ IDS_CAD_NOTLIST, TDCCA_NOTALIST },
	{ IDS_CAD_AUTOLIST, TDCCA_AUTOLIST },
	{ IDS_CAD_FIXEDLIST, TDCCA_FIXEDLIST },
	{ IDS_CAD_AUTOMULTILIST, TDCCA_AUTOMULTILIST },
	{ IDS_CAD_FIXEDMULTILIST, TDCCA_FIXEDMULTILIST },
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
// CTDLCustomAttributeDlg dialog

CTDLCustomAttributeDlg::CTDLCustomAttributeDlg(const CToDoCtrl& tdc, const CUIThemeFile& theme, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLCustomAttributeDlg::IDD, pParent), 
	m_eTaskfile(FES_NOBROWSE), 
	m_theme(theme),
	m_eUniqueID(_T(". \r\n\t"), ME_EXCLUDE),
	m_btInsertSymbol(1, 0, (MBS_DOWN | MBS_RETURNCMD)),
	m_sTaskFile(tdc.GetFilePath()),
	m_ilCheck(tdc.GetCheckImageList()),
	m_ilIcons(tdc.GetTaskIconImageList())
{
	//{{AFX_DATA_INIT(CTDLCustomAttributeDlg)
	m_sUniqueID = _T("");
	//}}AFX_DATA_INIT
	m_sColumnTitle = _T("");
	m_dwDataType = TDCCA_STRING;
	m_dwListType = TDCCA_NOTALIST;
	m_sDefaultListData = _T("");
	m_dwFeatures = TDCCAF_SORT;
	m_nAlignment = DT_LEFT;

	tdc.GetCustomAttributeDefs(m_aAttrib);

#ifdef _UNICODE
	m_eColumnTitle.AddButton(1, 0x2211, CEnString(IDS_SYMBOLS), CALC_BTNWIDTH);
	m_eColumnTitle.SetDropMenuButton(1);
#endif
}

void CTDLCustomAttributeDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLCustomAttributeDlg)
	DDX_Control(pDX, IDC_FEATURES, m_cbFeatures);
	DDX_Control(pDX, IDC_UNIQUEID, m_eUniqueID);
	DDX_Control(pDX, IDC_LISTTYPE, m_cbListType);
	DDX_Control(pDX, IDC_DATATYPE, m_cbDataType);
	DDX_Control(pDX, IDC_ALIGNMENT, m_cbAlign);
	DDX_Control(pDX, IDC_ATTRIBUTELIST, m_lcAttributes);
	DDX_Text(pDX, IDC_TASKFILE, m_sTaskFile);
	DDX_Text(pDX, IDC_COLUMNTITLE, m_sColumnTitle);
	DDX_Text(pDX, IDC_DEFAULTLISTDATA, m_sDefaultListData);
	DDX_CBIndex(pDX, IDC_ALIGNMENT, m_nAlignment);
	DDX_Text(pDX, IDC_UNIQUEID, m_sUniqueID);
	DDX_Control(pDX, IDC_INSERTSYMBOL, m_btInsertSymbol);
	DDX_Control(pDX, IDC_BROWSEIMAGES, m_btBrowseImages);
	DDX_Control(pDX, IDC_COLUMNTITLE, m_eColumnTitle);
	DDX_Control(pDX, IDC_DEFAULTLISTDATA, m_eListData);
	DDX_Control(pDX, IDC_TASKFILE, m_eTaskfile);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nSel = m_cbDataType.GetCurSel();
		m_dwDataType = (nSel == -1) ? TDCCA_STRING : m_cbDataType.GetItemData(nSel);

		nSel = m_cbListType.GetCurSel();
		m_dwListType = (nSel == -1) ? TDCCA_NOTALIST : m_cbListType.GetItemData(nSel);

		m_dwFeatures = m_cbFeatures.GetSelectedFeatures();
	}
	else
	{
		CDialogHelper::SelectItemByData(m_cbDataType, m_dwDataType);
		CDialogHelper::SelectItemByData(m_cbListType, m_dwListType);
	}
}


BEGIN_MESSAGE_MAP(CTDLCustomAttributeDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLCustomAttributeDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ATTRIBUTELIST, OnItemchangedAttriblist)
	ON_CBN_SELCHANGE(IDC_DATATYPE, OnSelchangeDatatype)
	ON_CBN_SELCHANGE(IDC_ALIGNMENT, OnSelchangeAlignment)
	ON_CBN_SELCHANGE(IDC_LISTTYPE, OnSelchangeListtype)
	ON_EN_CHANGE(IDC_COLUMNTITLE, OnChangeColumntitle)
	ON_EN_CHANGE(IDC_DEFAULTLISTDATA, OnChangeDefaultlistdata)
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
	ON_BN_CLICKED(IDC_BROWSEIMAGES, OnBrowseimages)
	ON_BN_CLICKED(IDC_INSERTSYMBOL, OnInsertsymbol)
	ON_CBN_CLOSEUP(IDC_FEATURES, OnChangeFeatures)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CUSTATTRIB_EDIT, OnEditAttribute)
	ON_UPDATE_COMMAND_UI(ID_CUSTATTRIB_EDIT, OnUpdateEditAttribute)
	ON_REGISTERED_MESSAGE(WM_EE_BTNCLICK, OnEEClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCustomAttributeDlg message handlers

BOOL CTDLCustomAttributeDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	// disable localization because we do it ourselves by using CEnString
	CLocalizer::EnableTranslation(m_cbListType, FALSE);
	CLocalizer::EnableTranslation(m_cbDataType, FALSE);
	CLocalizer::EnableTranslation(ListView_GetHeader(m_lcAttributes), FALSE);

	VERIFY(InitializeToolbar());

	BuildDataTypeCombo();
	
	// build list ctrl from attribute list
	CRect rList;
	m_lcAttributes.GetClientRect(rList);

	m_lcAttributes.InsertColumn(COL_ATTRIBLABEL, CEnString(IDS_CAD_COLATTRIBLABEL), LVCFMT_LEFT, 125);
	m_lcAttributes.InsertColumn(COL_DATATYPE, CEnString(IDS_CAD_COLDATATYPE), LVCFMT_LEFT, 125);
	m_lcAttributes.InsertColumn(COL_COLUMNTITLE, CEnString(IDS_CAD_COLCOLTITLE), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_ALIGNMENT, CEnString(IDS_CAD_COLALIGNMENT), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_FEATURES, CEnString(IDS_CAD_COLFEATURES), LVCFMT_LEFT, 75);
	m_lcAttributes.InsertColumn(COL_LISTTYPE, CEnString(IDS_CAD_COLLISTTYPE), LVCFMT_LEFT, rList.Width() - 475);

	m_lcAttributes.SetExtendedStyle(m_lcAttributes.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	
	for (int nAttrib = 0; nAttrib < m_aAttrib.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nAttrib];
		VERIFY (AddAttributeToListCtrl(attrib, FALSE) >= 0);
	}

	m_lcAttributes.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	OnItemchangedAttriblist(NULL, NULL);

	ListView_SetImageList(m_lcAttributes, m_ilCheck, LVSIL_SMALL);
	ListView_SetExtendedListViewStyle(m_lcAttributes, LVS_EX_FULLROWSELECT);

	CThemed::SetWindowTheme(&m_lcAttributes, _T("Explorer"));

	// initialize buttons
	m_btBrowseImages.SetIcon(AfxGetApp()->LoadIcon(IDI_CUST_ATTRIB_ICONS));
	m_btBrowseImages.SetTooltip(CEnString(IDS_CAD_BROWSEIMAGES));

	m_btInsertSymbol.SetWindowText(0x2211);
	m_btInsertSymbol.SetTooltip(CEnString(IDS_CAD_INSERTSYMBOL));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLCustomAttributeDlg::InitializeToolbar()
{
	if (m_toolbar.GetSafeHwnd())
		return TRUE;

	if (!m_toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP))
		return FALSE;

	if (!m_toolbar.LoadToolBar(IDR_CUSTATTRIB_TOOLBAR))
		return FALSE;

	// toolbar images
	if (m_theme.HasToolbarImageFile(_T("CUSTOM_ATTRIB")))
	{
		COLORREF crMask = CLR_NONE;
		CString sImagePath = m_theme.GetToolbarImageFile(_T("CUSTOM_ATTRIB"), crMask);

		VERIFY(m_toolbar.SetImage(sImagePath, crMask));
	}
	else 
		m_toolbar.SetImage(IDB_CUSTATTRIB_TOOLBAR_STD, RGB(255, 0, 255));
	
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

int CTDLCustomAttributeDlg::GetAttributes(CTDCCustomAttribDefinitionArray& aAttrib) const
{
	aAttrib.Copy(m_aAttrib);
	return aAttrib.GetSize();
}

void CTDLCustomAttributeDlg::OnOK()
{
	// check for duplicate unique IDs
	for (int nAttrib = 0; nAttrib < m_aAttrib.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nAttrib];

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

void CTDLCustomAttributeDlg::BuildListTypeCombo(DWORD dwDataType)
{
	m_cbListType.ResetContent();

	int nNumList = NUM_LISTTYPES;

	for (int nList = 0; nList < nNumList; nList++)
	{
		DWORD dwListType = LIST_TYPES[nList].dwType;

		switch (dwDataType)
		{
		case TDCCA_DATE:
		case TDCCA_BOOL:
			if (dwListType != TDCCA_NOTALIST)
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
		}

		int nIndex = m_cbListType.AddString(CEnString(LIST_TYPES[nList].nIDName));
		m_cbListType.SetItemData(nIndex, LIST_TYPES[nList].dwType);
	}

	// restore selection
	CDialogHelper::SelectItemByData(m_cbListType, m_dwListType);
}

void CTDLCustomAttributeDlg::OnItemchangedAttriblist(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];

		m_sColumnTitle = attrib.sColumnTitle;
		m_dwFeatures = attrib.dwFeatures;
		m_nAlignment = attrib.nTextAlignment;
		m_dwDataType = attrib.GetDataType();
		m_dwListType = attrib.GetListType();
		m_sDefaultListData = Misc::FormatArray(attrib.aDefaultListData, _T("\r\n"));

		// unique ID is special
		m_sUniqueID = attrib.sUniqueID;
		m_sUniqueID.MakeLower();

		BuildListTypeCombo(m_dwDataType);

		m_cbFeatures.SetAttributeDefintion(attrib);
	}
	else
	{
		m_sColumnTitle.Empty();
		m_dwFeatures = TDCCAF_SORT;
		m_nAlignment = DT_LEFT;
		m_dwDataType = TDCCA_STRING;
		m_dwListType = TDCCA_NOTALIST;
		m_sDefaultListData.Empty();
		m_sUniqueID.Empty();
		m_cbFeatures.ResetContent();
	}

	EnableControls();
	UpdateListDataMask();
	UpdateData(FALSE);
}

void CTDLCustomAttributeDlg::EnableControls()
{
	int nSel = GetCurSel();

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
	}
	else
	{
		m_eUniqueID.EnableWindow(FALSE);
	}
	
	// certain data types cannot be lists
	BOOL bEnableList = (nSel >= 0);
	BOOL bEnableListData = bEnableList;

	if (bEnableList)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
		
		DWORD dwDataType = attrib.GetDataType();
		DWORD dwListType = attrib.GetListType();

		switch (dwDataType)
		{
		case TDCCA_DATE:
		case TDCCA_BOOL:
			bEnableList = bEnableListData = FALSE;
			break;

		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_ICON:
			bEnableListData = (dwListType != TDCCA_NOTALIST);
			break;
		}

		GetDlgItem(IDC_BROWSEIMAGES)->EnableWindow(bEnableListData && (dwDataType == TDCCA_ICON));
	}

	GetDlgItem(IDC_LISTTYPE)->EnableWindow(bEnableList);
	GetDlgItem(IDC_DEFAULTLISTDATA)->EnableWindow(bEnableListData);
	GetDlgItem(IDC_INSERTSYMBOL)->EnableWindow(bEnableListData);
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
	int nSel = GetCurSel();

	// update data type
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
	attrib.SetDataType(m_dwDataType);

	CString sDataType, sUnused;
	GetTypeStrings(attrib, sDataType, sUnused);

	m_lcAttributes.SetItemText(nSel, COL_DATATYPE, sDataType);

	// update feature combo
	m_cbFeatures.SetAttributeDefintion(attrib);

	// Update alignment if it changed
	if (m_nAlignment != (int)attrib.nTextAlignment)
	{
		m_nAlignment = (int)attrib.nTextAlignment;
		UpdateData(FALSE);

		m_lcAttributes.SetItemText(nSel, COL_ALIGNMENT, CEnString(ALIGNMENT[m_nAlignment]));
	}

	// update list type in case it has changed
	m_dwListType = attrib.GetListType();
	
	BuildListTypeCombo(m_dwDataType);
	UpdateListDataMask();

	EnableControls();
}

void CTDLCustomAttributeDlg::UpdateListDataMask()
{
	switch (m_dwDataType)
	{
	case TDCCA_STRING:
	case TDCCA_DATE:
	case TDCCA_BOOL:
	case TDCCA_ICON:
		m_eListData.SetMask(_T("")); // clear mask
		break;

	case TDCCA_INTEGER:
		m_eListData.SetMask(_T("0123456789"));
		break;
			
	case TDCCA_DOUBLE:
		m_eListData.SetMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
		break;
	}
}

void CTDLCustomAttributeDlg::OnSelchangeAlignment() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
	attrib.nTextAlignment = m_nAlignment;

	// and list
	m_lcAttributes.SetItemText(nSel, COL_ALIGNMENT, CEnString(ALIGNMENT[m_nAlignment]));
}

void CTDLCustomAttributeDlg::OnSelchangeListtype() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
	attrib.SetListType(m_dwListType);

	// update list type in case it has changed
	m_dwListType = attrib.GetListType();

	EnableControls();

	// and list
	CString sDummy, sListType;
	GetTypeStrings(attrib, sDummy, sListType);

	m_lcAttributes.SetItemText(nSel, COL_LISTTYPE, sListType);

	// set list data mask
	UpdateListDataMask();
}

void CTDLCustomAttributeDlg::OnChangeColumntitle() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
	attrib.sColumnTitle = m_sColumnTitle;

	// and list
	m_lcAttributes.SetItemText(nSel, COL_COLUMNTITLE, m_sColumnTitle);
}

void CTDLCustomAttributeDlg::OnChangeDefaultlistdata() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];

	Misc::Split(m_sDefaultListData, attrib.aDefaultListData, '\n');
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
	return (m_aAttrib.Find(sID, nIgnore) != -1);
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
		
		if (bCancelled)
			sItem = m_lcAttributes.GetItemText(nSel, 0);

		// disallow empty strings
		else if (sItem.IsEmpty())
		{
			*pResult = FALSE;
			return;
		}
		
		TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
		
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
}

void CTDLCustomAttributeDlg::OnChangeUniqueid() 
{
	UpdateData();
	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
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
							CEnString(IDS_TDLFILEOPENFILTER), 
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

				m_aAttrib.Add(attribDef);
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
			TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[pNMIA->iItem];
			
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
					TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];
					
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
	if (BuildSymbolPopupMenu(menu))
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

void CTDLCustomAttributeDlg::OnInsertsymbol() 
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

BOOL CTDLCustomAttributeDlg::BuildSymbolPopupMenu(CMenu& menu) const
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

void CTDLCustomAttributeDlg::OnNewAttribute() 
{
	// add to attrib array
	TDCCUSTOMATTRIBUTEDEFINITION attrib;
	attrib.sLabel = CEnString(IDS_CAD_NEWATTRIB);
	attrib.dwFeatures = TDCCAF_SORT;

	m_aAttrib.Add(attrib);
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
	pCmdUI->Enable(m_lcAttributes.GetItemCount() < 128);
}

void CTDLCustomAttributeDlg::OnDeleteAttribute() 
{
	int nSel = GetCurSel();

	if (nSel >= 0)
	{
		m_lcAttributes.DeleteItem(nSel);
		m_aAttrib.RemoveAt(nSel);

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
	TDCCUSTOMATTRIBUTEDEFINITION attrib = m_aAttrib[nRow];

	// delete attrib
	m_aAttrib.RemoveAt(nRow);
	m_lcAttributes.DeleteItem(nRow);

	// reinsert attribute
	nRow += nRows;

	m_aAttrib.InsertAt(nRow, attrib);
	nRow = AddAttributeToListCtrl(attrib, FALSE, nRow);

	// restore selection
	m_lcAttributes.SetItemState(nRow, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CTDLCustomAttributeDlg::OnBrowseimages() 
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
	CTDLTaskIconDlg dialog(m_ilIcons, aImages);

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

void CTDLCustomAttributeDlg::OnChangeFeatures() 
{
	UpdateData();

	int nSel = GetCurSel();

	// update attribute
	TDCCUSTOMATTRIBUTEDEFINITION& attrib = m_aAttrib[nSel];

	if (attrib.dwFeatures != m_dwFeatures)
	{
		attrib.dwFeatures = m_dwFeatures;
		m_lcAttributes.SetItemText(nSel, COL_FEATURES, FormatFeatureList(m_dwFeatures));
	}
}
