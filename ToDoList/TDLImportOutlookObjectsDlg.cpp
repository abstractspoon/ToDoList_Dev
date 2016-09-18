// TDLImportOutlookMsgsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLImportOutlookObjectsDlg.h"

#include "..\shared\preferences.h"
#include "..\shared\MSoutlookhelper.h"
#include "..\shared\misc.h"

#include "..\3rdparty\msoutl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct OUTLOOK_FIELD
{
	OUTLOOK_FIELD(OUTLOOK_FIELDTYPE type, UINT nID, TDC_ATTRIBUTE attrib)
		: nFieldType(type), nIDFieldName(nID), nTDCAttrib(attrib)
	{
	}

	OUTLOOK_FIELDTYPE nFieldType;
	UINT nIDFieldName;
	TDC_ATTRIBUTE nTDCAttrib;
};

static OUTLOOK_FIELD FIELDS[] = 
{
	OUTLOOK_FIELD(OA_BCC, IDS_OA_BCC, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_BILLINGINFORMATION, IDS_OA_BILLINGINFO, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_BODY, IDS_OA_BODY, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CATEGORIES, IDS_OA_CATEGORIES, TDCA_CATEGORY),
	OUTLOOK_FIELD(OA_CC, IDS_OA_CC, TDCA_ALLOCTO),
	OUTLOOK_FIELD(OA_COMPANIES, IDS_OA_COMPANIES, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CONVERSATIONTOPIC, IDS_OA_CONVERSATIONTOPIC, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_CREATIONTIME, IDS_OA_CREATIONTIME, TDCA_CREATIONDATE),
	OUTLOOK_FIELD(OA_ENTRYID, IDS_OA_ENTRYID, TDCA_FILEREF),
	OUTLOOK_FIELD(OA_EXPIRYTIME, IDS_OA_EXPIRYTIME, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_FLAGREQUEST, IDS_OA_FLAGREQUEST, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_IMPORTANCE, IDS_OA_IMPORTANCE, TDCA_PRIORITY),
	OUTLOOK_FIELD(OA_LASTMODIFICATIONTIME, IDS_OA_LASTMODIFICATIONTIME, TDCA_LASTMOD),
	OUTLOOK_FIELD(OA_MILEAGE, IDS_OA_MILEAGE, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_PERMISSION, IDS_OA_PERMISSION, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_RECEIVEDBYNAME, IDS_OA_RECEIVEDBYNAME, TDCA_ALLOCTO),
	OUTLOOK_FIELD(OA_RECEIVEDTIME, IDS_OA_RECEIVEDTIME, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_REMINDERTIME, IDS_OA_REMINDERTIME, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_REPLYRECIPIENTNAMES, IDS_OA_REPLYRECIPIENTNAMES, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENDEREMAILADDRESS, IDS_OA_SENDEREMAILADDRESS, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENDERNAME, IDS_OA_SENDERNAME, TDCA_ALLOCBY),
	OUTLOOK_FIELD(OA_SENSITIVITY, IDS_OA_SENSITIVITY, TDCA_RISK),
	OUTLOOK_FIELD(OA_SENTON, IDS_OA_SENTON, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SENTONBEHALFOFNAME, IDS_OA_SENTONBEHALFOFNAME, TDCA_COMMENTS),
	OUTLOOK_FIELD(OA_SUBJECT, IDS_OA_SUBJECT, TDCA_TASKNAME),
	OUTLOOK_FIELD(OA_TASKCOMPLETEDDATE, IDS_OA_TASKCOMPLETEDDATE, TDCA_DONEDATE),
	OUTLOOK_FIELD(OA_TASKDUEDATE, IDS_OA_TASKDUEDATE, TDCA_DUEDATE),
	OUTLOOK_FIELD(OA_TASKSTARTDATE, IDS_OA_TASKSTARTDATE, TDCA_STARTDATE),
//	OUTLOOK_FIELD(OA_TASKSUBJECT, IDS_OA_TASKSUBJECT, TDCA_TASKNAME),
	OUTLOOK_FIELD(OA_TO, IDS_OA_TO, TDCA_ALLOCTO)
};

const UINT NUM_FIELDS = sizeof(FIELDS) / sizeof(OUTLOOK_FIELD);

/////////////////////////////////////////////////////////////////////////////
// CTDLImportOutlookObjectsDlg dialog

CEnString CTDLImportOutlookObjectsDlg::CONFIDENTIAL;

