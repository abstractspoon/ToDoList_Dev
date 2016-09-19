// OutlookHelper.cpp: implementation of the COutlookHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSOutlookHelper.h"
#include "enstring.h"
#include "filemisc.h"
#include "misc.h"
#include "fileregister.h"
#include "regkey.h"

#include "..\3rdparty\msoutl.h"

#include <afxole.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OutlookAPI;

/////////////////////////////////////////////////////////////////////////////

CMSOutlookItemDataMap::CMSOutlookItemDataMap()
{

}

CMSOutlookItemDataMap::CMSOutlookItemDataMap(const CMSOutlookItemDataMap& map)
{
	Misc::CopyT<OUTLOOK_FIELDTYPE, CString>(map, *this);

	ASSERT(map.GetCount() == GetCount());
}

CString CMSOutlookItemDataMap::GetFieldData(OUTLOOK_FIELDTYPE nField) const
{
	CString sData;
	Lookup(nField, sData);
	return sData;
}

BOOL CMSOutlookItemDataMap::HasFieldData(OUTLOOK_FIELDTYPE nField) const
{
	return !(GetFieldData(nField).IsEmpty());
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OutlookAPI::_Application* CMSOutlookHelper::s_pOutlook = NULL;
int CMSOutlookHelper::s_nRefCount = 0;
BOOL CMSOutlookHelper::s_bDenyConfidential = FALSE;

//////////////////////////////////////////////////////////////////////

#define CFSTR_RENPRIVATEMESSAGES  _T("RenPrivateMessages") 
#define CFSTR_RENPRIVATESRCFOLDER _T("RenPrivateSourceFolder")

//////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);
static const CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);	
static const CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);
static const CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);

//////////////////////////////////////////////////////////////////////

const CLIPFORMAT CMSOutlookHelper::CF_OUTLOOK = CF_RENPRIVATEMESSAGES;
const DATE CMSOutlookHelper::NULL_DATE = 949998.0;

//////////////////////////////////////////////////////////////////////

struct OUTLOOKDATAITEM
{
	OUTLOOK_FIELDTYPE nField;
	BOOL bConfidential;
};	

//////////////////////////////////////////////////////////////////////

OUTLOOKDATAITEM OUTLOOKDATA[] = 
{
	{ OA_BCC, TRUE },
	{ OA_BILLINGINFORMATION, FALSE },
	{ OA_BODY, TRUE },
	{ OA_CATEGORIES, FALSE },
	{ OA_CC, TRUE },
	{ OA_COMPANIES, FALSE },
	{ OA_CONVERSATIONTOPIC, FALSE },
	{ OA_CREATIONTIME, FALSE },
	{ OA_ENTRYID, FALSE },
	{ OA_EXPIRYTIME, FALSE },
	{ OA_FLAGREQUEST, FALSE },
	{ OA_IMPORTANCE, FALSE },
	{ OA_LASTMODIFICATIONTIME, FALSE },
	{ OA_MESSAGECLASS, FALSE },
	{ OA_MILEAGE, FALSE },
	{ OA_PERMISSION, FALSE },
	{ OA_RECEIVEDBYNAME, TRUE },
	{ OA_RECEIVEDTIME, FALSE },
	{ OA_REMINDERTIME, FALSE },
	{ OA_REPLYRECIPIENTNAMES, TRUE },
	{ OA_SENDEREMAILADDRESS, TRUE },
	{ OA_SENDERNAME, TRUE },
	{ OA_SENSITIVITY, FALSE },
	{ OA_SENTON, FALSE },
	{ OA_SENTONBEHALFOFNAME, TRUE },
	{ OA_SUBJECT, FALSE },
	{ OA_TASKCOMPLETEDDATE, FALSE },
	{ OA_TASKDUEDATE, FALSE },
	{ OA_TASKSTARTDATE, FALSE },
//	{ OA_TASKSUBJECT, FALSE },
	{ OA_TO, TRUE },
};
const int NUMOUTLOOKDATA = sizeof(OUTLOOKDATA) / sizeof(OUTLOOKDATAITEM);

//////////////////////////////////////////////////////////////////////

CMSOutlookHelper::CMSOutlookHelper()
{
	if (s_pOutlook == NULL && IsOutlookInstalled())
	{
		ASSERT(s_nRefCount == 0);

		s_pOutlook = new _Application;

		if (!s_pOutlook->CreateDispatch(_T("Outlook.Application")))
		{
			delete s_pOutlook;
			s_pOutlook = NULL;
		}
		else
		{
			FileMisc::LogTextRaw(_T("COutlookHelper::COutlookHelper(Outlook started)"));
		}
	}

	s_nRefCount++;
}

