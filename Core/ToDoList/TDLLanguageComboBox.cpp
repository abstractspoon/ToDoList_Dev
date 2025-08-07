// TDLLanguageComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLLanguageComboBox.h"

#include "..\Shared\filemisc.h"
#include "..\Shared\enbitmap.h"
#include "..\Shared\localizer.h"
#include "..\Shared\dialoghelper.h"
#include "..\Shared\misc.h"
#include "..\Shared\graphicsmisc.h"

#include "..\3rdParty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const CString DEFAULT_LANG(_T("English (UK)"));

static const CString NEED_TRANSLATION	= _T("NEED_TRANSLATION");
static const CString PRIMARY_LANGID		= _T("PRIMARY_LANGID");
static const CString TRANSLATED			= _T("TRANSLATED");

const int FLAG_SIZE		= GraphicsMisc::ScaleByDPIFactor(16);
const int COL_SPACING	= GraphicsMisc::ScaleByDPIFactor(10);

/////////////////////////////////////////////////////////////////////////////

CString CTDLLanguageComboBox::GetDefaultLanguage()
{
	return DEFAULT_LANG;
}

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageComboBox

CTDLLanguageComboBox::CTDLLanguageComboBox(LPCTSTR szFilter) 
	: 
	COwnerdrawComboBoxBase(30, 0),
	m_sSelLanguage(DEFAULT_LANG), 
	m_sFilter(szFilter),
	m_nLangCountryColWidth(0)
{
}

CTDLLanguageComboBox::~CTDLLanguageComboBox()
{
}

BEGIN_MESSAGE_MAP(CTDLLanguageComboBox, COwnerdrawComboBoxBase)
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageComboBox message handlers

CString CTDLLanguageComboBox::GetTranslationFolder()
{
	return FileMisc::GetAppResourceFolder(_T("Resources\\Translations"));
}

BOOL CTDLLanguageComboBox::OnDropDown()
{
	InitialiseDropWidth();

	return FALSE; // continue routing
}

void CTDLLanguageComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	// build the language list from csv files in the Resources\Translations folder
	// These will come out sorted by default
	CString sFolder = GetTranslationFolder();
	CStringArray aFiles;

	int nNumFiles = FileMisc::FindFiles(sFolder, aFiles, FALSE, m_sFilter);
	int bDefaultAdded = FALSE;

	int nHeading = AddString(CEnString(IDS_LANGCOMBO_LANGCOUNTRY), 0x00, CEnString(IDS_LANGCOMBO_COMPLETION), NULL);
	SetHeadingItem(nHeading);
	
	for (int nFile = 0; nFile < nNumFiles; nFile++)
	{
		CString sFilePath(aFiles[nFile]), sFileName;
		FileMisc::SplitPath(sFilePath, NULL, NULL, &sFileName, NULL);

		// Because COwnerdrawComboBoxBase does not support sorting, we need
		// to dynamically determine the default language position
		if (!bDefaultAdded && (DEFAULT_LANG.CompareNoCase(sFileName) < 0))
			bDefaultAdded = AddDefaultLanguage();

		// load icon file
		CString sIconPath(sFilePath);
		FileMisc::ReplaceExtension(sIconPath, _T("png"));

		HBITMAP hbmFlag = CEnBitmap::LoadImageFile(sIconPath);
		LANGID nLangID = GetLanguageID(sFilePath);
		CString sPercent = GetPercentTranslated(sFilePath);

		AddString(sFileName, nLangID, sPercent, hbmFlag);
	}

	if (!bDefaultAdded)
		AddDefaultLanguage();

	m_il.ScaleByDPIFactor();

	SelectLanguage(m_sSelLanguage);
}

BOOL CTDLLanguageComboBox::AddDefaultLanguage()
{
	CBitmap bm;
	bm.LoadBitmap(IDB_UK_FLAG);

	return (CB_ERR != AddString(DEFAULT_LANG, LANG_ENGLISH, _T("100%"), (HBITMAP)bm.Detach(), colorMagenta));
}

LANGID CTDLLanguageComboBox::GetLanguageID(const CString& sTransFile)
{
	CStringArray aHeader;

	if (FileMisc::LoadFile(sTransFile, aHeader, 2) != 2)
	{
		ASSERT(0);
		return LANG_NEUTRAL;
	}

	// 2nd line is language ID
	CString sLangID;

	if (!Misc::Split(aHeader[1], sLangID, ' ') || (aHeader[1] != PRIMARY_LANGID))
	{
		ASSERT(0);
		return LANG_NEUTRAL;
	}

	return (LANGID)PRIMARYLANGID(_ttoi(sLangID));
}

