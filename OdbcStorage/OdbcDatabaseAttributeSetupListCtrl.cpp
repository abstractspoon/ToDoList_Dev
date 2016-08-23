// OdbcStorageSetupListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OdbcDatabaseAttributeSetupListCtrl.h"

#include "..\Shared\Themed.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\graphicsmisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum 
{
	IDC_TABLE_COMBO = 10,
	IDC_FIELD_COMBO
};

enum 
{
	LISTCOL_TASKATTRIB,
	LISTCOL_TASKFIELD,
	LISTCOL_ATTRIBTABLE,
	LISTCOL_ATTRIBKEYFIELD,
	LISTCOL_ATTRIBCONTENTFIELD,
	LISTCOL_LINKTABLE,
	LISTCOL_LINKTASKFIELD,
	LISTCOL_LINKATTRIBFIELD,
	LISTCOL_LINKTASKLISTFIELD,

	NUM_LISTCOL
};

const UINT LISTCOL_NAMES[NUM_LISTCOL] = 
{
	IDS_TASKATTRIB_COL,
	IDS_TASKFIELD_COL,
	IDS_ATTRIBTABLE_COL,
	IDS_ATTRIBKEYFIELD_COL,
	IDS_ATTRIBCONTENTFIELD_COL,
	IDS_LINKTABLE_COL,
	IDS_LINKTASKFIELD_COL,
	IDS_LINKATTRIBFIELD_COL,
	IDS_LINKTASKLISTFIELD_COL,
};

const int COL_WIDTH = 90;

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseAttributeSetupListCtrl

COdbcDatabaseAttributeSetupListCtrl::COdbcDatabaseAttributeSetupListCtrl() 
	: 
	m_pDB(NULL)
{
}

COdbcDatabaseAttributeSetupListCtrl::~COdbcDatabaseAttributeSetupListCtrl()
{
}


BEGIN_MESSAGE_MAP(COdbcDatabaseAttributeSetupListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(COdbcDatabaseAttributeSetupListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_SELENDCANCEL(IDC_TABLE_COMBO, OnTableEditCancel)
	ON_CBN_SELENDOK(IDC_TABLE_COMBO, OnTableEditOK)
	ON_CBN_SELENDCANCEL(IDC_FIELD_COMBO, OnFieldEditCancel)
	ON_CBN_SELENDOK(IDC_FIELD_COMBO, OnFieldEditOK)
	ON_NOTIFY(NM_CUSTOMDRAW, 0, OnHeaderCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseAttributeSetupListCtrl message handlers

void COdbcDatabaseAttributeSetupListCtrl::OnHeaderCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCD = (NMCUSTOMDRAW*)pNMHDR;
	*pResult = CDRF_DODEFAULT;

	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult |= CDRF_NOTIFYITEMDRAW;	
		break;
	
	case CDDS_ITEMPREPAINT:
		{
			int nItem = (int)pNMCD->dwItemSpec;
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);

			CRect rItem(pNMCD->rc);
			rItem.OffsetRect(-1, 0);

			// draw header background
			CThemed th;

			if (th.AreControlsThemed() && th.Open(&m_header, _T("HEADER")))
			{
				th.DrawBackground(pDC, HP_HEADERITEM, HIS_NORMAL, rItem);
			}
			else
			{
				pDC->FillSolidRect(rItem, ::GetSysColor(COLOR_3DFACE));
				pDC->Draw3dRect(rItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
			}
	
			// text
			CString sItem = m_header.GetItemText(nItem);

			if (!sItem.IsEmpty())
			{
				rItem.DeflateRect(2, 2);

				pDC->SetBkMode(TRANSPARENT);
				pDC->DrawText(sItem, rItem, (DT_WORDBREAK | DT_CENTER | GraphicsMisc::GetRTLDrawTextFlags(*this)));
			}
		}
		*pResult = CDRF_SKIPDEFAULT;
		break;
	}
}

int COdbcDatabaseAttributeSetupListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildListColumns();
	
	return 0;
}

void COdbcDatabaseAttributeSetupListCtrl::PreSubclassWindow() 
{
	BuildListColumns();
	
	CInputListCtrl::PreSubclassWindow();
}

