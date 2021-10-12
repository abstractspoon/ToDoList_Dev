// OutlookImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TasklistOutlookImporter.h"
#include "TDLImportOutlookDlg.h"
#include "TDLImportOutlookObjectsDlg.h"
#include "TDCOutlookImportHelper.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\enstring.h"
#include "..\shared\holdredraw.h"
#include "..\shared\themed.h"
#include "..\shared\icon.h"
#include "..\shared\GraphicsMisc.h"

#include "..\Interfaces\IPreferences.h"
#include "..\Interfaces\ITaskList.h"

#include "..\3rdparty\msoutl.h"
#include "..\3rdparty\XNamedColors.h"

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

using namespace OutlookAPI;

/////////////////////////////////////////////////////////////////////////////

const int OOC_FLAGGEDMAIL = -1;

static int ICONS[] = 
{
	OOC_FOLDER, 
	OOC_MAIL, 
	OOC_FLAGGEDMAIL, 
	OOC_NOTE, 	
	OOC_TASK, 	
	OOC_JOURNAL, 
	OOC_CONTACT, 
	OOC_APPOINTMENT
};
int NUM_ICONS = (sizeof(ICONS) / sizeof(ICONS[0]));

/////////////////////////////////////////////////////////////////////////////
// COutlookImportDlg dialog

CTDLImportOutlookDlg::CTDLImportOutlookDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_IMPORT_OUTLOOK_DIALOG, pParent), 
	m_pDestTaskFile(NULL), 
	m_pOutlook(NULL),
	m_pFolder(NULL)
{
	//{{AFX_DATA_INIT(COutlookImportDlg)
	m_sCurFolder = _T("");
	//}}AFX_DATA_INIT
}

void CTDLImportOutlookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutlookImportDlg)
	DDX_Check(pDX, IDC_REMOVEOUTLOOKTASKS, m_bRemoveOutlookTasks);
	DDX_Text(pDX, IDC_CURFOLDER, m_sCurFolder);
	DDX_Control(pDX, IDC_TASKLIST, m_tcTasks);
	DDX_Check(pDX, IDC_HIDEUNFLAGGEDEMAIL, m_bHideUnflaggedEmails);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLImportOutlookDlg, CDialog)
	//{{AFX_MSG_MAP(COutlookImportDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHOOSEFOLDER, OnChoosefolder)
	ON_NOTIFY(NM_CLICK, IDC_TASKLIST, OnClickTasklist)
	ON_BN_CLICKED(IDC_HIDEUNFLAGGEDEMAIL, OnHideUnflaggedEmails)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutlookImportDlg message handlers

IIMPORTEXPORT_RESULT CTDLImportOutlookDlg::ImportTasks(ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_pDestTaskFile = GetITLInterface<ITASKLISTBASE>(pDestTaskFile, IID_TASKLISTBASE);

	if (m_pDestTaskFile == NULL)
	{
		ASSERT(0);
		return IIER_BADINTERFACE;
	}

	CString sKey(szKey);
	sKey += _T("\\Outlook");

	m_bHideUnflaggedEmails = pPrefs->GetProfileInt(sKey, _T("HideUnflaggedEmails"), TRUE);
	m_bRemoveOutlookTasks = pPrefs->GetProfileInt(sKey, _T("RemoveOutlookTasks"), FALSE);

	if (DoModal() != IDOK)
		return IIER_CANCELLED;

	// else
	pPrefs->WriteProfileInt(sKey, _T("HideUnflaggedEmails"), m_bHideUnflaggedEmails);
	pPrefs->WriteProfileInt(sKey, _T("RemoveOutlookTasks"), m_bRemoveOutlookTasks);

	return IIER_SUCCESS;
}