CTDLImportOutlookObjectsDlg::CTDLImportOutlookObjectsDlg(OutlookAPI::_MailItem& refItem, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_OUTLOOKMSGIMPORT_DIALOG, pParent), 
	m_refItem(refItem), 
	m_bHideUnmapped(FALSE), 
	m_bHideConfidential(TRUE)
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

	// modify first column header
	m_lcFieldMapping.SetColumnText(0, CEnString(IDS_OUTLOOK_FIELDNAME));

	// force first column to take up any slack
	m_lcFieldMapping.SetFirstColumnStretchy(TRUE);
	
	// take a copy of the master mapping and remove any unwanted fields before
	// passing to list control
	CTDCCsvColumnMapping aMapping;
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
		for (int nField = 0; nField < NUM_FIELDS; nField++)
		{
			const OUTLOOK_FIELD& oaField = FIELDS[nField];
			CEnString sFieldAndData = FormatFieldAndData(oaField);

			m_aMasterMapping.Add(CSVCOLUMNMAPPING(sFieldAndData, oaField.nTDCAttrib, oaField.nFieldType)); 
		}
	}

	ASSERT(m_aMasterMapping.GetSize() == NUM_FIELDS);

	// restore last state from preferences
	for (int nField = 0; nField < NUM_FIELDS; nField++)
	{
		CString sKey = Misc::MakeKey(_T("Field%d"), nField);
		OUTLOOK_FIELDTYPE nFieldType = (OUTLOOK_FIELDTYPE)prefs.GetProfileInt(sSection, sKey, -1);
		
		sKey = Misc::MakeKey(_T("Attrib%d"), nField);
		TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)prefs.GetProfileInt(sSection, sKey, TDCA_NONE);

		if (nFieldType != -1)
			m_aMasterMapping[nField].nTDCAttrib = nAttrib;
	}
}

void CTDLImportOutlookObjectsDlg::RemoveUnwantedAttributes(CTDCCsvColumnMapping& aMapping) const
{
	if (m_bHideUnmapped || m_bHideConfidential)
	{
		int nField = NUM_FIELDS;

		while (nField--)
		{
			const CSVCOLUMNMAPPING& col = aMapping[nField];

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

	for (int nField = 0; nField < NUM_FIELDS; nField++)
	{
		CString sKey = Misc::MakeKey(_T("Field%d"), nField);
		prefs.WriteProfileInt(sSection, sKey, m_aMasterMapping[nField].dwItemData);
		
		sKey = Misc::MakeKey(_T("Attrib%d"), nField);
		prefs.WriteProfileInt(sSection, sKey, m_aMasterMapping[nField].nTDCAttrib);
	}
}

void CTDLImportOutlookObjectsDlg::UpdateMasterMapping()
{
	CTDCCsvColumnMapping aMapping;
	m_lcFieldMapping.GetColumnMapping(aMapping);

	for (int nField = 0; nField < aMapping.GetSize(); nField++)
	{
		const CSVCOLUMNMAPPING& col = aMapping[nField];

		// find this field in the master mapping
		int nMaster = FindField(m_aMasterMapping, (OUTLOOK_FIELDTYPE)col.dwItemData);
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

int CTDLImportOutlookObjectsDlg::GetColumnMapping(CTDCCsvColumnMapping& aMapping) 
{ 
	// get the latest changes from the list control
	UpdateMasterMapping();

	// take a copy and remove unwanted fields
	aMapping.Copy(m_aMasterMapping);
	RemoveUnwantedAttributes(aMapping);

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
	BOOL bHideConfidential = m_bHideConfidential;
	UpdateData();

	// if the user _now_ wants to see confidential data we must
	// rebuild the text of the master mapping
	if (bHideConfidential && !m_bHideConfidential)
	{
		CMSOutlookHelper::ResetDenyConfidential();
		CMSOutlookHelper::GetItemData(m_refItem, m_mapRefData, TRUE);

		for (int nField = 0; nField < NUM_FIELDS; nField++)
		{
			const OUTLOOK_FIELD& oaField = FIELDS[nField];
			CEnString sFieldAndData = FormatFieldAndData(oaField);

			CSVCOLUMNMAPPING& col = m_aMasterMapping[nField];
			col.sColumnName = sFieldAndData; 
			//col.nTDCAttrib = oaField.nTDCAttrib;
		}
	}

	// then update the list ctrl
	CTDCCsvColumnMapping aMapping;
	aMapping.Copy(m_aMasterMapping);

	RemoveUnwantedAttributes(aMapping);

	m_lcFieldMapping.SetColumnMapping(aMapping);
}

int CTDLImportOutlookObjectsDlg::FindField(const CTDCCsvColumnMapping& aMapping, OUTLOOK_FIELDTYPE nFieldType)
{
	int nField = aMapping.GetSize();

	while (nField--)
	{
		if (aMapping[nField].dwItemData == (DWORD)nFieldType)
			return nField;
	}

	// else
	return -1;
}

TDC_ATTRIBUTE CTDLImportOutlookObjectsDlg::GetFieldMapping(const CTDCCsvColumnMapping& aMapping, OUTLOOK_FIELDTYPE nFieldType)
{
	int nField = FindField(aMapping, nFieldType);

	if (nField != -1)
		return aMapping[nField].nTDCAttrib;

	// else
	return TDCA_NONE;
}

