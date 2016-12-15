// TDLFileEncodingComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLFileEncodingComboBox.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFileEncodingComboBox

CTDLFileEncodingComboBox::CTDLFileEncodingComboBox()
{
}

CTDLFileEncodingComboBox::~CTDLFileEncodingComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFileEncodingComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTDLFileEncodingComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFileEncodingComboBox message handlers

void CTDLFileEncodingComboBox::PreSubclassWindow() 
{
	BuildCombo();
	
	CComboBox::PreSubclassWindow();
}

int CTDLFileEncodingComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BuildCombo();
	
	return 0;
}

BOOL CTDLFileEncodingComboBox::SetSelectedEncoding(SFE_FORMAT nFormat)
{
	return CDialogHelper::SelectItemByData(*this, nFormat);
}

SFE_FORMAT CTDLFileEncodingComboBox::GetSelectedEncoding() const
{
	return (SFE_FORMAT)CDialogHelper::GetSelectedItemData(*this);
}

void CTDLFileEncodingComboBox::BuildCombo()
{
	// Once only
	if (GetCount() == 0)
	{
		CDialogHelper::AddString(*this, IDS_UTF16,		SFEF_UTF16);
		CDialogHelper::AddString(*this, IDS_UTF8,		SFEF_UTF8WITHOUTBOM);
		CDialogHelper::AddString(*this, IDS_UTF8WBOM,	SFEF_UTF8);
	}
}
