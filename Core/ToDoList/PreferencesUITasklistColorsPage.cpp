// PreferencesUITasklistColorsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUITasklistColorsPage.h"
#include "tdcstatic.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\ColorBrewer.h"

#include "..\3rdparty\colordef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const int DEFFONTSIZE = 8;
const TDC_ATTRIBUTE DEFCOLORATTRIB = TDCA_CATEGORY;

const COLORREF DEF_LIGHTGRAY = RGB(240, 240, 240);

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_PRIORITYCOLOR[] =
{
	RGB(30, 225, 0),
	RGB(30, 225, 0),
	RGB(30, 225, 0),
	RGB(30, 225, 0),
	RGB(0, 0, 255),
	RGB(0, 0, 255),
	RGB(0, 0, 255),
	RGB(255, 0, 0),
	RGB(255, 0, 0),
	RGB(255, 0, 0),
	RGB(255, 0, 0)
};
const int NUM_DEFPRIORITY = (sizeof(DEF_PRIORITYCOLOR) / sizeof(DEF_PRIORITYCOLOR[0]));

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistColorsPage property page

CPreferencesUITasklistColorsPage::CPreferencesUITasklistColorsPage() 
	: 
	CPreferencesPageBase(IDD_PREFUITASKLISTCOLORS_PAGE),
	m_nPriorityColorOption(PRIORITYOPT_GRADIENT),
	m_nTextColorOption(TEXTOPT_DEFAULT), 
	m_cbAttributes(CCBS_DRAWNOCOLOR, ACBS_ALLOWDELETE), 
	m_nColorAttribute(DEFCOLORATTRIB),
	m_cbPriorityColors(FALSE, FALSE),
	m_nNumPriorityRiskLevels(NUM_DEFPRIORITY)
{
	//{{AFX_DATA_INIT(CPreferencesUITasklistColorsPage)
	//}}AFX_DATA_INIT
	m_nSelPriorityColor = 0; 
}

CPreferencesUITasklistColorsPage::~CPreferencesUITasklistColorsPage()
{
}

void CPreferencesUITasklistColorsPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesUITasklistColorsPage)
	DDX_Control(pDX, IDC_PRIORITYSCHEMES, m_cbPriorityScheme);
	DDX_Control(pDX, IDC_ATTRIBUTETOCOLORBY, m_cbColorByAttribute);
	DDX_Check(pDX, IDC_COLORTASKBKGND, m_bColorTaskBackground);
	DDX_Check(pDX, IDC_USEHLSGRADIENT, m_bHLSColorGradient);
	DDX_Check(pDX, IDC_ALTERNATELINECOLOR, m_bSpecifyAlternateLineColor);
	DDX_Check(pDX, IDC_SPECIFYGROUPHEADERBKCOLOR, m_bSpecifyGroupHeaderBkgndColor);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_COMMENTSUSETREEFONT, m_bCommentsUseTreeFont);
	DDX_Check(pDX, IDC_REMINDERSUSETREEFONT, m_bRemindersUseTreeFont);
	DDX_Check(pDX, IDC_FINDTASKSUSETREEFONT, m_bFindTasksUseTreeFont);
	DDX_CBString(pDX, IDC_ATTRIBUTECOLORS, m_sSelAttribValue);
	DDX_Control(pDX, IDC_SETATTRIBUTECOLOR, m_btAttribColor);
	DDX_Control(pDX, IDC_SETGROUPHEADERBKCOLOR, m_btGroupHeaderBkgndColor);
	DDX_Control(pDX, IDC_ATTRIBUTECOLORS, m_cbAttributes);
	DDX_Radio(pDX, IDC_COLORTEXTBYATTRIBUTE, (int&)m_nTextColorOption);
	DDX_Check(pDX, IDC_DUETASKCOLOR, m_bSpecifyDueColor);
	DDX_Check(pDX, IDC_DUETODAYTASKCOLOR, m_bSpecifyDueTodayColor);
	DDX_Check(pDX, IDC_STARTTASKCOLOR, m_bSpecifyStartColor);
	DDX_Check(pDX, IDC_STARTTODAYTASKCOLOR, m_bSpecifyStartTodayColor);
	DDX_Control(pDX, IDC_SETSTARTTASKCOLOR, m_btStartColor);
	DDX_Control(pDX, IDC_SETSTARTTODAYTASKCOLOR, m_btStartTodayColor);
	DDX_Control(pDX, IDC_SETDUETASKCOLOR, m_btDueColor);
	DDX_Control(pDX, IDC_SETDUETODAYTASKCOLOR, m_btDueTodayColor);
	DDX_Control(pDX, IDC_SETDONECOLOR, m_btDoneColor);
	DDX_Control(pDX, IDC_SETGRIDLINECOLOR, m_btGridlineColor);
	DDX_Control(pDX, IDC_TREEFONTSIZE, m_cbTreeFontSize);
	DDX_Control(pDX, IDC_TREEFONTLIST, m_cbTreeFonts);
	DDX_Control(pDX, IDC_COMMENTSFONTSIZE, m_cbCommentsFontSize);
	DDX_Control(pDX, IDC_COMMENTSFONTLIST, m_cbCommentsFonts);
	DDX_Check(pDX, IDC_SPECIFYTREEFONT, m_bSpecifyTreeFont);
	DDX_Check(pDX, IDC_SPECIFYCOMMENTSFONT, m_bSpecifyCommentsFont);
	DDX_Check(pDX, IDC_SPECIFYGRIDLINECOLOR, m_bSpecifyGridColor);
	DDX_Check(pDX, IDC_SPECIFYDONECOLOR, m_bSpecifyDoneColor);
	DDX_Check(pDX, IDC_SPECIFYFLAGGEDCOLOR, m_bSpecifyFlaggedColor);
	DDX_Check(pDX, IDC_SPECIFYREFERENCECOLOR, m_bSpecifyReferenceColor);
	DDX_Control(pDX, IDC_SETFLAGGEDCOLOR, m_btFlaggedColor);
	DDX_Control(pDX, IDC_SETREFERENCECOLOR, m_btReferenceColor);
	DDX_Control(pDX, IDC_SETALTLINECOLOR, m_btAltLineColor);
	DDX_Control(pDX, IDC_SETPRIORITYCOLOR, m_btPriorityColor);
	DDX_Control(pDX, IDC_LOWPRIORITYCOLOR, m_btPriorityLowColor);
	DDX_Control(pDX, IDC_HIGHPRIORITYCOLOR, m_btPriorityHighColor);
	DDX_Check(pDX, IDC_COLORPRIORITY, m_bColorPriority);
	DDX_Radio(pDX, IDC_INDIVIDUALPRIORITYCOLORS, (int&)m_nPriorityColorOption);
	DDX_CBIndex(pDX, IDC_PRIORITYCOLORS, m_nSelPriorityColor);
	DDX_Control(pDX, IDC_PRIORITYCOLORS, m_cbPriorityColors);

	CDialogHelper::DDX_CBValue(pDX, m_cbTreeFontSize, m_nTreeFontSize, 9);
	CDialogHelper::DDX_CBValue(pDX, m_cbCommentsFontSize, m_nCommentsFontSize, 9);
	CDialogHelper::DDX_CBData(pDX, m_cbColorByAttribute, m_nColorAttribute, DEFCOLORATTRIB);

	m_cbTreeFonts.DDX(pDX, m_sTreeFont);
	m_cbCommentsFonts.DDX(pDX, m_sCommentsFont);
	m_cbPriorityScheme.DDX(pDX, m_aPriorityScheme);
}

BEGIN_MESSAGE_MAP(CPreferencesUITasklistColorsPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesUITasklistColorsPage)
	ON_BN_CLICKED(IDC_SETALTLINECOLOR, OnAltlinecolor)
	ON_BN_CLICKED(IDC_ALTERNATELINECOLOR, OnSpecifyAlternatelinecolor)
	ON_BN_CLICKED(IDC_COMMENTSUSETREEFONT, OnCommentsusetreefont)
	ON_BN_CLICKED(IDC_SPECIFYCOMMENTSFONT, OnSpecifycommentsfont)
	ON_BN_CLICKED(IDC_POPULATEATTRIBLIST, OnPopulateattriblist)
	ON_BN_CLICKED(IDC_SETATTRIBUTECOLOR, OnSetAttribValuecolor)
	ON_CBN_EDITCHANGE(IDC_ATTRIBUTECOLORS, OnEditAttribValue)
	ON_CBN_SELCHANGE(IDC_ATTRIBUTECOLORS, OnSelchangeAttribValue)
	ON_CBN_SELCHANGE(IDC_COMMENTSFONTLIST, OnSelchangeCommentsFont)
	ON_CBN_SELCHANGE(IDC_COMMENTSFONTSIZE, OnSelchangeCommentsFont)
	ON_CBN_SELCHANGE(IDC_TREEFONTLIST, OnSelchangeTreeFont)
	ON_CBN_SELCHANGE(IDC_TREEFONTSIZE, OnSelchangeTreeFont)
	ON_BN_CLICKED(IDC_COLORTEXTBYATTRIBUTE, OnChangeTextColorOption)
	ON_BN_CLICKED(IDC_COLORTASKSBYCOLOR, OnChangeTextColorOption)
	ON_BN_CLICKED(IDC_COLORTEXTBYPRIORITY, OnChangeTextColorOption)
	ON_BN_CLICKED(IDC_NOCOLORTASKS, OnChangeTextColorOption)
	ON_CBN_SELCHANGE(IDC_ATTRIBUTETOCOLORBY, OnSelchangeAttributetocolorby)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STARTTASKCOLOR, OnStarttaskcolor)
	ON_BN_CLICKED(IDC_SETSTARTTASKCOLOR, OnSetStarttaskcolor)
	ON_BN_CLICKED(IDC_STARTTODAYTASKCOLOR, OnStarttodaytaskcolor)
	ON_BN_CLICKED(IDC_SETSTARTTODAYTASKCOLOR, OnSetStarttodaytaskcolor)
	ON_BN_CLICKED(IDC_DUETASKCOLOR, OnDuetaskcolor)
	ON_BN_CLICKED(IDC_SETDUETASKCOLOR, OnSetduetaskcolor)
	ON_BN_CLICKED(IDC_DUETODAYTASKCOLOR, OnDuetodaytaskcolor)
	ON_BN_CLICKED(IDC_SETDUETODAYTASKCOLOR, OnSetduetodaytaskcolor)
	ON_BN_CLICKED(IDC_SPECIFYTREEFONT, OnSpecifytreefont)
	ON_BN_CLICKED(IDC_SETGRIDLINECOLOR, OnSetgridlinecolor)
	ON_BN_CLICKED(IDC_SPECIFYGRIDLINECOLOR, OnSpecifygridlinecolor)
	ON_BN_CLICKED(IDC_SETDONECOLOR, OnSetdonecolor)
	ON_BN_CLICKED(IDC_SPECIFYDONECOLOR, OnSpecifydonecolor)
	ON_BN_CLICKED(IDC_LOWPRIORITYCOLOR, OnLowprioritycolor)
	ON_BN_CLICKED(IDC_HIGHPRIORITYCOLOR, OnHighprioritycolor)
	ON_BN_CLICKED(IDC_SETPRIORITYCOLOR, OnSetprioritycolor)
	ON_BN_CLICKED(IDC_COLORPRIORITY, OnColorPriority)
	ON_BN_CLICKED(IDC_SETFLAGGEDCOLOR, OnSetflaggedcolor)
	ON_BN_CLICKED(IDC_SETREFERENCECOLOR, OnSetReferencecolor)
	ON_BN_CLICKED(IDC_SPECIFYFLAGGEDCOLOR, OnSpecifyflaggedcolor)
	ON_BN_CLICKED(IDC_SPECIFYREFERENCECOLOR, OnSpecifyReferencecolor)
	ON_BN_CLICKED(IDC_SPECIFYGROUPHEADERBKCOLOR, OnSpecifyGroupHeaderBkgndcolor)
	ON_BN_CLICKED(IDC_SETGROUPHEADERBKCOLOR, OnSetGroupHeaderBkgndcolor)
	ON_CBN_SELCHANGE(IDC_PRIORITYCOLORS, OnSelchangePrioritycolors)
	ON_BN_CLICKED(IDC_GRADIENTPRIORITYCOLORS, OnChangePriorityColorOption)
	ON_BN_CLICKED(IDC_INDIVIDUALPRIORITYCOLORS, OnChangePriorityColorOption)
	ON_BN_CLICKED(IDC_SCHEMEPRIORITYCOLORS, OnChangePriorityColorOption)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMADDED, OnAttribValueAdded)
	ON_REGISTERED_MESSAGE(WM_ACBN_ITEMDELETED, OnAttribValueDeleted)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistColorsPage message handlers

BOOL CPreferencesUITasklistColorsPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	// disable translation of user-entered text
	CLocalizer::EnableTranslation(m_cbAttributes, FALSE);
	CLocalizer::EnableTranslation(m_cbTreeFonts, FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesUITasklistColorsPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();

	AddGroupLine(IDC_TASKCOLOURGROUP);

	GetDlgItem(IDC_GRADIENTPRIORITYCOLORS)->EnableWindow(m_bColorPriority);
	GetDlgItem(IDC_INDIVIDUALPRIORITYCOLORS)->EnableWindow(m_bColorPriority);
	GetDlgItem(IDC_SCHEMEPRIORITYCOLORS)->EnableWindow(m_bColorPriority);

	GetDlgItem(IDC_PRIORITYCOLORS)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_INDIVIDUAL));
	GetDlgItem(IDC_USEHLSGRADIENT)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));
	GetDlgItem(IDC_PRIORITYSCHEMES)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_SCHEME));

	GetDlgItem(IDC_TREEFONTSIZE)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZELABEL)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTLIST)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_COMMENTSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_REMINDERSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_FINDTASKSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_SPECIFYCOMMENTSFONT)->EnableWindow(!m_bCommentsUseTreeFont || !m_bSpecifyTreeFont);

	BOOL bColorByAttrib = (m_nTextColorOption == TEXTOPT_ATTRIB);
	BOOL bHasAttrib = (m_nColorAttribute != TDCA_NONE);

	GetDlgItem(IDC_ATTRIBUTETOCOLORBY)->EnableWindow(bColorByAttrib);
	GetDlgItem(IDC_ATTRIBUTECOLORS)->EnableWindow(bColorByAttrib && bHasAttrib);
	GetDlgItem(IDC_POPULATEATTRIBLIST)->EnableWindow(bColorByAttrib && bHasAttrib);

	BOOL bEnableCommentsFont = m_bSpecifyCommentsFont && (!m_bSpecifyTreeFont || !m_bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZE)->EnableWindow(bEnableCommentsFont);
	GetDlgItem(IDC_COMMENTSFONTSIZELABEL)->EnableWindow(bEnableCommentsFont);
	GetDlgItem(IDC_COMMENTSFONTLIST)->EnableWindow(bEnableCommentsFont);

	m_btPriorityColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_INDIVIDUAL));
	m_btPriorityLowColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));
	m_btPriorityHighColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));
	m_btGridlineColor.EnableWindow(m_bSpecifyGridColor);
	m_btDoneColor.EnableWindow(m_bSpecifyDoneColor);
	m_btAltLineColor.EnableWindow(m_bSpecifyAlternateLineColor);
	m_btStartColor.EnableWindow(m_bSpecifyStartColor);
	m_btStartTodayColor.EnableWindow(m_bSpecifyStartTodayColor);
	m_btDueColor.EnableWindow(m_bSpecifyDueColor);
	m_btDueTodayColor.EnableWindow(m_bSpecifyDueTodayColor);
	m_btAttribColor.EnableWindow(bColorByAttrib && bHasAttrib && !m_sSelAttribValue.IsEmpty());
	m_btFlaggedColor.EnableWindow(m_bSpecifyFlaggedColor);
	m_btReferenceColor.EnableWindow(m_bSpecifyReferenceColor);
	m_btGroupHeaderBkgndColor.EnableWindow(m_bSpecifyGroupHeaderBkgndColor);

	m_btGridlineColor.SetColor(m_crGridlines);
	m_btPriorityLowColor.SetColor(m_crPriorityLow);
	m_btPriorityHighColor.SetColor(m_crPriorityHigh);
	m_btPriorityColor.SetColor(m_aPriorityColors[0]);
	m_btDoneColor.SetColor(m_crDone);
	m_btAltLineColor.SetColor(m_crAltLine);
	m_btStartColor.SetColor(m_crStart);
	m_btStartTodayColor.SetColor(m_crStartToday);
	m_btDueColor.SetColor(m_crDue);
	m_btDueTodayColor.SetColor(m_crDueToday);
	m_btFlaggedColor.SetColor(m_crFlagged);
	m_btReferenceColor.SetColor(m_crReference);
	m_btGroupHeaderBkgndColor.SetColor(m_crGroupHeaderBkgnd);

	// priority colors
	ASSERT(m_aPriorityColors.GetSize() >= m_nNumPriorityRiskLevels);

	for (int nPriority = 0; nPriority < m_nNumPriorityRiskLevels; nPriority++)
		m_cbPriorityColors.SetColor(nPriority, (COLORREF)m_aPriorityColors[nPriority]);

	// Priority colors schemes
	CColorBrewer brewer(CBF_SYNTHESIZE | CBF_TEXTSAFE);
	CColorBrewerPaletteArray aPalettes;

	brewer.GetPalettes(CBPT_SEQUENTIAL, aPalettes, m_nNumPriorityRiskLevels);
	brewer.GetPalettes(CBPT_DIVERGING, aPalettes, m_nNumPriorityRiskLevels, TRUE);

	m_cbPriorityScheme.Initialize(aPalettes);

	if (m_cbPriorityScheme.SetSelectedPalette(m_aPriorityScheme) == CB_ERR)
		m_cbPriorityScheme.SetCurSel(0);
	
	// attribute colors
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_CATEGORY),		TDCA_CATEGORY);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_STATUS),		TDCA_STATUS);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_ALLOCTO),		TDCA_ALLOCTO);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_ALLOCBY),		TDCA_ALLOCBY);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_VERSION),		TDCA_VERSION);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_EXTERNALID),	TDCA_EXTERNALID);
	AddStringT(m_cbColorByAttribute, CEnString(IDS_TDLBC_TAGS),			TDCA_TAGS);

	int nColor = m_aAttribColors.GetSize();
	
	while (nColor--)
	{
		const ATTRIBCOLOR& ac = m_aAttribColors[nColor];
		
		if (!ac.sAttrib.IsEmpty())
			m_cbAttributes.AddColor(ac.color, ac.sAttrib);
	}
	
	nColor = FindAttribValue(m_sSelAttribValue);
	
	if (nColor != -1)
		m_btAttribColor.SetColor(m_aAttribColors[nColor].color);

	UpdateData(FALSE);
}

void CPreferencesUITasklistColorsPage::OnLowprioritycolor() 
{
	m_crPriorityLow = m_btPriorityLowColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnHighprioritycolor() 
{
	m_crPriorityHigh = m_btPriorityHighColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetprioritycolor() 
{
	VERIFY(m_nSelPriorityColor >= 0);

	m_aPriorityColors.SetAt(m_nSelPriorityColor, m_btPriorityColor.GetColor());
	m_cbPriorityColors.SetColor(m_nSelPriorityColor, m_btPriorityColor.GetColor());

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnChangePriorityColorOption() 
{
	UpdateData();

	GetDlgItem(IDC_PRIORITYCOLORS)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_INDIVIDUAL));
	GetDlgItem(IDC_USEHLSGRADIENT)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));
	GetDlgItem(IDC_PRIORITYSCHEMES)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_SCHEME));

	m_btPriorityColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_INDIVIDUAL) && m_nSelPriorityColor >= 0);
	m_btPriorityLowColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));
	m_btPriorityHighColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == PRIORITYOPT_GRADIENT));

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnColorPriority() 
{
	UpdateData();

	GetDlgItem(IDC_GRADIENTPRIORITYCOLORS)->EnableWindow(m_bColorPriority);
	GetDlgItem(IDC_INDIVIDUALPRIORITYCOLORS)->EnableWindow(m_bColorPriority);
	GetDlgItem(IDC_SCHEMEPRIORITYCOLORS)->EnableWindow(m_bColorPriority);

	OnChangePriorityColorOption(); // to handle the other controls

	// if the text color option is COLOROPT_PRIORITY and 
	// the user has turned off priority coloring then switch to default
	if (!m_bColorPriority && (m_nTextColorOption == TEXTOPT_PRIORITY))
	{
		m_nTextColorOption = TEXTOPT_DEFAULT;
		UpdateData(FALSE);
	}

	CPreferencesPageBase::OnControlChange();
}

int CPreferencesUITasklistColorsPage::GetPriorityColors(CDWordArray& aColors) const 
{ 
	aColors.RemoveAll();

	if (m_bColorPriority)
	{
		switch (m_nPriorityColorOption)
		{
		case PRIORITYOPT_INDIVIDUAL:
			aColors.Copy(m_aPriorityColors); 
			break;

		case PRIORITYOPT_GRADIENT:	
			GraphicsMisc::CalculateColorGradient(m_crPriorityLow, m_crPriorityHigh, m_nNumPriorityRiskLevels, aColors, !m_bHLSColorGradient);
			break;

		case PRIORITYOPT_SCHEME:
			aColors.Copy(m_aPriorityScheme);
			break;
		}
	}
	else // gray scale
	{
		GraphicsMisc::CalculateColorGradient(DEF_LIGHTGRAY, 0, m_nNumPriorityRiskLevels, aColors, TRUE);
	}
	
	return aColors.GetSize(); 
}

void CPreferencesUITasklistColorsPage::GetStartedTaskColors(COLORREF& crStarted, COLORREF& crStartedToday) const 
{ 
	crStarted = m_bSpecifyStartColor ? m_crStart : -1; 
	crStartedToday = m_bSpecifyStartTodayColor ? m_crStartToday : -1; 
}

void CPreferencesUITasklistColorsPage::GetDueTaskColors(COLORREF& crDue, COLORREF& crDueToday) const 
{ 
	crDue = m_bSpecifyDueColor ? m_crDue : -1; 
	crDueToday = m_bSpecifyDueTodayColor ? m_crDueToday : -1; 
}

TDC_ATTRIBUTE CPreferencesUITasklistColorsPage::GetAttributeColors(CTDCColorMap& colors) const
{
	colors.RemoveAll();

	if (m_nColorAttribute != TDCA_NONE)
	{
		int nColor = m_aAttribColors.GetSize();

		while (nColor--)
		{
			const ATTRIBCOLOR& color = m_aAttribColors[nColor];
			colors.AddColor(color.sAttrib, color.color);
		}
	}

	return m_nColorAttribute;
}

void CPreferencesUITasklistColorsPage::OnSelchangePrioritycolors() 
{
	UpdateData();

	ASSERT (m_nSelPriorityColor >= 0);
	
	if (m_nSelPriorityColor >= 0)
	{
		m_btPriorityColor.SetColor(m_aPriorityColors[m_nSelPriorityColor]);
		m_btPriorityColor.EnableWindow(TRUE);
	}
	else
		m_btPriorityColor.EnableWindow(FALSE);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifytreefont() 
{
	UpdateData();

	GetDlgItem(IDC_TREEFONTLIST)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZE)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZELABEL)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_COMMENTSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_REMINDERSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_FINDTASKSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);

	BOOL bCommentsUseTreeFont = (m_bSpecifyTreeFont && m_bCommentsUseTreeFont);

	GetDlgItem(IDC_SPECIFYCOMMENTSFONT)->EnableWindow(!bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZE)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZELABEL)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTLIST)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);

	CPreferencesPageBase::OnControlChange();
}

BOOL CPreferencesUITasklistColorsPage::GetTreeFont(CString& sFaceName, int& nPointSize) const
{
	if (m_bSpecifyTreeFont)
	{
		sFaceName = m_sTreeFont;
		nPointSize = m_nTreeFontSize;
	}

	return m_bSpecifyTreeFont;
}

BOOL CPreferencesUITasklistColorsPage::GetTreeFont(CString& sFaceName, int& nPointSize, HFONT hFontFallback) const
{
	if (!hFontFallback)
	{
		ASSERT(0);
		return FALSE;
	}

	if (!GetTreeFont(sFaceName, nPointSize))
		nPointSize = GraphicsMisc::GetFontNameAndPointSize(hFontFallback, sFaceName);

	return TRUE;
}

BOOL CPreferencesUITasklistColorsPage::IncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback)
{
	CString sFaceName;
	int nPointSize;

	VERIFY(GetTreeFont(sFaceName, nPointSize, hFontFallback));
	
	int nNewSize = GetNextTreeFontIncrement(nPointSize, bLarger);

	if (nNewSize == nPointSize)
		return FALSE;

	// else
	m_nTreeFontSize = nNewSize;
	m_sTreeFont = sFaceName;
	m_bSpecifyTreeFont = TRUE;
	
	return TRUE;
}

int CPreferencesUITasklistColorsPage::GetNextTreeFontIncrement(int nPointSize, BOOL bLarger) const
{
	static int FONTSIZES[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24 };
	static int NUM_FONTSIZES = (sizeof(FONTSIZES) / sizeof(int));

	int nFind = NUM_FONTSIZES;

	while (nFind--)
	{
		if (FONTSIZES[nFind] == nPointSize)
			break;
	}

	if (nFind == -1)
	{
		if (nPointSize < FONTSIZES[0])
			nFind = 0;
		else
			nFind = (NUM_FONTSIZES - 1);
	}

	nFind += (bLarger ? 1 : -1);
	nFind = max(0, min(nFind, (NUM_FONTSIZES - 1)));

	return FONTSIZES[nFind];
}

BOOL CPreferencesUITasklistColorsPage::CanIncrementTreeFontSize(BOOL bLarger, HFONT hFontFallback) const
{
	CString sFaceName;
	int nPointSize;

	VERIFY(GetTreeFont(sFaceName, nPointSize, hFontFallback));

	return (GetNextTreeFontIncrement(nPointSize, bLarger) != nPointSize);
}

BOOL CPreferencesUITasklistColorsPage::RestoreTreeFontSize(HFONT hFontDefault)
{
	CString sFaceName;
	int nPointSize;

	if (!GetTreeFont(sFaceName, nPointSize))
		return FALSE;

	CString sDefFaceName;
	int nDefPointSize = GraphicsMisc::GetFontNameAndPointSize(hFontDefault, sDefFaceName);

	// If the font name is the same just turn off the tree font altogether
	if (sFaceName == sDefFaceName)
	{
		m_bSpecifyTreeFont = FALSE;
	}
	else 
	{
		m_nTreeFontSize = nDefPointSize;
	}
	
	return TRUE;
}

BOOL CPreferencesUITasklistColorsPage::GetCommentsFont(CString& sFaceName, int& nPointSize) const
{
	if ((m_bSpecifyTreeFont && m_bCommentsUseTreeFont) || !m_bSpecifyCommentsFont)
		return FALSE;

	sFaceName = m_sCommentsFont;
	nPointSize = m_nCommentsFontSize;

	return m_bSpecifyCommentsFont;
}

void CPreferencesUITasklistColorsPage::OnSetgridlinecolor() 
{
	m_crGridlines = m_btGridlineColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifygridlinecolor() 
{
	UpdateData();	

	m_btGridlineColor.EnableWindow(m_bSpecifyGridColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetdonecolor() 
{
	m_crDone = m_btDoneColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetReferencecolor() 
{
	m_crReference = m_btReferenceColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetflaggedcolor() 
{
	m_crFlagged = m_btFlaggedColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifydonecolor() 
{
	UpdateData();	

	m_btDoneColor.EnableWindow(m_bSpecifyDoneColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifyflaggedcolor() 
{
	UpdateData();	

	m_btFlaggedColor.EnableWindow(m_bSpecifyFlaggedColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifyReferencecolor() 
{
	UpdateData();	

	m_btReferenceColor.EnableWindow(m_bSpecifyReferenceColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnAltlinecolor() 
{
	m_crAltLine = m_btAltLineColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifyAlternatelinecolor() 
{
	UpdateData();	
	
	m_btAltLineColor.EnableWindow(m_bSpecifyAlternateLineColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifyGroupHeaderBkgndcolor()
{
	UpdateData();	
	
	m_btGroupHeaderBkgndColor.EnableWindow(m_bSpecifyGroupHeaderBkgndColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetGroupHeaderBkgndcolor()
{
	m_crGroupHeaderBkgnd = m_btGroupHeaderBkgndColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnChangeTextColorOption() 
{
	UpdateData();

	// if the text color option is COLOROPT_PRIORITY and 
	// the user has got priority coloring turned off then switch it on
	if (!m_bColorPriority && (m_nTextColorOption == TEXTOPT_PRIORITY))
	{
		m_bColorPriority = TRUE;
		UpdateData(FALSE);

		OnColorPriority();
	}

	BOOL bColorByAttrib = (m_nTextColorOption == TEXTOPT_ATTRIB);
	BOOL bHasAttrib = (m_nColorAttribute != TDCA_NONE);

	GetDlgItem(IDC_ATTRIBUTETOCOLORBY)->EnableWindow(bColorByAttrib);
	GetDlgItem(IDC_ATTRIBUTECOLORS)->EnableWindow(bColorByAttrib && bHasAttrib);
	GetDlgItem(IDC_POPULATEATTRIBLIST)->EnableWindow(bColorByAttrib && bHasAttrib);

	m_btAttribColor.EnableWindow(bColorByAttrib && bHasAttrib && !m_sSelAttribValue.IsEmpty());

	GetParent()->SendMessage(WM_PUITCP_TEXTCOLOROPTION);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetAttribValuecolor() 
{
	UpdateData();

	COLORREF crAttrib = m_btAttribColor.GetColor();
	int nColor = FindAttribValue(m_sSelAttribValue);

	if (nColor >= 0)
	{
		m_aAttribColors[nColor].color = crAttrib;
	}
	else
	{
		ATTRIBCOLOR ac;
		ac.sAttrib = m_sSelAttribValue;
		ac.color = crAttrib;

		nColor = m_aAttribColors.Add(ac);
		ASSERT(nColor == (m_cbAttributes.GetCount() - 1));
	}

	m_cbAttributes.SetColor(m_sSelAttribValue, crAttrib);

	CPreferencesPageBase::OnControlChange();
}

int CPreferencesUITasklistColorsPage::FindAttribValue(LPCTSTR szAttrib)
{
	int nColor = m_aAttribColors.GetSize();

	while (nColor--)
	{
		ATTRIBCOLOR& ac = m_aAttribColors[nColor];

		if (ac.sAttrib.CompareNoCase(szAttrib) == 0)
			return nColor;
	}

	return -1;
}

void CPreferencesUITasklistColorsPage::OnEditAttribValue() 
{
	UpdateData();

	m_btAttribColor.EnableWindow(m_nTextColorOption == TEXTOPT_ATTRIB && 
								!m_sSelAttribValue.IsEmpty());

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSelchangeAttribValue() 
{
	UpdateData();

	int nSel = m_cbAttributes.GetCurSel();

	if (nSel != CB_ERR)
		m_sSelAttribValue = m_cbAttributes.GetItemText(nSel);
	
	int nColor = FindAttribValue(m_sSelAttribValue);

	if (nColor >= 0)
		m_btAttribColor.SetColor(m_aAttribColors[nColor].color);

	m_btAttribColor.EnableWindow(m_nTextColorOption == TEXTOPT_ATTRIB && 
								!m_sSelAttribValue.IsEmpty());
}

LRESULT CPreferencesUITasklistColorsPage::OnAttribValueAdded(WPARAM /*wParam*/, LPARAM lParam)
{
	CString sAttrib((LPCTSTR)lParam);
	
	if (!sAttrib.IsEmpty() && FindAttribValue(sAttrib) == -1)
	{
		ATTRIBCOLOR ac;
		ac.sAttrib = sAttrib;
		ac.color = CLR_NONE;
		
		m_aAttribColors.Add(ac);

		OnSelchangeAttribValue();
		UpdateData(FALSE);
		
		CPreferencesPageBase::OnControlChange();
	}
	
	return 0L;
}

LRESULT CPreferencesUITasklistColorsPage::OnAttribValueDeleted(WPARAM /*wParam*/, LPARAM lParam)
{
	CString sAttrib((LPCTSTR)lParam);
	
	if (!sAttrib.IsEmpty())
	{
		int nDel = FindAttribValue(sAttrib);
		
		if (nDel != -1)
		{
			m_aAttribColors.RemoveAt(nDel);

			OnSelchangeAttribValue();
			UpdateData(FALSE);
		}
		
		CPreferencesPageBase::OnControlChange();
	}
	
	return 0L;
}

void CPreferencesUITasklistColorsPage::OnCommentsusetreefont() 
{
	UpdateData();

	BOOL bCommentsUseTreeFont = (m_bCommentsUseTreeFont && m_bSpecifyTreeFont);

	GetDlgItem(IDC_SPECIFYCOMMENTSFONT)->EnableWindow(!bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZE)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZELABEL)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTLIST)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);

	CPreferencesPageBase::OnControlChange(IDC_COMMENTSFONTLIST);
}

void CPreferencesUITasklistColorsPage::OnSpecifycommentsfont() 
{
	UpdateData();
	
	BOOL bCommentsUseTreeFont = (m_bCommentsUseTreeFont && m_bSpecifyTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZE)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZELABEL)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTLIST)->EnableWindow(m_bSpecifyCommentsFont && !bCommentsUseTreeFont);
	
	CPreferencesPageBase::OnControlChange(IDC_COMMENTSFONTLIST);
}

void CPreferencesUITasklistColorsPage::OnSelchangeCommentsFont() 
{
	UpdateData();	

	CPreferencesPageBase::OnControlChange(IDC_COMMENTSFONTLIST);
}

void CPreferencesUITasklistColorsPage::OnSelchangeTreeFont() 
{
	UpdateData();	
	
	if (m_bCommentsUseTreeFont)
		CPreferencesPageBase::OnControlChange(IDC_COMMENTSFONTLIST);
}

void CPreferencesUITasklistColorsPage::OnDuetaskcolor() 
{
	UpdateData();	
	
	m_btDueColor.EnableWindow(m_bSpecifyDueColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnStarttodaytaskcolor() 
{
	UpdateData();	
	
	m_btStartTodayColor.EnableWindow(m_bSpecifyStartTodayColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetStarttaskcolor() 
{
	m_crStart = m_btStartColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetStarttodaytaskcolor() 
{
	m_crStartToday = m_btStartTodayColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnStarttaskcolor() 
{
	UpdateData();	
	
	m_btStartColor.EnableWindow(m_bSpecifyStartColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnDuetodaytaskcolor() 
{
	UpdateData();	
	
	m_btDueTodayColor.EnableWindow(m_bSpecifyDueTodayColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetduetaskcolor() 
{
	m_crDue = m_btDueColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetduetodaytaskcolor() 
{
	m_crDueToday = m_btDueTodayColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bColorPriority = pPrefs->GetProfileInt(szKey, _T("ColorPriority"), TRUE);
	m_sTreeFont = pPrefs->GetProfileString(szKey, _T("TreeFont"), _T("Arial"));
	m_nTreeFontSize = pPrefs->GetProfileInt(szKey, _T("FontSize"), DEFFONTSIZE);
	m_bSpecifyTreeFont = pPrefs->GetProfileInt(szKey, _T("SpecifyTreeFont"), FALSE);
	m_sCommentsFont = pPrefs->GetProfileString(szKey, _T("CommentsFont"), _T("Arial"));
	m_nCommentsFontSize = pPrefs->GetProfileInt(szKey, _T("CommentsFontSize"), DEFFONTSIZE);
	m_bSpecifyCommentsFont = pPrefs->GetProfileInt(szKey, _T("SpecifyCommentsFont"), TRUE);
	m_bSpecifyGridColor = pPrefs->GetProfileInt(szKey, _T("SpecifyGridColor"), TRUE);
	m_bSpecifyDoneColor = pPrefs->GetProfileInt(szKey, _T("SpecifyDoneColor"), TRUE);
	m_bSpecifyStartColor = pPrefs->GetProfileInt(szKey, _T("SpecifyStartColor"), FALSE);
	m_bSpecifyStartTodayColor = pPrefs->GetProfileInt(szKey, _T("SpecifyStartTodayColor"), FALSE);
	m_bSpecifyDueColor = pPrefs->GetProfileInt(szKey, _T("SpecifyDueColor"), TRUE);
	m_bSpecifyDueTodayColor = pPrefs->GetProfileInt(szKey, _T("SpecifyDueTodayColor"), TRUE);
	m_bColorTaskBackground = pPrefs->GetProfileInt(szKey, _T("ColorTaskBackground"), FALSE);
	m_bCommentsUseTreeFont = pPrefs->GetProfileInt(szKey, _T("CommentsUseTreeFont"), FALSE);
	m_bRemindersUseTreeFont = pPrefs->GetProfileInt(szKey, _T("RemindersUseTreeFont"), FALSE);
	m_bFindTasksUseTreeFont = pPrefs->GetProfileInt(szKey, _T("FindTasksUseTreeFont"), FALSE);
	m_bHLSColorGradient = pPrefs->GetProfileInt(szKey, _T("HLSColorGradient"), TRUE);
	m_bSpecifyAlternateLineColor = pPrefs->GetProfileInt(szKey, _T("AlternateLineColor"), TRUE);
	m_bSpecifyFlaggedColor = pPrefs->GetProfileInt(szKey, _T("FlaggedColor"), FALSE);
	m_bSpecifyReferenceColor = pPrefs->GetProfileInt(szKey, _T("ReferenceColor"), FALSE);
	m_bSpecifyGroupHeaderBkgndColor = pPrefs->GetProfileInt(szKey, _T("SpecifyGroupHeaderBkgndColor"), FALSE);

	m_nPriorityColorOption = (PUITCP_PRIORITYCOLOROPTION)pPrefs->GetProfileInt(szKey, _T("PriorityColorOption"), m_nPriorityColorOption);
	m_nTextColorOption = (PUITCP_TEXTCOLOROPTION)pPrefs->GetProfileInt(szKey, _T("TextColorOption"), m_nTextColorOption);
	m_nColorAttribute = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(szKey, _T("ColorAttribute"), TDCA_CATEGORY);

	// colors
	CString sColorKey(szKey);
	sColorKey += _T("\\Colors");

	m_crGridlines = pPrefs->GetProfileInt(sColorKey, _T("Gridlines"), DEF_GRIDLINECOLOR);
	m_crDone = pPrefs->GetProfileInt(sColorKey, _T("TaskDone"), DEF_TASKDONECOLOR);
	m_crStart = pPrefs->GetProfileInt(sColorKey, _T("TaskStart"), DEF_TASKSTARTCOLOR);
	m_crStartToday = pPrefs->GetProfileInt(sColorKey, _T("TaskStartToday"), DEF_TASKSTARTCOLOR);
	m_crDue = pPrefs->GetProfileInt(sColorKey, _T("TaskDue"), DEF_TASKDUECOLOR);
	m_crDueToday = pPrefs->GetProfileInt(sColorKey, _T("TaskDueToday"), DEF_TASKDUETODAYCOLOR);
	m_crAltLine = pPrefs->GetProfileInt(sColorKey, _T("AlternateLines"), DEF_ALTERNATELINECOLOR);
	m_crFlagged = pPrefs->GetProfileInt(sColorKey, _T("Flagged"), DEF_FLAGGEDCOLOR);
	m_crReference = pPrefs->GetProfileInt(sColorKey, _T("Reference"), DEF_REFERENCECOLOR);
	m_crGroupHeaderBkgnd = pPrefs->GetProfileInt(sColorKey, _T("GroupHeaderBkgnd"), DEF_GROUPHEADERBKCOLOR);

	if (pPrefs->GetProfileInt(sColorKey, _T("Low"), CLR_NONE) == CLR_NONE)
	{
		// Priority Range
		m_crPriorityLow = pPrefs->GetProfileInt(sColorKey, _T("PriorityLow"), DEF_PRIORITYLOWCOLOR);
		m_crPriorityHigh = pPrefs->GetProfileInt(sColorKey, _T("PriorityHigh"), DEF_PRIORITYHIGHCOLOR);

		// Priority Individual colours
		if (!Misc::Split(pPrefs->GetProfileString(sColorKey, _T("PriorityColors")), m_aPriorityColors, '|'))
			GetDefaultPriorityColors(m_aPriorityColors);

		// Priority Scheme
		Misc::Split(pPrefs->GetProfileString(sColorKey, _T("PriorityScheme")), m_aPriorityScheme, '|');

		// Attribute colours
		CStringArray aAttribColors;
		int nNumColors = Misc::Split(pPrefs->GetProfileString(sColorKey, _T("AttribColors")), aAttribColors, '|');

		m_aAttribColors.SetSize(nNumColors);
		CString sColor;

		for (int nColor = 0; nColor < nNumColors; nColor++)
		{
			ATTRIBCOLOR& at = m_aAttribColors[nColor];

			at.sAttrib = aAttribColors[nColor];
			at.color = (Misc::Split(at.sAttrib, sColor, ':') ? _ttoi(sColor) : CLR_NONE);
		}
	}
	else // Backwards compatibility
	{
		// Priority Range
		m_crPriorityLow = pPrefs->GetProfileInt(sColorKey, _T("Low"), DEF_PRIORITYLOWCOLOR);
		m_crPriorityHigh = pPrefs->GetProfileInt(sColorKey, _T("High"), DEF_PRIORITYHIGHCOLOR);

		// Priority Individual colours
		int nColor;
		m_aPriorityColors.SetSize(NUM_DEFPRIORITY);

		for (nColor = 0; nColor < NUM_DEFPRIORITY; nColor++)
		{
			CString sKey = Misc::MakeKey(_T("P%d"), nColor);
			m_aPriorityColors[nColor] = pPrefs->GetProfileInt(sColorKey, sKey, DEF_PRIORITYCOLOR[nColor]);
		}

		// Priority Scheme
		m_aPriorityScheme.SetSize(NUM_DEFPRIORITY);

		if (pPrefs->GetProfileInt(sColorKey, _T("S0"), CLR_NONE) != CLR_NONE)
		{
			for (int nColor = 0; nColor < NUM_DEFPRIORITY; nColor++)
			{
				CString sKey = Misc::MakeKey(_T("S%d"), nColor);
				m_aPriorityScheme.Add(pPrefs->GetProfileInt(sColorKey, sKey, CLR_NONE));
			}
		}

		// Attribute colours
		m_nColorAttribute = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(_T("Preferences\\AttribColors"), _T("Attribute"), TDCA_CATEGORY);

		CString sKey = Misc::MakeKey(_T("AttribColors"), NULL, szKey), sAttrib(_T("Attrib"));
		int nNumColor = pPrefs->GetProfileInt(sKey, _T("Count"), 0);

		for (nColor = 0; nColor < nNumColor; nColor++)
		{
			CString sColorKey = Misc::MakeKey(_T("\\P%d"), nColor, sKey);

			ATTRIBCOLOR ac;
			ac.color = pPrefs->GetProfileInt(sColorKey, _T("Color"), CLR_NONE);
			ac.sAttrib = pPrefs->GetProfileString(sColorKey, sAttrib);

			if (!ac.sAttrib.IsEmpty())
			{
				m_aAttribColors.Add(ac);
			}
		}
	}

	// Restore customised colour dialog colours
	CColourButton::LoadPreferences(pPrefs);
}

void CPreferencesUITasklistColorsPage::AddDefaultListItemsToAttributeColors()
{
	const CStringArray* pDefAttribs = NULL;

	switch (m_nColorAttribute)
	{
	case TDCA_CATEGORY:	pDefAttribs = &m_defaultListData.aCategory;	break;
	case TDCA_ALLOCBY:	pDefAttribs = &m_defaultListData.aAllocBy;	break;
	case TDCA_ALLOCTO:	pDefAttribs = &m_defaultListData.aAllocTo;	break;
	case TDCA_STATUS:	pDefAttribs = &m_defaultListData.aStatus;	break;
	case TDCA_VERSION:	pDefAttribs = &m_defaultListData.aVersion;	break;
	case TDCA_TAGS:		pDefAttribs = &m_defaultListData.aTags;		break;

	default: 
		return;
	}
	ASSERT(pDefAttribs);

	int nNumAttrib = pDefAttribs->GetSize();

	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		const CString& sDef = pDefAttribs->GetAt(nAtt);
		
		if (sDef.IsEmpty())
			continue;
		
		// make sure this attrib is not already specified
		if (FindAttribValue(sDef) != -1)
			continue; // skip
		
		ATTRIBCOLOR ac;
		ac.color = CLR_NONE;
		ac.sAttrib = sDef;
		
		m_aAttribColors.Add(ac);

		// add to combo
		if (m_cbAttributes.GetSafeHwnd())
			m_cbAttributes.AddColor(ac.color, ac.sAttrib);
	}
}

void CPreferencesUITasklistColorsPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("TextColorOption"), m_nTextColorOption);
	pPrefs->WriteProfileInt(szKey, _T("ColorPriority"), m_bColorPriority);
	pPrefs->WriteProfileInt(szKey, _T("PriorityColorOption"), m_nPriorityColorOption);
	pPrefs->WriteProfileString(szKey, _T("TreeFont"), m_sTreeFont);
	pPrefs->WriteProfileInt(szKey, _T("FontSize"), m_nTreeFontSize);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyTreeFont"), m_bSpecifyTreeFont);
	pPrefs->WriteProfileString(szKey, _T("CommentsFont"), m_sCommentsFont);
	pPrefs->WriteProfileInt(szKey, _T("CommentsFontSize"), m_nCommentsFontSize);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyCommentsFont"), m_bSpecifyCommentsFont);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyGridColor"), m_bSpecifyGridColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyDoneColor"), m_bSpecifyDoneColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyDueColor"), m_bSpecifyDueColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyDueTodayColor"), m_bSpecifyDueTodayColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyStartColor"), m_bSpecifyStartColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyStartTodayColor"), m_bSpecifyStartTodayColor);
	pPrefs->WriteProfileInt(szKey, _T("ColorTaskBackground"), m_bColorTaskBackground);
	pPrefs->WriteProfileInt(szKey, _T("CommentsUseTreeFont"), m_bCommentsUseTreeFont);
	pPrefs->WriteProfileInt(szKey, _T("RemindersUseTreeFont"), m_bRemindersUseTreeFont);
	pPrefs->WriteProfileInt(szKey, _T("FindTasksUseTreeFont"), m_bFindTasksUseTreeFont);
	pPrefs->WriteProfileInt(szKey, _T("HLSColorGradient"), m_bHLSColorGradient);
	pPrefs->WriteProfileInt(szKey, _T("AlternateLineColor"), m_bSpecifyAlternateLineColor);
	pPrefs->WriteProfileInt(szKey, _T("FlaggedColor"), m_bSpecifyFlaggedColor);
	pPrefs->WriteProfileInt(szKey, _T("ReferenceColor"), m_bSpecifyReferenceColor);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyGroupHeaderBkgndColor"), m_bSpecifyGroupHeaderBkgndColor);
	pPrefs->WriteProfileInt(szKey, _T("ColorAttribute"), m_nColorAttribute);

	// Colors
	CString sColorKey(szKey);
	sColorKey += _T("\\Colors");

	// Remove old entries
	pPrefs->DeleteProfileSection(sColorKey, true);
	pPrefs->DeleteProfileSection(_T("Preferences\\AttribColors"), true);
	
	pPrefs->WriteProfileInt(sColorKey, _T("Gridlines"), m_crGridlines);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDone"), m_crDone);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskStart"), m_crStart);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskStartToday"), m_crStartToday);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDue"), m_crDue);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDueToday"), m_crDueToday);
	pPrefs->WriteProfileInt(sColorKey, _T("AlternateLines"), m_crAltLine);
	pPrefs->WriteProfileInt(sColorKey, _T("Flagged"), m_crFlagged);
	pPrefs->WriteProfileInt(sColorKey, _T("Reference"), m_crReference);
	pPrefs->WriteProfileInt(sColorKey, _T("GroupHeaderBkgnd"), m_crGroupHeaderBkgnd);

	// priority colors
	pPrefs->WriteProfileInt(sColorKey, _T("PriorityLow"), m_crPriorityLow);
	pPrefs->WriteProfileInt(sColorKey, _T("PriorityHigh"), m_crPriorityHigh);

	pPrefs->WriteProfileString(sColorKey, _T("PriorityColors"), Misc::FormatArray(m_aPriorityColors, '|'));
	pPrefs->WriteProfileString(sColorKey, _T("PriorityScheme"), Misc::FormatArray(m_aPriorityScheme, '|'));

	// attrib colors
	CString sAttribColors;
	int nNumColor = m_aAttribColors.GetSize();

	for (int nColor = 0; nColor < nNumColor; nColor++)
	{
		const ATTRIBCOLOR& ac = m_aAttribColors[nColor];
		sAttribColors += Misc::Format(_T("%s:%ld|"), ac.sAttrib, ac.color);
	}
	pPrefs->WriteProfileString(sColorKey, _T("AttribColors"), sAttribColors);

	// Save customised colour dialog colours
	CColourButton::SavePreferences(pPrefs);
}

void CPreferencesUITasklistColorsPage::OnPopulateattriblist() 
{
	AddDefaultListItemsToAttributeColors();
}

void CPreferencesUITasklistColorsPage::SetDefaultListData(const TDCAUTOLISTDATA& defaultListData)
{
	m_defaultListData.Copy(defaultListData, TDCA_ALL);
}

void CPreferencesUITasklistColorsPage::SetNumPriorityRiskLevels(int nNumLevels)
{
	ASSERT(TDC::IsValidNumPriorityRiskLevels(nNumLevels));

	m_nNumPriorityRiskLevels = nNumLevels;
	m_cbPriorityColors.SetNumLevels(nNumLevels);

	CColorBrewer brewer(CBF_SYNTHESIZE | CBF_TEXTSAFE);
	CColorBrewerPaletteArray aPalettes;

	brewer.GetPalettes(CBPT_SEQUENTIAL, aPalettes, nNumLevels);
	brewer.GetPalettes(CBPT_DIVERGING, aPalettes, nNumLevels, TRUE);

	m_cbPriorityScheme.Initialize(aPalettes);

}

void CPreferencesUITasklistColorsPage::OnSelchangeAttributetocolorby() 
{
	UpdateData(TRUE);
}

void CPreferencesUITasklistColorsPage::GetDefaultPriorityColors(CDWordArray& aColors)
{
	aColors.SetSize(NUM_DEFPRIORITY);

	for (int nColor = 0; nColor < NUM_DEFPRIORITY; nColor++)
		aColors[nColor] = DEF_PRIORITYCOLOR[nColor];
}
