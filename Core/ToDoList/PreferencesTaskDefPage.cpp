// PreferencesTaskDefPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskDefPage.h"
#include "tdcenum.h"
#include "tdcmapping.h"
#include "todoitem.h"
#include "tdltaskicondlg.h"
#include "tdlcontentmgr.h"
#include "tdcdialoghelper.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\graphicsmisc.h"

#include "..\3rdParty\Base64Coder.h"

#include "..\Interfaces\uithemefile.h"
#include "..\interfaces\icontentcontrol.h"
#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const UINT WM_PTDP_INITCOMMENTS = (WM_APP + 1);
const UINT COMMENTS_COMBOLENDLU = 122;

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ENDL		= _T("\r\n");
const LPCTSTR NO_SOUND	= _T("None");

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage property page

IMPLEMENT_DYNCREATE(CPreferencesTaskDefPage, CPreferencesPageBase)

CPreferencesTaskDefPage::CPreferencesTaskDefPage(const CTDLContentMgr* pMgrContent) 
	: 
	CPreferencesPageBase(CPreferencesTaskDefPage::IDD),
	m_pMgrContent(pMgrContent), 
	m_ctrlComments(FALSE, FALSE, COMMENTS_COMBOLENDLU, pMgrContent, NULL),
	m_cbDefReminder(TDLRPC_SHOWNONE | TDLRPC_SHOWZERO),
	m_cbDefPriority(FALSE),
	m_cbDefRisk(FALSE),
	m_nDefReminderLeadinMins(TDLRPC_NOREMINDER)
{
	//{{AFX_DATA_INIT(CPreferencesTaskDefPage)
	m_sReminderSound = _T("");
	//}}AFX_DATA_INIT
	m_bReminderBeforeDue = TRUE;
	m_eCost.SetMask(_T("@.0123456789"), ME_LOCALIZEDECIMAL);
}

CPreferencesTaskDefPage::~CPreferencesTaskDefPage()
{
}

void CPreferencesTaskDefPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskDefPage)
	DDX_Control(pDX, IDC_PLAYSOUND, m_eReminderSound);
	DDX_Text(pDX, IDC_PLAYSOUND, m_sReminderSound);
	DDX_Control(pDX, IDC_DEFREMINDER, m_cbDefReminder);
	DDX_Control(pDX, IDC_DEFAULTRISK, m_cbDefRisk);
	DDX_Control(pDX, IDC_DEFAULTPRIORITY, m_cbDefPriority);
	DDX_CBIndex(pDX, IDC_DEFREMINDERDATE, m_bReminderBeforeDue);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEFAULTTIMESPENT, m_eTimeSpent);
	DDX_Control(pDX, IDC_DEFAULTTIMEEST, m_eTimeEst);
	DDX_Control(pDX, IDC_DEFAULTCOST, m_eCost);
	DDX_Control(pDX, IDC_SETDEFAULTCOLOR, m_btDefColor);
	DDX_Control(pDX, IDC_SETDEFAULTICON, m_btDefIcon);

	DDX_Text(pDX, IDC_DEFAULTCREATEDBY, m_sDefCreatedBy);
	DDX_Text(pDX, IDC_DEFAULTALLOCTO, m_sDefAllocTo);
	DDX_Text(pDX, IDC_DEFAULTALLOCBY, m_sDefAllocBy);
	DDX_Text(pDX, IDC_DEFAULTSTATUS, m_sDefStatus);
	DDX_Text(pDX, IDC_DEFAULTTAGS, m_sDefTags);
	DDX_Text(pDX, IDC_DEFAULTCATEGORY, m_sDefCategory);
	DDX_Check(pDX, IDC_USECREATIONFORDEFSTARTDATE, m_bUseCreationDateForDefStartDate);
	DDX_Check(pDX, IDC_USECREATIONTIMEFORDEFSTARTDATE, m_bUseCreationTimeForDefStartDate);
	DDX_Check(pDX, IDC_USECREATIONFORDEFDUEDATE, m_bUseCreationDateForDefDueDate);

	CTDCDialogHelper::DDX_Text(pDX, IDC_DEFAULTCOST, m_defCost);
	CTDCDialogHelper::DDX_Text(pDX, m_eTimeEst, m_defTimeEst);
	CTDCDialogHelper::DDX_Text(pDX, m_eTimeSpent, m_defTimeSpent);

	m_cbDefPriority.DDX(pDX, m_nDefPriority);
	m_cbDefRisk.DDX(pDX, m_nDefRisk);
	m_cbDefReminder.DDX(pDX, m_nDefReminderLeadinMins);
}