CString CTDLLanguageComboBox::GetPercentTranslated(const CString& sTransFile)
{
	CString sPercent;
	CStringArray aHeader;

	if (FileMisc::LoadFile(sTransFile, aHeader, 4) == 4)
	{
		// If 4th line is NEED_TRANSLATION it's followed by the current UNTRANSLATED percent
		Misc::Split(aHeader[3], sPercent, ' ');
	
		if (aHeader[3] != NEED_TRANSLATION)
		{
			ASSERT(aHeader[3] == TRANSLATED);
			sPercent = _T("100%"); // all translated
		}
		else if (!sPercent.IsEmpty())
		{
			ASSERT(Misc::IsNumber(sPercent));
			sPercent.Format(_T("%d%%"), (100 - _ttoi(sPercent)));
		}
	}
	else
	{
		ASSERT(0);
	}

	return sPercent;
}

int CTDLLanguageComboBox::AddString(LPCTSTR szLanguage, LANGID nLangID, const CString& sCompletion, HBITMAP hbmFlag, COLORREF crBack)
{
	int nItem = COwnerdrawComboBoxBase::AddString(szLanguage);

	// create the image list first time around
	if (m_il.GetSafeHandle() == NULL)
		m_il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);

	if (hbmFlag == NULL)
	{
		CBitmap bm;
		bm.LoadBitmap(IDB_YOURLANG_FLAG);

		hbmFlag = (HBITMAP)bm.Detach();
		crBack = colorMagenta;
	}

	CBitmap tmp;
	tmp.Attach(hbmFlag); // will auto cleanup
	VERIFY(m_il.Add(&tmp, crBack) == nItem);

	LCB_ITEMDATA* pItemData = (LCB_ITEMDATA*)GetAddExtItemData(nItem);
	ASSERT(pItemData);

	if (pItemData)
	{
		pItemData->dwItemData = nLangID;
		pItemData->sCompletion = sCompletion;
	}

	return nItem;
}

void CTDLLanguageComboBox::SelectLanguageFile(LPCTSTR szFile)
{
	if (GetDefaultLanguage() == szFile)
	{
		if (GetSafeHwnd())
			SelectLanguage(szFile);
		else
			m_sSelLanguage = szFile;
	}
	else
	{
		CString sPath = FileMisc::GetFullPath(szFile, FileMisc::GetAppFolder());
		
		if (FileMisc::FileExists(sPath))
		{
			FileMisc::SplitPath(szFile, NULL, NULL, &m_sSelLanguage, NULL);

			if (GetSafeHwnd())
				SelectLanguage(m_sSelLanguage);
		}
	}
}

void CTDLLanguageComboBox::OnDestroy() 
{
	int nSel = GetCurSel();

	if (nSel == -1)
		m_sSelLanguage = DEFAULT_LANG;
	else
		m_sSelLanguage = CDialogHelper::GetItem(*this, nSel);

	m_nLangCountryColWidth = 0;

	COwnerdrawComboBoxBase::OnDestroy();
}

CString CTDLLanguageComboBox::GetSelectedLanguageFile(LPCTSTR szLanguage, LPCTSTR szExt, BOOL bRelative) // static
{
	ASSERT(!Misc::IsEmpty(szLanguage));

	CString sLanguageFile(GetDefaultLanguage());

	if (GetDefaultLanguage() != szLanguage)
	{
		if (bRelative)
			FileMisc::MakePath(sLanguageFile, NULL, _T(".\\Resources\\Translations"), szLanguage, szExt);
		else
			FileMisc::MakePath(sLanguageFile, NULL, GetTranslationFolder(), szLanguage, szExt);
	}

	return sLanguageFile;
}

LANGID CTDLLanguageComboBox::GetSelectedPrimaryLanguageID() const
{
	return (LANGID)CDialogHelper::GetSelectedItemData(*this);
}

CString CTDLLanguageComboBox::GetSelectedLanguageFile(BOOL bRelative) const
{
	CString sLanguageFile(GetDefaultLanguage());

	if (GetSafeHwnd() && GetCount())
	{
		int nSel = GetCurSel();

		if (nSel == -1)
			m_sSelLanguage = DEFAULT_LANG;
		else
			m_sSelLanguage = CDialogHelper::GetItem(*this, nSel);
	}

	LPCTSTR szExt = (m_sFilter.Find(_T(".csv")) != -1) ? _T("csv") : _T("txt");
	return GetSelectedLanguageFile(m_sSelLanguage, szExt, bRelative);
}

void CTDLLanguageComboBox::GetLanguageAndCountry(const CString& sItem, CString& sLanguage, CString& sCountry)
{
	sLanguage = sItem;

	Misc::Split(sLanguage, sCountry, '(');
	sCountry.TrimRight(')');

	ASSERT(!sLanguage.IsEmpty() && (!sCountry.IsEmpty() || (sLanguage == _T("YourLanguage"))));
}

BOOL CTDLLanguageComboBox::IsDefaultLanguageSelected() const
{
	return (GetSelectedLanguageFile() == DEFAULT_LANG);
}

