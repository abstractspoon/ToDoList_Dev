// OutlookImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TasklistOutlookImporter.h"
#include "TDLImportOutlookDlg.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\msoutlookhelper.h"
#include "..\shared\enstring.h"
#include "..\shared\holdredraw.h"

#include "..\Interfaces\IPreferences.h"
#include "..\Interfaces\ITaskList.h"

#include "..\3rdparty\msoutl.h"

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

using namespace OutlookAPI;

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR PATHDELIM = _T(" \\ ");

const int OOC_FLAGGEDMAIL = -1;

static int ICONS[][2] = 
{
	{ OOC_FOLDER,		IDI_FOLDER }, 
	{ OOC_MAIL,			IDI_MAIL }, 
	{ OOC_FLAGGEDMAIL,	IDI_FLAGGEDMAIL }, 
	{ OOC_NOTE,			IDI_NOTE }, 	
	{ OOC_TASK,			IDI_TASK }, 	
	{ OOC_JOURNAL,		IDI_JOURNAL }, 
	{ OOC_CONTACT,		IDI_CONTACT }, 
	{ OOC_APPOINTMENT,	IDI_APPOINTMENT }
};
int NUM_ICONS = (sizeof(ICONS) / (2 * sizeof(UINT)));

/////////////////////////////////////////////////////////////////////////////

class _Item : public COleDispatchDriver
{
public:
	_Item(LPDISPATCH pItem) : COleDispatchDriver(pItem, FALSE) {}

