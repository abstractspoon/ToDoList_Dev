// tdlimportexportcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "importexportcombobox.h"
#include "GraphicsMisc.h"
#include "Misc.h"
#include "DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox

CImportExportComboBox::CImportExportComboBox(const CImportExportMgr& mgrImpExp, BOOL bImport, BOOL bFileBasedOnly)
	: 
	m_mgrImpExp(mgrImpExp), 
	m_bImporting(bImport), 
	m_bFileBasedOnly(bFileBasedOnly)
{
}

CImportExportComboBox::~CImportExportComboBox()
{
}


BEGIN_MESSAGE_MAP(CImportExportComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLImportExportComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox message handlers

void CImportExportComboBox::SetFileBasedOnly(BOOL bFileBased)
{
	m_bFileBasedOnly = bFileBased;

	if (GetSafeHwnd())
	{
		ResetContent();
		BuildCombo();
	}
}

void CImportExportComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	COwnerdrawComboBoxBase::PreSubclassWindow();
}

int CImportExportComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CImportExportComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	int nImpExp = (int)dwItemData;

	// draw icon
	CPoint ptDraw(rect.TopLeft());
	
	if (!bList)
		ptDraw.y--;
	
	HICON hIcon = GetImpExpIcon(nImpExp);
	int nImageSize = GraphicsMisc::ScaleByDPIFactor(16);
	
	if (hIcon)
	{
		::DrawIconEx(dc, ptDraw.x, ptDraw.y, hIcon, nImageSize, nImageSize, 0, NULL, DI_NORMAL);
	}
	else // fallback on file handler icon
	{
		CString sFileExt = GetImpExpFileExtension(nImpExp);
		int nImage = -1;
		
		if (!sFileExt.IsEmpty())
		{
			CString sDummyFile;
			sDummyFile.Format(_T("dummy.%s"), sFileExt);
			
			nImage = m_ilImages.GetFileImageIndex(sDummyFile, TRUE);
		}
		
		if (nImage != -1)
		{
			m_ilImages.GetImageList()->Draw(&dc, nImage, ptDraw, ILD_TRANSPARENT);
		}
	}

	// draw text
	CRect rText(rect);
	rText.left += (nImageSize + 4); // for icon always

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
}

int CImportExportComboBox::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = COwnerdrawComboBoxBase::CalcMinItemHeight(bList);

	return (nMinHeight + 2); // icon padding
}

void CImportExportComboBox::BuildCombo()
{
	// once only
	if (GetCount())
		return; 

	int nNumImpExp = (m_bImporting ? m_mgrImpExp.GetNumImporters() : m_mgrImpExp.GetNumExporters());

	for (int nImpExp = 0; nImpExp < nNumImpExp; nImpExp++)
	{
		CString sExt = GetImpExpFileExtension(nImpExp);

		if (!m_bFileBasedOnly || !sExt.IsEmpty())
		{
			CString sItem, sMenu(GetImpExpMenuText(nImpExp));

			if (!sExt.IsEmpty())
				sItem.Format(_T("%s (.%s)"), sMenu, sExt);
			else
				sItem = sMenu;

			CDialogHelper::AddString(*this, sItem, nImpExp);
		}
	}
}

CString CImportExportComboBox::GetImpExpMenuText(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterMenuText(nImpExp);

	// else
	return m_mgrImpExp.GetExporterMenuText(nImpExp);
}

CString CImportExportComboBox::GetImpExpFileExtension(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterFileExtension(nImpExp, FALSE);

	// else
	return m_mgrImpExp.GetExporterFileExtension(nImpExp, FALSE);
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

	int nImpExp = (int)CDialogHelper::GetSelectedItemData(*this);
		
	return (m_bImporting ? 
			m_mgrImpExp.GetImporterTypeID(nImpExp) :
			m_mgrImpExp.GetExporterTypeID(nImpExp));
}

int CImportExportComboBox::SetSelectedTypeID(LPCTSTR szTypeID)
{
	int nImpExp = CB_ERR;

	if (!Misc::IsEmpty(szTypeID))
	{
		if (m_bImporting)
			nImpExp = m_mgrImpExp.FindImporterByType(szTypeID);
		else
			nImpExp = m_mgrImpExp.FindExporterByType(szTypeID);
	}

	if (nImpExp == CB_ERR)
		SetCurSel(CB_ERR);
	else
		CDialogHelper::SelectItemByData(*this, nImpExp);

	return nImpExp;
}