BEGIN_MESSAGE_MAP(CPreferencesTaskDefPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesTaskDefPage)
	ON_CBN_SELCHANGE(IDC_DEFREMINDER, OnSelchangeReminder)
	ON_BN_CLICKED(IDC_USECREATIONFORDEFSTARTDATE, OnUseCreationDateForDefStartDate)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETDEFAULTICON, OnSetdefaulticon)
	ON_BN_CLICKED(IDC_SETDEFAULTCOLOR, OnSetdefaultcolor)
	ON_CBN_SELENDOK(IDC_COMMENTS, OnSelchangeCommentsformat)
	ON_REGISTERED_MESSAGE(WM_ICC_CONTENTCHANGE, OnCommentsChange)
	ON_MESSAGE(WM_PTDP_INITCOMMENTS, OnInitComments)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage message handlers

BOOL CPreferencesTaskDefPage::OnInitDialog()
{
	CPreferencesPageBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesTaskDefPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();
	
	AddGroupLine(IDC_DEFGROUP);

	GetDlgItem(IDC_DEFREMINDERDATE)->EnableWindow(m_nDefReminderLeadinMins != TDLRPC_NOREMINDER);
	GetDlgItem(IDC_PLAYSOUND)->EnableWindow(m_nDefReminderLeadinMins != TDLRPC_NOREMINDER);
	GetDlgItem(IDC_USECREATIONTIMEFORDEFSTARTDATE)->EnableWindow(m_bUseCreationDateForDefStartDate);	

	m_btDefColor.SetColor(m_crDef);
	m_ilTaskIcons.LoadDefaultImages();

	int nIcon = m_ilTaskIcons.GetImageIndex(m_sDefIcon);
	m_btDefIcon.SetIcon((nIcon == -1) ? NULL : m_ilTaskIcons.ExtractIcon(nIcon));

	// init edit prompts()
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTALLOCTO, *this, CEnString(IDS_TDC_NOBODY));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTALLOCBY, *this, CEnString(IDS_TDC_NOBODY));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTSTATUS, *this, CEnString(IDS_TDC_NONE));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTTAGS, *this, CEnString(IDS_TDC_NONE));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTCATEGORY, *this, CEnString(IDS_TDC_NONE));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTCREATEDBY, *this, Misc::GetUserName());
	m_mgrPrompts.SetEditPrompt(IDC_PLAYSOUND, *this, CEnString(IDS_NOSOUND));

	VERIFY(m_ctrlComments.Create(this, IDC_COMMENTS));

	// Delay initialisation of comments until after any font changes
	PostMessage(WM_PTDP_INITCOMMENTS);
}

BOOL CPreferencesTaskDefPage::GetReminder(TDCREMINDER& rem) const
{
	if (m_nDefReminderLeadinMins == TDLRPC_NOREMINDER)
		return FALSE;

	// else
	rem.bRelative = TRUE;
	rem.dRelativeDaysLeadIn = (m_nDefReminderLeadinMins / (24 * 60.0));
	rem.nRelativeFromWhen = (m_bReminderBeforeDue ? TDCR_DUEDATE : TDCR_STARTDATE);
	rem.sSoundFile = m_sReminderSound;

	return TRUE;
}

void CPreferencesTaskDefPage::SetPriorityColors(const CDWordArray& aColors)
{
	m_cbDefPriority.SetColors(aColors);
}

void CPreferencesTaskDefPage::SetDefaultCommentsFont(const CString& sFaceName, int nPointSize)
{
	if (!GraphicsMisc::SameFont(m_fontDefComments, sFaceName, nPointSize))
	{
		VERIFY(GraphicsMisc::CreateFont(m_fontDefComments, sFaceName, nPointSize));
		m_ctrlComments.SetContentFont(m_fontDefComments);
	}
}

