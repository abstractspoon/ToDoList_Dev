// TDLColumnAttribVisibilityListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLColumnAttribVisibilityListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum
{
	COL_NAME,
	COL_COLUMNVIS,
	COL_EDITVIS,
	COL_FILTERVIS,
};

/////////////////////////////////////////////////////////////////////////////

struct TDCCOLATTRIBITEM
{
	UINT nIDName;
	TDC_COLUMN nCol;
};

const TDCCOLATTRIBITEM ERRITEM = { 0, TDCC_NONE };

TDCCOLATTRIBITEM ITEMS[] = 
{
	{ IDS_TDLBC_ALLOCBY,		TDCC_ALLOCBY		},
	{ IDS_TDLBC_ALLOCTO,		TDCC_ALLOCTO		},
	{ IDS_TDLBC_CATEGORY,		TDCC_CATEGORY		},
	{ IDS_TDLBC_COLOR,			TDCC_COLOR			},
	{ IDS_TDLBC_COST,			TDCC_COST			},
	{ IDS_TDLBC_CREATEDATE,		TDCC_CREATIONDATE	},
	{ IDS_TDLBC_CREATEDBY,		TDCC_CREATEDBY		},
	{ IDS_TDLBC_DEPENDS,		TDCC_DEPENDENCY		},
	{ IDS_TDLBC_DONE,			TDCC_DONE			},
	{ IDS_TDLBC_DONEDATE,		TDCC_DONEDATE		},
	{ IDS_TDLBC_DONETIME,		TDCC_DONETIME		},
	{ IDS_TDLBC_DUEDATE,		TDCC_DUEDATE		},
	{ IDS_TDLBC_DUETIME,		TDCC_DUETIME		},
	{ IDS_TDLBC_EXTERNALID,		TDCC_EXTERNALID		},
	{ IDS_TDLBC_FILEREF,		TDCC_FILEREF		},
	{ IDS_TDLBC_FLAG,			TDCC_FLAG,			},
	{ IDS_TDLBC_ICON,			TDCC_ICON,			},
	{ IDS_TDLBC_ID,				TDCC_ID,			},	
	{ IDS_TDLBC_MODIFYDATE,		TDCC_LASTMOD,		},
	{ IDS_TDLBC_PARENTID,		TDCC_PARENTID,		},
	{ IDS_TDLBC_PATH,			TDCC_PATH,			},
	{ IDS_TDLBC_PERCENT,		TDCC_PERCENT,		},
	{ IDS_TDLBC_POS,			TDCC_POSITION,		},
	{ IDS_TDLBC_PRIORITY,		TDCC_PRIORITY,		},
	{ IDS_TDLBC_RECENTEDIT,		TDCC_RECENTEDIT,	},
	{ IDS_TDLBC_RECURRENCE,		TDCC_RECURRENCE,	},
	{ IDS_TDLBC_REMAINING,		TDCC_REMAINING,		},
	{ IDS_TDLBC_REMINDER,		TDCC_REMINDER,		},
	{ IDS_TDLBC_RISK,			TDCC_RISK,			},
	{ IDS_TDLBC_STARTDATE,		TDCC_STARTDATE,		},
	{ IDS_TDLBC_STARTTIME,		TDCC_STARTTIME,		},
	{ IDS_TDLBC_STATUS,			TDCC_STATUS,		},
	{ IDS_TDLBC_TAGS,			TDCC_TAGS,			},
	{ IDS_TDLBC_TIMEEST,		TDCC_TIMEEST,		},
	{ IDS_TDLBC_TIMESPENT,		TDCC_TIMESPENT,		},
	{ IDS_TDLBC_TRACKTIME,		TDCC_TRACKTIME,		},
	{ IDS_TDLBC_VERSION,		TDCC_VERSION,		},
	{ IDS_TDLBC_SUBTASKDONE,	TDCC_SUBTASKDONE,	},

};
const int NUM_ITEMS = (sizeof(ITEMS) / sizeof(TDCCOLATTRIBITEM));

const TDCCOLATTRIBITEM& GetColAttrib(TDC_COLUMN nCol)
{
	int nItem = NUM_ITEMS;

	while (nItem--)
	{
		if (ITEMS[nItem].nCol == nCol)
			return ITEMS[nItem];
	}

	// not found
	ASSERT(0);
	return ERRITEM;	
}

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnAttribVisibilityListCtrl

