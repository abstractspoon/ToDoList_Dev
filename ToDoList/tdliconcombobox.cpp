// tdliconcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "tdliconcombobox.h"
#include "tdcimagelist.h"
#include "tdcstruct.h"

#include "..\Shared\GraphicsMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int IMAGE_WIDTH = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////
// CTDLIconComboBox

CTDLIconComboBox::CTDLIconComboBox(const CTDCImageList& ilImages, BOOL bMultiSel, BOOL bFilter)
	:
	CEnCheckComboBox(bMultiSel, (bFilter ? IDS_TDC_NONE : 0), (bFilter ? IDS_TDC_ANY : 0)),
	m_ilImages(ilImages)
{
}

CTDLIconComboBox::~CTDLIconComboBox()
{
}

IMPLEMENT_DYNAMIC(CTDLIconComboBox, CEnCheckComboBox)

BEGIN_MESSAGE_MAP(CTDLIconComboBox, CEnCheckComboBox)
	//{{AFX_MSG_MAP(CTDLIconComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLIconComboBox message handlers

void CTDLIconComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CRect rImage(rect);
	rImage.bottom = (rImage.top + m_ilImages.GetImageSize());

	GraphicsMisc::CentreRect(rImage, rect, FALSE, TRUE);

	CStringArray aImages;
	int nNumImage = 0;
	
	if (bList)
	{
		if (m_bMultiSel)
		{
			// draw checkbox with no text
			DrawCheckBox(dc, rect, nItem, nItemState, dwItemData, FALSE);

			// update image rect
			rImage.left += IMAGE_WIDTH;
		}

		aImages.Add(sItem);
		nNumImage = 1;
	}
	else // static text portion
	{
		if (m_bMultiSel)
		{
			nNumImage = Misc::Split(m_sText, aImages, _T(""), HasItemNone());
		}
		else if (!sItem.IsEmpty())
		{
			aImages.Add(sItem);
			nNumImage = 1;
		}
	}

	if (nNumImage > 0)
	{
		for (int nImg = 0; nImg < nNumImage; nImg++)
		{
			CString sImage, sName;

			if (TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aImages[nImg], sImage, sName))
			{
				// draw image
				if (m_ilImages.GetSafeHandle() && !sImage.IsEmpty())
				{
					CPoint pt = rImage.TopLeft();

					m_ilImages.Draw(&dc, sImage, pt, ILD_TRANSPARENT);
					rImage.left += (IMAGE_WIDTH + 2);
				}

				// draw optional text bypassing base class checkbox drawing
				if (bList && !sName.IsEmpty())
				{
					rImage.left += 2;

					CAutoComboBox::DrawItemText(dc, rImage, nItem, nItemState, dwItemData, sName, bList, crText);
				}
			}
			else if (bList)
			{
				// Draw none/any
				CEnCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
			}
			else
			{
				ASSERT(nImg == 0);
				ASSERT(HasItemNone());

				CAutoComboBox::DrawItemText(dc, rImage, nItem, nItemState, dwItemData, m_sNone, bList, crText);
				rImage.left += (dc.GetTextExtent(m_sNone).cx + 2);

				if (nNumImage > 1)
				{
					CAutoComboBox::DrawItemText(dc, rImage, nItem, nItemState, dwItemData, Misc::GetListSeparator(), bList, crText);
					rImage.left += (dc.GetTextExtent(Misc::GetListSeparator()).cx);
				}
			}
		}
	}
	else if (!bList)
	{
		CEnCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
	}
}

int CTDLIconComboBox::SelectImage(const CString& sImage)
{
	CString sPartial = TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(sImage, _T(""));

	return CComboBox::SelectString(-1, sPartial);
}

CString CTDLIconComboBox::GetSelectedImage() const
{
	CString sItem = GetItemText(GetCurSel());
	
	if (sItem.IsEmpty())
		GetWindowText(sItem);

	if (!sItem.IsEmpty())
	{
		CString sImage, sName;
		TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(sItem, sImage, sName);
		
		return sImage;
	}

	return sItem; // empty
}

int CTDLIconComboBox::GetChecked(CStringArray& aItems, CCB_CHECKSTATE nCheck) const
{
	CStringArray aTemp;
	CEnCheckComboBox::GetChecked(aTemp, nCheck);

	return DecodeImageTags(aTemp, aItems);
}

BOOL CTDLIconComboBox::SetChecked(const CStringArray& aItems)
{
	CStringArray aEncodedItems;
	EncodeImageTags(aItems, aEncodedItems, FALSE);

	return CEnCheckComboBox::SetChecked(aEncodedItems);
}

BOOL CTDLIconComboBox::SetChecked(const CStringArray& aChecked, const CStringArray& aMixed)
{
	CStringArray aEncodedChecked, aEncodedMixed;

	EncodeImageTags(aChecked, aEncodedChecked, FALSE);
	EncodeImageTags(aMixed, aEncodedMixed, FALSE);

	return CEnCheckComboBox::SetChecked(aEncodedChecked, aEncodedMixed);
}

int CTDLIconComboBox::SetStrings(const CStringArray& aItems)
{
	CStringArray aEncodedItems;
	EncodeImageTags(aItems, aEncodedItems, TRUE);

	return CEnCheckComboBox::SetStrings(aEncodedItems);
}

int CTDLIconComboBox::GetItems(CStringArray& aItems) const
{
	CStringArray aTemp;
	CEnCheckComboBox::GetItems(aTemp);

	return DecodeImageTags(aTemp, aItems);
}

int CTDLIconComboBox::EncodeImageTags(const CStringArray& aImages, CStringArray& aEncodedTags, BOOL bAdding) const
{
	int nNumItems = aImages.GetSize();
	aEncodedTags.RemoveAll();

	for (int nImg = 0; nImg < nNumItems; nImg++)
	{
		CString sImage;

		if (!aImages[nImg].IsEmpty())
		{
			sImage = TDCCUSTOMATTRIBUTEDEFINITION::EncodeImageTag(aImages[nImg], _T(""));

			if (!bAdding)
			{
				// Find the full string corresponding to this partial string
				int nFull = FindString(-1, sImage);

				if (nFull == -1)
				{
					ASSERT(0);
					continue;
				}

				CString sFullImage = GetItemText(nFull);

				if (sFullImage.Find(sImage) != 0)
				{
					ASSERT(0);
					continue;
				}

				sImage = sFullImage;
			}
		}

		aEncodedTags.Add(sImage);
	}

	ASSERT(aEncodedTags.GetSize() == aImages.GetSize());
	return aEncodedTags.GetSize();
}

int CTDLIconComboBox::DecodeImageTags(const CStringArray& aImages, CStringArray& aDecodedTags) const
{
	int nNumItems = aImages.GetSize();
	aDecodedTags.RemoveAll();

	for (int nImg = 0; nImg < nNumItems; nImg++)
	{
		CString sImage, sUnused;

		if (!aImages[nImg].IsEmpty())
			TDCCUSTOMATTRIBUTEDEFINITION::DecodeImageTag(aImages[nImg], sImage, sUnused);

		aDecodedTags.Add(sImage);
	}

	ASSERT(aDecodedTags.GetSize() == aImages.GetSize());
	return aDecodedTags.GetSize();
}
