// contenttypecombobox.cpp : implementation file
//

#include "stdafx.h"
#include "contenttypecombobox.h"
#include "ContentMgr.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int ICON_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox

CContentTypeComboBox::CContentTypeComboBox(const CContentMgr* pContentMgr, UINT nNullIconID) 
	:
	m_pContentMgr(pContentMgr), 
	m_iconNull(nNullIconID, 16)
{
}

CContentTypeComboBox::~CContentTypeComboBox()
{
}


BEGIN_MESSAGE_MAP(CContentTypeComboBox, COwnerdrawComboBoxBase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentTypeComboBox message handlers

void CContentTypeComboBox::BuildCombo()
{
	if (COwnerdrawComboBoxBase::GetCount())
		return;

	ASSERT (m_pContentMgr);

	if (m_pContentMgr)
	{
		for (int nContent = 0; nContent < m_pContentMgr->GetNumContent(); nContent++)
		{
			CString sItem = m_pContentMgr->GetContentDescription(nContent);
			VERIFY(CDialogHelper::AddStringT(*this, sItem, nContent) != CB_ERR);
		}
	}

	SetCurSel(0);
}

int CContentTypeComboBox::GetSelectedFormat(CONTENTFORMAT& cf) const
{
	int nSel = GetCurSel();

	if (nSel != CB_ERR)
	{
		int nContent = GetItemData(nSel);
		cf = m_pContentMgr->GetContentFormat(nContent);
	}

	return nSel;
}

int CContentTypeComboBox::SetSelectedFormat(const CONTENTFORMAT& cf)
{
	int nContent = (m_pContentMgr ? m_pContentMgr->FindContent(cf) : -1);

	if (nContent == -1)
		return SetCurSel(CB_ERR);

	// else
	CheckBuildCombo();

	return CDialogHelper::SelectItemByDataT(*this, nContent);
}

int CContentTypeComboBox::GetCount() const
{
	if (m_pContentMgr)
		return m_pContentMgr->GetNumContent();

	// else
	return COwnerdrawComboBoxBase::GetCount();
}

void CContentTypeComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
										  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// Draw image
	if (nItem != CB_ERR)
	{
		int nContent = (int)dwItemData;
	
		if (nContent != -1)
		{
			GraphicsMisc::DrawCentred(&dc,
									  m_pContentMgr->GetContentIcon(nContent),
									  rect,
									  FALSE,
									  TRUE); // vertically centred
		}
	}

	CRect rText(rect);
	rText.left += (ICON_SIZE + 2);

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
}

int CContentTypeComboBox::GetExtraListboxWidth() const
{
	return (COwnerdrawComboBoxBase::GetExtraListboxWidth() + ICON_SIZE + 2);
}

int CContentTypeComboBox::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = COwnerdrawComboBoxBase::CalcMinItemHeight(bList);

	if (bList)
		nMinHeight = max(nMinHeight, (ICON_SIZE + 2));

	return nMinHeight;
}