void CPreferencesTaskDefPage::OnOK() 
{
	CPreferencesPageBase::OnOK();

	if (m_sReminderSound.IsEmpty())
		m_sReminderSound = NO_SOUND;
}

BOOL CPreferencesTaskDefPage::PreTranslateMessage(MSG* pMsg)
{
	if (m_ctrlComments.ProcessMessage(pMsg))
		return TRUE;

	return CPreferencesPageBase::PreTranslateMessage(pMsg);
}

void CPreferencesTaskDefPage::OnSetdefaultcolor() 
{
	m_crDef = m_btDefColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskDefPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_nDefPriority = pPrefs->GetProfileInt(szKey, _T("DefaultPriority"), 5); 
	m_nDefRisk = pPrefs->GetProfileInt(szKey, _T("DefaultRisk"), 0); 
	m_sDefAllocTo = pPrefs->GetProfileString(szKey, _T("DefaultAllocTo"));
	m_sDefAllocBy = pPrefs->GetProfileString(szKey, _T("DefaultAllocBy"));
	m_sDefStatus = pPrefs->GetProfileString(szKey, _T("DefaultStatus"));
	m_sDefTags = pPrefs->GetProfileString(szKey, _T("DefaultTags"));
	m_sDefCategory = pPrefs->GetProfileString(szKey, _T("DefaultCategory"));
	m_sDefCreatedBy = pPrefs->GetProfileString(szKey, _T("DefaultCreatedBy"));
	m_crDef = pPrefs->GetProfileInt(szKey, _T("DefaultColor"), 0);
	m_bUseCreationDateForDefStartDate = pPrefs->GetProfileInt(szKey, _T("UseCreationForDefStartDate"), TRUE);
	m_bUseCreationTimeForDefStartDate = pPrefs->GetProfileInt(szKey, _T("UseCreationTimeForDefStartDate"), FALSE);
	m_bUseCreationDateForDefDueDate = pPrefs->GetProfileInt(szKey, _T("UseCreationForDefDueDate"), FALSE);
	m_defCost = pPrefs->GetProfileString(szKey, _T("DefaultCost"), _T("0"));
	m_defTimeEst.dAmount = pPrefs->GetProfileDouble(szKey, _T("DefaultTimeEstimate"), 0);
	m_defTimeEst.SetTHUnits((TH_UNITS)pPrefs->GetProfileInt(szKey, _T("DefaultTimeEstUnits"), THU_HOURS), FALSE);
	m_defTimeSpent.dAmount = pPrefs->GetProfileDouble(szKey, _T("DefaultTimeSpent"), 0);
	m_defTimeSpent.SetTHUnits((TH_UNITS)pPrefs->GetProfileInt(szKey, _T("DefaultTimeSpentUnits"), THU_HOURS), FALSE);
	m_sDefIcon = pPrefs->GetProfileString(szKey, _T("DefaultIcon"));
	m_nDefReminderLeadinMins = pPrefs->GetProfileInt(szKey, _T("DefaultReminderLeadin"), TDLRPC_NOREMINDER);
	m_bReminderBeforeDue = pPrefs->GetProfileInt(szKey, _T("ReminderBeforeDue"), TRUE);

	m_sReminderSound = pPrefs->GetProfileString(szKey, _T("ReminderSound"));

	if (m_sReminderSound == NO_SOUND)
	{
		m_sReminderSound.Empty();
	}
	else if (m_sReminderSound.IsEmpty()) // first time initialisation
	{
		m_sReminderSound = CSoundEdit::GetWindowsSound(_T("tada"));
	}
	
	CColourButton::LoadPreferences(pPrefs);

	// comments format
	m_cfDefault = pPrefs->GetProfileString(szKey, _T("DefaultCommentsFormatID"));

	// fallback
	if (m_pMgrContent && (m_pMgrContent->FindContent(m_cfDefault) == -1))
	{
		ASSERT(m_pMgrContent->GetNumContent());
		m_cfDefault = m_pMgrContent->GetContentFormat(0);
	}

	CString sB64CustomComments = pPrefs->GetProfileString(szKey, _T("DefaultCustomComments"));
	m_defCustomComments.Base64Decode(sB64CustomComments);

	CString sB64Comments = pPrefs->GetProfileString(szKey, _T("DefaultComments"));
	m_sDefTextComments = Base64Coder::Decode(sB64Comments);

	if (m_sDefTextComments.IsEmpty())
		m_defCustomComments.Empty();
}

void CPreferencesTaskDefPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("DefaultPriority"), m_nDefPriority);
	pPrefs->WriteProfileInt(szKey, _T("DefaultRisk"), m_nDefRisk);
	pPrefs->WriteProfileString(szKey, _T("DefaultAllocTo"), m_sDefAllocTo);
	pPrefs->WriteProfileString(szKey, _T("DefaultAllocBy"), m_sDefAllocBy);
	pPrefs->WriteProfileString(szKey, _T("DefaultStatus"), m_sDefStatus);
	pPrefs->WriteProfileString(szKey, _T("DefaultTags"), m_sDefTags);
	pPrefs->WriteProfileString(szKey, _T("DefaultCategory"), m_sDefCategory);
	pPrefs->WriteProfileString(szKey, _T("DefaultCreatedBy"), m_sDefCreatedBy);
	pPrefs->WriteProfileString(szKey, _T("DefaultIcon"), m_sDefIcon);
	pPrefs->WriteProfileInt(szKey, _T("DefaultColor"), m_crDef);
	pPrefs->WriteProfileInt(szKey, _T("UseCreationForDefStartDate"), m_bUseCreationDateForDefStartDate);
	pPrefs->WriteProfileInt(szKey, _T("UseCreationTimeForDefStartDate"), m_bUseCreationTimeForDefStartDate);
	pPrefs->WriteProfileInt(szKey, _T("UseCreationForDefDueDate"), m_bUseCreationDateForDefDueDate);
	pPrefs->WriteProfileString(szKey, _T("DefaultCost"), m_defCost.Format());
	pPrefs->WriteProfileDouble(szKey, _T("DefaultTimeEstimate"), m_defTimeEst.dAmount);
	pPrefs->WriteProfileInt(szKey, _T("DefaultTimeEstUnits"), m_defTimeEst.GetTHUnits());
	pPrefs->WriteProfileDouble(szKey, _T("DefaultTimeSpent"), m_defTimeSpent.dAmount);
	pPrefs->WriteProfileInt(szKey, _T("DefaultTimeSpentUnits"), m_defTimeSpent.GetTHUnits());
	pPrefs->WriteProfileInt(szKey, _T("DefaultReminderLeadin"), m_nDefReminderLeadinMins);
	pPrefs->WriteProfileInt(szKey, _T("ReminderBeforeDue"), m_bReminderBeforeDue);
	pPrefs->WriteProfileString(szKey, _T("ReminderSound"), m_sReminderSound);

	m_btDefColor.SavePreferences(pPrefs);
	
	// comments format
	if (m_pMgrContent)
	{
		pPrefs->WriteProfileString(szKey, _T("DefaultCommentsFormatID"), m_cfDefault);

		Base64Coder b64;
		CString sEncoded;

		pPrefs->DeleteProfileEntry(szKey, _T("DefaultComments"));
		pPrefs->DeleteProfileEntry(szKey, _T("DefaultCustomComments"));

		if (!m_sDefTextComments.IsEmpty())
		{
			pPrefs->WriteProfileString(szKey, _T("DefaultComments"), Base64Coder::Encode(m_sDefTextComments));

			if (!m_defCustomComments.IsEmpty() && m_defCustomComments.Base64Encode(sEncoded))
			{
				pPrefs->WriteProfileString(szKey, _T("DefaultCustomComments"), sEncoded);
			}
		}
	}
}

