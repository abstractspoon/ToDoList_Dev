// TDLTaskAttributeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLTaskAttributeListCtrl.h"

#include "tdcstatic.h"
#include "tdcstruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl

CTDLTaskAttributeListCtrl::CTDLTaskAttributeListCtrl()
{
}

CTDLTaskAttributeListCtrl::~CTDLTaskAttributeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLTaskAttributeListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLTaskAttributeListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskAttributeListCtrl message handlers

int CTDLTaskAttributeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CInputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetView(LVS_REPORT);
	EnableSorting(TRUE);
	ShowGrid(TRUE, TRUE);
	
	// Add columns
	AddCol(_T("Attribute"));
	AddCol(_T("Value"));
	
	// Add attributes as rows
	for (int nAttrib = 1; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		const TDCATTRIBUTE& attrib = ATTRIBUTES[nAttrib];

		// Exclude some
		switch (attrib.nAttribID)
		{
		case TDCA_PROJECTNAME:
		case TDCA_ANYTEXTATTRIBUTE:
		case TDCA_TASKNAMEORCOMMENTS:
			continue;
		}
		
		int nItem = AddRow(CEnString(attrib.nAttribResID));
		SetItemData(nItem, attrib.nAttribID);
	}

	Sort();
	
	return 0;
}

void CTDLTaskAttributeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CInputListCtrl::OnSize(nType, cx, cy);
	
	if (GetColumnCount())
	{
		SetColumnWidth(0, (cx / 2));
		SetColumnWidth(1, (cx / 2) - 1);
	}
}

void CTDLTaskAttributeListCtrl::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CInputListCtrl::OnDropFiles(hDropInfo);
}

BOOL CTDLTaskAttributeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnEraseBkgnd(pDC);
}

BOOL CTDLTaskAttributeListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CInputListCtrl::OnSetCursor(pWnd, nHitTest, message);
}

IL_COLUMNTYPE CTDLTaskAttributeListCtrl::GetCellType(int nRow, int nCol) const
{
	if (nCol == 0)
		return ILCT_TEXT;

	// else
	switch (GetItemData(nRow))
	{
		// Text fields
		case TDCA_TASKNAME:
		case TDCA_COST:
		case TDCA_EXTERNALID:
		case TDCA_PERCENT:
		case TDCA_TIMEESTIMATE:
		case TDCA_TIMESPENT:
			return ILCT_TEXT;

		// Date fields
		case TDCA_DONEDATE:
		case TDCA_DUEDATE:
		case TDCA_STARTDATE:
			return ILCT_DATE;

		// Drop lists
		case TDCA_PRIORITY:
		case TDCA_COLOR:
		case TDCA_ALLOCTO:
		case TDCA_ALLOCBY:
		case TDCA_STATUS:
		case TDCA_CATEGORY:
		case TDCA_TAGS:
		case TDCA_FILELINK:
		case TDCA_RISK:
		case TDCA_VERSION:
			return ILCT_DROPLIST;

		// Browse-like fields
		case TDCA_ICON:
		case TDCA_RECURRENCE:
		case TDCA_DEPENDENCY:
			return ILCT_BROWSE;

		// Checkbox fields
		case TDCA_FLAG:
		case TDCA_LOCK:
			return ILCT_CHECK;

		// Read-only fields
		case TDCA_COMMENTS:
		case TDCA_CREATEDBY:
		case TDCA_PATH:
		case TDCA_POSITION:
		case TDCA_CREATIONDATE:
		case TDCA_LASTMODDATE:
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
		case TDCA_SUBTASKDONE:
		case TDCA_LASTMODBY:
		case TDCA_ID:
		case TDCA_PARENTID:
			return ILCT_TEXT;
	}

	// All else
	return ILCT_TEXT;
}

BOOL CTDLTaskAttributeListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (nCol == 0)
		return FALSE;

	// else
	switch (GetItemData(nRow))
	{
		// Read-only fields
		case TDCA_COMMENTS:
		case TDCA_CREATEDBY:
		case TDCA_PATH:
		case TDCA_POSITION:
		case TDCA_CREATIONDATE:
		case TDCA_LASTMODDATE:
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
		case TDCA_SUBTASKDONE:
		case TDCA_LASTMODBY:
		case TDCA_ID:
		case TDCA_PARENTID:
			return FALSE;
	}

	// All else
	return TRUE;
}

COLORREF CTDLTaskAttributeListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if ((nCol == 1) && !CanEditCell(nItem, nCol))
		return GetSysColor(COLOR_3DFACE);

	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}