CTDLColumnAttribVisibilityListCtrl::CTDLColumnAttribVisibilityListCtrl()
{
}

CTDLColumnAttribVisibilityListCtrl::~CTDLColumnAttribVisibilityListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLColumnAttribVisibilityListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLColumnAttribVisibilityListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnAttribVisibilityListCtrl message handlers

void CTDLColumnAttribVisibilityListCtrl::SetVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	m_vis = vis;

	if (GetSafeHwnd())
		UpdateVisibility();
}

void CTDLColumnAttribVisibilityListCtrl::GetVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	vis = m_vis;
}

void CTDLColumnAttribVisibilityListCtrl::BuildListCtrl()
{
	// build once only
	if (GetColumnCount() == 0)
	{
		AddCol(CEnString(IDS_COLATTRIBNAME), 200);

		SetColumnType(AddCol(CEnString(IDS_COLUMNVISIBILITY), 100), ILCT_CHECK);
		SetColumnType(AddCol(CEnString(IDS_ATTRIBVISIBILITY), 100), ILCT_CHECK);
		SetColumnType(AddCol(CEnString(IDS_FILTERVISIBILITY), 100), ILCT_CHECK);

		for (int nItem = 0; nItem < NUM_ITEMS; nItem++)
		{
			const TDCCOLATTRIBITEM& caItem = ITEMS[nItem];

			int nIndex = InsertItem(nItem, CEnString(caItem.nIDName));
			SetItemData(nIndex, caItem.nCol);
		}

		ASSERT(GetItemCount() == NUM_ITEMS);

		SetMinItemHeight(16 + 2); // checkbox + padding
	}

	UpdateVisibility();
}

void CTDLColumnAttribVisibilityListCtrl::InitState()
{
	CInputListCtrl::InitState();
	
	ShowGrid(TRUE, TRUE);
}

void CTDLColumnAttribVisibilityListCtrl::PreSubclassWindow() 
{
	CInputListCtrl::PreSubclassWindow();
	
	BuildListCtrl();
}

int CTDLColumnAttribVisibilityListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildListCtrl();
	
	return 0;
}

void CTDLColumnAttribVisibilityListCtrl::UpdateVisibility()
{
	int nItem = GetItemCount();

	while (nItem--)
	{
		TDC_COLUMN nColumn = GetItemColumn(nItem);
		TDC_ATTRIBUTE nAttrib = GetItemAttrib(nItem);
		
		// set column visibility
		BOOL bVisible = ((nColumn != TDCC_NONE) && m_vis.IsColumnVisible(nColumn));
		SetCellText(nItem, COL_COLUMNVIS, (bVisible ? _T("+") : _T("")));

		// set edit visibility
		bVisible = ((nAttrib != TDCA_NONE) && m_vis.IsEditFieldVisible(nAttrib));
		SetCellText(nItem, COL_EDITVIS, (bVisible ? _T("+") : _T("")));

		// set filter visibility
		bVisible = ((nAttrib != TDCA_NONE) && m_vis.IsFilterFieldVisible(nAttrib));
		SetCellText(nItem, COL_FILTERVIS, (bVisible ? _T("+") : _T("")));
	}

	Invalidate(FALSE);
}

TDC_COLUMN CTDLColumnAttribVisibilityListCtrl::GetItemColumn(int nItem) const
{
	return (TDC_COLUMN)GetItemData(nItem);
}

TDC_ATTRIBUTE CTDLColumnAttribVisibilityListCtrl::GetItemAttrib(int nItem) const
{
	return TDC::MapColumnToAttribute(GetItemColumn(nItem));
}

BOOL CTDLColumnAttribVisibilityListCtrl::CanEditCell(int nRow, int nCol) const
{
	switch (nCol)
	{
	case COL_COLUMNVIS:
		return m_vis.IsSupportedColumn(GetItemColumn(nRow));

	case COL_EDITVIS:
		return m_vis.IsSupportedEdit(GetItemAttrib(nRow));
		
	case COL_FILTERVIS:
		return m_vis.IsSupportedFilter(GetItemAttrib(nRow));
	}

	// all else
	return FALSE;
}

