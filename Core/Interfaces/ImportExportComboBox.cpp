// tdlimportexportcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "importexportcombobox.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\Misc.h"
#include "..\shared\FileMisc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\FileIcons.h"
#include "..\shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int ICON_SIZE = GraphicsMisc::ScaleByDPIFactor(16);

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox

CImportExportComboBox::CImportExportComboBox(const CImportExportMgr& mgrImpExp, BOOL bImport, 
											 BOOL bFileBasedOnly, LPCTSTR szFileExts)
	: 
	m_mgrImpExp(mgrImpExp), 
	m_bImporting(bImport), 
	m_bFileBasedOnly(FALSE)
{
	SetFileBasedOnly(bFileBasedOnly, szFileExts);
}

CImportExportComboBox::~CImportExportComboBox()
{
}


BEGIN_MESSAGE_MAP(CImportExportComboBox, COwnerdrawComboBoxBase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox message handlers

void CImportExportComboBox::SetFileBasedOnly(BOOL bFileBased, LPCTSTR szFileExts)
{
	ASSERT(bFileBased || (szFileExts == NULL));

	m_bFileBasedOnly = bFileBased;

	if (bFileBased && !Misc::IsEmpty(szFileExts))
	{
		int nExt = Misc::Split(szFileExts, m_aFileExt, ';');

		while (nExt--)
			m_aFileExt[nExt] = FileMisc::GetExtension(m_aFileExt[nExt], FALSE);
	}
	else
	{
		m_aFileExt.RemoveAll();
	}

	RebuildCombo();
}

void CImportExportComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CRect rText(rect);

	if (nItem != -1)
	{
		int nImpExp = (int)dwItemData;

		// draw icon
		if (!GraphicsMisc::DrawCentred(&dc, 
									   GetImpExpIcon(nImpExp),
									   rect,
									   FALSE,
									   TRUE))
		{
			// fallback on file handler icon
			CString sFileExt = GetImpExpFileExtension(nImpExp, TRUE);

			if (!sFileExt.IsEmpty())
			{
				CRect rIcon = GraphicsMisc::CalcCentredRect(ICON_SIZE, rect, FALSE, TRUE);
				CFileIcons::Draw(&dc, sFileExt, rIcon.TopLeft());
			}
		}

		// draw text
		rText.left += (ICON_SIZE + 4); // for icon always
	}

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
}

void CImportExportComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	// Cache current selection
	CString sSelTypeID = GetSelectedTypeID();
	int nNumImpExp = (m_bImporting ? m_mgrImpExp.GetNumImporters() : m_mgrImpExp.GetNumExporters());

	for (int nImpExp = 0; nImpExp < nNumImpExp; nImpExp++)
	{
		CString sExt = GetImpExpFileExtension(nImpExp, FALSE);

		if (m_bFileBasedOnly)
		{
			if (sExt.IsEmpty())
				continue;

			if (m_aFileExt.GetSize() && !Misc::Contains(sExt, m_aFileExt, FALSE, TRUE))
				continue;
		}

		CString sItem, sMenu(GetImpExpMenuText(nImpExp));

		if (!sExt.IsEmpty())
			sItem.Format(_T("%s (.%s)"), sMenu, sExt);
		else
			sItem = sMenu;

		CDialogHelper::AddStringT(*this, sItem, nImpExp);
	}

	// Restore previous selection
	if (SetSelectedTypeID(sSelTypeID) == CB_ERR)
		VERIFY(SetCurSel(0) != CB_ERR);

	CLocalizer::EnableTranslation(*this, FALSE);
}

CString CImportExportComboBox::GetImpExpMenuText(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterMenuText(nImpExp);

	// else
	return m_mgrImpExp.GetExporterMenuText(nImpExp);
}

CString CImportExportComboBox::GetImpExpFileExtension(int nImpExp, BOOL bWithDot) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterFileExtension(nImpExp, bWithDot);

	// else
	return m_mgrImpExp.GetExporterFileExtension(nImpExp, bWithDot);
}

HICON CImportExportComboBox::GetImpExpIcon(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterIcon(nImpExp);

	// else
	return m_mgrImpExp.GetExporterIcon(nImpExp);
}

CString CImportExportComboBox::GetSelectedTypeID() const
{
	if (GetCurSel() == CB_ERR)
		return _T("");

	int nImpExp = CDialogHelper::GetSelectedItemDataT(*this, 0);
		
	return (m_bImporting ? 
			m_mgrImpExp.GetImporterTypeID(nImpExp) :
			m_mgrImpExp.GetExporterTypeID(nImpExp));
}

int CImportExportComboBox::SetSelectedTypeID(LPCTSTR szTypeID)
{
	CheckBuildCombo();

	return SetCurSel(FindItem(szTypeID));
}

int CImportExportComboBox::FindItem(LPCTSTR szTypeID) const
{
	int nItem = CB_ERR;

	if (!Misc::IsEmpty(szTypeID))
	{
		int nImpExp = -1;

		if (m_bImporting)
			nImpExp = m_mgrImpExp.FindImporterByType(szTypeID);
		else
			nImpExp = m_mgrImpExp.FindExporterByType(szTypeID);

		nItem = CDialogHelper::FindItemByDataT(*this, nImpExp);
	}

	return nItem;
}

void CImportExportComboBox::DDX(CDataExchange* pDX, CString& value)
{
	if (pDX->m_bSaveAndValidate)
		value = GetSelectedTypeID();
	else
		SetSelectedTypeID(value);
}

int CImportExportComboBox::GetExtraListboxWidth() const
{
	return (COwnerdrawComboBoxBase::GetExtraListboxWidth() + ICON_SIZE + 2);
}

int CImportExportComboBox::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = COwnerdrawComboBoxBase::CalcMinItemHeight(bList);

	if (bList)
		nMinHeight = max(nMinHeight, (ICON_SIZE + 2));

	return nMinHeight;
}

