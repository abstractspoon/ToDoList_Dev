// TDColumnListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLColumnListBox.h"

#include "..\shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnListBox

CTDLColumnListBox::CTDLColumnListBox()
{
	// same order as enum
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_POS,			TDCC_POSITION,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_ID,			TDCC_ID,		TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_PRIORITY,	TDCC_PRIORITY,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_PERCENT,		TDCC_PERCENT,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_TIMEEST,		TDCC_TIMEEST,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_TIMESPENT,	TDCC_TIMESPENT, TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_TRACKTIME,	TDCC_TRACKTIME, TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_CREATEDBY,	TDCC_CREATEDBY, FALSE));
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_CREATEDATE,	TDCC_CREATIONDATE, FALSE));
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_STARTDATE,	TDCC_STARTDATE, FALSE));
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_DUEDATE,		TDCC_DUEDATE,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_DONEDATE,	TDCC_DONEDATE,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_ALLOCTO,		TDCC_ALLOCTO,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_ALLOCBY,		TDCC_ALLOCBY,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_STATUS,		TDCC_STATUS,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_CATEGORY,	TDCC_CATEGORY,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_FILELINK,	TDCC_FILELINK,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_FLAG,		TDCC_FLAG,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_LOCK,		TDCC_LOCK,		TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_DONE,		TDCC_DONE,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_LASTMODDATE,	TDCC_LASTMODDATE,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_RISK,		TDCC_RISK,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_EXTERNALID,	TDCC_EXTERNALID, FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_COST,		TDCC_COST,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_DEPENDS,		TDCC_DEPENDENCY, FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_RECURRENCE,	TDCC_RECURRENCE, TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_VERSION,		TDCC_VERSION,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_RECENTEDIT,	TDCC_RECENTEDIT,FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_REMAINING,	TDCC_REMAINING,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_ICON,		TDCC_ICON,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_REMINDER,	TDCC_REMINDER,	TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_PARENTID,	TDCC_PARENTID,	FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_PATH,		TDCC_PATH,		FALSE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_TAGS,		TDCC_TAGS,		TRUE)); 
	m_aColumns.Add(COLUMNVIS(IDS_TDLBC_SUBTASKDONE,	TDCC_SUBTASKDONE, FALSE)); 
}

CTDLColumnListBox::~CTDLColumnListBox()
{
}


BEGIN_MESSAGE_MAP(CTDLColumnListBox, CCheckListBoxEx)
	//{{AFX_MSG_MAP(CTDLColumnListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CLBN_CHKCHANGE, OnReflectCheckChange)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLColumnListBox message handlers

void CTDLColumnListBox::PreSubclassWindow() 
{
	CCheckListBoxEx::PreSubclassWindow();
}

int CTDLColumnListBox::FindColumn(TDC_COLUMN nTDCCol) const
{
	int nIndex = m_aColumns.GetSize();
	
	while (nIndex--)
	{
		if (m_aColumns[nIndex].nTDCCol == nTDCCol)
			return nIndex;
	}

	ASSERT(0);
	return -1;
}

void CTDLColumnListBox::SetAllColumnsVisible(BOOL bVisible)
{
	int nIndex = m_aColumns.GetSize();
	
	while (nIndex--)
	{
		m_aColumns[nIndex].bVisible = bVisible;

		if (GetSafeHwnd())
			SetCheck(nIndex, bVisible ? 1 : 0);
	}
}

int CTDLColumnListBox::GetAllColumns(CTDCColumnIDArray& aColumns) const
{
	int nIndex = m_aColumns.GetSize();
	
	while (nIndex--)
	{
		TDC_COLUMN col = m_aColumns[nIndex].nTDCCol;
		VERIFY(aColumns.AddUnique(col));
	}

	return aColumns.GetSize();
}

BOOL CTDLColumnListBox::OnReflectCheckChange()
{
	// update all check states because we don't know which one changed
	int nIndex = m_aColumns.GetSize();
	
	while (nIndex--)
	{
		TDC_COLUMN col = (TDC_COLUMN)GetItemData(nIndex);
		int nCol = FindColumn(col);

		if (nCol != -1)
			m_aColumns[nCol].bVisible = GetCheck(nIndex);
	}

	// continue routing
	return FALSE;
}

HBRUSH CTDLColumnListBox::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// build the list box if not already done
	if (GetCount() == 0)
	{
		for (int nIndex = 0; nIndex < (int)m_aColumns.GetSize(); nIndex++)
		{
			const COLUMNVIS& cs = m_aColumns[nIndex];

			int nPos = AddString(cs.sName); // same order as enum
			SetCheck(nPos, cs.bVisible ? 1 : 0);
			SetItemData(nPos, (DWORD)cs.nTDCCol);
		}

		CDialogHelper::RefreshMaxColumnWidth(*this);
	}

	// continue routing
	return NULL;
}


void CTDLColumnListBox::SetColumnVisible(TDC_COLUMN nTDCCol, BOOL bVisible)
{
	int nCol = FindColumn(nTDCCol);
	
	if (nCol != -1)
	{
		m_aColumns[nCol].bVisible = bVisible;
		
		if (GetSafeHwnd())
		{
			// flip the check box
			int nIndex = m_aColumns.GetSize();
			
			while (nIndex--)
			{
				if ((DWORD)nTDCCol == GetItemData(nIndex))
				{
					SetCheck(nIndex, bVisible ? 1 : 0);
					break;
				}
			}
		}
	}
}

void CTDLColumnListBox::SetVisibleColumns(const CTDCColumnIDArray& aColumns)
{
	SetAllColumnsVisible(FALSE);

	int nCol = (int)aColumns.GetSize();
	
	while (nCol--)
		SetColumnVisible(aColumns[nCol]);
}

int CTDLColumnListBox::GetVisibleColumns(CTDCColumnIDArray& aColumns) const
{
	aColumns.RemoveAll();

	int nIndex = m_aColumns.GetSize();
	
	while (nIndex--)
	{
		COLUMNVIS cs = m_aColumns[nIndex];

		if (cs.bVisible)
			VERIFY(aColumns.AddUnique(cs.nTDCCol));
	}

	return aColumns.GetSize();

}
