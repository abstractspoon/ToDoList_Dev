// TDLImportOutlookMsgsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLImportOutlookObjectsDlg.h"

#include "..\shared\MSoutlookhelper.h"
#include "..\shared\misc.h"

#include "..\3rdparty\msoutl.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct OUTLOOK_FIELD
{
	OUTLOOK_FIELD(OUTLOOK_FIELDTYPE field, int obj, UINT nID, TDC_ATTRIBUTE attrib)
		: nFieldType(field), nObjType(obj), nIDFieldName(nID), nTDCAttrib(attrib)
	{
	}

	OUTLOOK_FIELDTYPE nFieldType;
	int nObjType;
	UINT nIDFieldName;
	TDC_ATTRIBUTE nTDCAttrib;
};

static OUTLOOK_FIELD FIELDS[] = 
{
	OUTLOOK_FIELD(OA_BILLINGINFORMATION,	OOC_ITEM,			IDS_OA_BILLINGINFO,			TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_BODY,					OOC_ITEM,			IDS_OA_BODY,				TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CATEGORIES,			OOC_ITEM,			IDS_OA_CATEGORIES,			TDCA_CATEGORY),
	OUTLOOK_FIELD(OA_COMPANIES,				OOC_ITEM,			IDS_OA_COMPANIES,			TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CREATIONTIME,			OOC_ITEM,			IDS_OA_CREATIONTIME,		TDCA_CREATIONDATE),
	OUTLOOK_FIELD(OA_ENTRYID,				OOC_ITEM,			IDS_OA_ENTRYID,				TDCA_FILELINK),
	OUTLOOK_FIELD(OA_IMPORTANCE,			OOC_ITEM,			IDS_OA_IMPORTANCE,			TDCA_PRIORITY),
	OUTLOOK_FIELD(OA_LASTMODIFICATIONTIME,	OOC_ITEM,			IDS_OA_LASTMODTIME,			TDCA_LASTMODDATE),
	OUTLOOK_FIELD(OA_MILEAGE,				OOC_ITEM,			IDS_OA_MILEAGE,				TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENSITIVITY,			OOC_ITEM,			IDS_OA_SENSITIVITY,			TDCA_RISK),
	OUTLOOK_FIELD(OA_TITLE,					OOC_ITEM,			IDS_OA_SUBJECT,				TDCA_TASKNAME),

	OUTLOOK_FIELD(OA_BCC,					OOC_MAIL,			IDS_OA_BCC,					TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CC,					OOC_MAIL,			IDS_OA_CC,					TDCA_ALLOCTO),
	OUTLOOK_FIELD(OA_EXPIRYTIME,			OOC_MAIL,			IDS_OA_EXPIRYTIME,			TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_FLAGREQUEST,			OOC_MAIL,			IDS_OA_FLAGREQUEST,			TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_PERMISSION,			OOC_MAIL,			IDS_OA_PERMISSION,			TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_RECEIVEDBYNAME,		OOC_MAIL,			IDS_OA_RECEIVEDBYNAME,		TDCA_ALLOCTO),
	OUTLOOK_FIELD(OA_RECEIVEDTIME,			OOC_MAIL,			IDS_OA_RECEIVEDTIME,		TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_REMINDERTIME,			OOC_MAIL,			IDS_OA_REMINDERTIME,		TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_REPLYRECIPIENTNAMES,	OOC_MAIL,			IDS_OA_REPLYRECIPIENTNAMES, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENDEREMAILADDRESS,	OOC_MAIL,			IDS_OA_SENDEREMAILADDRESS,	TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENDERNAME,			OOC_MAIL,			IDS_OA_SENDERNAME,			TDCA_ALLOCBY),
	OUTLOOK_FIELD(OA_SENTON,				OOC_MAIL,			IDS_OA_SENTON,				TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENTONBEHALFOFNAME,	OOC_MAIL,			IDS_OA_SENTONBEHALFOFNAME,	TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_TASKCOMPLETEDDATE,		OOC_MAIL,			IDS_OA_TASKCOMPLETEDDATE,	TDCA_DONEDATE),
	OUTLOOK_FIELD(OA_TASKDUEDATE,			OOC_MAIL,			IDS_OA_TASKDUEDATE,			TDCA_DUEDATE),
	OUTLOOK_FIELD(OA_TASKSTARTDATE,			OOC_MAIL,			IDS_OA_TASKSTARTDATE,		TDCA_STARTDATE),
	OUTLOOK_FIELD(OA_TO,					OOC_MAIL,			IDS_OA_TO,					TDCA_ALLOCTO),

 	OUTLOOK_FIELD(OA_ACTUALWORK,			OOC_TASK,			IDS_OA_ACTUALWORK,			TDCA_TIMESPENT),
 	OUTLOOK_FIELD(OA_DATECOMPLETED,			OOC_TASK,			IDS_OA_DATECOMPLETED,		TDCA_DONEDATE),
 	OUTLOOK_FIELD(OA_DELEGATOR,				OOC_TASK,			IDS_OA_DELEGATOR,			TDCA_ALLOCTO),
 	OUTLOOK_FIELD(OA_DUEDATE,				OOC_TASK,			IDS_OA_DUEDATE,				TDCA_DUEDATE),
// 	OUTLOOK_FIELD(OA_ISRECURRING,			OOC_TASK,			IDS_OA_ISRECURRING,			TDCA_RECURRENCE),
 	OUTLOOK_FIELD(OA_OWNER,					OOC_TASK,			IDS_OA_OWNER,				TDCA_CREATEDBY),
 	OUTLOOK_FIELD(OA_PERCENTCOMPLETE,		OOC_TASK,			IDS_OA_PERCENTCOMPLETE,		TDCA_PERCENT),
 	OUTLOOK_FIELD(OA_SCHEDULEPLUSPRIORITY,	OOC_TASK,			IDS_OA_SCHEDULEPLUSPRIORITY, TDCA_PRIORITY),
 	OUTLOOK_FIELD(OA_STARTDATE,				OOC_TASK,			IDS_OA_STARTDATE,			TDCA_STARTDATE),
 	OUTLOOK_FIELD(OA_STATUS,				OOC_TASK,			IDS_OA_STATUS,				TDCA_STATUS),
 	OUTLOOK_FIELD(OA_TOTALWORK,				OOC_TASK,			IDS_OA_TOTALWORK,			TDCA_TIMEEST),

	OUTLOOK_FIELD(OA_DURATION,				OOC_APPOINTMENT,	IDS_OA_DURATION,			TDCA_TIMEEST),
	OUTLOOK_FIELD(OA_END,					OOC_APPOINTMENT,	IDS_OA_ENDDATE,				TDCA_DUEDATE),
	OUTLOOK_FIELD(OA_START,					OOC_APPOINTMENT,	IDS_OA_STARTDATE,			TDCA_STARTDATE),

};