int CTDLLanguageComboBox::SelectLanguage(LPCTSTR szLanguage)
{
	int nIndex = GetCount();

	while (nIndex--)
	{
		if (Misc::Find(szLanguage, CDialogHelper::GetItem(*this, nIndex)) == 0)
		{
			SetCurSel(nIndex);
			return nIndex;
		}
	}

	if (GetCurSel() == -1)
		SetCurSel(0);

	return 0;
}

BOOL CTDLLanguageComboBox::SelectUserLanguage()
{
	if (GetSafeHwnd())
	{
		LANGID nLangID = (LANGID)PRIMARYLANGID(Misc::GetUserDefaultUILanguage());

		if (nLangID == LANG_ENGLISH)
		{
			// Try current keyboard layout
			nLangID = (LANGID)PRIMARYLANGID(Misc::GetUserKeyboardLanguage());
		}

		if (CDialogHelper::FindItemByDataT(*this, nLangID) == CB_ERR)
			nLangID = LANG_ENGLISH;
		
		return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, nLangID));
	}

	// else
	return FALSE;
}

BOOL CTDLLanguageComboBox::HasLanguages()
{
	return FileMisc::FolderContainsFiles(GetTranslationFolder(), FALSE, _T("*.csv"));
}

int CTDLLanguageComboBox::CalcMinItemHeight(BOOL bList) const
{
	int nMinHeight = COwnerdrawComboBoxBase::CalcMinItemHeight(bList);

	if (bList)
		nMinHeight = max(nMinHeight, (FLAG_SIZE + 2));

	return nMinHeight;
}

void CTDLLanguageComboBox::InitialiseDropWidth()
{
	if (m_nLangCountryColWidth == 0)
	{
		CClientDC dc(this);
		HFONT hOldFont = GraphicsMisc::PrepareDCFont(&dc, *this);

		// Completion column always defined by heading text
		int nDoneColWidth = MulDiv(dc.GetTextExtent(CEnString(IDS_LANGCOMBO_COMPLETION)).cx, 3, 2);
		int nMaxLangColWidth = 0, nMaxCountryColWidth = 0;
		int nIndex = GetCount();

		while (nIndex--)
		{
			CString sLanguage, sCountry;
			GetLanguageAndCountry(CDialogHelper::GetItem(*this, nIndex), sLanguage, sCountry);

			// Language
			int nWidth = dc.GetTextExtent(sLanguage).cx;

			if (nIndex == 0) // heading
				nWidth = MulDiv(nWidth, 3, 2); // ~ bold

			nMaxLangColWidth = max(nWidth, nMaxLangColWidth);

			// Country
			nWidth = dc.GetTextExtent(sCountry).cx;

			if (nIndex == 0) // heading
				nWidth = MulDiv(nWidth, 3, 2); // ~ bold

			nMaxCountryColWidth = max(nWidth, nMaxCountryColWidth);
		}
		dc.SelectObject(hOldFont);

		m_nLangCountryColWidth = max(nMaxLangColWidth, nMaxCountryColWidth);

		int nReqWidth = ((2 * m_nLangCountryColWidth) +
						 nDoneColWidth +
						 (FLAG_SIZE + 5) +
						 (2 * COL_SPACING));

		SetDroppedWidth(nReqWidth);
	}
}

void CTDLLanguageComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
									  DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (nItem == -1)
	{
		COwnerdrawComboBoxBase::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
		return;
	}

	LCB_ITEMDATA* pItemData = (LCB_ITEMDATA*)GetExtItemData(nItem);
	ASSERT(pItemData);

	CRect rText(rect);
	
	if (!pItemData->bHeading)
	{
		CRect rIcon(rect);
		rIcon.bottom = (rIcon.top + FLAG_SIZE);

		GraphicsMisc::CentreRect(rIcon, rText, FALSE, TRUE);
		VERIFY(m_il.Draw(&dc, nItem, rIcon.TopLeft(), ILD_TRANSPARENT));
		
		rText.left += FLAG_SIZE + 5;
	}

	if (bList)
	{
		CString sLanguage, sCountry;
		GetLanguageAndCountry(sItem, sLanguage, sCountry);

		// Draw language
		COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sLanguage, bList, crText);
		rText.left += m_nLangCountryColWidth + COL_SPACING;
		
		if (pItemData->bHeading)
			rText.left += FLAG_SIZE + 5;

		// Draw country
		COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sCountry, bList, crText);
		rText.left += m_nLangCountryColWidth + COL_SPACING;

		// Draw % completion
		COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, pItemData->sCompletion, bList, crText);
	}
	else // edit
	{
		COwnerdrawComboBoxBase::DrawItemText(dc, rText, nItem, nItemState, dwItemData, sItem, bList, crText);
	}
}
