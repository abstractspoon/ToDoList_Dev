// tdliconcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "tdliconcombobox.h"
#include "tdcimagelist.h"
#include "tdcstruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLIconComboBox

CTDLIconComboBox::CTDLIconComboBox(const CTDCImageList& ilImages, BOOL bMultiSel)
	:
	CEnCheckComboBox(bMultiSel),
	m_ilImages(ilImages)
{
	m_sNone.Empty();
	m_sAny.Empty();
}

CTDLIconComboBox::~CTDLIconComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLIconComboBox, CEnCheckComboBox)
	//{{AFX_MSG_MAP(CTDLIconComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLIconComboBox message handlers

void CTDLIconComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList)
{
	CRect rImage(rect);
	const int nImgSpace = 18;

	CStringArray aImages;
	int nNumImage = 0;
	
	if (bList)
	{
		if (m_bMultiSel)
		{
			// draw checkbox with no text
			DrawCheckBox(dc, rect, nItem, dwItemData);

			// update image rect
			rImage.left += nImgSpace;
		}

		aImages.Add(sItem);
		nNumImage = 1;
	}
	else // static text portion
	{
		if (m_bMultiSel)
		{
			nNumImage = Misc::Split(m_sText, aImages);
		}
		else if (!sItem.IsEmpty())
		{
			aImages.Add(sItem);
			nNumImage = 1;
		}
	}

	for (int nImg = 0; nImg < nNumImage; nImg++)
	{
		CString sImage, sName;
		
		if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aImages[nImg], sImage, sName))
		{
			// draw image
			if (m_ilImages.GetSafeHandle() && !sImage.IsEmpty())
			{
				CPoint pt = rImage.TopLeft();
				pt.y--;

				m_ilImages.Draw(&dc, sImage, pt, ILD_TRANSPARENT);
				rImage.left += nImgSpace;
			}

			// draw optional text
			if (bList && !sName.IsEmpty())
				COwnerdrawComboBoxBase::DrawItemText(dc, rImage, nItem, nItemState, dwItemData, sName, bList);
		}
	}
}

int CTDLIconComboBox::SelectImage(const CString& sImage)
{
	CString sPartial = TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(sImage, _T(""));

	return CComboBox::SelectString(-1, sPartial);
}

CString CTDLIconComboBox::GetSelectedImage() const
{
	CString sItem;
	GetLBText(GetCurSel(), sItem);
	
	if (sItem.IsEmpty())
		GetWindowText(sItem);

	if (!sItem.IsEmpty())
	{
		CString sImage, sName;
		TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(sItem, sImage, sName);
		
		return sImage;
	}

	// else
	return sItem; // empty
}

int CTDLIconComboBox::GetChecked(CStringArray& aItems) const
{
	CStringArray aTemp;
	int nNumItems = CEnCheckComboBox::GetChecked(aTemp);

	aItems.RemoveAll();

	for (int nImg = 0; nImg < nNumItems; nImg++)
	{
		CString sImage, sName;
		TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aTemp[nImg], sImage, sName);

		aItems.Add(sImage);
	}

	return aItems.GetSize();
}

BOOL CTDLIconComboBox::SetChecked(const CStringArray& aItems)
{
	// clear existing checks first but don't update window
	int nCount = GetCount();
	
	for (int i = 0; i < nCount; i++)
		CCheckComboBox::SetCheck(i, FALSE, FALSE);

	// assume that all the correct items have already 
	// been added to the list
	int nItem = aItems.GetSize();

	while (nItem--)
	{
		CString sPartial = TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(aItems[nItem], _T(""));
		int nIndex = FindString(-1, sPartial);

		if (nIndex != CB_ERR)
			SetCheck(nIndex, TRUE);
		else
			return FALSE;
	}

	return TRUE;
}