BOOL CTDLImportOutlookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CLocalizer::EnableTranslation(m_tcTasks, FALSE);
	
	ASSERT(m_pOutlook == NULL);
	m_pOutlook = new _Application;

	if (m_pOutlook->CreateDispatch(_T("Outlook.Application")))
	{
		VERIFY(BuildItemImageList());

		_NameSpace nmspc(m_pOutlook->GetNamespace(_T("MAPI")));
		nmspc.m_lpDispatch->AddRef(); // to keep it alive

		m_pFolder = new MAPIFolder(nmspc.GetDefaultFolder(OFC_TASKS));
		m_pFolder->m_lpDispatch->AddRef(); // to keep it alive

		AddFolderItemsToTree(m_pFolder);
		
		m_sCurFolder = m_pFolder->GetName();
		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox(CEnString(IDS_OUTLOOK_UNABLETOCONNECT), MB_OK | MB_ICONERROR);
		EndDialog(IDCANCEL);
	}

	CThemed::SetWindowTheme(&m_tcTasks, _T("Explorer"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTDLImportOutlookDlg::BuildItemImageList()
{
	if (m_ilItemTypes.GetSafeHandle())
		return TRUE;

	if (!m_ilItemTypes.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))
		return FALSE;

	CBitmap bmp;
	bmp.LoadBitmap(IDB_OUTLOOKTYPES);

	m_ilItemTypes.Add(&bmp, colorMagenta);

	for (int nIcon = 0; nIcon < NUM_ICONS; nIcon++)
		m_mapItemTypes[ICONS[nIcon]] = nIcon;

	ASSERT(m_ilItemTypes.GetImageCount() == NUM_ICONS);

	m_ilItemTypes.ScaleByDPIFactor();
	m_tcTasks.SetImageList(&m_ilItemTypes, TVSIL_NORMAL);

	return TRUE;
}

int CTDLImportOutlookDlg::GetItemImage(int nClass) const
{
	int nImage = -1; // not supported

	m_mapItemTypes.Lookup(nClass, nImage);

	return nImage;
}

void CTDLImportOutlookDlg::AddFolderItemsToTree(MAPIFolder* pFolder, HTREEITEM htiParent)
{
	_Items items(pFolder->GetItems());
	items.m_lpDispatch->AddRef(); // to keep it alive

	int nItem, nCount = items.GetCount();

	for (nItem = 1; nItem <= nCount; nItem++)
	{
		try
		{
			LPDISPATCH lpd = items.Item(COleVariant((short)nItem));
			lpd->AddRef(); // to keep it alive

			_Item item(lpd);

			CString sItem;
			int nClass = item.GetClass();

			switch (nClass)
			{
			case OOC_FOLDER:	
				ASSERT(0);
				break;

			case OOC_MAIL:		
				{
					_MailItem mail(item);
					BOOL bFlagged = (mail.GetIsMarkedAsTask() && (mail.GetTaskCompletedDate() == CMSOutlookHelper::NULL_DATE));
			
					if (!m_bHideUnflaggedEmails || bFlagged)
					{
						sItem = mail.GetSubject();

						if (bFlagged)
							nClass = OOC_FLAGGEDMAIL;
					}
				}
				break;
				
			case OOC_NOTE:		
			case OOC_TASK:		
			case OOC_JOURNAL:	
			case OOC_CONTACT:	
			case OOC_APPOINTMENT:
				sItem = item.GetSubject();
				break;
			}

			if (!sItem.IsEmpty())
			{
				int nImage = GetItemImage(nClass);
				HTREEITEM hti = m_tcTasks.InsertItem(sItem, nImage, nImage, htiParent);
				
				//TRACE(_T("%s / %s (item), type = %ld\n"), m_tcTasks.GetItemText(htiParent), m_tcTasks.GetItemText(hti), nClass);
				
				m_tcTasks.SetItemData(hti, (DWORD)lpd);
				m_tcTasks.SetCheck(hti, TRUE);
			}
			else
			{
				lpd->Release();
			}
		}
		catch (...)
		{
			continue;
		}
	}

	// likewise for subfolders
	_Items folders(pFolder->GetFolders());
	nCount = folders.GetCount();

	for (nItem = 1; nItem <= nCount; nItem++)
	{
		try
		{
			LPDISPATCH lpd = folders.Item(COleVariant((short)nItem));
			lpd->AddRef(); // to keep it alive

			MAPIFolder folder(lpd);
			ASSERT(folder.GetClass() == OOC_FOLDER);

			int nImage = GetItemImage(OOC_FOLDER);
			ASSERT(nImage != -1);

			HTREEITEM hti = m_tcTasks.InsertItem(folder.GetName(), nImage, nImage, htiParent);

			//TRACE(_T("%s / %s (folder), type = %ld\n"), m_tcTasks.GetItemText(htiParent), m_tcTasks.GetItemText(hti), folder.GetClass());
			
			m_tcTasks.SetItemData(hti, (DWORD)lpd);
			m_tcTasks.SetCheck(hti, TRUE);

			AddFolderItemsToTree(&folder, hti); // RECURSIVE call

			// if the folder had no items added then delete ie
			if (!m_tcTasks.ItemHasChildren(hti))
			{
				m_tcTasks.DeleteItem(hti);
				lpd->Release();
			}
			else // expand
			{
				m_tcTasks.Expand(hti, TVE_EXPAND);
			}
		}
		catch (...)
		{
			continue;
		}
	}
}

void CTDLImportOutlookDlg::OnOK()
{
	CDialog::OnOK();

	ASSERT(m_pOutlook && m_pDestTaskFile);

	// make sure nothing has changed
	_NameSpace nmspc(m_pOutlook->GetNamespace(_T("MAPI")));
	MAPIFolder mapi(nmspc.GetDefaultFolder(OFC_TASKS));

	AddTreeItemsToTasks(NULL, NULL, &mapi);
}

void CTDLImportOutlookDlg::AddTreeItemsToTasks(HTREEITEM htiParent, HTASKITEM hTaskParent, MAPIFolder* pFolder)
{
	ASSERT ((htiParent && hTaskParent) || (!htiParent && !hTaskParent));

	// iterate the tree items adding checked items
	CTDCAttributeMapping aMapping;
	BOOL bWantConfidential = FALSE;

	HTREEITEM hti = m_tcTasks.GetChildItem(htiParent);

	while (hti)
	{
		if (m_tcTasks.GetCheck(hti))
		{
			// Get the field mapping once only for the first item
			LPDISPATCH lpd = (LPDISPATCH)m_tcTasks.GetItemData(hti);
			_Item obj(lpd);

			if (aMapping.GetSize() == 0)
			{
				CTDLImportOutlookObjectsDlg dialog(obj, CEnString(IDS_OUTLOOK_IMPORTDLG));

				if (dialog.DoModal() != IDOK)
				{
					EndDialog(IDCANCEL);
					return;
				}

				// else
				dialog.GetColumnMapping(aMapping);
				bWantConfidential = dialog.GetWantConfidentialAttributes();
			}
				
			// create a new task
			BOOL bLeafTask = !m_tcTasks.ItemHasChildren(hti);

			if (bLeafTask)
			{
				if (CTDCOutlookImportHelper::ImportTask(aMapping, &obj, bWantConfidential, m_pDestTaskFile, bLeafTask))
				{
					// delete the item as we go if required
					if (m_bRemoveOutlookTasks)
						DeleteItemFromFolder(_Item(lpd), pFolder);

				}
			}
			else // folder -> new parent
			{
				HTASKITEM hNewParent = m_pDestTaskFile->NewTask(m_tcTasks.GetItemText(hti), hTaskParent, 0);
				ASSERT(hNewParent);

				AddTreeItemsToTasks(hti, hNewParent, pFolder);
			}
		}

		hti = m_tcTasks.GetNextItem(hti, TVGN_NEXT);
	}
}

BOOL CTDLImportOutlookDlg::DeleteItemFromFolder(_Item& obj, MAPIFolder* pFolder)
{
	// look through this folders tasks first
	_Items items(pFolder->GetItems());
	int nItem, nTaskCount = items.GetCount();

	for (nItem = 1; nItem <= nTaskCount; nItem++)
	{
		LPDISPATCH lpd = items.Item(COleVariant((short)nItem));

		if (CMSOutlookHelper::PathsMatch(obj, _Item(lpd)))
		{
			items.Remove(nItem);
			return TRUE;
		}
	}

	// then for subfolders
	_Items folders(pFolder->GetFolders());
	int nCount = folders.GetCount();

	for (nItem = 1; nItem <= nCount; nItem++)
	{
		MAPIFolder folder(folders.Item(COleVariant((short)nItem)));

		if (DeleteItemFromFolder(obj, &folder))
			return TRUE;
	}

	return FALSE;
}

void CTDLImportOutlookDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete m_pOutlook;
	delete m_pFolder;

	m_pOutlook = NULL;
	m_pDestTaskFile = NULL;
	m_pFolder = NULL;
}

