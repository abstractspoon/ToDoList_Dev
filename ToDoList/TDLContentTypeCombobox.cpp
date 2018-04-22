// contenttypecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "tdlcontenttypecombobox.h"
#include "resource.h"

#include "..\shared\ContentMgr.h"
#include "..\shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int IMAGESIZE = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox

CTDLContentTypeComboBox::CTDLContentTypeComboBox(const CContentMgr* pContentMgr) : 
	m_pContentMgr(pContentMgr), m_nInitSel(0)
{
	SetMinDLUHeight(11);
}

CTDLContentTypeComboBox::~CTDLContentTypeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLContentTypeComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CContentTypeComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox message handlers

int CTDLContentTypeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_pContentMgr)
		return -1;
	
	FillCombo();
	
	return 0;
}

void CTDLContentTypeComboBox::FillCombo()
{
	if (COwnerdrawComboBoxBase::GetCount())
		return;

	ASSERT (m_pContentMgr);

	if (m_pContentMgr)
	{
		VERIFY(m_ilContent.Create(IMAGESIZE, IMAGESIZE, (ILC_MASK | ILC_COLOR32), 1, 1));
		
		for (int nContent = 0; nContent < m_pContentMgr->GetNumContent(); nContent++)
		{
			CString sItem = m_pContentMgr->GetContentDescription(nContent);
			int nItem = AddString(sItem);

			SetItemData(nItem, nContent);

			HICON hIcon = m_pContentMgr->GetContentIcon(nContent);

			if (hIcon == NULL)
				hIcon = AfxGetApp()->LoadIcon(IDI_NULL);

			VERIFY(m_ilContent.Add(hIcon) == nContent);
		}
	}

	if (m_nInitSel < GetCount())
		SetCurSel(m_nInitSel);
}

void CTDLContentTypeComboBox::PreSubclassWindow() 
{
	if (m_pContentMgr)
		FillCombo();
	
	COwnerdrawComboBoxBase::PreSubclassWindow();
}

int CTDLContentTypeComboBox::GetSelectedFormat(CONTENTFORMAT& cf) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
		cf = m_pContentMgr->GetContentFormat(nSel);

	return nSel;
}

int CTDLContentTypeComboBox::SetSelectedFormat(const CONTENTFORMAT& cf)
{
	int nSel = m_pContentMgr ? m_pContentMgr->FindContent(cf) : CB_ERR;

	SetCurSel(nSel);

	return nSel;
}

int CTDLContentTypeComboBox::GetCount() const
{
	if (m_pContentMgr)
		return m_pContentMgr->GetNumContent();

	// else
	return COwnerdrawComboBoxBase::GetCount();
}

void CTDLContentTypeComboBox::SetCurSel(int nSel)
{
	if (GetSafeHwnd())
		COwnerdrawComboBoxBase::SetCurSel(nSel);
	else
		m_nInitSel = nSel;
}

int CTDLContentTypeComboBox::GetCurSel() const
{
	if (GetSafeHwnd())
		return COwnerdrawComboBoxBase::GetCurSel();
	
	// else
	return m_nInitSel;
}

void CTDLContentTypeComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// Draw image
	if (nItem != CB_ERR)
	{
		int nImage = (int)dwItemData;
	
		if (nImage != -1)
		{
			CRect rImage(rect);
			rImage.bottom = rImage.top + IMAGESIZE;

			GraphicsMisc::CentreRect(rImage, rect, FALSE, TRUE);
			m_ilContent.Draw(&dc, nImage, rImage.TopLeft(), ILD_TRANSPARENT);
		}
	}

	CRect rText(rect);
	rText.left += (IMAGESIZE + 2);

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
}