CMSOutlookHelper::~CMSOutlookHelper()
{
	s_nRefCount--;

	if (s_nRefCount == 0)
	{
		ASSERT(s_pOutlook);

		delete s_pOutlook;
		s_pOutlook = NULL;

		FileMisc::LogTextRaw(_T("COutlookHelper::~COutlookHelper(Outlook closed)"));
	}
}

// static
BOOL CMSOutlookHelper::IsOutlookInstalled()
{
	return CFileRegister::IsRegisteredApp(_T("msg"), _T("OUTLOOK.EXE"), TRUE);
}

/*
BOOL COutlookHelper::IsUrlHandlerInstalled()
{
	if (IsOutlookInstalled())
	{
		CRegKey2 reg;
		CString sEntry;

		if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell\\open\\command"), TRUE) != ERROR_SUCCESS)
			return FALSE;

		return (reg.Read(_T(""), sEntry) == ERROR_SUCCESS && !sEntry.IsEmpty());
	}

	// all else
	return FALSE;
}

BOOL COutlookHelper::InstallUrlHandler()
{
	if (!IsOutlookInstalled())
		return FALSE;

	if (IsUrlHandlerInstalled())
		return TRUE;

	// cache whether the key already existed so we don't delete it on failure
	BOOL bKeyExists = CRegKey2::KeyExists(HKEY_CLASSES_ROOT, _T("outlook"));

	CRegKey2 reg;
	
	if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook"), FALSE) != ERROR_SUCCESS)
		return FALSE;

	BOOL bSuccess = (reg.Write(_T("URL Protocol"), _T("")) == ERROR_SUCCESS &&
					 reg.Write(_T(""), _T("URL:Outlook Folders")) == ERROR_SUCCESS);

	if (bSuccess)
	{
		reg.Close();

		if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell"), FALSE) == ERROR_SUCCESS)
		{
			bSuccess = (reg.Write(_T(""), _T("open")) == ERROR_SUCCESS);

			if (bSuccess)
			{
				reg.Close();

				if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell\\open\\command"), FALSE) == ERROR_SUCCESS)
				{
					CString sOutlookPath = CFileRegister::GetRegisteredAppPath(_T("msg")); // full path
					CString sCommand = sOutlookPath + _T(" /select \"%1\"");

					bSuccess = (reg.Write(_T(""), sCommand) == ERROR_SUCCESS);
				}
			}
		}
	}

	//[HKEY_CLASSES_ROOT\outlook\DefaultIcon]
	//@="C:\\PROGRA~1\\MICROS~3\\OFFICE12\\OUTLLIB.DLL,-9403"
	
	// delete key on failure
	reg.Close();

	if (!bSuccess && !bKeyExists)
		CRegKey2::DeleteKey(HKEY_CLASSES_ROOT, _T("outlook"));

	return bSuccess;
}
*/

BOOL CMSOutlookHelper::HandleUrl(HWND hWnd, LPCTSTR szURL)
{
	if (!IsOutlookUrl(szURL) || !IsOutlookInstalled())
		return FALSE;

	CString sOutlookPath = CFileRegister::GetRegisteredAppPath(_T("msg")); // full path

	CString sArguments;
	sArguments.Format(_T(" /recycle /select \"%s\""), szURL);

	FileMisc::LogText(_T("CMSOutlookHelper::HandleUrl(\"%s\", %s)"), sOutlookPath, sArguments);

	return (FileMisc::Run(hWnd, sOutlookPath, sArguments) > 32);
}

/*
BOOL COutlookHelper::QueryInstallUrlHandler(UINT nIDQuery, UINT nMBOptions, int nMBSuccess)
{
	if (!IsOutlookInstalled())
		return FALSE;

	if (IsUrlHandlerInstalled())
		return TRUE;

	if (nIDQuery)
	{
		if (AfxMessageBox(CEnString(nIDQuery), nMBOptions) != nMBSuccess)
			return FALSE;
	}

	return InstallUrlHandler();
}
*/