void COdbcDatabaseAttributeSetupListCtrl::BuildListColumns()
{
	// once only
	if (GetColumnCount())
		return;

	for (int nCol = 0; nCol < NUM_LISTCOL; nCol++)
	{
		InsertColumn(nCol, CEnString(LISTCOL_NAMES[nCol]), LVCFMT_LEFT, COL_WIDTH);

		if (nCol)
			SetColumnType(nCol, ILCT_DROPLIST);
	}

	// build the task attribute column
	for (int nAttrib = 0; nAttrib < NUM_ATTRIB; nAttrib++)
	{
		int nItem = InsertItem(nAttrib, CEnString(TASKATTRIB_NAMES[nAttrib].nIDName));

		SetItemData(nItem, TASKATTRIB_NAMES[nAttrib].nAttrib);
	}

	// create child controls
	CreateControl(m_cbTables, IDC_TABLE_COMBO);
	CreateControl(m_cbFields, IDC_FIELD_COMBO);

	if (GetHeader())
		GetHeader()->SetRowCount(2);
}

void COdbcDatabaseAttributeSetupListCtrl::InitState()
{
	CInputListCtrl::InitState();
	
	ShowGrid(TRUE, TRUE);
	DisableColumnEditing(0, TRUE);
}


BOOL COdbcDatabaseAttributeSetupListCtrl::Initialise(CDatabase& db, const CString& sTasksTable, const COdbcAttribSetupArray& aAttribSetup)
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd())
		return FALSE;

	ASSERT(!sTasksTable.IsEmpty());

	if (sTasksTable.IsEmpty())
		return FALSE;

	// else
	m_pDB = &db;
	m_sTaskTable = sTasksTable;

	BuildListColumns();
	InitializeCells(aAttribSetup);

	return TRUE;
}

void COdbcDatabaseAttributeSetupListCtrl::InitializeCells(const COdbcAttribSetupArray& aAttribSetup)
{
	if (GetSafeHwnd())
	{
		// fill-in cells
		int nNumAttrib = aAttribSetup.GetSize();

		for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
		{
			const ODBCATTRIBUTESETUP& attrib = aAttribSetup.GetData()[nAttrib];
			
			int nRow = FindRow(attrib.nAttrib);
			ASSERT(nRow != -1);

			if (nRow != -1)
			{
				SetItemText(nRow, LISTCOL_TASKFIELD, attrib.sTaskField);

				// attribute columns
				SetItemText(nRow, LISTCOL_ATTRIBTABLE, attrib.sAttribTable);

				if (!attrib.sAttribTable.IsEmpty())
				{
					SetItemText(nRow, LISTCOL_ATTRIBKEYFIELD, attrib.sAttribKeyField);
					SetItemText(nRow, LISTCOL_ATTRIBCONTENTFIELD, attrib.sAttribContentField);

					// link columns
					SetItemText(nRow, LISTCOL_LINKTABLE, attrib.sLinkTable);

					if (!attrib.sLinkTable.IsEmpty())
					{
						SetItemText(nRow, LISTCOL_LINKTASKFIELD, attrib.sLinkTaskKeyField);
						SetItemText(nRow, LISTCOL_LINKATTRIBFIELD, attrib.sLinkAttribKeyField);
						SetItemText(nRow, LISTCOL_LINKTASKLISTFIELD, attrib.sLinkTaskListKeyField);
					}
				}
			}
		}
	}
}

int COdbcDatabaseAttributeSetupListCtrl::FindRow(TDC_ATTRIBUTE nAttrib) const
{
	return FindItemFromData(nAttrib);
}

