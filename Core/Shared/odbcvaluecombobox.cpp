// odbcvaluecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "odbcvaluecombobox.h"

#include "..\shared\RecordsetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

enum 
{
	VALUE_KEY,
	VALUE_CONTENT,
};

/////////////////////////////////////////////////////////////////////////////
// COdbcValueComboBox

COdbcValueComboBox::COdbcValueComboBox()
{
}

COdbcValueComboBox::~COdbcValueComboBox()
{
}


BEGIN_MESSAGE_MAP(COdbcValueComboBox, CComboBox)
	//{{AFX_MSG_MAP(COdbcValueComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcValueComboBox message handlers

BOOL COdbcValueComboBox::Initialize(CDatabase* pDb, const CString& sTable, 
									const CString& sKeyField, 
									const CString& sContentField,
									BOOL bAddEmptyItem)
{
	ASSERT(GetSafeHwnd());

	CString sQuery;
	sQuery.Format(_T("SELECT %s, %s FROM %s"), sKeyField, sContentField, sTable);

	CRecordsetEx rs(pDb);

	if (!rs.ExecDirect(sQuery) || rs.IsBOF())
		return FALSE;

	ResetContent();

	while (!rs.IsEOF())
	{
		int nItem = AddString(rs.GetFieldValue(VALUE_CONTENT));

		if (nItem != CB_ERR)
		{
			// use key index as items data
			SetItemData(nItem, m_aKeys.GetSize());
			m_aKeys.Add(rs.GetFieldValue(VALUE_KEY));
		}

		rs.MoveNext();			
	}

	if (bAddEmptyItem)
	{
		int nItem = AddString(_T(""));
		SetItemData(nItem, m_aKeys.GetSize());
		m_aKeys.Add(_T(""));
	}

	return GetCount();
}

CString COdbcValueComboBox::GetSelectedValueKey() const
{
	ASSERT(GetSafeHwnd());

	int nSel = GetCurSel();

	if (nSel == -1)
		return _T("");

	// else
	int nKey = GetItemData(nSel);
	ASSERT(nKey >= 0 && nKey < m_aKeys.GetSize());

	if (nKey < 0 || nKey >= m_aKeys.GetSize())
		return _T("");

	return m_aKeys[nKey];
}

BOOL COdbcValueComboBox::SelectValueByKey(const CString& sKey)
{
	ASSERT(GetSafeHwnd());

	// first we find the key in the key array
	int nKey = m_aKeys.GetSize();

	while (nKey--)
	{
		if (m_aKeys[nKey].CompareNoCase(sKey) == 0)
		{
			// then find the item with that key as data
			int nItem = GetCount();

			while (nItem--)
			{
				if (GetItemData(nItem) == (DWORD)nKey)
				{
					SetCurSel(nItem);
					return TRUE;
				}
			}

		}
	}

	// not found
	return FALSE;
}
