// historycombobox.cpp : implementation file
//

#include "stdafx.h"
#include "historycombobox.h"

#include "..\interfaces\IPreferences.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox

CHistoryComboBox::CHistoryComboBox()
{
}

CHistoryComboBox::~CHistoryComboBox()
{
}


BEGIN_MESSAGE_MAP(CHistoryComboBox, CComboBox)
	//{{AFX_MSG_MAP(CHistoryComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox message handlers

void CHistoryComboBox::Load(const IPreferences* pPrefs, LPCTSTR szKey)
{
	ASSERT(pPrefs && !Misc::IsEmpty(szKey));

	int nItem = pPrefs->GetProfileInt(szKey, _T("HistoryCount"));

	while (nItem--)
	{
		CString sItemKey;
		sItemKey.Format(_T("HistoryItem%d"), nItem);

		CString sItem = pPrefs->GetProfileString(szKey, sItemKey);
		Misc::AddUniqueItem(sItem, m_aHistory, TRUE);
	}

	if (GetSafeHwnd())
		BuildCombo();
}

void CHistoryComboBox::Save(IPreferences* pPrefs, LPCTSTR szKey) const
{
	ASSERT(pPrefs && !Misc::IsEmpty(szKey));
	ASSERT(GetSafeHwnd());
	
	CStringArray aHistory;
	aHistory.Copy(m_aHistory);

	// also add current item
	CString sItem;
	GetWindowText(sItem);

	if (!sItem.IsEmpty())
		Misc::AddUniqueItem(sItem, aHistory, TRUE);

	// then save to prefs
	int nItem = aHistory.GetSize();
	pPrefs->WriteProfileInt(szKey, _T("HistoryCount"), nItem);

	while (nItem--)
	{
		CString sItemKey;
		sItemKey.Format(_T("HistoryItem%d"), nItem);
			
		pPrefs->WriteProfileString(szKey, sItemKey, aHistory[nItem]);
	}
}

void CHistoryComboBox::BuildCombo()
{
	int nItem = m_aHistory.GetSize();
	
	while (nItem--)
		AddString(m_aHistory[nItem]);
}

void CHistoryComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	CComboBox::PreSubclassWindow();
}

int CHistoryComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}