void CTDLColumnAttribVisibilityListCtrl::EditCell(int nItem, int nCol)
{
	// don't allow editing on the attribute column if
	// user is displaying attributes as columns
	if (m_vis.GetShowEditsAndFilters() != TDLSA_ANY)
	{
		switch (nCol)
		{
		case COL_EDITVIS:
		case COL_FILTERVIS:
			return;
		}
	}

	CInputListCtrl::EditCell(nItem, nCol);

	// update our data to match
	TDC_COLUMN nColumn = GetItemColumn(nItem);
	TDC_ATTRIBUTE nAttrib = GetItemAttrib(nItem);
	BOOL bVisible = !GetItemText(nItem, nCol).IsEmpty();
	
	switch (nCol)
	{
	case COL_COLUMNVIS:
		ASSERT(nColumn != TDCC_NONE);

		m_vis.SetColumnVisible(nColumn, bVisible);
		UpdateVisibility();
		break;
		
	case COL_EDITVIS:
		ASSERT(nAttrib != TDCA_NONE);
		ASSERT(m_vis.GetShowEditsAndFilters() == TDLSA_ANY);

		m_vis.SetEditFieldVisible(nAttrib, bVisible);
		UpdateVisibility();
		break;
		
	case COL_FILTERVIS:
		ASSERT(nAttrib != TDCA_NONE);
		ASSERT(m_vis.GetShowEditsAndFilters() == TDLSA_ANY);
		
		m_vis.SetFilterFieldVisible(nAttrib, bVisible);
		UpdateVisibility();
		break;
	}
}

COLORREF CTDLColumnAttribVisibilityListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (!bSelected)
	{
		if (!IsWindowEnabled())
			return GetSysColor(COLOR_3DLIGHT);

		if (!IsTimeCellEnabled(nItem, nCol))
			return GetSysColor(COLOR_3DLIGHT);
		
		switch (nCol)
		{
		case COL_EDITVIS:
			if (m_vis.GetShowEditsAndFilters() != TDLSA_ANY)
				return GetSysColor(COLOR_3DLIGHT);
			break;

		case COL_FILTERVIS:
			if (m_vis.GetShowEditsAndFilters() != TDLSA_ANY)
				return GetSysColor(COLOR_3DLIGHT);
			break;
		}
	}
	
	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

BOOL CTDLColumnAttribVisibilityListCtrl::IsButtonEnabled(int nRow, int nCol) const 
{ 
	if (!CInputListCtrl::IsButtonEnabled(nRow, nCol))
		return FALSE;

	switch (nCol)
	{
	case COL_EDITVIS:
		return (m_vis.GetShowEditsAndFilters() == TDLSA_ANY);
		
	case COL_FILTERVIS:
		return (m_vis.GetShowEditsAndFilters() == TDLSA_ANY);
	}

	// all else
	return TRUE; 
}

BOOL CTDLColumnAttribVisibilityListCtrl::IsTimeCellEnabled(int nItem, int nCol) const
{
	// time cells are disabled if the corresponding date cells are unticked
	TDC_COLUMN nColumn = GetItemColumn(nItem);

	switch (nCol)
	{
	case COL_COLUMNVIS:
		{
			switch (nColumn)
			{
			case TDCC_DUETIME:		return m_vis.IsColumnVisible(TDCC_DUEDATE);
			case TDCC_DONETIME:		return m_vis.IsColumnVisible(TDCC_DONEDATE);
			case TDCC_STARTTIME:	return m_vis.IsColumnVisible(TDCC_STARTDATE);
			}
		}
		break;
		
	case COL_EDITVIS:
		{
			switch (nColumn)
			{
			case TDCC_DUETIME:		return m_vis.IsEditFieldVisible(TDCA_DUEDATE);
			case TDCC_DONETIME:		return m_vis.IsEditFieldVisible(TDCA_DONEDATE);
			case TDCC_STARTTIME:	return m_vis.IsEditFieldVisible(TDCA_STARTDATE);
			}
		}
		break;
		
	case COL_FILTERVIS:
		break; // doesn't have time cells
	}

	// all else
	return TRUE;
}

void CTDLColumnAttribVisibilityListCtrl::SetAllVisible(BOOL bVisible)
{
	// update visibility data
	m_vis.SetAllColumnsVisible(bVisible);

	if (m_vis.GetShowEditsAndFilters() == TDLSA_ANY)
		m_vis.SetAllEditsAndFiltersVisible(bVisible);

	UpdateVisibility();
}

void CTDLColumnAttribVisibilityListCtrl::SetShowEditsAndFilters(TDL_SHOWATTRIB nShow)
{
	m_vis.SetShowEditsAndFilters(nShow);

	UpdateVisibility();
}
