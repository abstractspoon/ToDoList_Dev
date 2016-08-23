// OdbcDatabaseSelectionListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OdbcDatabaseSelectionListCtrl.h"
#include "OdbcDatabaseSetupdlg.h"
#include "Odbchelper.h"

#include "..\shared\databaseex.h"

#include "..\Interfaces\IPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum
{
	COL_DB,
	COL_SETUP,
};

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionListCtrl

COdbcDatabaseSelectionListCtrl::COdbcDatabaseSelectionListCtrl()
{
	ShowGrid(TRUE, TRUE);
	SetSortColumn(0, FALSE);
	SetSortAscending(FALSE);
	EnableHeaderTracking(FALSE);
}

COdbcDatabaseSelectionListCtrl::COdbcDatabaseSelectionListCtrl(const COdbcDatabaseSetupArray& aDbSetup)
{
	ShowGrid(TRUE, TRUE);
	SetSortColumn(0, FALSE);
	EnableHeaderTracking(FALSE);

	m_aDbSetup.Copy(aDbSetup);
}

COdbcDatabaseSelectionListCtrl::~COdbcDatabaseSelectionListCtrl()
{
}


BEGIN_MESSAGE_MAP(COdbcDatabaseSelectionListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(COdbcDatabaseSelectionListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionListCtrl message handlers

BOOL COdbcDatabaseSelectionListCtrl::Initialize()
{
	COdbcDatabaseSetupArray aTemp;
	aTemp.Copy(m_aDbSetup);

	return Initialize(aTemp);
}

BOOL COdbcDatabaseSelectionListCtrl::Initialize(const COdbcDatabaseSetupArray& aDbSetup)
{
	ASSERT(GetSafeHwnd());

	if (!GetSafeHwnd())
		return FALSE;

	CRect rList;
	GetClientRect(rList);

	AddCol(CEnString(IDS_DATABASE_COL), 250);
	AddCol(_T(""), rList.Width() - 250);

	DisableColumnEditing(0, TRUE);
	SetColumnType(COL_SETUP, ILCT_BROWSE);

	m_aDbSetup.Copy(aDbSetup);

	// add databases to list
	int nNumDb = m_aDbSetup.GetSize();

	for (int nDb = 0; nDb < nNumDb; nDb++)
	{
		ODBCDATABASESETUP& db = m_aDbSetup.ElementAt(nDb);

		int nItem = AddRow(FormatUniqueName(db));

		SetItemText(nItem, COL_SETUP, CEnString(IDS_DBSETUP_COL));
		SetItemData(nItem, nDb);
	}

	Sort();

	return TRUE;
}

CString COdbcDatabaseSelectionListCtrl::FormatUniqueName(const ODBCDATABASESETUP& dbSetup) const
{
	CString sDbTitle(COdbcHelper::FormatName(dbSetup));

	// create a unique name
	int nNext = 2;

	while (FindItemFromLabel(sDbTitle) >= 0)
	{
		CString sTemp(sDbTitle);
		sDbTitle.Format(_T("%s (%d)"), sTemp, nNext++);
	}

	return sDbTitle;
}


BOOL COdbcDatabaseSelectionListCtrl::AddDatabaseSetup(const ODBCDATABASESETUP& dbSetup, BOOL bDoSetup)
{
	ASSERT(!dbSetup.sDbName.IsEmpty());
	ASSERT(!dbSetup.sConnect.IsEmpty());

	// don't allow the same database to be added twice
	if (COdbcHelper::FindDatabase(dbSetup, m_aDbSetup) != -1)
		return FALSE;

	int nItem = AddRow(FormatUniqueName(dbSetup));

	if (nItem == -1)
		return FALSE;

	ODBCDATABASESETUP temp = dbSetup;

	m_aDbSetup.Add(temp);

	SetItemText(nItem, COL_SETUP, CEnString(IDS_DBSETUP_COL));
	SetItemData(nItem, (m_aDbSetup.GetSize() - 1));

	Sort();

	// restore selection
	nItem = FindItemFromData((m_aDbSetup.GetSize() - 1));
	ASSERT(nItem != -1);

	SetCurSel(nItem);
	ASSERT(GetCurSel() == nItem);

	if (bDoSetup)
		EditCell(nItem, COL_SETUP);

	return TRUE;
}

BOOL COdbcDatabaseSelectionListCtrl::GetSelectedDatabaseSetup(ODBCDATABASESETUP& dbSetup) const
{
	ASSERT(GetSafeHwnd());

	int nSel = GetCurSel();
	ASSERT(nSel != -1);

	if (nSel == -1)
		return FALSE;

	int nDb = GetItemData(nSel);

	dbSetup = m_aDbSetup[nDb];
	return TRUE;
}

BOOL COdbcDatabaseSelectionListCtrl::SetSelectedDatabaseModifiers(const CString& sTasklistID, const CString& sAllocToID)
{
	ASSERT(GetSafeHwnd());

	int nSel = GetCurSel();
	ASSERT(nSel != -1);

	if (nSel == -1)
		return FALSE;

	int nDb = GetItemData(nSel);

	m_aDbSetup[nDb].sTasklistID = sTasklistID;
	m_aDbSetup[nDb].sAllocToID = sAllocToID;

	return TRUE;

}

BOOL COdbcDatabaseSelectionListCtrl::DeleteSelectedDatabaseSetup()
{
	int nSel = GetCurSel();

	if (nSel != -1)
	{
		int nDb = (int)GetItemData(nSel);
		ASSERT((nDb >= 0) && (nDb < m_aDbSetup.GetSize()));

		if ((nDb >= 0) && (nDb < m_aDbSetup.GetSize()))
		{
			// fixup selection before deleting
			if (nSel > 0)
			{
				SetCurSel(nSel - 1);
			}
			else if (nSel < (GetItemCount() - 1))
			{
				SetCurSel(nSel + 1);
			}

			m_aDbSetup.RemoveAt(nDb);
			DeleteItem(nSel);

			return TRUE;
		}
	}

	// else
	ASSERT(0);
	return FALSE;
}

int COdbcDatabaseSelectionListCtrl::GetDatabaseSetups(COdbcDatabaseSetupArray& aDbSetup) const
{
	aDbSetup.Copy(m_aDbSetup);
	return aDbSetup.GetSize();
}

void COdbcDatabaseSelectionListCtrl::EditCell(int nItem, int nCol)
{
	ASSERT(nCol == 1);
	ASSERT(nItem < m_aDbSetup.GetSize());

	int nDb = GetItemData(nItem);

	ODBCDATABASESETUP& dbSetup = m_aDbSetup.ElementAt(nDb);
	CDatabaseEx db;

	if (!db.Connect(dbSetup.sConnect))
	{
		// TODO
		return;
	}

	COdbcDatabaseSetupDlg dialog(db, dbSetup.setup);

	if (dialog.DoModal() == IDOK)
	{
		dialog.GetStorageSetup(dbSetup.setup);
		NotifyParentEditCell(_T(""), nItem, nCol);
	}
}