BOOL COdbcDatabaseAttributeSetupListCtrl::GetAttributeSetup(COdbcAttribSetupArray& aAttribSetup) const
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd())
		return FALSE;

	int nNumRows = GetItemCount();
	aAttribSetup.RemoveAll();
	
	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		// only save mapped attributes
		if (!GetItemText(nRow, LISTCOL_TASKFIELD).IsEmpty())
		{
			ODBCATTRIBUTESETUP attrib;
			
			attrib.nAttrib = (TDC_ATTRIBUTE)GetItemData(nRow);
			attrib.sTaskField = GetItemText(nRow, LISTCOL_TASKFIELD);

			// attrib columns
			attrib.sAttribTable			= GetItemText(nRow, LISTCOL_ATTRIBTABLE);

			if (!attrib.sAttribTable.IsEmpty())
			{
				attrib.sAttribKeyField		= GetItemText(nRow, LISTCOL_ATTRIBKEYFIELD);
				attrib.sAttribContentField	= GetItemText(nRow, LISTCOL_ATTRIBCONTENTFIELD);

				// link columns
				attrib.sLinkTable = GetItemText(nRow, LISTCOL_LINKTABLE);

				if (!attrib.sLinkTable.IsEmpty())
				{
					attrib.sLinkTaskKeyField		= GetItemText(nRow, LISTCOL_LINKTASKFIELD);
					attrib.sLinkAttribKeyField		= GetItemText(nRow, LISTCOL_LINKATTRIBFIELD);
					attrib.sLinkTaskListKeyField	= GetItemText(nRow, LISTCOL_LINKTASKLISTFIELD);
				}
			}
			
			aAttribSetup.Add(attrib);
		}
	}

	return TRUE;
}

int COdbcDatabaseAttributeSetupListCtrl::CalcItemHeight() const
{
	CRect rCombo;
	m_cbTables.GetWindowRect(rCombo);

	return rCombo.Height();
}

BOOL COdbcDatabaseAttributeSetupListCtrl::CanEditCell(int nItem, int nCol) const
{
	switch (nCol)
	{
	case LISTCOL_TASKATTRIB:
		return FALSE; // always

	case LISTCOL_TASKFIELD:
		return TRUE; // always
		
	case LISTCOL_ATTRIBTABLE:
		// task field must be set
		return !GetItemText(nItem, LISTCOL_TASKFIELD).IsEmpty();

	case LISTCOL_ATTRIBKEYFIELD:
	case LISTCOL_ATTRIBCONTENTFIELD:
	case LISTCOL_LINKTABLE:
		// attribute table must be set
		return !GetItemText(nItem, LISTCOL_ATTRIBTABLE).IsEmpty();
		
	case LISTCOL_LINKTASKFIELD:
	case LISTCOL_LINKATTRIBFIELD:
	case LISTCOL_LINKTASKLISTFIELD:
		// link table must be set
		return !GetItemText(nItem, LISTCOL_LINKTABLE).IsEmpty();
	}

	ASSERT(0);
	return FALSE;
}


void COdbcDatabaseAttributeSetupListCtrl::EditCell(int nItem, int nCol)
{
	switch (nCol)
	{
	case LISTCOL_TASKATTRIB:
		ASSERT(0); // not editable
		break;
		
	case LISTCOL_ATTRIBTABLE:
	case LISTCOL_LINKTABLE:
		ShowControl(m_cbTables, nItem, nCol);
		break;
		
	case LISTCOL_TASKFIELD:
	case LISTCOL_ATTRIBKEYFIELD:
	case LISTCOL_ATTRIBCONTENTFIELD:
	case LISTCOL_LINKTASKFIELD:
	case LISTCOL_LINKATTRIBFIELD:
	case LISTCOL_LINKTASKLISTFIELD:
		ShowControl(m_cbFields, nItem, nCol);
		break;
	}
}

BOOL COdbcDatabaseAttributeSetupListCtrl::CanDeleteSelectedCell() const
{
	int nRow, nCol;

	if (GetCurSel(nRow, nCol))
		return (nCol != LISTCOL_TASKATTRIB);

	// else
	return FALSE;
}

BOOL COdbcDatabaseAttributeSetupListCtrl::DeleteSelectedCell()
{
	int nRow, nCol;

	if (GetCurSel(nRow, nCol))
	{
		SetItemText(nRow, nCol, _T(""));
		NotifyParentEditCell(_T(""), nRow, nCol);

		return TRUE;
	}

	// else
	return FALSE;
}