CString CMSOutlookHelper::FormatItemAsUrl(OutlookAPI::_MailItem& obj, DWORD dwFlags)
{
	CString sPath(GetItemID(obj)); // default

	if (dwFlags & OAFMT_NICE)
	{
		// get the parent path
		LPDISPATCH lpParent = obj.GetParent();
		
		if (lpParent)
		{
			CString sFolder = MAPIFolder(lpParent).GetFolderPath();
			CString sSubject = GetItemData(obj, OA_SUBJECT);

			// very odd bug if subject starts with two chars 
			// and then a colon -> Outlook fails to open the link
			// so we handle this specific issue by falling back on the itemID
			if ((sSubject.GetLength() <= 2) || (sSubject[2] != ':'))
			{
				sPath.Format(_T("%s\\~%s"), sFolder, sSubject);
			}
			// else use itemID from above
		}
	}

	CString sUrl;
	sUrl.Format(_T("outlook:%s"), sPath);

	if ((dwFlags & OAFMT_BRACED) && (sUrl.Find(' ', 0) > 0))
	{
		(sUrl = '<' + sUrl + '>');
	}
	else if (dwFlags & OAFMT_REPLACESPACES)
	{
		sUrl.Replace(_T(" "), _T("%20"));
	}
	sUrl.Replace('\\', '/');

	return sUrl;
}

CString CMSOutlookHelper::GetItemData(OutlookAPI::_MailItem& obj, OUTLOOK_FIELDTYPE nField)
{
	if (IsConfidential(nField) && s_bDenyConfidential)
		return _T("");

	CString sData;

	switch (nField)
	{
		case OA_BCC:					return obj.GetBcc();
		case OA_BILLINGINFORMATION:		return obj.GetBillingInformation();
		case OA_BODY:					return obj.GetBody();
		case OA_CATEGORIES:				return obj.GetCategories();
		case OA_CC:						return obj.GetCc();
		case OA_COMPANIES:				return obj.GetCompanies();
		case OA_CONVERSATIONTOPIC:		return obj.GetConversationTopic();
		case OA_CREATIONTIME:			return MapDate(obj.GetCreationTime());
		case OA_ENTRYID:				return obj.GetEntryID();
		case OA_EXPIRYTIME:				return MapDate(obj.GetExpiryTime());
		case OA_FLAGREQUEST:			return obj.GetFlagRequest();
		case OA_IMPORTANCE:				return Misc::Format(obj.GetImportance());
		case OA_LASTMODIFICATIONTIME:	return MapDate(obj.GetLastModificationTime());
		case OA_MESSAGECLASS:			return obj.GetMessageClass();
		case OA_MILEAGE:				return obj.GetMileage();
		case OA_PERMISSION:				return Misc::Format(obj.GetPermission());
		case OA_RECEIVEDBYNAME:			return obj.GetReceivedByName();
		case OA_RECEIVEDTIME:			return MapDate(obj.GetReceivedTime());
		case OA_REMINDERTIME:			return MapDate(obj.GetReminderTime());
		case OA_REPLYRECIPIENTNAMES:	return obj.GetReplyRecipientNames();
		case OA_SENDEREMAILADDRESS:		return obj.GetSenderEmailAddress();
		case OA_SENDERNAME:				return obj.GetSenderName();
		case OA_SENSITIVITY:			return Misc::Format(obj.GetSensitivity());
		case OA_SENTONBEHALFOFNAME:		return obj.GetSentOnBehalfOfName();
		case OA_SENTON:					return MapDate(obj.GetSentOn());
		case OA_SUBJECT:				return obj.GetSubject();
		case OA_TASKCOMPLETEDDATE:		return MapDate(obj.GetTaskCompletedDate());
		case OA_TASKDUEDATE:			return MapDate(obj.GetTaskDueDate());
		case OA_TASKSTARTDATE:			return MapDate(obj.GetTaskStartDate());
//		case OA_TASKSUBJECT:			return obj.GetTaskSubject();
		case OA_TO:						return obj.GetTo();
	}

	ASSERT(0);
	return _T("");
}

CString CMSOutlookHelper::GetItemID(OutlookAPI::_MailItem& obj)
{
	return GetItemData(obj, OA_ENTRYID);
}

CString CMSOutlookHelper::GetItemClass(OutlookAPI::_MailItem& obj)
{
	return GetItemData(obj, OA_MESSAGECLASS);
}

