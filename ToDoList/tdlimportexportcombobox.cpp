// tdlimportexportcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "tdlimportexportcombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox

CTDLImportExportComboBox::CTDLImportExportComboBox(const CImportExportMgr& mgrImpExp, BOOL bImport, BOOL bFileBasedOnly)
	: 
	COwnerdrawComboBoxBase(11),
	m_mgrImpExp(mgrImpExp), 
	m_bImporting(bImport), 
	m_bFileBasedOnly(bFileBasedOnly)
{
}

CTDLImportExportComboBox::~CTDLImportExportComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLImportExportComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CTDLImportExportComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportExportComboBox message handlers

void CTDLImportExportComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	COwnerdrawComboBoxBase::PreSubclassWindow();
}

int CTDLImportExportComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COwnerdrawComboBoxBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLImportExportComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState, 
								DWORD dwItemData, const CString& sItem, BOOL bList)
{
	int nImpExp = (int)dwItemData;

	// draw icon
	CPoint ptDraw(rect.TopLeft());
	
	if (!bList)
		ptDraw.y--;
	
	HICON hIcon = GetImpExpIcon(nImpExp);
	
	if (hIcon)
	{
		::DrawIconEx(dc, ptDraw.x, ptDraw.y, hIcon, 16, 16, 0, NULL, DI_NORMAL);
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
	rText.left += 20; // for icon always

	COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList);
}

void CTDLImportExportComboBox::BuildCombo()
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

			int nItem = AddString(sItem);
			SetItemData(nItem, nImpExp);
		}
	}
}

CString CTDLImportExportComboBox::GetImpExpMenuText(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterMenuText(nImpExp);

	// else
	return m_mgrImpExp.GetExporterMenuText(nImpExp);
}

CString CTDLImportExportComboBox::GetImpExpFileExtension(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterFileExtension(nImpExp);

	// else
	return m_mgrImpExp.GetExporterFileExtension(nImpExp);
}

HICON CTDLImportExportComboBox::GetImpExpIcon(int nImpExp) const
{
	if (m_bImporting)
		return m_mgrImpExp.GetImporterIcon(nImpExp);

	// else
	return m_mgrImpExp.GetExporterIcon(nImpExp);
}
