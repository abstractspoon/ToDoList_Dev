// TDLColumnAttribVisibilityListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLColumnAttribVisibilityListCtrl.h"

#include "..\shared\GraphicsMisc.h"

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
	{ IDS_TDLBC_COMMENTSSIZE,	TDCC_COMMENTSSIZE	},
	{ IDS_TDLBC_COST,			TDCC_COST			},
	{ IDS_TDLBC_CREATEDATE,		TDCC_CREATIONDATE	},
	{ IDS_TDLBC_CREATETIME,		TDCC_CREATIONTIME	},
	{ IDS_TDLBC_CREATEDBY,		TDCC_CREATEDBY		},
	{ IDS_TDLBC_DEPENDS,		TDCC_DEPENDENCY		},
	{ IDS_TDLBC_DONE,			TDCC_DONE			},
	{ IDS_TDLBC_DONEDATE,		TDCC_DONEDATE		},
	{ IDS_TDLBC_DONETIME,		TDCC_DONETIME		},
	{ IDS_TDLBC_DUEDATE,		TDCC_DUEDATE		},
	{ IDS_TDLBC_DUETIME,		TDCC_DUETIME		},
	{ IDS_TDLBC_EXTERNALID,		TDCC_EXTERNALID		},
	{ IDS_TDLBC_FILELINK,		TDCC_FILELINK		},
	{ IDS_TDLBC_FLAG,			TDCC_FLAG,			},
	{ IDS_TDLBC_ICON,			TDCC_ICON,			},
	{ IDS_TDLBC_ID,				TDCC_ID,			},	
	{ IDS_TDLBC_LOCK,			TDCC_LOCK,			},
	{ IDS_TDLBC_LASTMODDATE,	TDCC_LASTMODDATE,	},
	{ IDS_TDLBC_LASTMODBY,		TDCC_LASTMODBY,		},
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
	ASSERT(GetColumnCount() == 0);

	AddCol(CEnString(IDS_COLATTRIBNAME), GraphicsMisc::ScaleByDPIFactor(150));

	AddCol(CEnString(IDS_COLUMNVISIBILITY), GraphicsMisc::ScaleByDPIFactor(100), ILCT_CHECK);
	AddCol(CEnString(IDS_ATTRIBVISIBILITY), GraphicsMisc::ScaleByDPIFactor(100), ILCT_CHECK);
	AddCol(CEnString(IDS_FILTERVISIBILITY), GraphicsMisc::ScaleByDPIFactor(100), ILCT_CHECK);

	for (int nItem = 0; nItem < NUM_ITEMS; nItem++)
	{
		const TDCCOLATTRIBITEM& caItem = ITEMS[nItem];

		int nIndex = InsertRow(CEnString(caItem.nIDName), nItem);
		SetItemData(nIndex, caItem.nCol);
	}

	ASSERT(GetItemCount() == NUM_ITEMS);

	UpdateVisibility();
}

void CTDLColumnAttribVisibilityListCtrl::InitState()
{
	CInputListCtrl::InitState();

	BuildListCtrl();
	ShowGrid(TRUE, TRUE);
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

void CTDLColumnAttribVisibilityListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	// don't allow editing on the attribute column if
	// user is displaying attributes as columns
	if (m_vis.GetShowFields() != TDLSA_ANY)
	{
		switch (nCol)
		{
		case COL_EDITVIS:
		case COL_FILTERVIS:
			return;
		}
	}

	CInputListCtrl::EditCell(nItem, nCol, bBtnClick);

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
		ASSERT(m_vis.GetShowFields() == TDLSA_ANY);

		m_vis.SetEditFieldVisible(nAttrib, bVisible);
		UpdateVisibility();
		break;
		
	case COL_FILTERVIS:
		ASSERT(nAttrib != TDCA_NONE);
		ASSERT(m_vis.GetShowFields() == TDLSA_ANY);
		
		m_vis.SetFilterFieldVisible(nAttrib, bVisible);
		UpdateVisibility();
		break;
	}
}

BOOL CTDLColumnAttribVisibilityListCtrl::IsButtonEnabled(int nRow, int nCol) const 
{ 
	if (!CInputListCtrl::IsButtonEnabled(nRow, nCol))
		return FALSE;

	if (!IsTimeCellEnabled(nRow, nCol))
		return FALSE;

	switch (nCol)
	{
	case COL_EDITVIS:
		return (m_vis.GetShowFields() == TDLSA_ANY);
		
	case COL_FILTERVIS:
		return (m_vis.GetShowFields() == TDLSA_ANY);
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
			case TDCC_CREATIONTIME:	return m_vis.IsColumnVisible(TDCC_CREATIONDATE);
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

	if (m_vis.GetShowFields() == TDLSA_ANY)
		m_vis.SetAllFieldsVisible(bVisible);

	UpdateVisibility();
}

void CTDLColumnAttribVisibilityListCtrl::SetShowEditsAndFilters(TDL_SHOWATTRIB nShow)
{
	m_vis.SetShowFields(nShow);

	UpdateVisibility();
}
