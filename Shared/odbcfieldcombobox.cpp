// odbcfieldcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "odbcfields.h"
#include "odbcfieldcombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdbcFieldComboBox

COdbcFieldComboBox::COdbcFieldComboBox() : m_pDatabase(NULL)
{
}

COdbcFieldComboBox::~COdbcFieldComboBox()
{
}


BEGIN_MESSAGE_MAP(COdbcFieldComboBox, CComboBox)
	//{{AFX_MSG_MAP(COdbcFieldComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcFieldComboBox message handlers

int COdbcFieldComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pDatabase)
		Initialize(m_pDatabase, m_sTable);
		
	return 0;
}

void COdbcFieldComboBox::PreSubclassWindow() 
{
	if (m_pDatabase)
		Initialize(m_pDatabase, m_sTable);
	
	CComboBox::PreSubclassWindow();
}

BOOL COdbcFieldComboBox::Initialize(CDatabase* pDb, const CString& sTable, 
									const CString& sSelField, BOOL bAddEmptyItem)
{
	ASSERT(pDb);

	if (!pDb)
		return FALSE;

	m_pDatabase = pDb;

	SetTable(sTable, sSelField, bAddEmptyItem);

	return TRUE;
}

BOOL COdbcFieldComboBox::SetTable(const CString& sTable, const CString& sSelField, BOOL bAddEmptyItem)
{
	ASSERT(m_pDatabase);

	if (!m_pDatabase)
		return FALSE;

	if (GetSafeHwnd())
	{
		ResetContent(); // always

		if (!sTable.IsEmpty())
		{
			CStringArray aFields;

			if (COdbcFields::GetFieldNames(*m_pDatabase, sTable, aFields) == 0)
			{
				ASSERT(0);
				return FALSE;
			}

			// else
			int i = aFields.GetSize();
				
			while (i--)
				AddString(aFields[i]);

			if (bAddEmptyItem)
				AddString(_T(""));

			SelectString(-1, sSelField);
		}
	}
	else
	{
		m_sTable = sTable;
	}
	
	return TRUE;
}