void COdbcDatabaseAttributeSetupListCtrl::PrepareControl(CWnd& /*ctrl*/, int nRow, int nCol)
{
	CString sCell = GetItemText(nRow, nCol);

	switch (nCol)
	{
	case LISTCOL_TASKATTRIB:
		ASSERT(0); // not editable
		break;
		
	case LISTCOL_ATTRIBTABLE:
	case LISTCOL_LINKTABLE:
		m_cbTables.Initialize(m_pDB, _T(""), TRUE);
		m_cbTables.SelectString(-1, sCell);
		break;
		
	case LISTCOL_TASKFIELD:
		m_cbFields.Initialize(m_pDB, m_sTaskTable, _T(""), TRUE);
		m_cbFields.SelectString(-1, sCell);
		break;

	case LISTCOL_ATTRIBKEYFIELD:
	case LISTCOL_ATTRIBCONTENTFIELD:
		m_cbFields.Initialize(m_pDB, GetItemText(nRow, LISTCOL_ATTRIBTABLE));
		m_cbFields.SelectString(-1, sCell);
		break;

	case LISTCOL_LINKTASKFIELD:
	case LISTCOL_LINKATTRIBFIELD:
	case LISTCOL_LINKTASKLISTFIELD:
		m_cbFields.Initialize(m_pDB, GetItemText(nRow, LISTCOL_LINKTABLE));
		m_cbFields.SelectString(-1, sCell);
		break;
	}

	CDialogHelper::RefreshMaxDropWidth(m_cbFields);
	CDialogHelper::RefreshMaxDropWidth(m_cbTables);
}

COLORREF COdbcDatabaseAttributeSetupListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (!IsWindowEnabled() || (nCol > 0 && !CanEditCell(nItem, nCol)))
		return GetSysColor(COLOR_3DLIGHT);

	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void COdbcDatabaseAttributeSetupListCtrl::OnTableEditOK()
{
	HideControl(m_cbTables);

	// update cell text
	int nSel = m_cbTables.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sTable;
		m_cbTables.GetLBText(nSel, sTable);

		int nRow, nCol;
		
		if (GetCurSel(nRow, nCol))
		{
			SetItemText(nRow, nCol, sTable);
			NotifyParentEditCell(sTable, nRow, nCol);
		}
	}
}

void COdbcDatabaseAttributeSetupListCtrl::OnTableEditCancel()
{
	HideControl(m_cbTables);
}

void COdbcDatabaseAttributeSetupListCtrl::OnFieldEditOK()
{
	HideControl(m_cbFields);

	// update cell text
	int nSel = m_cbFields.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sField;
		m_cbFields.GetLBText(nSel, sField);

		int nRow, nCol;
		
		if (GetCurSel(nRow, nCol))
		{
			SetItemText(nRow, nCol, sField);
			NotifyParentEditCell(sField, nRow, nCol);
		}
	}
}

void COdbcDatabaseAttributeSetupListCtrl::OnFieldEditCancel()
{
	HideControl(m_cbFields);
}

BOOL COdbcDatabaseAttributeSetupListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	BOOL bCleared = !(lpszText && *lpszText);
	CString sPrev = GetItemText(nItem, nSubItem);

	BOOL bRes = CInputListCtrl::SetItemText(nItem, nSubItem, lpszText);

	// extra-over processing
	if (bRes && (sPrev.CompareNoCase(lpszText) != 0))
	{
		int nClearEnd = -1;

		switch (nSubItem)
		{
			case LISTCOL_TASKFIELD:
				// clear all
				nClearEnd = NUM_LISTCOL;
				break;

			case LISTCOL_ATTRIBTABLE:
				// clear associated attribute columns and link cols if cleared
				nClearEnd = (bCleared ? NUM_LISTCOL : LISTCOL_LINKTABLE);
				break;

			case LISTCOL_LINKTABLE:
				// clear associated link columns
				nClearEnd = NUM_LISTCOL;
				break;
		}
		
		// clear away!
		if (nClearEnd != -1)
		{
			for (int nCol = nSubItem + 1; nCol < nClearEnd; nCol++)
				CInputListCtrl::SetItemText(nItem, nCol, _T(""));
		}
	}

	return bRes;
}


