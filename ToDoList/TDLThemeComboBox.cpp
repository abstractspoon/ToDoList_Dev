// TDLThemeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLThemeComboBox.h"
#include "tdcenum.h"

#include "..\shared\holdredraw.h"
#include "..\shared\uithemefile.h"
#include "..\shared\filemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLThemeComboBox

CTDLThemeComboBox::CTDLThemeComboBox()
{
}

CTDLThemeComboBox::~CTDLThemeComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLThemeComboBox, CColorComboBox)
	//{{AFX_MSG_MAP(CTDLThemeComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLThemeComboBox message handlers

int CTDLThemeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CColorComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

void CTDLThemeComboBox::PreSubclassWindow() 
{
	CColorComboBox::PreSubclassWindow();
	
	BuildCombo();
}

CString CTDLThemeComboBox::GetThemePath() const
{
	ASSERT(GetSafeHwnd());

	CString sThemeName;
	GetWindowText(sThemeName);

	return FormatThemePath(sThemeName);
}

BOOL CTDLThemeComboBox::SetThemePath(LPCTSTR szThemePath)
{
	ASSERT(GetSafeHwnd());

	CString sFullPath = FileMisc::GetFullPath(szThemePath, GetThemeFolder());

	return (SelectString(-1, ExtractThemeName(sFullPath)) != CB_ERR);
}

CString CTDLThemeComboBox::GetThemeFolder()
{
	return FileMisc::GetAppResourceFolder(_T("Resources\\Themes"));
}

void CTDLThemeComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	CHoldRedraw hr(*this);

	CString sThemeFolder(GetThemeFolder());
	ASSERT(FileMisc::FolderExists(sThemeFolder));

	CStringArray aThemeFiles;
	int nTheme = FileMisc::FindFiles(sThemeFolder, aThemeFiles, FALSE, _T("*.xml"));

	CUIThemeFile theme;

	while (nTheme--)
	{	
		LPCTSTR szThemePath = aThemeFiles.GetData()[nTheme];

		if (theme.LoadThemeFile(aThemeFiles[nTheme]))
		{
			AddColor(theme.crAppBackDark, ExtractThemeName(szThemePath));
		}
	}
}

CString CTDLThemeComboBox::ExtractThemeName(LPCTSTR szThemePath)
{
	CString sThemeName(FileMisc::GetFileNameFromPath(szThemePath, FALSE));

	if (Misc::ToUpper(sThemeName).Find(_T("THEME")) == 0)
		sThemeName = sThemeName.Mid(5);

	return sThemeName;
}

CString CTDLThemeComboBox::FormatThemePath(LPCTSTR szThemeName)
{
	CString sThemeFolder(GetThemeFolder());

	// Try without 'theme' prefix first
	CString sThemePath;
	FileMisc::MakePath(sThemePath, NULL, sThemeFolder, szThemeName, _T("xml"));
	
	if (!FileMisc::FileExists(sThemePath))
	{
		CString sThemeFile(_T("theme"));
		sThemeFile += szThemeName;
		
		FileMisc::MakePath(sThemePath, NULL, sThemeFolder, sThemeFile, _T("xml"));
		ASSERT(FileMisc::FileExists(sThemePath));

		FileMisc::MakeRelativePath(sThemePath, sThemeFolder, FALSE);
	}

	return sThemePath;
}

