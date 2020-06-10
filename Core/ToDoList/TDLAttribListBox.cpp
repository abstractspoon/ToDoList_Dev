// TDAttributeListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLAttribListBox.h"

#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_INITLISTBOX (WM_APP+1)

/////////////////////////////////////////////////////////////////////////////
// CTDLAttributeListBox

CTDLAttributeListBox::CTDLAttributeListBox(const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_ALLOCBY,			TDCA_ALLOCBY,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_ALLOCTO,			TDCA_ALLOCTO,		TRUE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_CATEGORY,		TDCA_CATEGORY,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_COMMENTS,		TDCA_COMMENTS,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_COST,			TDCA_COST,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_CREATEDATE,		TDCA_CREATIONDATE,	FALSE));
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_CREATEDBY,		TDCA_CREATEDBY,		FALSE));
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_DEPENDS,			TDCA_DEPENDENCY,	FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_DONEDATE,		TDCA_DONEDATE,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_DUEDATE,			TDCA_DUEDATE,		TRUE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_EXTERNALID,		TDCA_EXTERNALID,	FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_FILELINK,		TDCA_FILELINK,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_FLAG,			TDCA_FLAG,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_LASTMODDATE,		TDCA_LASTMODDATE,	FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_LASTMODBY,		TDCA_LASTMODBY,		FALSE)); 
//	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_LOCK,			TDCA_LOCK,			FALSE)); 
//	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_PATH,			TDCA_PATH,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_PERCENT,			TDCA_PERCENT,		TRUE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_POS,				TDCA_POSITION,		TRUE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_PRIORITY,		TDCA_PRIORITY,		TRUE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_RECURRENCE,		TDCA_RECURRENCE,	FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_RISK,			TDCA_RISK,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_STARTDATE,		TDCA_STARTDATE,		FALSE));
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_STATUS,			TDCA_STATUS,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_SUBTASKDONE,		TDCA_SUBTASKDONE,	FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_TAGS,			TDCA_TAGS,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_TASKPARENTIDS,	TDCA_ID,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_TIMEEST,			TDCA_TIMEEST,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_TIMESPENT,		TDCA_TIMESPENT,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_VERSION,			TDCA_VERSION,		FALSE)); 

	// Custom attributes
	int nIndex = aAttribDefs.GetSize();
	ATTRIBVIS vis;

	while (nIndex--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nIndex];

		if (attribDef.bEnabled)
		{
			vis.sName.Format(IDS_CUSTOMCOLUMN, attribDef.sLabel);
			vis.nTDCAttrib = attribDef.GetAttributeID();
			vis.sCustAttribID = attribDef.sUniqueID;
			vis.bVisible = TRUE;

			m_aAttribs.Add(vis);
		}
	}
}

CTDLAttributeListBox::~CTDLAttributeListBox()
{
}

BEGIN_MESSAGE_MAP(CTDLAttributeListBox, CCheckListBoxEx)
	//{{AFX_MSG_MAP(CTDLAttributeListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CLBN_CHKCHANGE, OnReflectCheckChange)
	ON_MESSAGE(WM_INITLISTBOX, OnInitListBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLAttributeListBox message handlers

void CTDLAttributeListBox::PreSubclassWindow() 
{
	CCheckListBoxEx::PreSubclassWindow();

	// too early to initialize the list box so we post a message
	PostMessage(WM_INITLISTBOX);
}

int CTDLAttributeListBox::FindAttribute(TDC_ATTRIBUTE nAttrib) const
{
	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		int nLBAttrib = m_aAttribs[nIndex].nTDCAttrib;

		if (nLBAttrib == nAttrib)
			return nIndex;

		// parent ID piggy-backs on task ID
		if ((nLBAttrib == TDCA_ID) && (nAttrib == TDCA_PARENTID))
			return nIndex;
	}

	return -1;
}

BOOL CTDLAttributeListBox::OnReflectCheckChange()
{
	// update all check states because we don't know which one changed
	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nIndex);
		int nFind = FindAttribute(nAttribID);

		if (nFind != -1)
			m_aAttribs[nFind].bVisible = GetCheck(nIndex);
	}

	// continue routing
	return FALSE;
}

LRESULT CTDLAttributeListBox::OnInitListBox(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT (GetCount() == 0);

	ModifyStyle(0, CBS_SORT);

	for (int nIndex = 0; nIndex < (int)m_aAttribs.GetSize(); nIndex++)
	{
		const ATTRIBVIS& cs = m_aAttribs[nIndex];

		int nPos = AddString(cs.sName); 
		SetCheck(nPos, cs.bVisible ? 1 : 0);
		SetItemData(nPos, (DWORD)cs.nTDCAttrib);
	}

	CDialogHelper::RefreshMaxColumnWidth(*this);

	return 0L;
}

void CTDLAttributeListBox::SetSelectedAttributes(const CTDCAttributeMap& mapAttrib, const CStringSet& mapCustomAttribIDs)
{
	int nAttrib = m_aAttribs.GetSize();
	
	while (nAttrib--)
	{
		ATTRIBVIS& vis = m_aAttribs[nAttrib];

		if (vis.sCustAttribID.IsEmpty())
		{
			vis.bVisible = mapAttrib.Has(vis.nTDCAttrib);
		}
		else
		{
			ASSERT (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(vis.nTDCAttrib));

			vis.bVisible = mapCustomAttribIDs.Has(vis.sCustAttribID);
		}
		
		if (GetSafeHwnd())
			SetCheck(nAttrib, vis.bVisible);
	}
}

int CTDLAttributeListBox::GetSelectedAttributes(CTDCAttributeMap& mapAttrib, CStringSet& mapCustomAttribIDs) const
{
	mapAttrib.RemoveAll();
	mapCustomAttribIDs.RemoveAll();

	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		const ATTRIBVIS& vis = m_aAttribs[nIndex];

		if (vis.bVisible)
		{
			if (vis.sCustAttribID.IsEmpty())
			{
				mapAttrib.Add(vis.nTDCAttrib);

				// parent ID
				if (vis.nTDCAttrib == TDCA_ID)
					mapAttrib.Add(TDCA_PARENTID);
			}
			else
			{
				ASSERT (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(vis.nTDCAttrib));

				mapCustomAttribIDs.Add(vis.sCustAttribID);
			}
		}
	}

	return (mapAttrib.GetCount() + mapCustomAttribIDs.GetCount());
}

int CTDLAttributeListBox::GetSelectedAttributes(CTDCAttributeMap& mapAttrib) const
{
	mapAttrib.RemoveAll();

	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		const ATTRIBVIS& vis = m_aAttribs[nIndex];

		if (vis.bVisible)
		{
			mapAttrib.Add(vis.nTDCAttrib);

			// parent ID
			if (vis.nTDCAttrib == TDCA_ID)
				mapAttrib.Add(TDCA_PARENTID);
		}
	}

	return mapAttrib.GetCount();
}
