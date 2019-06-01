// odbctablecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "odbctables.h"
#include "odbctablecombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdbcTableComboBox

COdbcTableComboBox::COdbcTableComboBox() : m_pDatabase(NULL)
{
}

COdbcTableComboBox::~COdbcTableComboBox()
{
}


BEGIN_MESSAGE_MAP(COdbcTableComboBox, CComboBox)
	//{{AFX_MSG_MAP(COdbcTableComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcTableComboBox message handlers

int COdbcTableComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pDatabase)
		Initialize(m_pDatabase);
	
	return 0;
}

void COdbcTableComboBox::PreSubclassWindow() 
{
	if (m_pDatabase)
		Initialize(m_pDatabase);
	
	CComboBox::PreSubclassWindow();
}

BOOL COdbcTableComboBox::Initialize(CDatabase* pDb, const CString& sSelTable, BOOL bAddEmptyItem)
{
	ASSERT(pDb);

	if (!pDb)
		return FALSE;

	if (GetSafeHwnd())
	{
		CStringArray aTables;
		
		if (COdbcTables::GetTableNames(*pDb, aTables))
		{
			ResetContent();

			int i = aTables.GetSize();
			
			while (i--)
				AddString(aTables[i]);

			if (bAddEmptyItem)
				AddString(_T(""));

			SelectString(-1, sSelTable);
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}
	else // cache for later
	{
		m_pDatabase = pDb;
	}

	return TRUE;
}
