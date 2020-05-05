// tdlimportexportcombobox.cpp : implementation file
//

#include "stdafx.h"
#include "importexportcombobox.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\Misc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\FileIcons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	//{{AFX_MSG_MAP(CTDLImportExportComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
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
			m_aFileExt[nExt].TrimLeft(_T(" *."));
	}
	else
	{
		m_aFileExt.RemoveAll();
	}

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
		CString sFileExt = GetImpExpFileExtension(nImpExp, TRUE);
		int nImage = -1;
		
		if (!sFileExt.IsEmpty())
			CFileIcons::Draw(&dc, sFileExt, ptDraw);
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

		CDialogHelper::AddString(*this, sItem, nImpExp);
	}
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

	int nImpExp = CDialogHelper::GetSelectedItemData(*this, 0);
		
	return (m_bImporting ? 
			m_mgrImpExp.GetImporterTypeID(nImpExp) :
			m_mgrImpExp.GetExporterTypeID(nImpExp));
}

int CImportExportComboBox::SetSelectedTypeID(LPCTSTR szTypeID)
{
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

		nItem = CDialogHelper::FindItemByData(*this, nImpExp);
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