void CTDLImportOutlookDlg::OnChoosefolder() 
{
	_NameSpace nmspc(m_pOutlook->GetNamespace(_T("MAPI")));
	nmspc.m_lpDispatch->AddRef(); // to keep it alive

	LPDISPATCH pDisp = nmspc.PickFolder();

	if (pDisp)
	{
		delete m_pFolder;
		m_pFolder = new MAPIFolder(pDisp);
		
		m_sCurFolder = m_pFolder->GetName();
		UpdateData(FALSE);
		
		OnRefresh();
	}
}

void CTDLImportOutlookDlg::OnClickTasklist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	CPoint ptClick(GetMessagePos());
	m_tcTasks.ScreenToClient(&ptClick);

	UINT nFlags = 0;
	HTREEITEM hti = m_tcTasks.HitTest(ptClick, &nFlags);

	if (hti && (TVHT_ONITEMSTATEICON & nFlags))
	{
		BOOL bChecked = !m_tcTasks.GetCheck(hti); // check hasn't happened yet

		// clear or set all children
		SetChildItemsChecked(hti, bChecked);

		// check parents if necessary
		if (bChecked)
		{
			HTREEITEM htiParent = m_tcTasks.GetParentItem(hti);

			while (htiParent)
			{
				// item
				m_tcTasks.SetCheck(htiParent, bChecked);
					
				// then parent
				htiParent = m_tcTasks.GetParentItem(htiParent);
			}
		}
    }

	*pResult = 0;
}

void CTDLImportOutlookDlg::SetChildItemsChecked(HTREEITEM hti, BOOL bChecked)
{
	HTREEITEM htiChild = m_tcTasks.GetChildItem(hti);

	while (htiChild)
	{
		// item
		m_tcTasks.SetCheck(htiChild, bChecked);
		
		// then children
		SetChildItemsChecked(htiChild, bChecked); // RECURSIVE

		htiChild = m_tcTasks.GetNextItem(htiChild, TVGN_NEXT);
	}
}

void CTDLImportOutlookDlg::OnHideUnflaggedEmails() 
{
	UpdateData();

	OnRefresh();
}

void CTDLImportOutlookDlg::OnRefresh() 
{
	m_wndPrompt.SetPrompt(m_tcTasks, IDS_OUTLOOK_BUILDINGLIST, TVM_GETCOUNT);

	// re-add items
	CWaitCursor wait;
	m_tcTasks.DeleteAllItems();
	
	CHoldRedraw hr(m_tcTasks);
	AddFolderItemsToTree(m_pFolder);

	m_wndPrompt.SetPrompt(m_tcTasks, IDS_OUTLOOK_FOLDERNOITEMS, TVM_GETCOUNT);
}
