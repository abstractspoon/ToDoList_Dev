// contenttypecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "contenttypecombobox.h"

#include "ContentMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox

CContentTypeComboBox::CContentTypeComboBox(const CContentMgr* pContentMgr) : 
	m_pContentMgr(pContentMgr), m_nInitSel(0)
{
}

CContentTypeComboBox::~CContentTypeComboBox()
{
}


BEGIN_MESSAGE_MAP(CContentTypeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CContentTypeComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox message handlers

int CContentTypeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_pContentMgr)
		return -1;
	
	FillCombo();
	
	return 0;
}

void CContentTypeComboBox::FillCombo()
{
	if (CComboBox::GetCount())
		return;

	ASSERT (m_pContentMgr);

	if (m_pContentMgr)
	{
		for (int nItem = 0; nItem < m_pContentMgr->GetNumContent(); nItem++)
		{
			CString sItem = m_pContentMgr->GetContentTypeDescription(nItem);
			AddString(sItem);
		}
	}

	if (m_nInitSel < GetCount())
		SetCurSel(m_nInitSel);
}

void CContentTypeComboBox::PreSubclassWindow() 
{
	if (m_pContentMgr)
		FillCombo();
	
	CComboBox::PreSubclassWindow();
}

int CContentTypeComboBox::GetSelectedFormat(CONTENTFORMAT& cf) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
		cf = m_pContentMgr->GetContentFormat(nSel);

	return nSel;
}

int CContentTypeComboBox::SetSelectedFormat(LPCTSTR szTypeID)
{
	int nSel = m_pContentMgr ? m_pContentMgr->FindContent(szTypeID) : CB_ERR;

	SetCurSel(nSel);

	return nSel;
}

int CContentTypeComboBox::GetCount() const
{
	if (m_pContentMgr)
		return m_pContentMgr->GetNumContent();

	// else
	return CComboBox::GetCount();
}

void CContentTypeComboBox::SetCurSel(int nSel)
{
	if (GetSafeHwnd())
		CComboBox::SetCurSel(nSel);
	else
		m_nInitSel = nSel;
}

int CContentTypeComboBox::GetCurSel() const
{
	if (GetSafeHwnd())
		return CComboBox::GetCurSel();
	
	// else
	return m_nInitSel;
}
