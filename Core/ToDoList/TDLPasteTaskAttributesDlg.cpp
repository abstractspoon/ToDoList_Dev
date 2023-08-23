// TDLPasteTaskAttributesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLPasteTaskAttributesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteTaskAttributesDlg dialog


CTDLPasteTaskAttributesDlg::CTDLPasteTaskAttributesDlg(const CTDCCustomAttribDefinitionArray& aCustAttribs, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(CTDLPasteTaskAttributesDlg::IDD, _T(""), pParent),
	m_lbSpecificAttributes(aCustAttribs, FALSE)
{
	//{{AFX_DATA_INIT(CTDLPasteTaskAttributesDlg)
	m_bIncludeComments = FALSE;
	m_bIncludeCustomAttribs = FALSE;
	m_bIncludeDates = FALSE;
	m_bOnlyOverwriteEmpty = TRUE;
	m_bOnlyOverwriteWithNonEmpty = TRUE;
	m_bSelectSpecificAttribs = 0;
	//}}AFX_DATA_INIT
}


void CTDLPasteTaskAttributesDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLPasteTaskAttributesDlg)
	DDX_Control(pDX, IDC_SPECIFICATTRIBS, m_lbSpecificAttributes);
	DDX_Check(pDX, IDC_INCLUDECOMMENTS, m_bIncludeComments);
	DDX_Check(pDX, IDC_INCLUDECUSTOMATTRIB, m_bIncludeCustomAttribs);
	DDX_Check(pDX, IDC_INCLUDEDATES, m_bIncludeDates);
	DDX_Check(pDX, IDC_ONLYOVERWRITEMPTY, m_bOnlyOverwriteEmpty);
	DDX_Check(pDX, IDC_ONLYOVERWRITEWITHNONEMPTY, m_bOnlyOverwriteWithNonEmpty);
	DDX_Radio(pDX, IDC_SELECTALL, m_bSelectSpecificAttribs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLPasteTaskAttributesDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLPasteTaskAttributesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLPasteTaskAttributesDlg message handlers
