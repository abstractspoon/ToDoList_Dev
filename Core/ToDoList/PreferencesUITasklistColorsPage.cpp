// PreferencesUITasklistColorsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUITasklistColorsPage.h"

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
// CPreferencesUITasklistColorsPage property page

// default colors
const COLORREF PRIORITYLOWCOLOR		= RGB(30, 225, 0);
const COLORREF PRIORITYHIGHCOLOR	= RGB(255, 0, 0);
const COLORREF CBMASKCOLOR			= RGB(255, 0, 0);
const COLORREF ALTERNATELINECOLOR	= RGB(235, 235, 255);
const COLORREF GRIDLINECOLOR		= RGB(192, 192, 192);
const COLORREF TASKDONECOLOR		= RGB(128, 128, 128);
const COLORREF TASKDUECOLOR			= RGB(255, 0, 0);
const COLORREF TASKDUETODAYCOLOR	= RGB(255, 128, 0);
const COLORREF TASKSTARTCOLOR		= RGB(0, 255, 0);
const COLORREF FILTEREDCOLOR		= RGB(200, 200, 200);
const COLORREF FLAGGEDCOLOR			= RGB(128, 64, 0);
const COLORREF REFERENCECOLOR		= RGB(128, 0, 64);

const int DEFFONTSIZE = 8;
const TDC_ATTRIBUTE DEFCOLORATTRIB = TDCA_CATEGORY;

enum
{
	COLORPRIORITYBY_INDIVIDUAL,
	COLORPRIORITYBY_GRADIENT,
	COLORPRIORITYBY_SCHEME,
};

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUITasklistColorsPage property page

//IMPLEMENT_DYNCREATE(CPreferencesUITasklistColorsPage, CPreferencesPageBase)

CPreferencesUITasklistColorsPage::CPreferencesUITasklistColorsPage() 
	: 
	CPreferencesPageBase(CPreferencesUITasklistColorsPage::IDD),
	m_nTextColorOption(COLOROPT_DEFAULT), 
	m_cbAttributes(CCBS_DRAWNOCOLOR, ACBS_ALLOWDELETE), 
	m_nColorAttribute(DEFCOLORATTRIB)
{
	//{{AFX_DATA_INIT(CPreferencesUITasklistColorsPage)
	//}}AFX_DATA_INIT
	// priority colors
	m_nSelPriorityColor = 0; 
	m_nPriorityColorOption = COLORPRIORITYBY_GRADIENT;
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
	DDX_Check(pDX, IDC_COMMENTSUSETREEFONT, m_bCommentsUseTreeFont);
	DDX_Check(pDX, IDC_USEHLSGRADIENT, m_bHLSColorGradient);
	DDX_Check(pDX, IDC_HIDEPRIORITYNUMBER, m_bHidePriorityNumber);
	DDX_Check(pDX, IDC_ALTERNATELINECOLOR, m_bAlternateLineColor);
	//}}AFX_DATA_MAP
	DDX_CBString(pDX, IDC_ATTRIBUTECOLORS, m_sSelAttribValue);
	DDX_Control(pDX, IDC_SETATTRIBUTECOLOR, m_btAttribColor);
	DDX_Control(pDX, IDC_ATTRIBUTECOLORS, m_cbAttributes);
	DDX_Radio(pDX, IDC_COLORTEXTBYATTRIBUTE, m_nTextColorOption);
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
	DDX_Control(pDX, IDC_SETPRIORITYCOLOR, m_btSetColor);
	DDX_Control(pDX, IDC_LOWPRIORITYCOLOR, m_btLowColor);
	DDX_Control(pDX, IDC_HIGHPRIORITYCOLOR, m_btHighColor);
	DDX_Check(pDX, IDC_COLORPRIORITY, m_bColorPriority);
	DDX_Radio(pDX, IDC_INDIVIDUALPRIORITYCOLORS, m_nPriorityColorOption);
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
	ON_BN_CLICKED(IDC_COLORTEXTBYATTRIBUTE, OnChangeTextColorOption)
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
	ON_BN_CLICKED(IDC_COLORTASKSBYCOLOR, OnChangeTextColorOption)
	ON_BN_CLICKED(IDC_COLORTEXTBYPRIORITY, OnChangeTextColorOption)
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

	GetDlgItem(IDC_PRIORITYCOLORS)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_INDIVIDUAL));
	GetDlgItem(IDC_USEHLSGRADIENT)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));
	GetDlgItem(IDC_PRIORITYSCHEMES)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_SCHEME));

	GetDlgItem(IDC_TREEFONTSIZE)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZELABEL)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTLIST)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_COMMENTSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_SPECIFYCOMMENTSFONT)->EnableWindow(!m_bCommentsUseTreeFont || !m_bSpecifyTreeFont);

	BOOL bColorByAttrib = (m_nTextColorOption == COLOROPT_ATTRIB);
	BOOL bHasAttrib = (m_nColorAttribute != TDCA_NONE);

	GetDlgItem(IDC_ATTRIBUTETOCOLORBY)->EnableWindow(bColorByAttrib);
	GetDlgItem(IDC_ATTRIBUTECOLORS)->EnableWindow(bColorByAttrib && bHasAttrib);
	GetDlgItem(IDC_POPULATEATTRIBLIST)->EnableWindow(bColorByAttrib && bHasAttrib);

	BOOL bEnableCommentsFont = m_bSpecifyCommentsFont && (!m_bSpecifyTreeFont || !m_bCommentsUseTreeFont);
	GetDlgItem(IDC_COMMENTSFONTSIZE)->EnableWindow(bEnableCommentsFont);
	GetDlgItem(IDC_COMMENTSFONTSIZELABEL)->EnableWindow(bEnableCommentsFont);
	GetDlgItem(IDC_COMMENTSFONTLIST)->EnableWindow(bEnableCommentsFont);

	m_btSetColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_INDIVIDUAL));
	m_btLowColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));
	m_btHighColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));
	m_btGridlineColor.EnableWindow(m_bSpecifyGridColor);
	m_btDoneColor.EnableWindow(m_bSpecifyDoneColor);
	m_btAltLineColor.EnableWindow(m_bAlternateLineColor);
	m_btStartColor.EnableWindow(m_bSpecifyStartColor);
	m_btStartTodayColor.EnableWindow(m_bSpecifyStartTodayColor);
	m_btDueColor.EnableWindow(m_bSpecifyDueColor);
	m_btDueTodayColor.EnableWindow(m_bSpecifyDueTodayColor);
	m_btAttribColor.EnableWindow(bColorByAttrib && bHasAttrib && !m_sSelAttribValue.IsEmpty());
	m_btFlaggedColor.EnableWindow(m_bSpecifyFlaggedColor);
	m_btReferenceColor.EnableWindow(m_bSpecifyReferenceColor);
	
	m_btGridlineColor.SetColor(m_crGridlines);
	m_btLowColor.SetColor(m_crLow);
	m_btHighColor.SetColor(m_crHigh);
	m_btSetColor.SetColor(m_aPriorityColors[0]);
	m_btDoneColor.SetColor(m_crDone);
	m_btAltLineColor.SetColor(m_crAltLine);
	m_btStartColor.SetColor(m_crStart);
	m_btStartTodayColor.SetColor(m_crStartToday);
	m_btDueColor.SetColor(m_crDue);
	m_btDueTodayColor.SetColor(m_crDueToday);
	m_btFlaggedColor.SetColor(m_crFlagged);
	m_btReferenceColor.SetColor(m_crReference);

	// priority colors
	for (int nPriority = 0; nPriority < m_aPriorityColors.GetSize(); nPriority++)
		m_cbPriorityColors.SetColor(nPriority, (COLORREF)m_aPriorityColors[nPriority]);

	// Priority colors schemes
	CColorBrewer brewer(CBF_SYNTHESIZE | CBF_TEXTSAFE);
	CColorBrewerPaletteArray aPalettes;

	brewer.GetPalettes(CBPT_SEQUENTIAL, aPalettes, 11);
	brewer.GetPalettes(CBPT_DIVERGING, aPalettes, 11, TRUE);

	m_cbPriorityScheme.Initialize(aPalettes);

	if (m_cbPriorityScheme.SetSelectedPalette(m_aPriorityScheme) == CB_ERR)
		m_cbPriorityScheme.SetCurSel(0);
	
	// attribute colors
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_CATEGORY),	TDCA_CATEGORY);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_STATUS),	TDCA_STATUS);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_ALLOCTO),	TDCA_ALLOCTO);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_ALLOCBY),	TDCA_ALLOCBY);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_VERSION),	TDCA_VERSION);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_EXTERNALID),TDCA_EXTERNALID);
	AddString(m_cbColorByAttribute, CEnString(IDS_TDLBC_TAGS),		TDCA_TAGS);

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
	m_crLow = m_btLowColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnHighprioritycolor() 
{
	m_crHigh = m_btHighColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSetprioritycolor() 
{
	VERIFY(m_nSelPriorityColor >= 0);

	m_aPriorityColors.SetAt(m_nSelPriorityColor, m_btSetColor.GetColor());
	m_cbPriorityColors.SetColor(m_nSelPriorityColor, m_btSetColor.GetColor());

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnChangePriorityColorOption() 
{
	UpdateData();

	GetDlgItem(IDC_PRIORITYCOLORS)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_INDIVIDUAL));
	GetDlgItem(IDC_USEHLSGRADIENT)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));
	GetDlgItem(IDC_PRIORITYSCHEMES)->EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_SCHEME));

	m_btSetColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_INDIVIDUAL) && m_nSelPriorityColor >= 0);
	m_btLowColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));
	m_btHighColor.EnableWindow(m_bColorPriority && (m_nPriorityColorOption == COLORPRIORITYBY_GRADIENT));

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
	if (!m_bColorPriority && (m_nTextColorOption == COLOROPT_PRIORITY))
	{
		m_nTextColorOption = COLOROPT_DEFAULT;
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
		case COLORPRIORITYBY_INDIVIDUAL:
			aColors.Copy(m_aPriorityColors); 
			break;

		case COLORPRIORITYBY_GRADIENT:	
			GraphicsMisc::CalculateColorGradient(m_crLow, m_crHigh, 11, aColors, !m_bHLSColorGradient);
			break;

		case COLORPRIORITYBY_SCHEME:
			aColors.Copy(m_aPriorityScheme);
			break;
		}
	}
	else // grayscale
	{
		GraphicsMisc::CalculateColorGradient(RGB(240, 240, 240), 0, 11, aColors, TRUE);
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
		m_btSetColor.SetColor(m_aPriorityColors[m_nSelPriorityColor]);
		m_btSetColor.EnableWindow(TRUE);
	}
	else
		m_btSetColor.EnableWindow(FALSE);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnSpecifytreefont() 
{
	UpdateData();

	GetDlgItem(IDC_TREEFONTLIST)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZE)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_TREEFONTSIZELABEL)->EnableWindow(m_bSpecifyTreeFont);
	GetDlgItem(IDC_COMMENTSUSETREEFONT)->EnableWindow(m_bSpecifyTreeFont);

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
	
	m_btAltLineColor.EnableWindow(m_bAlternateLineColor);

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesUITasklistColorsPage::OnChangeTextColorOption() 
{
	UpdateData();

	// if the text color option is COLOROPT_PRIORITY and 
	// the user has got priority coloring turned off then switch it on
	if (!m_bColorPriority && (m_nTextColorOption == COLOROPT_PRIORITY))
	{
		m_bColorPriority = TRUE;
		UpdateData(FALSE);

		OnColorPriority();
	}

	BOOL bColorByAttrib = (m_nTextColorOption == COLOROPT_ATTRIB);
	BOOL bHasAttrib = (m_nColorAttribute != TDCA_NONE);

	GetDlgItem(IDC_ATTRIBUTETOCOLORBY)->EnableWindow(bColorByAttrib);
	GetDlgItem(IDC_ATTRIBUTECOLORS)->EnableWindow(bColorByAttrib && bHasAttrib);
	GetDlgItem(IDC_POPULATEATTRIBLIST)->EnableWindow(bColorByAttrib && bHasAttrib);

	m_btAttribColor.EnableWindow(bColorByAttrib && bHasAttrib && !m_sSelAttribValue.IsEmpty());

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

	m_btAttribColor.EnableWindow(m_nTextColorOption == COLOROPT_ATTRIB && 
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

	m_btAttribColor.EnableWindow(m_nTextColorOption == COLOROPT_ATTRIB && 
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
	// prefs
	m_bColorPriority = pPrefs->GetProfileInt(szKey, _T("ColorPriority"), TRUE);
	m_nPriorityColorOption = pPrefs->GetProfileInt(szKey, _T("PriorityColorOption"), -1);
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
	m_bHLSColorGradient = pPrefs->GetProfileInt(szKey, _T("HLSColorGradient"), TRUE);
	m_bHidePriorityNumber = pPrefs->GetProfileInt(szKey, _T("HidePriorityNumber"), FALSE);
	m_bAlternateLineColor = pPrefs->GetProfileInt(szKey, _T("AlternateLineColor"), TRUE);
	m_bSpecifyFlaggedColor = pPrefs->GetProfileInt(szKey, _T("FlaggedColor"), FALSE);
	m_bSpecifyReferenceColor = pPrefs->GetProfileInt(szKey, _T("ReferenceColor"), FALSE);

	CColourButton::LoadPreferences(pPrefs);

	// colors
	CString sColorKey(szKey);
	sColorKey += _T("\\Colors");
	
	m_aPriorityColors.RemoveAll();
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P0"), RGB(30, 225, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P1"), RGB(30, 225, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P2"), RGB(30, 225, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P3"), RGB(30, 225, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P4"), RGB(0, 0, 255)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P5"), RGB(0, 0, 255)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P6"), RGB(0, 0, 255)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P7"), RGB(0, 0, 255)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P8"), RGB(255, 0, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P9"), RGB(255, 0, 0)));
	m_aPriorityColors.Add(pPrefs->GetProfileInt(sColorKey, _T("P10"), RGB(255, 0, 0)));

	m_aPriorityScheme.RemoveAll();

	if (pPrefs->GetProfileInt(sColorKey, _T("S0"), -1) != -1)
	{
		for (int nColor = 0; nColor < 11; nColor++)
		{
			CString sKey = Misc::MakeKey(_T("S%d"), nColor);
			m_aPriorityScheme.Add(pPrefs->GetProfileInt(sColorKey, sKey));
		}
	}

	m_crLow = pPrefs->GetProfileInt(sColorKey, _T("Low"), PRIORITYLOWCOLOR);
	m_crHigh = pPrefs->GetProfileInt(sColorKey, _T("High"), PRIORITYHIGHCOLOR);
	m_crGridlines = pPrefs->GetProfileInt(sColorKey, _T("Gridlines"), GRIDLINECOLOR);
	m_crDone = pPrefs->GetProfileInt(sColorKey, _T("TaskDone"), TASKDONECOLOR);
	m_crStart = pPrefs->GetProfileInt(sColorKey, _T("TaskStart"), TASKSTARTCOLOR);
	m_crStartToday = pPrefs->GetProfileInt(sColorKey, _T("TaskStartToday"), TASKSTARTCOLOR);
	m_crDue = pPrefs->GetProfileInt(sColorKey, _T("TaskDue"), TASKDUECOLOR);
	m_crDueToday = pPrefs->GetProfileInt(sColorKey, _T("TaskDueToday"), TASKDUETODAYCOLOR);
	m_crAltLine = pPrefs->GetProfileInt(sColorKey, _T("AlternateLines"), ALTERNATELINECOLOR);
	m_crFlagged = pPrefs->GetProfileInt(sColorKey, _T("Flagged"), FLAGGEDCOLOR);
	m_crReference = pPrefs->GetProfileInt(sColorKey, _T("Reference"), REFERENCECOLOR);

	// bkwds compatibility
	if (pPrefs->GetProfileInt(szKey, _T("ColorByPriority"), FALSE))
		m_nTextColorOption = COLOROPT_PRIORITY;

	// bkwds compatibility
	if (m_nPriorityColorOption == -1)
	{
		if (pPrefs->GetProfileInt(szKey, _T("IndividualPriorityColors"), FALSE))
			m_nPriorityColorOption = COLORPRIORITYBY_INDIVIDUAL;
		else
			m_nPriorityColorOption = COLORPRIORITYBY_GRADIENT;
	}
	
	m_nTextColorOption = pPrefs->GetProfileInt(szKey, _T("TextColorOption"), m_nTextColorOption);

	// attribute colors
	m_nColorAttribute = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(_T("Preferences\\AttribColors"), _T("Attribute"), TDCA_CATEGORY);

	CString sKey = Misc::MakeKey(_T("AttribColors"), NULL, szKey), sAttrib(_T("Attrib"));
	int nNumColor = pPrefs->GetProfileInt(sKey, _T("Count"), -1);

	for (int nColor = 0; nColor < nNumColor; nColor++)
	{
		CString sColorKey = Misc::MakeKey(_T("\\P%d"), nColor, sKey);

		ATTRIBCOLOR ac;
		ac.color = pPrefs->GetProfileInt(sColorKey, _T("Color"), 0);
		ac.sAttrib = pPrefs->GetProfileString(sColorKey, sAttrib);

		if (!ac.sAttrib.IsEmpty())
		{
			m_aAttribColors.Add(ac);
		}
	}
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

	for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
	{
		const CString& sDef = pDefAttribs->GetAt(nAttrib);
		
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
	// save settings
	CString sColorKey(szKey);
	sColorKey += _T("\\Colors");
	
	// priority colors
	pPrefs->WriteProfileInt(sColorKey, _T("Low"), m_crLow);
	pPrefs->WriteProfileInt(sColorKey, _T("High"), m_crHigh);

	int nColor = m_aPriorityColors.GetSize();

	while (nColor--)
	{
		CString sKey = Misc::MakeKey(_T("P%d"), nColor);
		pPrefs->WriteProfileInt(sColorKey, sKey, m_aPriorityColors[nColor]);
	}

	nColor = m_aPriorityScheme.GetSize();

	while (nColor--)
	{
		CString sKey = Misc::MakeKey(_T("S%d"), nColor);
		pPrefs->WriteProfileInt(sColorKey, sKey, m_aPriorityScheme[nColor]);
	}

	// attrib colors
	int nNumColor = m_aAttribColors.GetSize();

	pPrefs->WriteProfileInt(_T("Preferences\\AttribColors"), _T("Attribute"), m_nColorAttribute);
	pPrefs->WriteProfileInt(_T("Preferences\\AttribColors"), _T("Count"), nNumColor);

	for (nColor = 0; nColor < nNumColor; nColor++)
	{
		CString sKey = Misc::MakeKey(_T("Preferences\\AttribColors\\P%d"), nColor);

		const ATTRIBCOLOR& ac = m_aAttribColors[nColor];
		pPrefs->WriteProfileInt(sKey, _T("Color"), ac.color);
		pPrefs->WriteProfileString(sKey, _T("Attrib"), ac.sAttrib);
	}

	// save settings
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
	pPrefs->WriteProfileInt(szKey, _T("HLSColorGradient"), m_bHLSColorGradient);
	pPrefs->WriteProfileInt(szKey, _T("HidePriorityNumber"), m_bHidePriorityNumber);
	pPrefs->WriteProfileInt(szKey, _T("AlternateLineColor"), m_bAlternateLineColor);
	pPrefs->WriteProfileInt(szKey, _T("FlaggedColor"), m_bSpecifyFlaggedColor);
	pPrefs->WriteProfileInt(szKey, _T("ReferenceColor"), m_bSpecifyReferenceColor);

	CColourButton::SavePreferences(pPrefs);

	pPrefs->WriteProfileInt(sColorKey, _T("Gridlines"), m_crGridlines);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDone"), m_crDone);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskStart"), m_crStart);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskStartToday"), m_crStartToday);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDue"), m_crDue);
	pPrefs->WriteProfileInt(sColorKey, _T("TaskDueToday"), m_crDueToday);
	pPrefs->WriteProfileInt(sColorKey, _T("AlternateLines"), m_crAltLine);
	pPrefs->WriteProfileInt(sColorKey, _T("Flagged"), m_crFlagged);
	pPrefs->WriteProfileInt(sColorKey, _T("Reference"), m_crReference);
}

void CPreferencesUITasklistColorsPage::OnPopulateattriblist() 
{
	AddDefaultListItemsToAttributeColors();
}

void CPreferencesUITasklistColorsPage::SetDefaultListData(const TDCAUTOLISTDATA& defaultListData)
{
	m_defaultListData.Copy(defaultListData, TDCA_ALL);
}

void CPreferencesUITasklistColorsPage::OnSelchangeAttributetocolorby() 
{
	UpdateData(TRUE);
}
