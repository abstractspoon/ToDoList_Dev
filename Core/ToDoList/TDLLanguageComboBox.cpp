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

#include "..\3rdParty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const CString DEFAULT_LANG(_T("English (UK)"));

CString CTDLLanguageComboBox::GetDefaultLanguage()
{
	return DEFAULT_LANG;
}

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageComboBox

CTDLLanguageComboBox::CTDLLanguageComboBox(LPCTSTR szFilter) : m_sSelLanguage(DEFAULT_LANG), m_sFilter(szFilter)
{
}

CTDLLanguageComboBox::~CTDLLanguageComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLLanguageComboBox, CComboBoxEx)
	//{{AFX_MSG_MAP(CTDLLanguageComboBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageComboBox message handlers

void CTDLLanguageComboBox::PreSubclassWindow() 
{
	CComboBoxEx::PreSubclassWindow();

	CLocalizer::EnableTranslation(*this, FALSE);
	BuildLanguageList();
}

int CTDLLanguageComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CLocalizer::EnableTranslation(*this, FALSE);
	BuildLanguageList();
	
	return 0;
}

CString CTDLLanguageComboBox::GetTranslationFolder()
{
	return FileMisc::GetAppResourceFolder(_T("Resources\\Translations"));
}

void CTDLLanguageComboBox::BuildLanguageList()
{
	if (GetCount())
		return; // already done

	// build the language list from csv files in the Resources\Translations folder
	// These will come out sorted by default
	CString sFolder = GetTranslationFolder();
	CStringArray aFiles;

	int nNumFiles = FileMisc::FindFiles(sFolder, aFiles, FALSE, m_sFilter);
	int bDefaultAdded = FALSE;
	
	for (int nFile = 0; nFile < nNumFiles; nFile++)
	{
		CString sFileName;
		FileMisc::SplitPath(aFiles[nFile], NULL, NULL, &sFileName, NULL);

		// Because CComboBoxEx does not support sorting, we need
		// to dynamically determine the default language position
		if (!bDefaultAdded && (DEFAULT_LANG.CompareNoCase(sFileName) < 0))
		{
			CBitmap bm;
			bm.LoadBitmap(IDB_UK_FLAG);

			AddString(DEFAULT_LANG, (HBITMAP)bm.Detach(), LANG_ENGLISH, colorMagenta);

			bDefaultAdded = TRUE;
		}

		// load icon file
		CString sIconPath(aFiles[nFile]);
		FileMisc::ReplaceExtension(sIconPath, _T("png"));

		HBITMAP hbmFlag = CEnBitmap::LoadImageFile(sIconPath);

		LANGID nLangID = GetLanguageID(aFiles[nFile]);
		AddString(sFileName, hbmFlag, nLangID);
	}

	if (!bDefaultAdded)
	{
		CBitmap bm;
		bm.LoadBitmap(IDB_UK_FLAG);

		AddString(DEFAULT_LANG, (HBITMAP)bm.Detach(), LANG_ENGLISH, colorMagenta);
	}

	m_il.ScaleByDPIFactor();
	SetImageList(&m_il);

	SelectLanguage(m_sSelLanguage);
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

	if (!Misc::Split(aHeader[1], sLangID, ' ') || (aHeader[1] != _T("PRIMARY_LANGID")))
	{
		ASSERT(0);
		return LANG_NEUTRAL;
	}

	return (LANGID)PRIMARYLANGID(_ttoi(sLangID));
}

int CTDLLanguageComboBox::AddString(LPCTSTR szLanguage, HBITMAP hbmFlag, LANGID nLangID, COLORREF crBack)
{
	// create and associate the image list first time around
	if (m_il.GetSafeHandle() == NULL)
	{
		m_il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
		SetImageList(&m_il);
	}

	COMBOBOXEXITEM cbe;

	cbe.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT | CBEIF_LPARAM;
	cbe.iItem = GetCount();
	cbe.pszText = (LPTSTR)szLanguage;
	cbe.lParam = nLangID;
	cbe.iImage = cbe.iSelectedImage = GetCount();

	if (hbmFlag == NULL)
	{
		CBitmap bm;
		bm.LoadBitmap(IDB_YOURLANG_FLAG);

		hbmFlag = (HBITMAP)bm.Detach();
		crBack = colorMagenta;
	}

	CBitmap tmp;
	tmp.Attach(hbmFlag); // will auto cleanup
	m_il.Add(&tmp, crBack);

	return InsertItem(&cbe);
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
		GetLBText(nSel, m_sSelLanguage);

	CComboBoxEx::OnDestroy();
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

	if (GetSafeHwnd())
	{
		int nSel = GetCurSel();

		if (nSel == -1)
			m_sSelLanguage = DEFAULT_LANG;
		else
			GetLBText(nSel, m_sSelLanguage);
	}

	LPCTSTR szExt = (m_sFilter.Find(_T(".csv")) != -1) ? _T("csv") : _T("txt");
	return GetSelectedLanguageFile(m_sSelLanguage, szExt, bRelative);
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
		CString sLanguage;
		GetLBText(nIndex, sLanguage);

		if (sLanguage.CompareNoCase(szLanguage) == 0)
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