	long GetClass()
	{
		long result;
		InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	CString GetSubject()
	{
		CString result;
		InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	LPDISPATCH GetParent()
	{
		LPDISPATCH result;
		InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	CString GetBody()
	{
		CString result;
		InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	CString GetCategories()
	{
		CString result;
		InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	
	long GetImportance()
	{
		long result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	
	CString GetEntryID()
	{
		CString result;
		InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	
	DATE GetCreationTime()
	{
		DATE result;
		InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
		return result;
	}
	
	DATE GetLastModificationTime()
	{
		DATE result;
		InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
		return result;
	}

	CString GetFullPath()
	{
		CString sPath(GetSubject()), sFolder;
		LPDISPATCH lpd = GetParent();
		
		do
		{
			try
			{
				MAPIFolder folder(lpd);
				sFolder = folder.GetName(); // will throw when we hit the highest level
				sPath = sFolder + PATHDELIM + sPath;
				
				lpd = folder.GetParent(); 
			}
			catch (...)
			{
				break;
			}
		}
		while (true);
		
		return sPath;
	}

	static BOOL PathsMatch(LPDISPATCH lpd1, LPDISPATCH lpd2)
	{
		CString sPath1 = _Item(lpd1).GetFullPath();
		CString sPath2 = _Item(lpd2).GetFullPath();
		
		return (sPath1 == sPath2);
	}

};

/////////////////////////////////////////////////////////////////////////////
// COutlookImportDlg dialog

COutlookImportDlg::COutlookImportDlg(CWnd* pParent /*=NULL*/)
: CDialog(IDD_IMPORT_OUTLOOK_DIALOG, pParent), m_pDestTaskFile(NULL), m_pOutlook(NULL)
{
	//{{AFX_DATA_INIT(COutlookImportDlg)
	m_sCurFolder = _T("");
	//}}AFX_DATA_INIT
}

void COutlookImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutlookImportDlg)
	DDX_Check(pDX, IDC_REMOVEOUTLOOKTASKS, m_bRemoveOutlookTasks);
	DDX_Text(pDX, IDC_CURFOLDER, m_sCurFolder);
	DDX_Control(pDX, IDC_TASKLIST, m_tcTasks);
	DDX_Check(pDX, IDC_HIDEUNFLAGGEDEMAIL, m_bHideUnflaggedEmails);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutlookImportDlg, CDialog)
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

IIMPORT_RESULT COutlookImportDlg::ImportTasks(ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_pDestTaskFile = GetITLInterface<ITaskList10>(pDestTaskFile, IID_TASKLIST10);

	if (!m_pDestTaskFile)
	{
		ASSERT(0);
		return IIR_BADINTERFACE;
	}

	CString sKey(szKey);
	sKey += _T("\\Outlook");

	m_bHideUnflaggedEmails = pPrefs->GetProfileInt(sKey, _T("HideUnflaggedEmails"), TRUE);
	m_bRemoveOutlookTasks = pPrefs->GetProfileInt(sKey, _T("RemoveOutlookTasks"), FALSE);

	if (DoModal() != IDOK)
		return IIR_CANCELLED;

	// else
	pPrefs->WriteProfileInt(sKey, _T("HideUnflaggedEmails"), m_bHideUnflaggedEmails);
	pPrefs->WriteProfileInt(sKey, _T("RemoveOutlookTasks"), m_bRemoveOutlookTasks);

	return IIR_SUCCESS;
}

BOOL COutlookImportDlg::OnInitDialog() 
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COutlookImportDlg::BuildItemImageList()
{
	if (m_ilItemTypes.GetSafeHandle())
		return TRUE;

	if (!m_ilItemTypes.Create(16, 16, ILC_COLOR24 | ILC_MASK, NUM_ICONS, 1))
		return FALSE;

	for (int nIcon = 0; nIcon < NUM_ICONS; nIcon++)
	{
		HICON hIcon = AfxGetApp()->LoadIcon(ICONS[nIcon][1]);

		if (!hIcon)
			return FALSE;

		m_mapItemTypes[ICONS[nIcon][0]] = m_ilItemTypes.Add(hIcon);
		::DestroyIcon(hIcon);
	}

	ASSERT(m_ilItemTypes.GetImageCount() == NUM_ICONS);

	m_tcTasks.SetImageList(&m_ilItemTypes, TVSIL_NORMAL);

	return TRUE;
}

int COutlookImportDlg::GetItemImage(int nClass) const
{
	int nImage = -1; // not supported

	m_mapItemTypes.Lookup(nClass, nImage);

	return nImage;
}

void COutlookImportDlg::AddFolderItemsToTree(MAPIFolder* pFolder, HTREEITEM htiParent)
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
					_MailItem mail(lpd);
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

void COutlookImportDlg::OnOK()
{
	CDialog::OnOK();

	ASSERT(m_pOutlook && m_pDestTaskFile);

	// make sure nothing has changed
	_NameSpace nmspc(m_pOutlook->GetNamespace(_T("MAPI")));
	MAPIFolder mapi(nmspc.GetDefaultFolder(OFC_TASKS));

	AddTreeItemsToTasks(NULL, NULL, &mapi);
}

void COutlookImportDlg::AddTreeItemsToTasks(HTREEITEM htiParent, HTASKITEM hTaskParent, MAPIFolder* pFolder)
{
	ASSERT ((htiParent && hTaskParent) || (!htiParent && !hTaskParent));

	// iterate the tree items adding checked items
	HTREEITEM hti = m_tcTasks.GetChildItem(htiParent);

	while (hti)
	{
		if (m_tcTasks.GetCheck(hti))
		{
			// create a new task
			CString sTitle = m_tcTasks.GetItemText(hti);

			HTASKITEM hTask = m_pDestTaskFile->NewTask(sTitle, hTaskParent, 0);
			ASSERT(hTask);

			// get the Outlook item that this item points to
			LPDISPATCH lpd = (LPDISPATCH)m_tcTasks.GetItemData(hti);
				
			// add attributes for leaf tasks
			if (!m_tcTasks.ItemHasChildren(hti))
			{
				SetTaskAttributes(hTask, lpd);
				
				// delete the item as we go if required
				if (m_bRemoveOutlookTasks)
					DeleteItemFromFolder(lpd, pFolder);
			}
			else // process RECURSIVELY
			{
				AddTreeItemsToTasks(hti, hTask, pFolder);
			}
			
			lpd->Release();
		}

		hti = m_tcTasks.GetNextItem(hti, TVGN_NEXT);
	}
}

BOOL COutlookImportDlg::DeleteItemFromFolder(LPDISPATCH pItem, MAPIFolder* pFolder)
{
	// look through this folders tasks first
	_Items items(pFolder->GetItems());
	int nItem, nTaskCount = items.GetCount();

	for (nItem = 1; nItem <= nTaskCount; nItem++)
	{
		LPDISPATCH lpd = items.Item(COleVariant((short)nItem));

		if (_Item::PathsMatch(pItem, lpd))
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

		if (DeleteItemFromFolder(pItem, &folder))
			return TRUE;
	}

	return FALSE;
}

int COutlookImportDlg::SetCommonTaskAttributes(HTASKITEM hTask, LPDISPATCH lpd)
{
	_Item item(lpd);

	// set it's attributes
	m_pDestTaskFile->SetTaskComments(hTask, item.GetBody());
	
	// can have multiple categories
	CStringArray aCats;
	Misc::Split(item.GetCategories(), aCats);
	
	for (int nCat = 0; nCat < aCats.GetSize(); nCat++)
		m_pDestTaskFile->AddTaskCategory(hTask, aCats[nCat]);
	
	m_pDestTaskFile->SetTaskCreationDate(hTask, ConvertDate(item.GetCreationTime()));
	m_pDestTaskFile->SetTaskLastModified(hTask, ConvertDate(item.GetLastModificationTime()));
	m_pDestTaskFile->SetTaskPriority(hTask, (unsigned char)(item.GetImportance() * 5));
	
	// save outlook ID unless removing from Outlook
	if (!m_bRemoveOutlookTasks)
	{
		CString sFileLink, sEntryID(item.GetEntryID());
		sFileLink.Format(_T("outlook:%s"), sEntryID);
		m_pDestTaskFile->SetTaskFileLinkPath(hTask, sFileLink);
		
		// and save entry ID as meta-data so we
		// maintain the association for synchronization
		m_pDestTaskFile->SetTaskMetaData(hTask, OUTLOOK_METADATAKEY, sEntryID);
	}

	return item.GetClass();
}

void COutlookImportDlg::SetTaskAttributes(HTASKITEM hTask, LPDISPATCH lpd)
{
	lpd->AddRef(); // to keep it alive

	// common attributes
	int nClass = SetCommonTaskAttributes(hTask, lpd);

	// class specific attributes
	switch (nClass)
	{
	case OOC_MAIL:		
		{
			_MailItem mail(lpd);
			
			// TODO
		}
		break;
		
	case OOC_NOTE:		
		{
			_NoteItem note(lpd);
			
			// TODO
		}
		break;
		
	case OOC_TASK:		
		{
			_TaskItem task(lpd);

			if (task.GetComplete())
				m_pDestTaskFile->SetTaskDoneDate(hTask, ConvertDate(task.GetDateCompleted()));
			
			m_pDestTaskFile->SetTaskDueDate(hTask, ConvertDate(task.GetDueDate()));
			m_pDestTaskFile->SetTaskStartDate(hTask, ConvertDate(task.GetStartDate()));
			m_pDestTaskFile->SetTaskAllocatedBy(hTask, task.GetDelegator());
			m_pDestTaskFile->SetTaskAllocatedTo(hTask, task.GetOwner());
		}
		break;

	case OOC_JOURNAL:	
		{
			_JournalItem journal(lpd);
			
			// TODO
		}
		break;
		
	case OOC_CONTACT:	
		{
			_ContactItem contact(lpd);
			
			// TODO
		}
		break;
		
	case OOC_APPOINTMENT:
		{
			_AppointmentItem appointment(lpd);
			
			// TODO
		}
		break;
		

	case OOC_FOLDER:	
	default:
		ASSERT(0);
		break;
	}
}

void COutlookImportDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete m_pOutlook;
	delete m_pFolder;

	m_pOutlook = NULL;
	m_pDestTaskFile = NULL;
	m_pFolder = NULL;
}

time_t COutlookImportDlg::ConvertDate(DATE date)
{
	if (date <= 0.0)
		return 0;

	SYSTEMTIME st;
	COleDateTime dt(date);

	dt.GetAsSystemTime(st);

	tm t = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, 0 };
	return mktime(&t);
}

void COutlookImportDlg::OnChoosefolder() 
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

void COutlookImportDlg::OnClickTasklist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
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

void COutlookImportDlg::SetChildItemsChecked(HTREEITEM hti, BOOL bChecked)
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

void COutlookImportDlg::OnHideUnflaggedEmails() 
{
	UpdateData();

	OnRefresh();
}

void COutlookImportDlg::OnRefresh() 
{
	m_wndPrompt.SetPrompt(m_tcTasks, IDS_OUTLOOK_BUILDINGLIST, TVM_GETCOUNT);

	// re-add items
	CWaitCursor wait;
	m_tcTasks.DeleteAllItems();
	
	CHoldRedraw hr(m_tcTasks);
	AddFolderItemsToTree(m_pFolder);

	m_wndPrompt.SetPrompt(m_tcTasks, IDS_OUTLOOK_FOLDERNOITEMS, TVM_GETCOUNT);
}
