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

CTDLAttributeListBox::CTDLAttributeListBox()
{
	// same order as enum
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
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_FILEREF,			TDCA_FILEREF,		FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_FLAG,			TDCA_FLAG,			FALSE)); 
	m_aAttribs.Add(ATTRIBVIS(IDS_TDLBC_MODIFYDATE,		TDCA_LASTMOD,		FALSE)); 
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

	// custom attributes are always visible
	if (nAttrib != TDCA_CUSTOMATTRIB)
		ASSERT(0);

	return -1;
}

void CTDLAttributeListBox::SetAllAttributesVisible(BOOL bVisible)
{
	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		m_aAttribs[nIndex].bVisible = bVisible;

		if (GetSafeHwnd())
			SetCheck(nIndex, bVisible ? 1 : 0);
	}
}

int CTDLAttributeListBox::GetAllAttributes(CTDCAttributeMap& mapAttrib) const
{
	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
		mapAttrib.AddAttribute(m_aAttribs[nIndex].nTDCAttrib);

	mapAttrib.AddAttribute(TDCA_CUSTOMATTRIB); // always

	return mapAttrib.GetCount();
}

BOOL CTDLAttributeListBox::OnReflectCheckChange()
{
	// update all check states because we don't know which one changed
	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		TDC_ATTRIBUTE col = (TDC_ATTRIBUTE)GetItemData(nIndex);
		int nAttrib = FindAttribute(col);

		if (nAttrib != -1)
			m_aAttribs[nAttrib].bVisible = GetCheck(nIndex);
	}

	// continue routing
	return FALSE;
}

LRESULT CTDLAttributeListBox::OnInitListBox(WPARAM /*wp*/, LPARAM /*lp*/)
{
	ASSERT (GetCount() == 0);

	for (int nIndex = 0; nIndex < (int)m_aAttribs.GetSize(); nIndex++)
	{
		const ATTRIBVIS& cs = m_aAttribs[nIndex];

		int nPos = AddString(cs.sName); // same order as enum
		SetCheck(nPos, cs.bVisible ? 1 : 0);
		SetItemData(nPos, (DWORD)cs.nTDCAttrib);
	}

	CDialogHelper::RefreshMaxColumnWidth(*this);

	return 0L;
}

void CTDLAttributeListBox::SetVisibleAttributes(const CTDCAttributeMap& mapAttrib)
{
	int nAttrib = m_aAttribs.GetSize();
	
	while (nAttrib--)
	{
		ATTRIBVIS& vis = m_aAttribs[nAttrib];

		vis.bVisible = mapAttrib.HasAttribute(vis.nTDCAttrib);

			if (GetSafeHwnd())
				SetCheck(nAttrib, vis.bVisible);
	}
}

int CTDLAttributeListBox::GetVisibleAttributes(CTDCAttributeMap& mapAttrib) const
{
	mapAttrib.RemoveAll();

	int nIndex = m_aAttribs.GetSize();
	
	while (nIndex--)
	{
		ATTRIBVIS vis = m_aAttribs[nIndex];

		if (vis.bVisible)
		{
			mapAttrib.AddAttribute(vis.nTDCAttrib);

			// parent ID
			if (vis.nTDCAttrib == TDCA_ID)
				mapAttrib.AddAttribute(TDCA_PARENTID);
		}
	}

	// custom attributes
	mapAttrib.AddAttribute(TDCA_CUSTOMATTRIB); // always

	return mapAttrib.GetCount();

}
