// TDLMultiSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLMultiSortDlg.h"
#include "tdcstatic.h"

#include "..\shared\enstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLMultiSortDlg dialog

CTDLMultiSortDlg::CTDLMultiSortDlg(const TDSORTCOLUMNS& sort, const CTDCColumnIDMap& mapVisibleColumns, 
								   const CTDCCustomAttribDefinitionArray& aAttribDefs, CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLMultiSortDlg::IDD, pParent), 
		m_sort(sort),
		m_mapVisibleColumns(mapVisibleColumns),
		m_aAttribDefs(aAttribDefs)
{
	//{{AFX_DATA_INIT(CTDLMultiSortDlg)
	//}}AFX_DATA_INIT
}


void CTDLMultiSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLMultiSortDlg)
	DDX_Control(pDX, IDC_SORTBY3, m_cbSortBy3);
	DDX_Control(pDX, IDC_SORTBY2, m_cbSortBy2);
	DDX_Control(pDX, IDC_SORTBY1, m_cbSortBy1);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ASCENDING1, m_sort.col1.bAscending);
	DDX_Check(pDX, IDC_ASCENDING2, m_sort.col2.bAscending);
	DDX_Check(pDX, IDC_ASCENDING3, m_sort.col3.bAscending);
}


BEGIN_MESSAGE_MAP(CTDLMultiSortDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLMultiSortDlg)
	ON_CBN_SELCHANGE(IDC_SORTBY1, OnSelchangeSortby1)
	ON_CBN_SELCHANGE(IDC_SORTBY2, OnSelchangeSortby2)
	ON_CBN_SELCHANGE(IDC_SORTBY3, OnSelchangeSortby3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLMultiSortDlg message handlers

BOOL CTDLMultiSortDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	BuildCombos();
	EnableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLMultiSortDlg::BuildCombos()
{
	CComboBox* COMBOS[3] = 
	{ 
		&m_cbSortBy1, 
		&m_cbSortBy2, 
		&m_cbSortBy3 
	};

	const TDC_COLUMN* SORTBY[3] = 
	{ 
		&m_sort.col1.nBy, 
		&m_sort.col2.nBy, 
		&m_sort.col3.nBy 
	};

	int nCol;

	// regular columns
	for (nCol = 0; nCol < NUM_COLUMNS; nCol++)
	{
		const TDCCOLUMN& col = COLUMNS[nCol];
		TDC_COLUMN nColID = col.nColID;

		if (IsColumnVisible(nColID))
		{
			CEnString sColumn(col.nIDLongName);
			
			for (int nSort = 0; nSort < 3; nSort++)
			{
				int nIndex = COMBOS[nSort]->AddString(sColumn);
				COMBOS[nSort]->SetItemData(nIndex, nColID);
				
				if (*(SORTBY[nSort]) == nColID)
					COMBOS[nSort]->SetCurSel(nIndex);
			}
		}
	}

	// custom columns
	for (nCol = 0; nCol < m_aAttribDefs.GetSize(); nCol++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs.GetData()[nCol];

		if (attribDef.bEnabled && attribDef.SupportsFeature(TDCCAF_SORT))
		{
			TDC_COLUMN nColID = attribDef.GetColumnID();
			CEnString sColumn(IDS_CUSTOMCOLUMN, attribDef.sLabel);

			for (int nSort = 0; nSort < 3; nSort++)
			{
				int nIndex = COMBOS[nSort]->AddString(sColumn);
				COMBOS[nSort]->SetItemData(nIndex, nColID);
				
				if (*(SORTBY[nSort]) == nColID)
					COMBOS[nSort]->SetCurSel(nIndex);
			}
		}
	}

	// add blank item at top of 2nd and 3rd combo
	for (int nSort = 1; nSort < 3; nSort++)
	{
		int nIndex = COMBOS[nSort]->InsertString(0, _T(""));
		COMBOS[nSort]->SetItemData(nIndex, TDCC_NONE);
		
		if (*(SORTBY[nSort]) == TDCC_NONE)
			COMBOS[nSort]->SetCurSel(nIndex);
	}

	// set selection to first item if first combo selection is not set
	if (m_cbSortBy1.GetCurSel() == -1)
	{
		m_cbSortBy1.SetCurSel(0);
		m_sort.col1.nBy = (TDC_COLUMN)m_cbSortBy1.GetItemData(0);
	}
}

BOOL CTDLMultiSortDlg::IsColumnVisible(TDC_COLUMN col) const
{
	// special cases:
	if (col == TDCC_CLIENT)
		return TRUE;

	else if (col == TDCC_NONE)
		return FALSE;

	// else test column
	return m_mapVisibleColumns.HasColumn(col);
}

void CTDLMultiSortDlg::OnSelchangeSortby1() 
{
	UpdateData();
	
	int nSel = m_cbSortBy1.GetCurSel();
	m_sort.col1.nBy = (TDC_COLUMN)m_cbSortBy1.GetItemData(nSel);

	EnableControls();
}

void CTDLMultiSortDlg::OnSelchangeSortby2() 
{
	UpdateData();
		
	int nSel = m_cbSortBy2.GetCurSel();
	m_sort.col2.nBy = (TDC_COLUMN)m_cbSortBy2.GetItemData(nSel);

	EnableControls();
}

void CTDLMultiSortDlg::OnSelchangeSortby3() 
{
	UpdateData();
		
	int nSel = m_cbSortBy3.GetCurSel();
	m_sort.col3.nBy = (TDC_COLUMN)m_cbSortBy3.GetItemData(nSel);

	EnableControls();
}

void CTDLMultiSortDlg::GetSortBy(TDSORTCOLUMNS& sort) const
{
	sort.col1.nBy = m_sort.col1.nBy;
	sort.col1.bAscending = m_sort.col1.bAscending;

	sort.col2.nBy = m_sort.col2.nBy;

	// if nBy2 is not set then make sure nBy3 isn't set
	if (sort.col2.IsSorting())
	{
		sort.col2.bAscending = m_sort.col2.bAscending;

		sort.col3.nBy = m_sort.col3.nBy;
		sort.col3.bAscending = m_sort.col3.bAscending;
	}
	else
	{
		sort.col3.nBy = TDCC_NONE;
	}
}

void CTDLMultiSortDlg::EnableControls()
{
	BOOL bSort1 = m_sort.col1.IsSorting();
	BOOL bSort2 = (bSort1 && m_sort.col2.IsSorting());
	BOOL bSort3 = (bSort2 && m_sort.col3.IsSorting());

	// disable 3rd combo if 2nd combo not set
	m_cbSortBy2.EnableWindow(bSort1);
	m_cbSortBy3.EnableWindow(bSort2);
	
	GetDlgItem(IDC_ASCENDING1)->EnableWindow(bSort1);
	GetDlgItem(IDC_ASCENDING2)->EnableWindow(bSort2);
	GetDlgItem(IDC_ASCENDING3)->EnableWindow(bSort3);
}