const UINT NUM_FIELDS = sizeof(FIELDS) / sizeof(OUTLOOK_FIELD);

/////////////////////////////////////////////////////////////////////////////
// CTDLImportOutlookObjectsDlg dialog

CEnString CTDLImportOutlookObjectsDlg::CONFIDENTIAL;

CTDLImportOutlookObjectsDlg::CTDLImportOutlookObjectsDlg(OutlookAPI::_Item& refItem, LPCTSTR szAltTitle, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_OUTLOOKMSGIMPORT_DIALOG, _T("ImportOutlook"), pParent), 
	m_refItem(refItem), 
	m_bHideUnmapped(FALSE), 
	m_bHideConfidential(TRUE),
	m_sAltTitle(szAltTitle)
{
	BuildMasterMapping();
}

void CTDLImportOutlookObjectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLImportOutlookObjectsDlg)
	DDX_Check(pDX, IDC_HIDEUNMAPPED, m_bHideUnmapped);
	DDX_Check(pDX, IDC_HIDECONFIDENTIAL, m_bHideConfidential);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FIELDMAPPING, m_lcFieldMapping);
}


BEGIN_MESSAGE_MAP(CTDLImportOutlookObjectsDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLImportOutlookObjectsDlg)
	ON_BN_CLICKED(IDC_HIDEUNMAPPED, OnHideAttributes)
	ON_BN_CLICKED(IDC_HIDECONFIDENTIAL, OnHideAttributes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLImportOutlookObjectsDlg message handlers

BOOL CTDLImportOutlookObjectsDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	if (!m_sAltTitle.IsEmpty())
		SetWindowText(m_sAltTitle);

	// modify first column header
	m_lcFieldMapping.SetColumnText(0, CEnString(IDS_OUTLOOK_FIELDNAME));

	// force first column to take up any slack
	m_lcFieldMapping.SetFirstColumnStretchy(TRUE);
	
	// take a copy of the master mapping and remove any unwanted fields before
	// passing to list control
	CTDCAttributeMapping aMapping;
	aMapping.Copy(m_aMasterMapping);

	RemoveUnwantedAttributes(aMapping);

	m_lcFieldMapping.SetColumnMapping(aMapping);

	// make sure we have the foreground focus because
	// the Outlook security popup can steal it.
	AfxGetMainWnd()->SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLImportOutlookObjectsDlg::OnOK()
{
	CTDLDialog::OnOK();

	UpdateMasterMapping();
	SaveMasterMapping();
}

void CTDLImportOutlookObjectsDlg::BuildMasterMapping()
{
	// restore last state from preferences
	CString sSection;
	sSection.Format(_T("OutlookFieldMapping\\%s"), CMSOutlookHelper::GetItemClass(m_refItem));

	CPreferences prefs;
	m_bHideUnmapped = prefs.GetProfileInt(sSection, _T("HideUnmapped"), TRUE);
	m_bHideConfidential = prefs.GetProfileInt(sSection, _T("HideConfidential"), TRUE);

	// initialize the mapped reference data
	CMSOutlookHelper::GetItemData(m_refItem, m_mapRefData, !m_bHideConfidential);

	// recheck the confidential status because the call to COutlookHelper::GetItemData
	// may have been denied
	if (!m_bHideConfidential)
		m_bHideConfidential = CMSOutlookHelper::HasDenyConfidential();

	// build the master mapping first time only
	if (m_aMasterMapping.GetSize() == 0)
	{
		int nObjType = m_refItem.GetClass();

		for (int nField = 0; nField < NUM_FIELDS; nField++)
		{
			const OUTLOOK_FIELD& oaField = FIELDS[nField];

			if ((oaField.nObjType == OOC_ITEM) || (oaField.nObjType == nObjType))
			{
				CString sFieldAndData = FormatFieldAndData(oaField);
				
				m_aMasterMapping.Add(TDCATTRIBUTEMAPPING(sFieldAndData, oaField.nTDCAttrib, oaField.nFieldType)); 
			}
		}
	}

	// restore last state from preferences
	int nNumMap = m_aMasterMapping.GetSize();

	for (int nMap = 0; nMap < nNumMap; nMap++)
	{
		CString sKey = Misc::MakeKey(_T("Field%d"), nMap);
		OUTLOOK_FIELDTYPE nFieldType = prefs.GetProfileEnum(sSection, sKey, OA_NONE);

		sKey = Misc::MakeKey(_T("Attrib%d"), nMap);
		TDC_ATTRIBUTE nAttrib = prefs.GetProfileEnum(sSection, sKey, TDCA_NONE);

		if (nFieldType != -1)
		{
			// find this field in the master mapping
			int nMaster = m_aMasterMapping.Find((DWORD)nFieldType);

			if (nMaster != -1)
				m_aMasterMapping[nMaster].nTDCAttrib = nAttrib;
		}
	}
}

void CTDLImportOutlookObjectsDlg::RemoveUnwantedAttributes(CTDCAttributeMapping& aMapping) const
{
	if (m_bHideUnmapped || m_bHideConfidential)
	{
		int nField = aMapping.GetSize();

		while (nField--)
		{
			const TDCATTRIBUTEMAPPING& col = aMapping[nField];

			if (m_bHideUnmapped && col.nTDCAttrib == TDCA_NONE)
			{
				aMapping.RemoveAt(nField);
			}
			else if (m_bHideConfidential && CMSOutlookHelper::IsConfidential((OUTLOOK_FIELDTYPE)col.dwItemData))
			{
				aMapping.RemoveAt(nField);
			}
		}
	}
}

void CTDLImportOutlookObjectsDlg::SaveMasterMapping() const
{
	CString sSection;
	sSection.Format(_T("OutlookFieldMapping\\%s"), CMSOutlookHelper::GetItemClass(m_refItem));

	CPreferences prefs;
	prefs.WriteProfileInt(sSection, _T("HideUnmapped"), m_bHideUnmapped);
	prefs.WriteProfileInt(sSection, _T("HideConfidential"), m_bHideConfidential);

	int nField = m_aMasterMapping.GetSize();

	while (nField--)
	{
		CString sKey = Misc::MakeKey(_T("Field%d"), nField);
		prefs.WriteProfileInt(sSection, sKey, m_aMasterMapping[nField].dwItemData);
		
		sKey = Misc::MakeKey(_T("Attrib%d"), nField);
		prefs.WriteProfileInt(sSection, sKey, m_aMasterMapping[nField].nTDCAttrib);
	}
}

void CTDLImportOutlookObjectsDlg::UpdateMasterMapping()
{
	CTDCAttributeMapping aMapping;
	m_lcFieldMapping.GetColumnMapping(aMapping);

	for (int nField = 0; nField < aMapping.GetSize(); nField++)
	{
		const TDCATTRIBUTEMAPPING& col = aMapping[nField];

		// find this field in the master mapping
		int nMaster = m_aMasterMapping.Find(col.dwItemData);
		ASSERT(nMaster != -1);

		if (nMaster != -1)
			m_aMasterMapping[nMaster].nTDCAttrib = col.nTDCAttrib;
	}
}

CString CTDLImportOutlookObjectsDlg::GetOutlookFieldName(OUTLOOK_FIELDTYPE nFieldType)
{
	for (int nField = 0; nField < NUM_FIELDS; nField++)
	{
		const OUTLOOK_FIELD& oaField = FIELDS[nField];
		
		if (oaField.nFieldType == nFieldType)
			return CEnString(oaField.nIDFieldName);
	}

	ASSERT(0);
	return _T("");
}

int CTDLImportOutlookObjectsDlg::GetColumnMapping(CTDCAttributeMapping& aMapping) 
{ 
	// get the latest changes from the list control
	UpdateMasterMapping();

	// take a copy and remove unwanted fields
	aMapping.Copy(m_aMasterMapping);
	RemoveUnwantedAttributes(aMapping);

	// If the email body is mapped to the comments, move it
	// to the bottom of the list so that any other attributes
	// to be added to the comments get processed first
	int nBody = aMapping.Find((DWORD)OA_BODY);

	if ((nBody != -1) && (aMapping[nBody].nTDCAttrib == TDCA_COMMENTS))
	{
		TDCATTRIBUTEMAPPING body = aMapping[nBody]; // copy
		aMapping.RemoveAt(nBody);
		aMapping.Add(body);
	}

	return aMapping.GetSize();
}

CString CTDLImportOutlookObjectsDlg::FormatFieldAndData(const OUTLOOK_FIELD& oaField) const
{
	CEnString sField(oaField.nIDFieldName);
	CEnString sData, sFieldAndData(sField);
	
	if (m_mapRefData.Lookup(oaField.nFieldType, sData) && !sData.IsEmpty())
	{
		sFieldAndData.Format(_T("%s (%s)"), sField, sData);
	}
	else if (CMSOutlookHelper::IsConfidential(oaField.nFieldType) && CMSOutlookHelper::HasDenyConfidential())
	{
		if (CONFIDENTIAL.IsEmpty())
			CONFIDENTIAL.LoadString(IDS_OUTLOOK_CONFIDENTIAL);

		sFieldAndData.Format(_T("%s [%s]"), sField, CONFIDENTIAL);
	}

	return sFieldAndData;
}

void CTDLImportOutlookObjectsDlg::OnHideAttributes() 
{
	UpdateMasterMapping();

	BOOL bHideConfidential = m_bHideConfidential;
	UpdateData();

	// if the user _now_ wants to see confidential data we must
	// rebuild the text of the master mapping
	if (bHideConfidential && !m_bHideConfidential)
	{
		CMSOutlookHelper::ResetDenyConfidential();
		CMSOutlookHelper::GetItemData(m_refItem, m_mapRefData, TRUE);

		int nField = NUM_FIELDS;

		while (nField--)
		{
			const OUTLOOK_FIELD& oaField = FIELDS[nField];
			int nMaster = m_aMasterMapping.Find((DWORD)oaField.nFieldType);

			if (nMaster != -1)
			{
				CEnString sFieldAndData = FormatFieldAndData(oaField);
				m_aMasterMapping[nMaster].sColumnName = sFieldAndData; 
			}
		}
	}

	// then update the list ctrl
	CTDCAttributeMapping aMapping;
	aMapping.Copy(m_aMasterMapping);

	RemoveUnwantedAttributes(aMapping);

	m_lcFieldMapping.SetColumnMapping(aMapping);
}