void CPreferencesTaskDefPage::GetTaskAttributes(TODOITEM& tdiDefault) const
{
	tdiDefault = TODOITEM(); // clear all attributes

	tdiDefault.sTitle = CEnString(IDS_TASK);
	tdiDefault.color = m_crDef;
	tdiDefault.sAllocBy = m_sDefAllocBy;
	tdiDefault.sStatus = m_sDefStatus;
	tdiDefault.timeEstimate = m_defTimeEst;
	tdiDefault.timeSpent = m_defTimeSpent;
	tdiDefault.cost = m_defCost;
	tdiDefault.nPriority = m_nDefPriority;
	tdiDefault.nRisk = m_nDefRisk;
	tdiDefault.sIcon = m_sDefIcon;

	if (!m_sDefCreatedBy.IsEmpty())
		tdiDefault.sCreatedBy = m_sDefCreatedBy;
	else
		tdiDefault.sCreatedBy = Misc::GetUserName();

	if (m_bUseCreationDateForDefStartDate)
	{
		if (m_bUseCreationTimeForDefStartDate)
			tdiDefault.dateStart = TODOITEM::dtUseCreationDateAndTime;
		else
			tdiDefault.dateStart = TODOITEM::dtUseCreationDateOnly;
	}

	if (m_bUseCreationDateForDefDueDate)
		tdiDefault.dateDue = TODOITEM::dtUseCreationDateOnly;
	
	Misc::Split(m_sDefCategory, tdiDefault.aCategories);
	Misc::Split(m_sDefAllocTo, tdiDefault.aAllocTo);
	Misc::Split(m_sDefTags, tdiDefault.aTags);

	tdiDefault.cfComments = m_cfDefault;
	tdiDefault.sComments = m_sDefTextComments;
	tdiDefault.customComments = m_defCustomComments;
}

void CPreferencesTaskDefPage::OnSetdefaulticon() 
{
	CTDLTaskIconDlg dialog(m_ilTaskIcons, m_sDefIcon);

	if (dialog.DoModal() == IDOK)
	{
		m_sDefIcon = dialog.GetIconName();
		
		int nIcon = m_ilTaskIcons.GetImageIndex(m_sDefIcon);
		m_btDefIcon.SetIcon((nIcon == -1) ? NULL : m_ilTaskIcons.ExtractIcon(nIcon));
	}
}

void CPreferencesTaskDefPage::OnSelchangeReminder() 
{
	UpdateData();

	GetDlgItem(IDC_DEFREMINDERDATE)->EnableWindow(m_nDefReminderLeadinMins != TDLRPC_NOREMINDER);
	GetDlgItem(IDC_PLAYSOUND)->EnableWindow(m_nDefReminderLeadinMins != TDLRPC_NOREMINDER);
}

void CPreferencesTaskDefPage::OnSelchangeCommentsformat() 
{
	m_ctrlComments.GetSelectedFormat(m_cfDefault);
	m_ctrlComments.GetContent(m_sDefTextComments, m_defCustomComments);
	
	CPreferencesPageBase::OnControlChange();
}

LRESULT CPreferencesTaskDefPage::OnCommentsChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_defCustomComments.Empty();
	m_sDefTextComments.Empty();

	m_ctrlComments.GetContent(m_sDefTextComments, m_defCustomComments);

	if (m_sDefTextComments.IsEmpty())
		m_defCustomComments.Empty();

	return 0L;
}

LRESULT CPreferencesTaskDefPage::OnInitComments(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UpdateCommentsSize();

	m_ctrlComments.SetSelectedFormat(m_cfDefault);
	m_ctrlComments.SetContent(m_sDefTextComments, m_defCustomComments, TRUE);

	CUIThemeFile theme;
	theme.crToolbarDark = theme.crToolbarLight = RGB(255, 255, 255);
	theme.crAppBackDark = theme.crAppBackLight = RGB(255, 255, 255);
	
	m_ctrlComments.SetUITheme(theme);

	return 0L;
}


void CPreferencesTaskDefPage::OnSize(UINT nType, int cx, int cy)
{
	CPreferencesPageBase::OnSize(nType, cx, cy);

	UpdateCommentsSize();
}

void CPreferencesTaskDefPage::UpdateCommentsSize()
{
	if (m_ctrlComments.GetSafeHwnd())
	{
		CRect rClient;
		GetClientRect(rClient);

		CRect rCtrl = GetCtrlRect(this, IDC_COMMENTSCTRLFRAME);
		rCtrl.right = (rClient.Width() - 10);
		rCtrl.bottom = (rClient.Height() - 10);

		m_ctrlComments.MoveWindow(rCtrl);
	}
}

void CPreferencesTaskDefPage::OnUseCreationDateForDefStartDate() 
{
	UpdateData();

	GetDlgItem(IDC_USECREATIONTIMEFORDEFSTARTDATE)->EnableWindow(m_bUseCreationDateForDefStartDate);	
}
