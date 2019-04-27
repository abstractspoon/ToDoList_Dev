// TDLMultiSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLMultiSortDlg.h"
#include "tdcstatic.h"

#include "..\shared\enstring.h"
#include "..\shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static const UINT IDC_SORTBY[3] = { IDC_SORTBY1, IDC_SORTBY2, IDC_SORTBY3 };
static const UINT IDC_ASCENDING[3] = { IDC_ASCENDING1, IDC_ASCENDING2, IDC_ASCENDING3 };

/////////////////////////////////////////////////////////////////////////////
// CTDLMultiSortDlg dialog

CTDLMultiSortDlg::CTDLMultiSortDlg(const TDSORTCOLUMNS& sort, const CTDCColumnIDMap& mapVisibleColumns, 
								   const CTDCCustomAttribDefinitionArray& aAttribDefs, CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLMultiSortDlg::IDD, _T("MultiSort"), pParent), 
		m_sort(sort),
		m_mapVisibleColumns(mapVisibleColumns),
		m_aAttribDefs(aAttribDefs)
{
	//{{AFX_DATA_INIT(CTDLMultiSortDlg)
	//}}AFX_DATA_INIT

	for (int nSort = 0; nSort < 3; nSort++)
		m_bSortAscending[nSort] = sort.Cols()[nSort].bAscending;
}

void CTDLMultiSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLMultiSortDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SORTBY[0], m_cbSortBy[0]);
	DDX_Control(pDX, IDC_SORTBY[1], m_cbSortBy[1]);
	DDX_Control(pDX, IDC_SORTBY[2], m_cbSortBy[2]);
	DDX_Check(pDX, IDC_ASCENDING[0], m_bSortAscending[0]);
	DDX_Check(pDX, IDC_ASCENDING[1], m_bSortAscending[1]);
	DDX_Check(pDX, IDC_ASCENDING[2], m_bSortAscending[2]);
}


BEGIN_MESSAGE_MAP(CTDLMultiSortDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLMultiSortDlg)
	ON_CBN_SELCHANGE(IDC_SORTBY[0], OnSelchangeSortby1)
	ON_CBN_SELCHANGE(IDC_SORTBY[1], OnSelchangeSortby2)
	ON_CBN_SELCHANGE(IDC_SORTBY[2], OnSelchangeSortby3)
	ON_BN_CLICKED(IDC_ASCENDING[0], OnClickSortAscending1)
	ON_BN_CLICKED(IDC_ASCENDING[1], OnClickSortAscending2)
	ON_BN_CLICKED(IDC_ASCENDING[2], OnClickSortAscending3)
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
	for (int nSort = 0; nSort < 3; nSort++)
	{
		CComboBox& combo = m_cbSortBy[nSort];
		int nCol;

		// regular columns
		for (nCol = 0; nCol < NUM_COLUMNS; nCol++)
		{
			const TDCCOLUMN& col = COLUMNS[nCol];
			TDC_COLUMN nColID = col.nColID;

			if (!IsColumnVisible(nColID))
				continue;
				
			AddString(combo, col.nIDLongName, nColID);
		}
	
		// custom columns
		for (nCol = 0; nCol < m_aAttribDefs.GetSize(); nCol++)
		{
			const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = m_aAttribDefs.GetData()[nCol];
			TDC_COLUMN nColID = attribDef.GetColumnID();

			if (!IsColumnVisible(nColID))
				continue;

			ASSERT(attribDef.bEnabled && attribDef.SupportsFeature(TDCCAF_SORT));

			CEnString sColumn(IDS_CUSTOMCOLUMN, attribDef.sLabel);
			AddString(combo, sColumn, nColID);
		}

		// add blank item at top of 2nd and 3rd combo
		if (nSort > 0)
			AddString(combo, _T(""), TDCC_NONE);

		SelectItemByData(combo, m_sort.GetSortBy(nSort));
	}

	// set selection to first item if first combo selection is not set
	if (m_cbSortBy[0].GetCurSel() == -1)
	{
		m_cbSortBy[0].SetCurSel(0);

		TDC_COLUMN nColID = GetSelectedItemData(m_cbSortBy[0], TDCC_NONE);
		m_sort.SetSortBy(0, nColID);
	}
}

BOOL CTDLMultiSortDlg::IsColumnVisible(TDC_COLUMN col) const
{
	// special cases:
	if (col == TDCC_CLIENT)
	{
		return TRUE;
	}
	else if (col == TDCC_NONE)
	{
		return FALSE;
	}

	// else test column
	return m_mapVisibleColumns.Has(col);
}

void CTDLMultiSortDlg::OnSelchangeSortby1() 
{
	OnSelchangeSortby(0);
}

void CTDLMultiSortDlg::OnSelchangeSortby2() 
{
	OnSelchangeSortby(1);
}

void CTDLMultiSortDlg::OnSelchangeSortby3() 
{
	OnSelchangeSortby(2);
}

void CTDLMultiSortDlg::OnSelchangeSortby(int nCol)
{
	TDC_COLUMN nColID = GetSelectedItemData(m_cbSortBy[nCol], TDCC_NONE);
	m_sort.SetSortBy(nCol, nColID);

	EnableControls();
}

void CTDLMultiSortDlg::OnClickSortAscending1() 
{
	OnClickSortAscending(0);
}

void CTDLMultiSortDlg::OnClickSortAscending2() 
{
	OnClickSortAscending(1);
}

void CTDLMultiSortDlg::OnClickSortAscending3() 
{
	OnClickSortAscending(2);
}

void CTDLMultiSortDlg::OnClickSortAscending(int nCol)
{
	ASSERT((nCol >= 0) && (nCol < 3));

	UpdateData();

	m_sort.SetSortBy(nCol, m_sort.GetSortBy(nCol), m_bSortAscending[nCol]);
}

void CTDLMultiSortDlg::GetSortBy(TDSORTCOLUMNS& sort) const
{
	sort.SetSortBy(m_sort);
}

void CTDLMultiSortDlg::EnableControls()
{
	BOOL bPrevSort = TRUE;

	for (int nCol = 0; nCol < 3; nCol++)
	{
		// Combo's enable state is defined by whether the previous
		// column has a valid sort
		m_cbSortBy[nCol].EnableWindow(bPrevSort);

		// Checkbox's enable state is additionally defined by whether 
		// this column has a valid sort
		BOOL bSort = (bPrevSort & m_sort.IsSorting(nCol));

		GetDlgItem(IDC_ASCENDING[nCol])->EnableWindow(bSort);
		bPrevSort = bSort;
	}
}