// static
int CMSOutlookHelper::GetItemData(OutlookAPI::_MailItem& obj, CMSOutlookItemDataMap& mapData, BOOL bIncludeConfidential)
{
	mapData.RemoveAll();

	// temporarily override confidentiality only if bIncludeConfidential is FALSE
	BOOL bDenyConfidential = s_bDenyConfidential;

	if (!s_bDenyConfidential)
		s_bDenyConfidential = (bIncludeConfidential == FALSE);

	if (obj.m_lpDispatch != NULL)
	{
		for (int nField = OA_FIRST; nField < OA_LAST; nField++)
		{
			try
			{
				mapData[(OUTLOOK_FIELDTYPE)nField] = GetItemData(obj, (OUTLOOK_FIELDTYPE)nField);
			}
			catch (COleException* e)
			{
				e->Delete();
				s_bDenyConfidential = TRUE;
			}
			catch (...)
			{
				ASSERT(0);
				s_bDenyConfidential = TRUE;
			}
		}
	}

	// restore confidentiality
	s_bDenyConfidential |= bDenyConfidential;

	return mapData.GetCount();
}

// static
CString CMSOutlookHelper::MapDate(DATE date)
{
	const DATE NODATE = 949998;

	if (date != NODATE)
		return COleDateTime(date).Format();

	// else
	return _T("");
}

// static
BOOL CMSOutlookHelper::IsOutlookObject(LPDATAOBJECT lpObject)
{
	if (lpObject)
	{
		COleDataObject dataobj;
		dataobj.Attach(lpObject, FALSE);

		return IsOutlookObject(&dataobj);
	}

	// else
	return FALSE;
}

// static
BOOL CMSOutlookHelper::IsOutlookObject(COleDataObject* pObject)
{
	return (pObject &&
			pObject->IsDataAvailable(CF_RENPRIVATEMESSAGES) && 
			pObject->IsDataAvailable(CF_RENPRIVATESRCFOLDER) &&
			pObject->IsDataAvailable(CF_FILECONTENTS) && 
			pObject->IsDataAvailable(CF_FILEDESCRIPTOR));
}

// static
BOOL CMSOutlookHelper::IsOutlookObject(LPCTSTR szFilePath)
{
	return FileMisc::HasExtension(szFilePath, _T(".msg"));
}

// static
BOOL CMSOutlookHelper::IsOutlookUrl(LPCTSTR szURL)
{
	return (CString(szURL).Find(_T("outlook:")) == 0);
}

OutlookAPI::_MailItem* CMSOutlookHelper::GetFirstFileObject(const CStringArray& aFiles)
{
	for (int nFile = 0; nFile < aFiles.GetSize(); nFile++)
	{
		OutlookAPI::_MailItem* pItem = GetFileObject(aFiles[nFile]);
		
		if (pItem)
			return pItem;
	}

	return NULL; // no outlook object found
}

_MailItem* CMSOutlookHelper::GetFileObject(LPCTSTR szFilePath)
{
	if (!IsOutlookObject(szFilePath) || s_pOutlook == NULL)
		return NULL;

	_NameSpace nmspc(s_pOutlook->GetNamespace(_T("MAPI")));
	_MailItem* pItem = new _MailItem(nmspc.OpenSharedItem(szFilePath));

	return pItem;
}

Selection* CMSOutlookHelper::GetSelection()
{
	if (s_pOutlook == NULL)
		return NULL;
	
	_Explorer explorer(s_pOutlook->ActiveExplorer());
	Selection* pSelection = new Selection(explorer.GetSelection());

	return pSelection;
}

int CMSOutlookHelper::GetSelectionCount()
{
	if (s_pOutlook == NULL)
		return NULL;
	
	_Explorer explorer(s_pOutlook->ActiveExplorer());
	Selection selection(explorer.GetSelection());

	return selection.GetCount();
}

_MailItem* CMSOutlookHelper::GetFirstSelectedObject()
{
	Selection* pSelection = GetSelection();
	_MailItem* pItem = GetFirstObject(pSelection);

	// cleanup
	delete pSelection;

	return pItem;
}

OutlookAPI::_MailItem* CMSOutlookHelper::GetFirstObject(OutlookAPI::Selection* pSelection)
{
	if (pSelection == NULL)
		return NULL;

	_MailItem* pItem = (pSelection->GetCount() ? new _MailItem(pSelection->Item(COleVariant((short)1))) : NULL);
	
	return pItem;
}

// static
BOOL CMSOutlookHelper::IsConfidential(OUTLOOK_FIELDTYPE nField)
{
	// sanity check
	ASSERT(NUMOUTLOOKDATA == OA_LAST);

	for (int i = 0; i < NUMOUTLOOKDATA; i++)
	{
		if (OUTLOOKDATA[i].nField == nField)
			return OUTLOOKDATA[i].bConfidential;
	}

	// fallback
	return TRUE;
}
