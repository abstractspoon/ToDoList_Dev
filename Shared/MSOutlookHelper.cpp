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
	int nObjectType;
	BOOL bConfidential;
};	

//////////////////////////////////////////////////////////////////////

OUTLOOKDATAITEM OUTLOOKDATA[] = 
{
	// Common
	{ OA_BILLINGINFORMATION,	OOC_ITEM, FALSE },
	{ OA_BODY,					OOC_ITEM, TRUE },
	{ OA_CATEGORIES,			OOC_ITEM, FALSE },
	{ OA_COMPANIES,				OOC_ITEM, FALSE },
	{ OA_CREATIONTIME,			OOC_ITEM, FALSE },
	{ OA_ENTRYID,				OOC_ITEM, FALSE },
	{ OA_EXPIRYTIME,			OOC_ITEM, FALSE },
	{ OA_FLAGREQUEST,			OOC_ITEM, FALSE },
	{ OA_IMPORTANCE,			OOC_ITEM, FALSE },
	{ OA_LASTMODIFICATIONTIME,	OOC_ITEM, FALSE },
	{ OA_MESSAGECLASS,			OOC_ITEM, FALSE },
	{ OA_MILEAGE,				OOC_ITEM, FALSE },
	{ OA_PERMISSION,			OOC_ITEM, FALSE },
	{ OA_SENSITIVITY,			OOC_ITEM, FALSE },
	{ OA_TITLE,					OOC_ITEM, FALSE },

	// Mail specific
	{ OA_BCC,					OOC_MAIL, TRUE },
	{ OA_CC,					OOC_MAIL, TRUE },
	{ OA_RECEIVEDBYNAME,		OOC_MAIL, TRUE },
	{ OA_RECEIVEDTIME,			OOC_MAIL, FALSE },
	{ OA_REPLYRECIPIENTNAMES,	OOC_MAIL, TRUE },
	{ OA_SENDEREMAILADDRESS,	OOC_MAIL, TRUE },
	{ OA_SENDERNAME,			OOC_MAIL, TRUE },
	{ OA_SENTON,				OOC_MAIL, FALSE },
	{ OA_SENTONBEHALFOFNAME,	OOC_MAIL, TRUE },
	{ OA_TASKCOMPLETEDDATE,		OOC_MAIL, FALSE },
	{ OA_TASKDUEDATE,			OOC_MAIL, FALSE },
	{ OA_TASKSTARTDATE,			OOC_MAIL, FALSE },
	{ OA_TO,					OOC_MAIL, TRUE },
	{ OA_REMINDERTIME,			OOC_MAIL, FALSE },

	// Task specific
	{ OA_ACTUALWORK,			OOC_TASK, FALSE },
	{ OA_COMPLETE,				OOC_TASK, FALSE },
	{ OA_DATECOMPLETED,			OOC_TASK, FALSE },
	{ OA_DELEGATOR,				OOC_TASK, FALSE },
	{ OA_DUEDATE,				OOC_TASK, FALSE },
	{ OA_ISRECURRING,			OOC_TASK, FALSE },
	{ OA_OWNER,					OOC_TASK, FALSE },
	{ OA_PERCENTCOMPLETE,		OOC_TASK, FALSE },
	{ OA_SCHEDULEPLUSPRIORITY,	OOC_TASK, FALSE },
	{ OA_STARTDATE,				OOC_TASK, FALSE },
	{ OA_STATUS,				OOC_TASK, FALSE },
	{ OA_TEAMTASK,				OOC_TASK, FALSE },
	{ OA_TOTALWORK,				OOC_TASK, FALSE },

	// Appointment specific
	{ OA_DURATION,				OOC_APPOINTMENT, FALSE },
	{ OA_END,					OOC_APPOINTMENT, FALSE },
	{ OA_START,					OOC_APPOINTMENT, FALSE },

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

CString CMSOutlookHelper::FormatItemAsUrl(OutlookAPI::_Item& obj, DWORD dwFlags)
{
	CString sPath(GetItemID(obj)); // default

	if (dwFlags & OAFMT_NICE)
	{
		// get the parent path
		LPDISPATCH lpParent = obj.GetParent();
		
		if (lpParent)
		{
			CString sFolder = MAPIFolder(lpParent).GetFolderPath();
			CString sSubject = GetItemData(obj, OA_TITLE);

			if (EscapeText(sSubject))
				sPath.Format(_T("%s\\~%s"), sFolder, sSubject);
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

BOOL CMSOutlookHelper::EscapeText(CString& sText)
{
	if (!CanEscapeText(sText))
		return FALSE;

	// Other 'dangerous' characters can be replaced by 
	// their HEX equivalent
	const CString sSearch(_T("#:<>-\"Яавйнопрстухщэ"));
	CString sReplace;

	for (int nChar = 0; nChar < sSearch.GetLength(); nChar++)
	{
		TCHAR cSearch[2] = { sSearch[nChar], 0 };

		sReplace.Format(_T("%%%02X"), sSearch[nChar]);
		sText.Replace(cSearch, sReplace);
	}

	return TRUE;
}

BOOL CMSOutlookHelper::CanEscapeText(const CString& sText)
{
	// Outlook will not accept the following characters in the 
	// URL even if they are encoded
	return (sText.FindOneOf(_T("%!/")) == -1);
}

CString CMSOutlookHelper::GetItemData(OutlookAPI::_Item& obj, OUTLOOK_FIELDTYPE nField)
{
	if (s_bDenyConfidential && IsConfidential(nField))
		return _T("");

	CString sData;

	switch (nField)
	{
		// Common
		case OA_BODY:					return obj.GetBody();
		case OA_BILLINGINFORMATION:		return obj.GetBillingInformation();
		case OA_CATEGORIES:				return obj.GetCategories();
		case OA_COMPANIES:				return obj.GetCompanies();
		//case OA_CONVERSATIONTOPIC:		return obj.GetConversationTopic();
		case OA_CREATIONTIME:			return MapDate(obj.GetCreationTime());
		case OA_ENTRYID:				return obj.GetEntryID();
		case OA_IMPORTANCE:				return Misc::Format(obj.GetImportance());
		case OA_LASTMODIFICATIONTIME:	return MapDate(obj.GetLastModificationTime());
		case OA_MESSAGECLASS:			return obj.GetMessageClass();
		case OA_MILEAGE:				return obj.GetMileage();
		case OA_SENSITIVITY:			return Misc::Format(obj.GetSensitivity());
		case OA_TITLE:					return obj.GetSubject();

		// Mail specific
		case OA_BCC:					return _MailItem(obj).GetBcc();
		case OA_CC:						return _MailItem(obj).GetCc();
		case OA_EXPIRYTIME:				return MapDate(_MailItem(obj).GetExpiryTime());
		case OA_FLAGREQUEST:			return _MailItem(obj).GetFlagRequest();
		case OA_PERMISSION:				return Misc::Format(_MailItem(obj).GetPermission());
		case OA_REMINDERTIME:			return MapDate(_MailItem(obj).GetReminderTime());
		case OA_RECEIVEDBYNAME:			return _MailItem(obj).GetReceivedByName();
		case OA_RECEIVEDTIME:			return MapDate(_MailItem(obj).GetReceivedTime());
		case OA_REPLYRECIPIENTNAMES:	return _MailItem(obj).GetReplyRecipientNames();
		case OA_SENDEREMAILADDRESS:		return _MailItem(obj).GetSenderEmailAddress();
		case OA_SENDERNAME:				return _MailItem(obj).GetSenderName();
		case OA_SENTONBEHALFOFNAME:		return _MailItem(obj).GetSentOnBehalfOfName();
		case OA_SENTON:					return MapDate(_MailItem(obj).GetSentOn());
		case OA_TASKCOMPLETEDDATE:		return MapDate(_MailItem(obj).GetTaskCompletedDate());
		case OA_TASKDUEDATE:			return MapDate(_MailItem(obj).GetTaskDueDate());
		case OA_TASKSTARTDATE:			return MapDate(_MailItem(obj).GetTaskStartDate());
		case OA_TO:						return _MailItem(obj).GetTo();

		// task specific
		case OA_ACTUALWORK:				return Misc::Format(_TaskItem(obj).GetActualWork());
		case OA_COMPLETE:				return Misc::Format(_TaskItem(obj).GetComplete());
		case OA_DATECOMPLETED:			return MapDate(_TaskItem(obj).GetDateCompleted());
		case OA_DELEGATOR:				return _TaskItem(obj).GetDelegator();
		case OA_DUEDATE:				return MapDate(_TaskItem(obj).GetDueDate());
		case OA_ISRECURRING:			return Misc::Format(_TaskItem(obj).GetIsRecurring());
		case OA_OWNER:					return _TaskItem(obj).GetOwner();
		case OA_PERCENTCOMPLETE:		return Misc::Format(_TaskItem(obj).GetPercentComplete());
		case OA_SCHEDULEPLUSPRIORITY:	return _TaskItem(obj).GetSchedulePlusPriority();
		case OA_STARTDATE:				return MapDate(_TaskItem(obj).GetStartDate());
		case OA_STATUS:					return Misc::Format(_TaskItem(obj).GetStatus());
		case OA_TEAMTASK:				return Misc::Format(_TaskItem(obj).GetTeamTask());
		case OA_TOTALWORK:				return Misc::Format(_TaskItem(obj).GetTotalWork());

		// Appointment specific
		case OA_DURATION:				return Misc::Format(_AppointmentItem(obj).GetDuration());
		case OA_END:					return MapDate(_AppointmentItem(obj).GetEnd());
		case OA_START:					return MapDate(_AppointmentItem(obj).GetStart());
			// TODO

		// Journal specific
			// TODO

		// Note specific
			// TODO

		// Contact Specific
			// TODO
	}

	ASSERT(0);
	return _T("");
}

CString CMSOutlookHelper::GetItemID(OutlookAPI::_Item& obj)
{
	return GetItemData(obj, OA_ENTRYID);
}

CString CMSOutlookHelper::GetItemClass(OutlookAPI::_Item& obj)
{
	return GetItemData(obj, OA_MESSAGECLASS);
}

// static
int CMSOutlookHelper::GetItemData(OutlookAPI::_Item& obj, CMSOutlookItemDataMap& mapData, BOOL bIncludeConfidential)
{
	mapData.RemoveAll();

	// temporarily override confidentiality only if bIncludeConfidential is FALSE
	BOOL bDenyConfidential = s_bDenyConfidential;

	if (!s_bDenyConfidential)
		s_bDenyConfidential = (bIncludeConfidential == FALSE);

	if (obj.m_lpDispatch != NULL)
	{
		int nObjType = obj.GetClass();

		for (int nData = 0; nData < NUMOUTLOOKDATA; nData++)
		{
			const OUTLOOKDATAITEM& item = OUTLOOKDATA[nData];

			if ((item.nObjectType == OOC_ITEM) || (item.nObjectType == nObjType))
			{
				try
				{
					mapData[item.nField] = GetItemData(obj, item.nField);
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

OutlookAPI::_Item* CMSOutlookHelper::GetFirstFileObject(const CStringArray& aFiles)
{
	for (int nFile = 0; nFile < aFiles.GetSize(); nFile++)
	{
		OutlookAPI::_Item* pItem = GetFileObject(aFiles[nFile]);
		
		if (pItem)
			return pItem;
	}

	return NULL; // no outlook object found
}

_Item* CMSOutlookHelper::GetFileObject(LPCTSTR szFilePath)
{
	if (!IsOutlookObject(szFilePath) || s_pOutlook == NULL)
		return NULL;

	_NameSpace nmspc(s_pOutlook->GetNamespace(_T("MAPI")));
	_Item* pItem = new _Item(nmspc.OpenSharedItem(szFilePath));

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

_Item* CMSOutlookHelper::GetFirstSelectedObject()
{
	Selection* pSelection = GetSelection();
	_Item* pItem = GetFirstObject(pSelection);

	// cleanup
	delete pSelection;

	return pItem;
}

OutlookAPI::_Item* CMSOutlookHelper::GetFirstObject(OutlookAPI::Selection* pSelection)
{
	if (pSelection == NULL)
		return NULL;

	_Item* pItem = (pSelection->GetCount() ? new _Item(pSelection->Item(COleVariant((short)1))) : NULL);
	
	return pItem;
}

// static
BOOL CMSOutlookHelper::IsConfidential(OUTLOOK_FIELDTYPE nField)
{
	for (int i = 0; i < NUMOUTLOOKDATA; i++)
	{
		if (OUTLOOKDATA[i].nField == nField)
			return OUTLOOKDATA[i].bConfidential;
	}

	// fallback
	return TRUE;
}

CString CMSOutlookHelper::GetFullPath(OutlookAPI::_Item& obj)
{
	CString sPath(obj.GetSubject()), sFolder;
	LPDISPATCH lpd = obj.GetParent();

	const LPCTSTR PATHDELIM = _T(" \\ ");

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

BOOL CMSOutlookHelper::PathsMatch(OutlookAPI::_Item& obj1, OutlookAPI::_Item& obj2)
{
	CString sPath1 = GetFullPath(obj1);
	CString sPath2 = GetFullPath(obj2);
	
	return (sPath1 == sPath2);
}
