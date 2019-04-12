// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msoutl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OutlookAPI;


/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _Application::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Application::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Application::GetVersion()
{
	CString result;
	InvokeHelper(0x116, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ActiveExplorer()
{
	LPDISPATCH result;
	InvokeHelper(0x111, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ActiveInspector()
{
	LPDISPATCH result;
	InvokeHelper(0x112, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::CreateItem(long ItemType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ItemType);
	return result;
}

LPDISPATCH _Application::CreateItemFromTemplate(LPCTSTR TemplatePath, const VARIANT& InFolder)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x10b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		TemplatePath, &InFolder);
	return result;
}

LPDISPATCH _Application::CreateObject(LPCTSTR ObjectName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x115, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ObjectName);
	return result;
}

LPDISPATCH _Application::GetNamespace(LPCTSTR Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x110, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type);
	return result;
}

void _Application::Quit()
{
	InvokeHelper(0x113, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Application::GetCOMAddIns()
{
	LPDISPATCH result;
	InvokeHelper(0x118, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetExplorers()
{
	LPDISPATCH result;
	InvokeHelper(0x119, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetInspectors()
{
	LPDISPATCH result;
	InvokeHelper(0x11a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetLanguageSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x11b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetProductCode()
{
	CString result;
	InvokeHelper(0x11c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::ActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x11f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::CopyFile(LPCTSTR FilePath, LPCTSTR DestFolderPath)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0xfa62, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FilePath, DestFolderPath);
	return result;
}

LPDISPATCH _Application::AdvancedSearch(LPCTSTR Scope, const VARIANT& Filter, const VARIANT& SearchSubFolders, const VARIANT& Tag)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfa65, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Scope, &Filter, &SearchSubFolders, &Tag);
	return result;
}

BOOL _Application::IsSearchSynchronous(LPCTSTR LookInFolders)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa6c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		LookInFolders);
	return result;
}

LPDISPATCH _Application::GetReminders()
{
	LPDISPATCH result;
	InvokeHelper(0xfa99, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Application::GetDefaultProfileName()
{
	CString result;
	InvokeHelper(0xfad6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Application::GetIsTrusted()
{
	BOOL result;
	InvokeHelper(0xfbf3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetObjectReference(LPDISPATCH Item, long ReferenceType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_I4;
	InvokeHelper(0xfbd6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Item, ReferenceType);
	return result;
}

LPDISPATCH _Application::GetAssistance()
{
	LPDISPATCH result;
	InvokeHelper(0xfc08, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Application::GetTimeZones()
{
	LPDISPATCH result;
	InvokeHelper(0xfc29, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NameSpace properties

/////////////////////////////////////////////////////////////////////////////
// _NameSpace operations

LPDISPATCH _NameSpace::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NameSpace::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetCurrentUser()
{
	LPDISPATCH result;
	InvokeHelper(0x2101, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetFolders()
{
	LPDISPATCH result;
	InvokeHelper(0x2103, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _NameSpace::GetType()
{
	CString result;
	InvokeHelper(0x2104, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetAddressLists()
{
	LPDISPATCH result;
	InvokeHelper(0x210d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::CreateRecipient(LPCTSTR RecipientName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x210a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		RecipientName);
	return result;
}

LPDISPATCH _NameSpace::GetDefaultFolder(long FolderType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x210b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FolderType);
	return result;
}

LPDISPATCH _NameSpace::GetFolderFromID(LPCTSTR EntryIDFolder, const VARIANT& EntryIDStore)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x2108, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EntryIDFolder, &EntryIDStore);
	return result;
}

LPDISPATCH _NameSpace::GetItemFromID(LPCTSTR EntryIDItem, const VARIANT& EntryIDStore)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x2109, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EntryIDItem, &EntryIDStore);
	return result;
}

LPDISPATCH _NameSpace::GetRecipientFromID(LPCTSTR EntryID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2107, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EntryID);
	return result;
}

LPDISPATCH _NameSpace::GetSharedDefaultFolder(LPDISPATCH Recipient, long FolderType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_I4;
	InvokeHelper(0x210c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Recipient, FolderType);
	return result;
}

void _NameSpace::Logoff()
{
	InvokeHelper(0x2106, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _NameSpace::Logon(const VARIANT& Profile, const VARIANT& Password, const VARIANT& ShowDialog, const VARIANT& NewSession)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x2105, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Profile, &Password, &ShowDialog, &NewSession);
}

LPDISPATCH _NameSpace::PickFolder()
{
	LPDISPATCH result;
	InvokeHelper(0x210e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetSyncObjects()
{
	LPDISPATCH result;
	InvokeHelper(0x2118, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _NameSpace::AddStore(const VARIANT& Store)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2119, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Store);
}

void _NameSpace::RemoveStore(LPDISPATCH Folder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x211a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Folder);
}

BOOL _NameSpace::GetOffline()
{
	BOOL result;
	InvokeHelper(0xfa4c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NameSpace::Dial(const VARIANT& ContactItem)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfa0d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &ContactItem);
}

long _NameSpace::GetExchangeConnectionMode()
{
	long result;
	InvokeHelper(0xfac1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NameSpace::AddStoreEx(const VARIANT& Store, long Type)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0xfac5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Store, Type);
}

LPDISPATCH _NameSpace::GetAccounts()
{
	LPDISPATCH result;
	InvokeHelper(0xfad0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _NameSpace::GetCurrentProfileName()
{
	CString result;
	InvokeHelper(0xfad5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetStores()
{
	LPDISPATCH result;
	InvokeHelper(0xfad8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetSelectNamesDialog()
{
	LPDISPATCH result;
	InvokeHelper(0xfae1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _NameSpace::SendAndReceive(BOOL showProgressDialog)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfad7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 showProgressDialog);
}

LPDISPATCH _NameSpace::GetDefaultStore()
{
	LPDISPATCH result;
	InvokeHelper(0xfaec, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetAddressEntryFromID(LPCTSTR ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb04, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

LPDISPATCH _NameSpace::GetGlobalAddressList()
{
	LPDISPATCH result;
	InvokeHelper(0xfb05, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::GetStoreFromID(LPCTSTR ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb06, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

LPDISPATCH _NameSpace::GetCategories()
{
	LPDISPATCH result;
	InvokeHelper(0xfba5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NameSpace::OpenSharedFolder(LPCTSTR Path, const VARIANT& Name, const VARIANT& DownloadAttachments, const VARIANT& UseTTL)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfbf6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Path, &Name, &DownloadAttachments, &UseTTL);
	return result;
}

LPDISPATCH _NameSpace::OpenSharedItem(LPCTSTR Path)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfbf7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Path);
	return result;
}

LPDISPATCH _NameSpace::CreateSharingItem(const VARIANT& Context, const VARIANT& Provider)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfbe4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Context, &Provider);
	return result;
}

CString _NameSpace::GetExchangeMailboxServerName()
{
	CString result;
	InvokeHelper(0xfc05, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _NameSpace::GetExchangeMailboxServerVersion()
{
	CString result;
	InvokeHelper(0xfc04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _NameSpace::CompareEntryIDs(LPCTSTR FirstEntryID, LPCTSTR SecondEntryID)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0xfbfc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FirstEntryID, SecondEntryID);
	return result;
}

CString _NameSpace::GetAutoDiscoverXml()
{
	CString result;
	InvokeHelper(0xfc03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _NameSpace::GetAutoDiscoverConnectionMode()
{
	long result;
	InvokeHelper(0xfc2e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// _Item operations

LPDISPATCH _Item::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Item::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Item::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

CString _Item::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

CString _Item::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

CString _Item::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

CString _Item::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _Item::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _Item::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Item::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		nNewValue);
}

DATE _Item::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _Item::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

CString _Item::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

BOOL _Item::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		bNewValue);
}

long _Item::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Item::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Item::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _Item::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		nNewValue);
}

long _Item::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Item::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		lpszNewValue);
}

BOOL _Item::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		bNewValue);
}

LPDISPATCH _Item::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		SaveMode);
}

LPDISPATCH _Item::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		&Modal);
}

LPDISPATCH _Item::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _Item::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		Path, &Type);
}



/////////////////////////////////////////////////////////////////////////////
// _ContactItem properties

/////////////////////////////////////////////////////////////////////////////
// _ContactItem operations

CString _ContactItem::GetAccount()
{
	CString result;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetAccount(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _ContactItem::GetAnniversary()
{
	DATE result;
	InvokeHelper(0x3a41, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetAnniversary(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x3a41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _ContactItem::GetAssistantName()
{
	CString result;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetAssistantName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetAssistantTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetAssistantTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _ContactItem::GetBirthday()
{
	DATE result;
	InvokeHelper(0x3a42, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBirthday(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x3a42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _ContactItem::GetBusiness2TelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusiness2TelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddress()
{
	CString result;
	InvokeHelper(0x801b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressCity()
{
	CString result;
	InvokeHelper(0x8046, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8046, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressCountry()
{
	CString result;
	InvokeHelper(0x8049, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressPostalCode()
{
	CString result;
	InvokeHelper(0x8048, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8048, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x804a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x804a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressState()
{
	CString result;
	InvokeHelper(0x8047, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessAddressStreet()
{
	CString result;
	InvokeHelper(0x8045, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8045, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessFaxNumber()
{
	CString result;
	InvokeHelper(0x3a24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessHomePage()
{
	CString result;
	InvokeHelper(0x3a51, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessHomePage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a51, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetBusinessTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetCallbackTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetCallbackTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetCarTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetCarTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetChildren()
{
	CString result;
	InvokeHelper(0x800c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetChildren(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x800c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetCompanyAndFullName()
{
	CString result;
	InvokeHelper(0x8018, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetCompanyLastFirstNoSpace()
{
	CString result;
	InvokeHelper(0x8032, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetCompanyLastFirstSpaceOnly()
{
	CString result;
	InvokeHelper(0x8033, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetCompanyMainTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a57, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetCompanyMainTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetCompanyName()
{
	CString result;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetComputerNetworkName()
{
	CString result;
	InvokeHelper(0x3a49, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetComputerNetworkName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetCustomerID()
{
	CString result;
	InvokeHelper(0x3a4a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetCustomerID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetDepartment()
{
	CString result;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetDepartment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail1Address()
{
	CString result;
	InvokeHelper(0x8083, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail1Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8083, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail1AddressType()
{
	CString result;
	InvokeHelper(0x8082, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail1AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8082, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail1DisplayName()
{
	CString result;
	InvokeHelper(0x8080, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetEmail1EntryID()
{
	CString result;
	InvokeHelper(0x8085, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetEmail2Address()
{
	CString result;
	InvokeHelper(0x8093, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail2Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8093, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail2AddressType()
{
	CString result;
	InvokeHelper(0x8092, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail2AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8092, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail2DisplayName()
{
	CString result;
	InvokeHelper(0x8090, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetEmail2EntryID()
{
	CString result;
	InvokeHelper(0x8095, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetEmail3Address()
{
	CString result;
	InvokeHelper(0x80a3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail3Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail3AddressType()
{
	CString result;
	InvokeHelper(0x80a2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetEmail3AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetEmail3DisplayName()
{
	CString result;
	InvokeHelper(0x80a0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetEmail3EntryID()
{
	CString result;
	InvokeHelper(0x80a5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetFileAs()
{
	CString result;
	InvokeHelper(0x8005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetFileAs(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetFirstName()
{
	CString result;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetFTPSite()
{
	CString result;
	InvokeHelper(0x3a4c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetFTPSite(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetFullName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetFullName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetFullNameAndCompany()
{
	CString result;
	InvokeHelper(0x8019, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _ContactItem::GetGender()
{
	long result;
	InvokeHelper(0x3a4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetGender(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3a4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _ContactItem::GetGovernmentIDNumber()
{
	CString result;
	InvokeHelper(0x3a07, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetGovernmentIDNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a07, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHobby()
{
	CString result;
	InvokeHelper(0x3a43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHobby(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHome2TelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHome2TelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddress()
{
	CString result;
	InvokeHelper(0x801a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressCity()
{
	CString result;
	InvokeHelper(0x3a59, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressCountry()
{
	CString result;
	InvokeHelper(0x3a5a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a5b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a5e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressState()
{
	CString result;
	InvokeHelper(0x3a5c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeAddressStreet()
{
	CString result;
	InvokeHelper(0x3a5d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeFaxNumber()
{
	CString result;
	InvokeHelper(0x3a25, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetHomeTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a09, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetHomeTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a09, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetInitials()
{
	CString result;
	InvokeHelper(0x3a0a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetInitials(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetInternetFreeBusyAddress()
{
	CString result;
	InvokeHelper(0x80d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetInternetFreeBusyAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetISDNNumber()
{
	CString result;
	InvokeHelper(0x3a2d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetISDNNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetJobTitle()
{
	CString result;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetJobTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _ContactItem::GetJournal()
{
	BOOL result;
	InvokeHelper(0x8025, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetJournal(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8025, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _ContactItem::GetLanguage()
{
	CString result;
	InvokeHelper(0x3a0c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetLastFirstAndSuffix()
{
	CString result;
	InvokeHelper(0x8036, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastFirstNoSpace()
{
	CString result;
	InvokeHelper(0x8030, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastFirstNoSpaceCompany()
{
	CString result;
	InvokeHelper(0x8034, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastFirstSpaceOnly()
{
	CString result;
	InvokeHelper(0x8031, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastFirstSpaceOnlyCompany()
{
	CString result;
	InvokeHelper(0x8035, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastName()
{
	CString result;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetLastNameAndFirstName()
{
	CString result;
	InvokeHelper(0x8017, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetMailingAddress()
{
	CString result;
	InvokeHelper(0x3a15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressCity()
{
	CString result;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressCountry()
{
	CString result;
	InvokeHelper(0x3a26, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a2b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressState()
{
	CString result;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMailingAddressStreet()
{
	CString result;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMailingAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetManagerName()
{
	CString result;
	InvokeHelper(0x3a4e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetManagerName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMiddleName()
{
	CString result;
	InvokeHelper(0x3a44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMiddleName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetMobileTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMobileTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetNetMeetingAlias()
{
	CString result;
	InvokeHelper(0x805f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetNetMeetingAlias(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x805f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetNetMeetingServer()
{
	CString result;
	InvokeHelper(0x8060, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetNetMeetingServer(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8060, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetNickName()
{
	CString result;
	InvokeHelper(0x3a4f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetNickName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOfficeLocation()
{
	CString result;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOfficeLocation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOrganizationalIDNumber()
{
	CString result;
	InvokeHelper(0x3a10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOrganizationalIDNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddress()
{
	CString result;
	InvokeHelper(0x801c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressCity()
{
	CString result;
	InvokeHelper(0x3a5f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressCountry()
{
	CString result;
	InvokeHelper(0x3a60, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a61, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a64, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressState()
{
	CString result;
	InvokeHelper(0x3a62, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherAddressStreet()
{
	CString result;
	InvokeHelper(0x3a63, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherFaxNumber()
{
	CString result;
	InvokeHelper(0x3a23, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetOtherTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetOtherTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetPagerNumber()
{
	CString result;
	InvokeHelper(0x3a21, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetPagerNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetPersonalHomePage()
{
	CString result;
	InvokeHelper(0x3a50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetPersonalHomePage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetPrimaryTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetPrimaryTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetProfession()
{
	CString result;
	InvokeHelper(0x3a46, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetProfession(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetRadioTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetRadioTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetReferredBy()
{
	CString result;
	InvokeHelper(0x3a47, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReferredBy(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ContactItem::GetSelectedMailingAddress()
{
	long result;
	InvokeHelper(0x8022, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetSelectedMailingAddress(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _ContactItem::GetSpouse()
{
	CString result;
	InvokeHelper(0x3a48, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetSpouse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetSuffix()
{
	CString result;
	InvokeHelper(0x3a05, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetSuffix(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetTelexNumber()
{
	CString result;
	InvokeHelper(0x3a2c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTelexNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetTitle()
{
	CString result;
	InvokeHelper(0x3a45, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetTTYTDDTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a4b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTTYTDDTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetUser1()
{
	CString result;
	InvokeHelper(0x804f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetUser1(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x804f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetUser2()
{
	CString result;
	InvokeHelper(0x8050, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetUser2(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8050, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetUser3()
{
	CString result;
	InvokeHelper(0x8051, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetUser3(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8051, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetUser4()
{
	CString result;
	InvokeHelper(0x8052, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetUser4(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8052, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetWebPage()
{
	CString result;
	InvokeHelper(0x802b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetWebPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetYomiCompanyName()
{
	CString result;
	InvokeHelper(0x802e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetYomiCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetYomiFirstName()
{
	CString result;
	InvokeHelper(0x802c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetYomiFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ContactItem::GetYomiLastName()
{
	CString result;
	InvokeHelper(0x802d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetYomiLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _ContactItem::ForwardAsVcard()
{
	LPDISPATCH result;
	InvokeHelper(0xf8a1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetLastFirstNoSpaceAndSuffix()
{
	CString result;
	InvokeHelper(0x8038, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _ContactItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ContactItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _ContactItem::GetIMAddress()
{
	CString result;
	InvokeHelper(0x8062, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetIMAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8062, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ContactItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _ContactItem::SetEmail1DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8080, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _ContactItem::SetEmail2DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8090, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _ContactItem::SetEmail3DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _ContactItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _ContactItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _ContactItem::AddPicture(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfabd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

void _ContactItem::RemovePicture()
{
	InvokeHelper(0xfabe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _ContactItem::GetHasPicture()
{
	BOOL result;
	InvokeHelper(0xfabf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactItem::ForwardAsBusinessCard()
{
	LPDISPATCH result;
	InvokeHelper(0xfb94, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _ContactItem::ShowBusinessCardEditor()
{
	InvokeHelper(0xfb95, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ContactItem::SaveBusinessCardImage(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb97, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

void _ContactItem::ShowCheckPhoneDialog(long PhoneNumber)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PhoneNumber);
}

CString _ContactItem::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _ContactItem::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _ContactItem::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _ContactItem::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _ContactItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _ContactItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ContactItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ContactItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _ContactItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _ContactItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _ContactItem::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _ContactItem::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _ContactItem::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _ContactItem::GetBusinessCardLayoutXml()
{
	CString result;
	InvokeHelper(0xfc0d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ContactItem::SetBusinessCardLayoutXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc0d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _ContactItem::ResetBusinessCard()
{
	InvokeHelper(0xfc0e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ContactItem::AddBusinessCardLogoPicture(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc0f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

long _ContactItem::GetBusinessCardType()
{
	long result;
	InvokeHelper(0xfc10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// MAPIFolder properties

/////////////////////////////////////////////////////////////////////////////
// MAPIFolder operations

LPDISPATCH MAPIFolder::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long MAPIFolder::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long MAPIFolder::GetDefaultItemType()
{
	long result;
	InvokeHelper(0x3106, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString MAPIFolder::GetDefaultMessageClass()
{
	CString result;
	InvokeHelper(0x3107, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString MAPIFolder::GetDescription()
{
	CString result;
	InvokeHelper(0x3004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString MAPIFolder::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetFolders()
{
	LPDISPATCH result;
	InvokeHelper(0x2103, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetItems()
{
	LPDISPATCH result;
	InvokeHelper(0x3100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString MAPIFolder::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString MAPIFolder::GetStoreID()
{
	CString result;
	InvokeHelper(0x3108, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long MAPIFolder::GetUnReadItemCount()
{
	long result;
	InvokeHelper(0x3603, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::CopyTo(LPDISPATCH DestinationFolder)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestinationFolder);
	return result;
}

void MAPIFolder::Delete()
{
	InvokeHelper(0xf045, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void MAPIFolder::Display()
{
	InvokeHelper(0x3104, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH MAPIFolder::GetExplorer(const VARIANT& DisplayMode)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3101, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&DisplayMode);
	return result;
}

void MAPIFolder::MoveTo(LPDISPATCH DestinationFolder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 DestinationFolder);
}

BOOL MAPIFolder::GetWebViewOn()
{
	BOOL result;
	InvokeHelper(0x3112, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetWebViewOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString MAPIFolder::GetWebViewURL()
{
	CString result;
	InvokeHelper(0x3113, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetWebViewURL(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3113, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void MAPIFolder::AddToPFFavorites()
{
	InvokeHelper(0x3115, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString MAPIFolder::GetAddressBookName()
{
	CString result;
	InvokeHelper(0xfa6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetAddressBookName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL MAPIFolder::GetShowAsOutlookAB()
{
	BOOL result;
	InvokeHelper(0xfa6f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetShowAsOutlookAB(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa6f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString MAPIFolder::GetFolderPath()
{
	CString result;
	InvokeHelper(0xfa78, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL MAPIFolder::GetInAppFolderSyncObject()
{
	BOOL result;
	InvokeHelper(0xfa4b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetInAppFolderSyncObject(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH MAPIFolder::GetCurrentView()
{
	LPDISPATCH result;
	InvokeHelper(0x2200, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL MAPIFolder::GetCustomViewsOnly()
{
	BOOL result;
	InvokeHelper(0xfa46, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetCustomViewsOnly(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH MAPIFolder::GetViews()
{
	LPDISPATCH result;
	InvokeHelper(0x3109, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL MAPIFolder::GetIsSharePointFolder()
{
	BOOL result;
	InvokeHelper(0xfab6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long MAPIFolder::GetShowItemCount()
{
	long result;
	InvokeHelper(0xfac2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void MAPIFolder::SetShowItemCount(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfac2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH MAPIFolder::GetStore()
{
	LPDISPATCH result;
	InvokeHelper(0xfad9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetStorage(LPCTSTR StorageIdentifier, long StorageIdentifierType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xfb08, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		StorageIdentifier, StorageIdentifierType);
	return result;
}

LPDISPATCH MAPIFolder::GetTable(const VARIANT& Filter, const VARIANT& TableContents)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfb1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Filter, &TableContents);
	return result;
}

LPDISPATCH MAPIFolder::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetCalendarExporter()
{
	LPDISPATCH result;
	InvokeHelper(0xfba2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH MAPIFolder::GetUserDefinedProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// _Items properties

/////////////////////////////////////////////////////////////////////////////
// _Items operations

LPDISPATCH _Items::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Items::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Items::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

BOOL _Items::GetIncludeRecurrences()
{
	BOOL result;
	InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Items::SetIncludeRecurrences(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xce, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Items::Add(const VARIANT& Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Type);
	return result;
}

LPDISPATCH _Items::Find(LPCTSTR Filter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filter);
	return result;
}

LPDISPATCH _Items::FindNext()
{
	LPDISPATCH result;
	InvokeHelper(0x63, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetFirst()
{
	LPDISPATCH result;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetLast()
{
	LPDISPATCH result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Items::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Items::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void _Items::ResetColumns()
{
	InvokeHelper(0x5d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Items::Restrict(LPCTSTR Filter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filter);
	return result;
}

void _Items::SetColumns(LPCTSTR Columns)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Columns);
}

void _Items::Sort(LPCTSTR Property_, const VARIANT& Descending)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Property_, &Descending);
}

/////////////////////////////////////////////////////////////////////////////
// _Explorer properties

/////////////////////////////////////////////////////////////////////////////
// _Explorer operations

LPDISPATCH _Explorer::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Explorer::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x2100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetCurrentFolder()
{
	LPDISPATCH result;
	InvokeHelper(0x2101, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Explorer::SetRefCurrentFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x2101, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

void _Explorer::Close()
{
	InvokeHelper(0x2103, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Explorer::Display()
{
	InvokeHelper(0x2104, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _Explorer::GetCaption()
{
	CString result;
	InvokeHelper(0x2111, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

VARIANT _Explorer::GetCurrentView()
{
	VARIANT result;
	InvokeHelper(0x2200, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _Explorer::SetCurrentView(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2200, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long _Explorer::GetHeight()
{
	long result;
	InvokeHelper(0x2114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Explorer::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Explorer::GetLeft()
{
	long result;
	InvokeHelper(0x2115, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Explorer::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2115, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Explorer::GetPanes()
{
	LPDISPATCH result;
	InvokeHelper(0x2201, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetSelection()
{
	LPDISPATCH result;
	InvokeHelper(0x2202, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Explorer::GetTop()
{
	long result;
	InvokeHelper(0x2116, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Explorer::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Explorer::GetWidth()
{
	long result;
	InvokeHelper(0x2117, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Explorer::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2117, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Explorer::GetWindowState()
{
	long result;
	InvokeHelper(0x2112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Explorer::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Explorer::Activate()
{
	InvokeHelper(0x2113, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Explorer::IsPaneVisible(long Pane)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2203, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Pane);
	return result;
}

void _Explorer::ShowPane(long Pane, BOOL Visible)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x2204, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Pane, Visible);
}

LPDISPATCH _Explorer::GetHTMLDocument()
{
	LPDISPATCH result;
	InvokeHelper(0xfa92, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorer::GetNavigationPane()
{
	LPDISPATCH result;
	InvokeHelper(0xfbb3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Explorer::ClearSearch()
{
	InvokeHelper(0xfbcd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Explorer::Search(LPCTSTR Query, long SearchScope)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xfa65, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Query, SearchScope);
}

/////////////////////////////////////////////////////////////////////////////
// Selection properties

/////////////////////////////////////////////////////////////////////////////
// Selection operations

LPDISPATCH Selection::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Selection::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Selection::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Selection::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Selection::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Selection::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// _MailItem properties

/////////////////////////////////////////////////////////////////////////////
// _MailItem operations

BOOL _MailItem::GetAlternateRecipientAllowed()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetAlternateRecipientAllowed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetAutoForwarded()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetAutoForwarded(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetBcc()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetBcc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetCc()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetCc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _MailItem::GetDeferredDeliveryTime()
{
	DATE result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetDeferredDeliveryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _MailItem::GetDeleteAfterSubmit()
{
	BOOL result;
	InvokeHelper(0xe01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetDeleteAfterSubmit(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MailItem::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MailItem::GetFlagRequest()
{
	CString result;
	InvokeHelper(0x8530, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetFlagRequest(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8530, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetHTMLBody()
{
	CString result;
	InvokeHelper(0xf404, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetHTMLBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf404, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetOriginatorDeliveryReportRequested()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetOriginatorDeliveryReportRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReadReceiptRequested()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReadReceiptRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetReceivedByEntryID()
{
	CString result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedByName()
{
	CString result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedOnBehalfOfEntryID()
{
	CString result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetReceivedOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _MailItem::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetRecipientReassignmentProhibited()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRecipientReassignmentProhibited(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _MailItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _MailItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _MailItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _MailItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _MailItem::GetRemoteStatus()
{
	long result;
	InvokeHelper(0x8511, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRemoteStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8511, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _MailItem::GetReplyRecipientNames()
{
	CString result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetReplyRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetSaveSentMessageFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xf401, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::SetRefSaveSentMessageFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf401, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MailItem::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetSent()
{
	BOOL result;
	InvokeHelper(0xf402, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE _MailItem::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetSentOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetSentOnBehalfOfName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MailItem::GetSubmitted()
{
	BOOL result;
	InvokeHelper(0xf403, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetTo()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTo(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetVotingOptions()
{
	CString result;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetVotingOptions(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MailItem::GetVotingResponse()
{
	CString result;
	InvokeHelper(0x8524, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetVotingResponse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8524, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _MailItem::ClearConversationIndex()
{
	InvokeHelper(0xf822, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _MailItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _MailItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MailItem::GetBodyFormat()
{
	long result;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetBodyFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _MailItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _MailItem::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _MailItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _MailItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _MailItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetSenderEmailAddress()
{
	CString result;
	InvokeHelper(0xc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MailItem::GetSenderEmailType()
{
	CString result;
	InvokeHelper(0xc1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _MailItem::GetPermission()
{
	long result;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetPermission(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _MailItem::GetPermissionService()
{
	long result;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailItem::SetPermissionService(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _MailItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::AddBusinessCard(LPDISPATCH contact)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb96, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 contact);
}

LPDISPATCH _MailItem::GetSendUsingAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MailItem::SetSendUsingAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MailItem::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _MailItem::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _MailItem::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _MailItem::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _MailItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MailItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _MailItem::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _MailItem::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _MailItem::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}



/////////////////////////////////////////////////////////////////////////////
// _AppointmentItem properties

/////////////////////////////////////////////////////////////////////////////
// _AppointmentItem operations

BOOL _AppointmentItem::GetAllDayEvent()
{
	BOOL result;
	InvokeHelper(0x8215, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetAllDayEvent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8215, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _AppointmentItem::GetBusyStatus()
{
	long result;
	InvokeHelper(0x8205, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetBusyStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8205, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _AppointmentItem::GetDuration()
{
	long result;
	InvokeHelper(0x8213, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetDuration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8213, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _AppointmentItem::GetEnd()
{
	DATE result;
	InvokeHelper(0x820e, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetEnd(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x820e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _AppointmentItem::GetIsRecurring()
{
	BOOL result;
	InvokeHelper(0x8223, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _AppointmentItem::GetLocation()
{
	CString result;
	InvokeHelper(0x8208, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetLocation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8208, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _AppointmentItem::GetMeetingStatus()
{
	long result;
	InvokeHelper(0x8217, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetMeetingStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8217, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _AppointmentItem::GetOptionalAttendees()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetOptionalAttendees(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _AppointmentItem::GetOrganizer()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AppointmentItem::GetRecurrenceState()
{
	long result;
	InvokeHelper(0xf545, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _AppointmentItem::GetReminderMinutesBeforeStart()
{
	long result;
	InvokeHelper(0x8501, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReminderMinutesBeforeStart(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8501, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _AppointmentItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _AppointmentItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _AppointmentItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _AppointmentItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _AppointmentItem::GetReplyTime()
{
	DATE result;
	InvokeHelper(0x8220, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetReplyTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8220, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _AppointmentItem::GetRequiredAttendees()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetRequiredAttendees(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _AppointmentItem::GetResources()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetResources(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _AppointmentItem::GetResponseRequested()
{
	BOOL result;
	InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetResponseRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _AppointmentItem::GetResponseStatus()
{
	long result;
	InvokeHelper(0x8218, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

DATE _AppointmentItem::GetStart()
{
	DATE result;
	InvokeHelper(0x820d, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetStart(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x820d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _AppointmentItem::ClearRecurrencePattern()
{
	InvokeHelper(0xf0a5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _AppointmentItem::ForwardAsVcal()
{
	LPDISPATCH result;
	InvokeHelper(0xf547, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetRecurrencePattern()
{
	LPDISPATCH result;
	InvokeHelper(0xf0a4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf502, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Response, &fNoUI, &fAdditionalTextDialog);
	return result;
}

void _AppointmentItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _AppointmentItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AppointmentItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _AppointmentItem::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _AppointmentItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _AppointmentItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _AppointmentItem::GetMeetingWorkspaceURL()
{
	CString result;
	InvokeHelper(0x8209, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _AppointmentItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AppointmentItem::GetSendUsingAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetSendUsingAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _AppointmentItem::GetGlobalAppointmentID()
{
	CString result;
	InvokeHelper(0xfbfd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _AppointmentItem::GetForceUpdateToAllAttendees()
{
	BOOL result;
	InvokeHelper(0xf53a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetForceUpdateToAllAttendees(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf53a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _AppointmentItem::GetStartUTC()
{
	DATE result;
	InvokeHelper(0x8268, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetStartUTC(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8268, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _AppointmentItem::GetEndUTC()
{
	DATE result;
	InvokeHelper(0x8269, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetEndUTC(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8269, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _AppointmentItem::GetStartInStartTimeZone()
{
	DATE result;
	InvokeHelper(0x8266, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetStartInStartTimeZone(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8266, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _AppointmentItem::GetEndInEndTimeZone()
{
	DATE result;
	InvokeHelper(0x8267, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetEndInEndTimeZone(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8267, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _AppointmentItem::GetStartTimeZone()
{
	LPDISPATCH result;
	InvokeHelper(0xfc27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetStartTimeZone(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfc27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _AppointmentItem::GetEndTimeZone()
{
	LPDISPATCH result;
	InvokeHelper(0xfc28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _AppointmentItem::SetEndTimeZone(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfc28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}



/////////////////////////////////////////////////////////////////////////////
// _JournalItem properties

/////////////////////////////////////////////////////////////////////////////
// _JournalItem operations

CString _JournalItem::GetContactNames()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetContactNames(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _JournalItem::GetDocPosted()
{
	BOOL result;
	InvokeHelper(0x8711, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetDocPosted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8711, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _JournalItem::GetDocPrinted()
{
	BOOL result;
	InvokeHelper(0x870e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetDocPrinted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x870e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _JournalItem::GetDocRouted()
{
	BOOL result;
	InvokeHelper(0x8710, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetDocRouted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8710, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _JournalItem::GetDocSaved()
{
	BOOL result;
	InvokeHelper(0x870f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetDocSaved(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x870f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _JournalItem::GetDuration()
{
	long result;
	InvokeHelper(0x8707, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetDuration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8707, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _JournalItem::GetEnd()
{
	DATE result;
	InvokeHelper(0x8708, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetEnd(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8708, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _JournalItem::GetType()
{
	CString result;
	InvokeHelper(0x8700, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8700, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _JournalItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

DATE _JournalItem::GetStart()
{
	DATE result;
	InvokeHelper(0x8706, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetStart(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8706, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _JournalItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalItem::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _JournalItem::StartTimer()
{
	InvokeHelper(0xf725, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _JournalItem::StopTimer()
{
	InvokeHelper(0xf726, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _JournalItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _JournalItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _JournalItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _JournalItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _JournalItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _JournalItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _JournalItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _JournalItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NoteItem properties

/////////////////////////////////////////////////////////////////////////////
// _NoteItem operations

long _NoteItem::GetHeight()
{
	long result;
	InvokeHelper(0x8b03, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NoteItem::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _NoteItem::GetLeft()
{
	long result;
	InvokeHelper(0x8b04, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NoteItem::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _NoteItem::GetTop()
{
	long result;
	InvokeHelper(0x8b05, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NoteItem::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _NoteItem::GetWidth()
{
	long result;
	InvokeHelper(0x8b02, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NoteItem::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _NoteItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NoteItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NoteItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NoteItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NoteItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _NoteItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _NoteItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NoteItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NoteItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TaskItem properties

/////////////////////////////////////////////////////////////////////////////
// _TaskItem operations

long _TaskItem::GetActualWork()
{
	long result;
	InvokeHelper(0x8110, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetActualWork(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8110, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _TaskItem::GetCardData()
{
	CString result;
	InvokeHelper(0x812b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetCardData(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x812b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskItem::GetComplete()
{
	BOOL result;
	InvokeHelper(0x811c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetComplete(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x811c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _TaskItem::GetContactNames()
{
	CString result;
	InvokeHelper(0x853c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetContactNames(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _TaskItem::GetDateCompleted()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetDateCompleted(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _TaskItem::GetDelegationState()
{
	long result;
	InvokeHelper(0x812a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskItem::GetDelegator()
{
	CString result;
	InvokeHelper(0x8121, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _TaskItem::GetDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _TaskItem::GetIsRecurring()
{
	BOOL result;
	InvokeHelper(0xf617, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TaskItem::GetOrdinal()
{
	long result;
	InvokeHelper(0x8123, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetOrdinal(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8123, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _TaskItem::GetOwner()
{
	CString result;
	InvokeHelper(0x811f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetOwner(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x811f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _TaskItem::GetOwnership()
{
	long result;
	InvokeHelper(0x8129, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _TaskItem::GetPercentComplete()
{
	long result;
	InvokeHelper(0xf61f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetPercentComplete(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf61f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _TaskItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

DATE _TaskItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _TaskItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TaskItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TaskItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _TaskItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _TaskItem::GetResponseState()
{
	long result;
	InvokeHelper(0xf623, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskItem::GetRole()
{
	CString result;
	InvokeHelper(0x8127, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetRole(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8127, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskItem::GetSchedulePlusPriority()
{
	CString result;
	InvokeHelper(0x812f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetSchedulePlusPriority(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x812f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _TaskItem::GetStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _TaskItem::GetStatus()
{
	long result;
	InvokeHelper(0x8101, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8101, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _TaskItem::GetStatusOnCompletionRecipients()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetStatusOnCompletionRecipients(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskItem::GetStatusUpdateRecipients()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetStatusUpdateRecipients(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskItem::GetTeamTask()
{
	BOOL result;
	InvokeHelper(0x8103, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetTeamTask(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8103, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TaskItem::GetTotalWork()
{
	long result;
	InvokeHelper(0x8111, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetTotalWork(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8111, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _TaskItem::Assign()
{
	LPDISPATCH result;
	InvokeHelper(0xf620, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskItem::CancelResponseState()
{
	InvokeHelper(0xf622, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskItem::ClearRecurrencePattern()
{
	InvokeHelper(0xf0a5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskItem::GetRecurrencePattern()
{
	LPDISPATCH result;
	InvokeHelper(0xf0a4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskItem::MarkComplete()
{
	InvokeHelper(0xf60d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskItem::Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf621, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Response, &fNoUI, &fAdditionalTextDialog);
	return result;
}

void _TaskItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _TaskItem::SkipRecurrence()
{
	BOOL result;
	InvokeHelper(0xf624, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskItem::StatusReport()
{
	LPDISPATCH result;
	InvokeHelper(0xf612, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskItem::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TaskItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TaskItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _TaskItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskItem::GetSendUsingAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetSendUsingAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _TaskItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _TaskItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

/*
/////////////////////////////////////////////////////////////////////////////
// _TaskRequestAcceptItem properties

/////////////////////////////////////////////////////////////////////////////
// _TaskRequestAcceptItem operations

LPDISPATCH _TaskRequestAcceptItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestAcceptItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestAcceptItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestAcceptItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestAcceptItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestAcceptItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _TaskRequestAcceptItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestAcceptItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _TaskRequestAcceptItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestAcceptItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestAcceptItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TaskRequestAcceptItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestAcceptItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TaskRequestAcceptItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TaskRequestAcceptItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestAcceptItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestAcceptItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TaskRequestAcceptItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _TaskRequestAcceptItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestAcceptItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _TaskRequestAcceptItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _TaskRequestAcceptItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestAcceptItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestAcceptItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _TaskRequestAcceptItem::GetAssociatedTask(BOOL AddToTaskList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf014, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		AddToTaskList);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestAcceptItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskRequestAcceptItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestAcceptItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestAcceptItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TaskRequestAcceptItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestAcceptItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestAcceptItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TaskRequestDeclineItem properties

/////////////////////////////////////////////////////////////////////////////
// _TaskRequestDeclineItem operations

LPDISPATCH _TaskRequestDeclineItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestDeclineItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestDeclineItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestDeclineItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestDeclineItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestDeclineItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _TaskRequestDeclineItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestDeclineItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _TaskRequestDeclineItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestDeclineItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestDeclineItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TaskRequestDeclineItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestDeclineItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TaskRequestDeclineItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TaskRequestDeclineItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestDeclineItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestDeclineItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TaskRequestDeclineItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _TaskRequestDeclineItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestDeclineItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _TaskRequestDeclineItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _TaskRequestDeclineItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestDeclineItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestDeclineItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _TaskRequestDeclineItem::GetAssociatedTask(BOOL AddToTaskList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf014, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		AddToTaskList);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestDeclineItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskRequestDeclineItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestDeclineItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestDeclineItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TaskRequestDeclineItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestDeclineItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestDeclineItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TaskRequestItem properties

/////////////////////////////////////////////////////////////////////////////
// _TaskRequestItem operations

LPDISPATCH _TaskRequestItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _TaskRequestItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _TaskRequestItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TaskRequestItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TaskRequestItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TaskRequestItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TaskRequestItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _TaskRequestItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _TaskRequestItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _TaskRequestItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _TaskRequestItem::GetAssociatedTask(BOOL AddToTaskList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf014, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		AddToTaskList);
	return result;
}

LPDISPATCH _TaskRequestItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskRequestItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TaskRequestItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TaskRequestUpdateItem properties

/////////////////////////////////////////////////////////////////////////////
// _TaskRequestUpdateItem operations

LPDISPATCH _TaskRequestUpdateItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestUpdateItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestUpdateItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestUpdateItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestUpdateItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestUpdateItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _TaskRequestUpdateItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestUpdateItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _TaskRequestUpdateItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TaskRequestUpdateItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestUpdateItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TaskRequestUpdateItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestUpdateItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TaskRequestUpdateItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TaskRequestUpdateItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TaskRequestUpdateItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TaskRequestUpdateItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TaskRequestUpdateItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _TaskRequestUpdateItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestUpdateItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _TaskRequestUpdateItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _TaskRequestUpdateItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestUpdateItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TaskRequestUpdateItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _TaskRequestUpdateItem::GetAssociatedTask(BOOL AddToTaskList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf014, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		AddToTaskList);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestUpdateItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TaskRequestUpdateItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TaskRequestUpdateItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TaskRequestUpdateItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TaskRequestUpdateItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _TaskRequestUpdateItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TaskRequestUpdateItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _FormRegion properties

/////////////////////////////////////////////////////////////////////////////
// _FormRegion operations

LPDISPATCH _FormRegion::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _FormRegion::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormRegion::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormRegion::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _FormRegion::GetIsExpanded()
{
	BOOL result;
	InvokeHelper(0xfb38, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormRegion::GetForm()
{
	LPDISPATCH result;
	InvokeHelper(0xfb39, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _FormRegion::GetDetail()
{
	CString result;
	InvokeHelper(0xfb35, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _FormRegion::SetDetail(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _FormRegion::GetItem()
{
	LPDISPATCH result;
	InvokeHelper(0xfa60, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormRegion::GetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _FormRegion::GetFormRegionMode()
{
	long result;
	InvokeHelper(0xfb34, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _FormRegion::GetLanguage()
{
	long result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _FormRegion::Reflow()
{
	InvokeHelper(0xfb3b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _FormRegion::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _FormRegion::GetInternalName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _FormRegion::SetControlItemProperty(LPDISPATCH Control, LPCTSTR PropertyName)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0xfac9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Control, PropertyName);
}

BOOL _FormRegion::GetSuppressControlReplacement()
{
	BOOL result;
	InvokeHelper(0xfc06, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _FormRegion::SetSuppressControlReplacement(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _FormRegion::Select()
{
	InvokeHelper(0x2109, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _FormRegion::GetEnableAutoLayout()
{
	BOOL result;
	InvokeHelper(0xfc1a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _FormRegion::SetEnableAutoLayout(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// FormRegionEvents properties

/////////////////////////////////////////////////////////////////////////////
// FormRegionEvents operations

void FormRegionEvents::Expanded(BOOL Expand)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb38, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Expand);
}

void FormRegionEvents::Close()
{
	InvokeHelper(0xf004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _TableView properties

/////////////////////////////////////////////////////////////////////////////
// _TableView operations

LPDISPATCH _TableView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TableView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TableView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TableView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _TableView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TableView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TableView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _TableView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TableView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TableView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _TableView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TableView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _TableView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _TableView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TableView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TableView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TableView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _TableView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _TableView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TableView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _TableView::GetViewFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb55, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetGroupByFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb58, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetSortFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb5a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TableView::GetMaxLinesInMultiLineView()
{
	long result;
	InvokeHelper(0xfb5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetMaxLinesInMultiLineView(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TableView::GetAutomaticGrouping()
{
	BOOL result;
	InvokeHelper(0xfb59, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetAutomaticGrouping(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TableView::GetDefaultExpandCollapseSetting()
{
	long result;
	InvokeHelper(0xfb80, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetDefaultExpandCollapseSetting(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb80, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TableView::GetAutomaticColumnSizing()
{
	BOOL result;
	InvokeHelper(0xfb5c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetAutomaticColumnSizing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TableView::GetMultiLine()
{
	long result;
	InvokeHelper(0xfb5d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetMultiLine(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _TableView::GetMultiLineWidth()
{
	long result;
	InvokeHelper(0xfb5e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetMultiLineWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TableView::GetAllowInCellEditing()
{
	BOOL result;
	InvokeHelper(0xfb5f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetAllowInCellEditing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TableView::GetShowNewItemRow()
{
	BOOL result;
	InvokeHelper(0xfb60, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetShowNewItemRow(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TableView::GetGridLineStyle()
{
	long result;
	InvokeHelper(0xfb61, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetGridLineStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TableView::GetShowItemsInGroups()
{
	BOOL result;
	InvokeHelper(0xfb62, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetShowItemsInGroups(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TableView::GetShowReadingPane()
{
	BOOL result;
	InvokeHelper(0xfb63, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetShowReadingPane(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TableView::GetHideReadingPaneHeaderInfo()
{
	BOOL result;
	InvokeHelper(0xfb64, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetHideReadingPaneHeaderInfo(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _TableView::GetShowUnreadAndFlaggedMessages()
{
	BOOL result;
	InvokeHelper(0xfb65, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TableView::SetShowUnreadAndFlaggedMessages(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TableView::GetRowFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb68, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetColumnFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb67, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TableView::GetAutoPreviewFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb69, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TableView::GetAutoPreview()
{
	long result;
	InvokeHelper(0xfb78, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TableView::SetAutoPreview(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _TableView::GetAutoFormatRules()
{
	LPDISPATCH result;
	InvokeHelper(0xfa3b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ViewFields properties

/////////////////////////////////////////////////////////////////////////////
// _ViewFields operations

LPDISPATCH _ViewFields::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ViewFields::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewFields::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewFields::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ViewFields::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewFields::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _ViewFields::Add(LPCTSTR PropertyName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PropertyName);
	return result;
}

LPDISPATCH _ViewFields::Insert(LPCTSTR PropertyName, const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xfb56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PropertyName, &Index);
	return result;
}

void _ViewFields::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// _ViewField properties

/////////////////////////////////////////////////////////////////////////////
// _ViewField operations

LPDISPATCH _ViewField::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ViewField::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewField::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewField::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ViewField::GetViewXMLSchemaName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewField::GetColumnFormat()
{
	LPDISPATCH result;
	InvokeHelper(0xfb8d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ColumnFormat properties

/////////////////////////////////////////////////////////////////////////////
// _ColumnFormat operations

LPDISPATCH _ColumnFormat::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ColumnFormat::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ColumnFormat::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ColumnFormat::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ColumnFormat::GetLabel()
{
	CString result;
	InvokeHelper(0xfa1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ColumnFormat::SetLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ColumnFormat::GetFieldFormat()
{
	long result;
	InvokeHelper(0xfb8c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ColumnFormat::SetFieldFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb8c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _ColumnFormat::GetWidth()
{
	long result;
	InvokeHelper(0x2117, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ColumnFormat::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2117, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _ColumnFormat::GetAlign()
{
	long result;
	InvokeHelper(0xfa22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ColumnFormat::SetAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _ColumnFormat::GetFieldType()
{
	long result;
	InvokeHelper(0xfb8e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _OrderFields properties

/////////////////////////////////////////////////////////////////////////////
// _OrderFields operations

LPDISPATCH _OrderFields::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OrderFields::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OrderFields::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OrderFields::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OrderFields::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OrderFields::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _OrderFields::Add(LPCTSTR PropertyName, const VARIANT& IsDescending)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PropertyName, &IsDescending);
	return result;
}

void _OrderFields::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

void _OrderFields::RemoveAll()
{
	InvokeHelper(0xfb57, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _OrderFields::Insert(LPCTSTR PropertyName, const VARIANT& Index, const VARIANT& IsDescending)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfb56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PropertyName, &Index, &IsDescending);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _OrderField properties

/////////////////////////////////////////////////////////////////////////////
// _OrderField operations

LPDISPATCH _OrderField::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OrderField::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OrderField::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OrderField::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _OrderField::GetViewXMLSchemaName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _OrderField::GetIsDescending()
{
	BOOL result;
	InvokeHelper(0xfb8a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OrderField::SetIsDescending(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb8a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _ViewFont properties

/////////////////////////////////////////////////////////////////////////////
// _ViewFont operations

LPDISPATCH _ViewFont::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ViewFont::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewFont::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ViewFont::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _ViewFont::GetBold()
{
	BOOL result;
	InvokeHelper(0xfb81, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetBold(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb81, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ViewFont::GetItalic()
{
	BOOL result;
	InvokeHelper(0xfb83, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetItalic(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb83, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _ViewFont::GetName()
{
	CString result;
	InvokeHelper(0xfb84, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb84, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ViewFont::GetSize()
{
	long result;
	InvokeHelper(0xfb85, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb85, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ViewFont::GetStrikethrough()
{
	BOOL result;
	InvokeHelper(0xfb86, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetStrikethrough(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _ViewFont::GetUnderline()
{
	BOOL result;
	InvokeHelper(0xfb87, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetUnderline(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb87, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ViewFont::GetColor()
{
	long result;
	InvokeHelper(0xfb8b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ViewFont::SetColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _AutoFormatRules properties

/////////////////////////////////////////////////////////////////////////////
// _AutoFormatRules operations

LPDISPATCH _AutoFormatRules::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AutoFormatRules::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AutoFormatRules::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AutoFormatRules::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AutoFormatRules::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AutoFormatRules::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _AutoFormatRules::Add(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

LPDISPATCH _AutoFormatRules::Insert(LPCTSTR Name, const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xfb56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Index);
	return result;
}

void _AutoFormatRules::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

void _AutoFormatRules::RemoveAll()
{
	InvokeHelper(0xfb57, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AutoFormatRules::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _AutoFormatRule properties

/////////////////////////////////////////////////////////////////////////////
// _AutoFormatRule operations

LPDISPATCH _AutoFormatRule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AutoFormatRule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AutoFormatRule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AutoFormatRule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _AutoFormatRule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AutoFormatRule::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _AutoFormatRule::GetFilter()
{
	CString result;
	InvokeHelper(0xfa18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _AutoFormatRule::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _AutoFormatRule::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AutoFormatRule::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _AutoFormatRule::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfa17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _AutoFormatRule::SetFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _AutoFormatRule::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _IconView properties

/////////////////////////////////////////////////////////////////////////////
// _IconView operations

LPDISPATCH _IconView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _IconView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _IconView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _IconView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _IconView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _IconView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _IconView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _IconView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _IconView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _IconView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _IconView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _IconView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _IconView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _IconView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _IconView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _IconView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _IconView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _IconView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _IconView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _IconView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _IconView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _IconView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _IconView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _IconView::GetSortFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb5a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _IconView::GetIconViewType()
{
	long result;
	InvokeHelper(0xfb6a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _IconView::SetIconViewType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _IconView::GetIconPlacement()
{
	long result;
	InvokeHelper(0xfb6b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _IconView::SetIconPlacement(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _CardView properties

/////////////////////////////////////////////////////////////////////////////
// _CardView operations

LPDISPATCH _CardView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CardView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CardView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CardView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _CardView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _CardView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _CardView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _CardView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _CardView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _CardView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CardView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _CardView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CardView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _CardView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CardView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _CardView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _CardView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _CardView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _CardView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CardView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _CardView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _CardView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CardView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _CardView::GetSortFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb5a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _CardView::GetAllowInCellEditing()
{
	BOOL result;
	InvokeHelper(0xfb5f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CardView::SetAllowInCellEditing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _CardView::GetShowEmptyFields()
{
	BOOL result;
	InvokeHelper(0xfb76, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CardView::SetShowEmptyFields(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb76, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _CardView::GetWidth()
{
	long result;
	InvokeHelper(0x2117, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CardView::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2117, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _CardView::GetMultiLineFieldHeight()
{
	long result;
	InvokeHelper(0xfb6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CardView::SetMultiLineFieldHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _CardView::GetViewFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb55, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CardView::GetHeadingsFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CardView::GetBodyFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb7a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CardView::GetAutoFormatRules()
{
	LPDISPATCH result;
	InvokeHelper(0xfa3b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _CalendarView properties

/////////////////////////////////////////////////////////////////////////////
// _CalendarView operations

LPDISPATCH _CalendarView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CalendarView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _CalendarView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _CalendarView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _CalendarView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _CalendarView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _CalendarView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _CalendarView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _CalendarView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _CalendarView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _CalendarView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _CalendarView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _CalendarView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _CalendarView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _CalendarView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _CalendarView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _CalendarView::GetStartField()
{
	CString result;
	InvokeHelper(0x2101, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetStartField(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2101, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _CalendarView::GetEndField()
{
	CString result;
	InvokeHelper(0xfb7b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetEndField(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _CalendarView::GetCalendarViewMode()
{
	long result;
	InvokeHelper(0xfb77, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetCalendarViewMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb77, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _CalendarView::GetDayWeekTimeScale()
{
	long result;
	InvokeHelper(0xfb6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetDayWeekTimeScale(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb6d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _CalendarView::GetMonthShowEndTime()
{
	BOOL result;
	InvokeHelper(0xfb71, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetMonthShowEndTime(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb71, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _CalendarView::GetBoldDatesWithItems()
{
	BOOL result;
	InvokeHelper(0xfb73, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetBoldDatesWithItems(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb73, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _CalendarView::GetDayWeekTimeFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb7c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarView::GetDayWeekFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb7d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarView::GetMonthFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb7f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarView::GetAutoFormatRules()
{
	LPDISPATCH result;
	InvokeHelper(0xfa3b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CalendarView::GetDaysInMultiDayMode()
{
	long result;
	InvokeHelper(0xfb82, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetDaysInMultiDayMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb82, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _CalendarView::GetDisplayedDates()
{
	VARIANT result;
	InvokeHelper(0xfc07, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

BOOL _CalendarView::GetBoldSubjects()
{
	BOOL result;
	InvokeHelper(0xfc11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarView::SetBoldSubjects(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _TimelineView properties

/////////////////////////////////////////////////////////////////////////////
// _TimelineView operations

LPDISPATCH _TimelineView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimelineView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimelineView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimelineView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _TimelineView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _TimelineView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _TimelineView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TimelineView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _TimelineView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _TimelineView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _TimelineView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _TimelineView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _TimelineView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _TimelineView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _TimelineView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TimelineView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _TimelineView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _TimelineView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TimelineView::GetStartField()
{
	CString result;
	InvokeHelper(0x2101, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetStartField(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2101, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _TimelineView::GetEndField()
{
	CString result;
	InvokeHelper(0xfb7b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetEndField(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _TimelineView::GetGroupByFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb58, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimelineView::GetDefaultExpandCollapseSetting()
{
	long result;
	InvokeHelper(0xfb80, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetDefaultExpandCollapseSetting(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb80, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TimelineView::GetShowWeekNumbers()
{
	BOOL result;
	InvokeHelper(0xfb75, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetShowWeekNumbers(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb75, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TimelineView::GetMaxLabelWidth()
{
	long result;
	InvokeHelper(0xfa2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetMaxLabelWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TimelineView::GetShowLabelWhenViewingByMonth()
{
	BOOL result;
	InvokeHelper(0xfb74, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetShowLabelWhenViewingByMonth(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb74, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TimelineView::GetUpperScaleFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfa28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimelineView::GetLowerScaleFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfa2c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimelineView::GetItemFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfa2d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimelineView::GetTimelineViewMode()
{
	long result;
	InvokeHelper(0xfb88, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TimelineView::SetTimelineViewMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb88, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _MailModule properties

/////////////////////////////////////////////////////////////////////////////
// _MailModule operations

LPDISPATCH _MailModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MailModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MailModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MailModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MailModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _MailModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MailModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _MailModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MailModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _MailModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationGroups properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationGroups operations

LPDISPATCH _NavigationGroups::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationGroups::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroups::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroups::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationGroups::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroups::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _NavigationGroups::Create(LPCTSTR GroupDisplayName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfbbd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		GroupDisplayName);
	return result;
}

void _NavigationGroups::Delete(LPDISPATCH Group)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbbe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Group);
}

LPDISPATCH _NavigationGroups::GetDefaultNavigationGroup(long DefaultFolderGroup)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbbf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DefaultFolderGroup);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationGroup properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationGroup operations

LPDISPATCH _NavigationGroup::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationGroup::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroup::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroup::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationGroup::GetGroupType()
{
	long result;
	InvokeHelper(0xfbc0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationGroup::GetNavigationFolders()
{
	LPDISPATCH result;
	InvokeHelper(0xfbc1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _NavigationGroup::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _NavigationGroup::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _NavigationGroup::GetPosition()
{
	long result;
	InvokeHelper(0xfbc7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NavigationGroup::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbc7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationFolders properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationFolders operations

LPDISPATCH _NavigationFolders::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationFolders::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolders::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolders::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationFolders::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolders::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _NavigationFolders::Add(LPDISPATCH Folder)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbc2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Folder);
	return result;
}

void _NavigationFolders::Remove(LPDISPATCH RemovableFolder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbc3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 RemovableFolder);
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationFolder properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationFolder operations

LPDISPATCH _NavigationFolder::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationFolder::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolder::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolder::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationFolder::GetFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfbc4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _NavigationFolder::GetIsSelected()
{
	BOOL result;
	InvokeHelper(0xfbc5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NavigationFolder::SetIsSelected(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbc5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _NavigationFolder::GetIsSideBySide()
{
	BOOL result;
	InvokeHelper(0xfbc6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NavigationFolder::SetIsSideBySide(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbc6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _NavigationFolder::GetPosition()
{
	long result;
	InvokeHelper(0xfbc7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NavigationFolder::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbc7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _NavigationFolder::GetIsRemovable()
{
	BOOL result;
	InvokeHelper(0xfbc8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _NavigationFolder::GetDisplayName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _CalendarModule properties

/////////////////////////////////////////////////////////////////////////////
// _CalendarModule operations

LPDISPATCH _CalendarModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CalendarModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CalendarModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _CalendarModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _CalendarModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CalendarModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _CalendarModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _CalendarModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ContactsModule properties

/////////////////////////////////////////////////////////////////////////////
// _ContactsModule operations

LPDISPATCH _ContactsModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ContactsModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactsModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ContactsModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ContactsModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ContactsModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _ContactsModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ContactsModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ContactsModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ContactsModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _ContactsModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TasksModule properties

/////////////////////////////////////////////////////////////////////////////
// _TasksModule operations

LPDISPATCH _TasksModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TasksModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TasksModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TasksModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TasksModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _TasksModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _TasksModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _TasksModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _TasksModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TasksModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _TasksModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _JournalModule properties

/////////////////////////////////////////////////////////////////////////////
// _JournalModule operations

LPDISPATCH _JournalModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _JournalModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _JournalModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _JournalModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _JournalModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _JournalModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _JournalModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _JournalModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _JournalModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _JournalModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NotesModule properties

/////////////////////////////////////////////////////////////////////////////
// _NotesModule operations

LPDISPATCH _NotesModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NotesModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NotesModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NotesModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NotesModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _NotesModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _NotesModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NotesModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _NotesModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NotesModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _NotesModule::GetNavigationGroups()
{
	LPDISPATCH result;
	InvokeHelper(0xfbbc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// NavigationPaneEvents_12 properties

/////////////////////////////////////////////////////////////////////////////
// NavigationPaneEvents_12 operations

void NavigationPaneEvents_12::ModuleSwitch(LPDISPATCH CurrentModule)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbc9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CurrentModule);
}


/////////////////////////////////////////////////////////////////////////////
// NavigationGroupsEvents_12 properties

/////////////////////////////////////////////////////////////////////////////
// NavigationGroupsEvents_12 operations

void NavigationGroupsEvents_12::SelectedChange(LPDISPATCH NavigationFolder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbca, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NavigationFolder);
}

void NavigationGroupsEvents_12::NavigationFolderAdd(LPDISPATCH NavigationFolder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbcb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NavigationFolder);
}

void NavigationGroupsEvents_12::NavigationFolderRemove()
{
	InvokeHelper(0xfbcc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _BusinessCardView properties

/////////////////////////////////////////////////////////////////////////////
// _BusinessCardView operations

LPDISPATCH _BusinessCardView::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _BusinessCardView::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _BusinessCardView::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _BusinessCardView::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _BusinessCardView::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void _BusinessCardView::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _BusinessCardView::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _BusinessCardView::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _BusinessCardView::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _BusinessCardView::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _BusinessCardView::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _BusinessCardView::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _BusinessCardView::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _BusinessCardView::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _BusinessCardView::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _BusinessCardView::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString _BusinessCardView::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _BusinessCardView::GetHeadingsFont()
{
	LPDISPATCH result;
	InvokeHelper(0xfb79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _BusinessCardView::GetSortFields()
{
	LPDISPATCH result;
	InvokeHelper(0xfb5a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _BusinessCardView::GetCardSize()
{
	long result;
	InvokeHelper(0xfbda, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _BusinessCardView::SetCardSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbda, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _FormRegionStartup properties

/////////////////////////////////////////////////////////////////////////////
// _FormRegionStartup operations

VARIANT _FormRegionStartup::GetFormRegionStorage(LPCTSTR FormRegionName, LPDISPATCH Item, long LCID, long FormRegionMode, long FormRegionSize)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0xfb36, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		FormRegionName, Item, LCID, FormRegionMode, FormRegionSize);
	return result;
}

void _FormRegionStartup::BeforeFormRegionShow(LPDISPATCH FormRegion)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FormRegion);
}

VARIANT _FormRegionStartup::GetFormRegionManifest(LPCTSTR FormRegionName, long LCID)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xfc33, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		FormRegionName, LCID);
	return result;
}

VARIANT _FormRegionStartup::GetFormRegionIcon(LPCTSTR FormRegionName, long LCID, long Icon)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0xfc34, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		FormRegionName, LCID, Icon);
	return result;
}
*/
/////////////////////////////////////////////////////////////////////////////
// _DRecipientControl properties

/////////////////////////////////////////////////////////////////////////////
// _DRecipientControl operations
/*
long _DRecipientControl::GetBackColor()
{
	long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DRecipientControl::SetBackColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _DRecipientControl::GetForeColor()
{
	long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DRecipientControl::SetForeColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _DRecipientControl::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DRecipientControl::SetFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _DRecipientControl::GetSpecialEffect()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DRecipientControl::SetSpecialEffect(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _DRecipientControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// _DRecipientControlEvents operations


/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControl properties

/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControl operations


/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControlEvents operations


/////////////////////////////////////////////////////////////////////////////
// OlkControl properties

/////////////////////////////////////////////////////////////////////////////
// OlkControl operations

CString OlkControl::GetItemProperty()
{
	CString result;
	InvokeHelper(0x8001f001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void OlkControl::SetItemProperty(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8001f001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString OlkControl::GetControlProperty()
{
	CString result;
	InvokeHelper(0x8001f003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void OlkControl::SetControlProperty(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8001f003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString OlkControl::GetPossibleValues()
{
	CString result;
	InvokeHelper(0x8001f004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void OlkControl::SetPossibleValues(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8001f004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long OlkControl::GetFormat()
{
	long result;
	InvokeHelper(0x8001f002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OlkControl::SetFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8001f002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL OlkControl::GetEnableAutoLayout()
{
	BOOL result;
	InvokeHelper(0x8001f011, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void OlkControl::SetEnableAutoLayout(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8001f011, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long OlkControl::GetMinimumWidth()
{
	long result;
	InvokeHelper(0x8001f012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OlkControl::SetMinimumWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8001f012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long OlkControl::GetMinimumHeight()
{
	long result;
	InvokeHelper(0x8001f013, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OlkControl::SetMinimumHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8001f013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long OlkControl::GetHorizontalLayout()
{
	long result;
	InvokeHelper(0x8001f014, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OlkControl::SetHorizontalLayout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8001f014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long OlkControl::GetVerticalLayout()
{
	long result;
	InvokeHelper(0x8001f015, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OlkControl::SetVerticalLayout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8001f015, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkTextBox properties

/////////////////////////////////////////////////////////////////////////////
// _OlkTextBox operations

BOOL _OlkTextBox::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTextBox::GetAutoTab()
{
	BOOL result;
	InvokeHelper(0xd9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetAutoTab(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTextBox::GetAutoWordSelect()
{
	BOOL result;
	InvokeHelper(0xda, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetAutoWordSelect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xda, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkTextBox::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTextBox::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkTextBox::GetDragBehavior()
{
	long result;
	InvokeHelper(0xe1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetDragBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTextBox::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkTextBox::GetEnterFieldBehavior()
{
	long result;
	InvokeHelper(0xe0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetEnterFieldBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTextBox::GetEnterKeyBehavior()
{
	BOOL result;
	InvokeHelper(0xfffffde0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetEnterKeyBehavior(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkTextBox::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkTextBox::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkTextBox::GetHideSelection()
{
	BOOL result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetHideSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTextBox::GetIntegralHeight()
{
	BOOL result;
	InvokeHelper(0x25c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetIntegralHeight(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x25c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTextBox::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkTextBox::GetMaxLength()
{
	long result;
	InvokeHelper(0xfffffdeb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetMaxLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdeb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkTextBox::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTextBox::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTextBox::GetMultiLine()
{
	BOOL result;
	InvokeHelper(0xfffffde7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetMultiLine(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _OlkTextBox::GetPasswordChar()
{
	CString result;
	InvokeHelper(0xfffffdea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetPasswordChar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffdea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkTextBox::GetScrollbars()
{
	long result;
	InvokeHelper(0xfffffde9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetScrollbars(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffde9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTextBox::GetSelectionMargin()
{
	BOOL result;
	InvokeHelper(0xdc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetSelectionMargin(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xdc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTextBox::GetTabKeyBehavior()
{
	BOOL result;
	InvokeHelper(0xfffffddf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetTabKeyBehavior(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffddf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _OlkTextBox::GetText()
{
	CString result;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkTextBox::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkTextBox::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

BOOL _OlkTextBox::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkTextBox::GetSelStart()
{
	long result;
	InvokeHelper(0xfffffddd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetSelStart(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffddd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkTextBox::GetSelLength()
{
	long result;
	InvokeHelper(0xfffffddc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::SetSelLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffddc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkTextBox::GetSelText()
{
	CString result;
	InvokeHelper(0xfffffdde, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkTextBox::Cut()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkTextBox::Copy()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkTextBox::Paste()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkTextBox::Clear()
{
	InvokeHelper(0xfffffdd6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OlkTextBoxEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkTextBoxEvents operations

void OlkTextBoxEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTextBoxEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTextBoxEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTextBoxEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTextBoxEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTextBoxEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTextBoxEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkTextBoxEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTextBoxEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkTextBoxEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTextBoxEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTextBoxEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTextBoxEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkLabel properties

/////////////////////////////////////////////////////////////////////////////
// _OlkLabel operations

CString _OlkLabel::GetAccelerator()
{
	CString result;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetAccelerator(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkLabel::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkLabel::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkLabel::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkLabel::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkLabel::GetCaption()
{
	CString result;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkLabel::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkLabel::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkLabel::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _OlkLabel::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkLabel::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkLabel::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkLabel::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkLabel::GetUseHeaderColor()
{
	BOOL result;
	InvokeHelper(0xfc19, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkLabel::SetUseHeaderColor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkLabelEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkLabelEvents operations

void OlkLabelEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkLabelEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkLabelEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkLabelEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkLabelEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkCommandButton properties

/////////////////////////////////////////////////////////////////////////////
// _OlkCommandButton operations

CString _OlkCommandButton::GetAccelerator()
{
	CString result;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetAccelerator(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkCommandButton::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _OlkCommandButton::GetCaption()
{
	CString result;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkCommandButton::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkCommandButton::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OlkCommandButton::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCommandButton::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkCommandButton::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkCommandButton::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkCommandButton::GetPicture()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetPicture(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCommandButton::GetPictureAlignment()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetPictureAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkCommandButton::GetDisplayDropArrow()
{
	BOOL result;
	InvokeHelper(0xfc20, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCommandButton::SetDisplayDropArrow(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkCommandButtonEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkCommandButtonEvents operations

void OlkCommandButtonEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCommandButtonEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCommandButtonEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCommandButtonEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCommandButtonEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCommandButtonEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCommandButtonEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkCommandButtonEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCommandButtonEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkCommandButtonEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCommandButtonEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCommandButtonEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkCheckBox properties

/////////////////////////////////////////////////////////////////////////////
// _OlkCheckBox operations

CString _OlkCheckBox::GetAccelerator()
{
	CString result;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetAccelerator(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkCheckBox::GetAlignment()
{
	long result;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long _OlkCheckBox::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCheckBox::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkCheckBox::GetCaption()
{
	CString result;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkCheckBox::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkCheckBox::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkCheckBox::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _OlkCheckBox::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCheckBox::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkCheckBox::GetTripleState()
{
	BOOL result;
	InvokeHelper(0x2bc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetTripleState(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2bc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

VARIANT _OlkCheckBox::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

BOOL _OlkCheckBox::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCheckBox::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkCheckBoxEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkCheckBoxEvents operations

void OlkCheckBoxEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCheckBoxEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCheckBoxEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCheckBoxEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCheckBoxEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCheckBoxEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCheckBoxEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkCheckBoxEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCheckBoxEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkCheckBoxEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCheckBoxEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCheckBoxEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCheckBoxEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkOptionButton properties

/////////////////////////////////////////////////////////////////////////////
// _OlkOptionButton operations

CString _OlkOptionButton::GetAccelerator()
{
	CString result;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetAccelerator(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffde1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkOptionButton::GetAlignment()
{
	long result;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long _OlkOptionButton::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkOptionButton::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkOptionButton::GetCaption()
{
	CString result;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _OlkOptionButton::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkOptionButton::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkOptionButton::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _OlkOptionButton::GetGroupName()
{
	CString result;
	InvokeHelper(0xfffffde3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetGroupName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfffffde3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _OlkOptionButton::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkOptionButton::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkOptionButton::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

BOOL _OlkOptionButton::GetWordWrap()
{
	BOOL result;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkOptionButton::SetWordWrap(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffde8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkOptionButtonEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkOptionButtonEvents operations

void OlkOptionButtonEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkOptionButtonEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkOptionButtonEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkOptionButtonEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkOptionButtonEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkOptionButtonEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkOptionButtonEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkOptionButtonEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkOptionButtonEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkOptionButtonEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkOptionButtonEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkOptionButtonEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkOptionButtonEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkComboBox properties

/////////////////////////////////////////////////////////////////////////////
// _OlkComboBox operations

BOOL _OlkComboBox::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkComboBox::GetAutoTab()
{
	BOOL result;
	InvokeHelper(0xd9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetAutoTab(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkComboBox::GetAutoWordSelect()
{
	BOOL result;
	InvokeHelper(0xda, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetAutoWordSelect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xda, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkComboBox::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkComboBox::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkComboBox::GetDragBehavior()
{
	long result;
	InvokeHelper(0xe1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetDragBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkComboBox::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkComboBox::GetEnterFieldBehavior()
{
	long result;
	InvokeHelper(0xe0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetEnterFieldBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkComboBox::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkComboBox::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkComboBox::GetHideSelection()
{
	BOOL result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetHideSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkComboBox::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkComboBox::GetMaxLength()
{
	long result;
	InvokeHelper(0xfffffdeb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetMaxLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdeb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkComboBox::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkComboBox::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkComboBox::GetSelectionMargin()
{
	BOOL result;
	InvokeHelper(0xdc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetSelectionMargin(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xdc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkComboBox::GetStyle()
{
	long result;
	InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x134, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkComboBox::GetText()
{
	CString result;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkComboBox::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkComboBox::GetTopIndex()
{
	long result;
	InvokeHelper(0x263, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetTopIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x263, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkComboBox::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long _OlkComboBox::GetListIndex()
{
	long result;
	InvokeHelper(0xfffffdf2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetListIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkComboBox::GetListCount()
{
	long result;
	InvokeHelper(0xfffffded, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _OlkComboBox::GetSelStart()
{
	long result;
	InvokeHelper(0xfffffddd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetSelStart(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffddd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkComboBox::GetSelLength()
{
	long result;
	InvokeHelper(0xfffffddc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkComboBox::SetSelLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffddc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkComboBox::GetSelText()
{
	CString result;
	InvokeHelper(0xfffffdde, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _OlkComboBox::GetItem(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2109, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void _OlkComboBox::SetItem(long Index, LPCTSTR Item)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0xfbde, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Item);
}

void _OlkComboBox::Cut()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkComboBox::Copy()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkComboBox::Paste()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkComboBox::Clear()
{
	InvokeHelper(0xfffffdd6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkComboBox::AddItem(LPCTSTR ItemText, const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xfffffdd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ItemText, &Index);
}

void _OlkComboBox::RemoveItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdd5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void _OlkComboBox::DropDown()
{
	InvokeHelper(0x3e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OlkComboBoxEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkComboBoxEvents operations

void OlkComboBoxEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkComboBoxEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkComboBoxEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkComboBoxEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkComboBoxEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkComboBoxEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkComboBoxEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkComboBoxEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::DropButtonClick()
{
	InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkComboBoxEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkListBox properties

/////////////////////////////////////////////////////////////////////////////
// _OlkListBox operations

long _OlkListBox::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long _OlkListBox::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkListBox::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkListBox::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkListBox::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkListBox::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkListBox::GetMatchEntry()
{
	long result;
	InvokeHelper(0x1f8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetMatchEntry(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkListBox::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkListBox::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkListBox::GetMultiSelect()
{
	long result;
	InvokeHelper(0xfffffdec, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetMultiSelect(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdec, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkListBox::GetText()
{
	CString result;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkListBox::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkListBox::GetTopIndex()
{
	long result;
	InvokeHelper(0x263, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetTopIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x263, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkListBox::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

long _OlkListBox::GetListIndex()
{
	long result;
	InvokeHelper(0xfffffdf2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkListBox::SetListIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkListBox::GetListCount()
{
	long result;
	InvokeHelper(0xfffffded, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _OlkListBox::GetItem(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2109, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void _OlkListBox::SetItem(long Index, LPCTSTR Item)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0xfbde, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Item);
}

BOOL _OlkListBox::GetSelected(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbdf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

void _OlkListBox::SetSelected(long Index, BOOL Selected)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0xfbe0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Selected);
}

void _OlkListBox::Copy()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkListBox::Clear()
{
	InvokeHelper(0xfffffdd6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _OlkListBox::AddItem(LPCTSTR ItemText, const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xfffffdd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ItemText, &Index);
}

void _OlkListBox::RemoveItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdd5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// OlkListBoxEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkListBoxEvents operations

void OlkListBoxEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkListBoxEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkListBoxEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkListBoxEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkListBoxEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkListBoxEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkListBoxEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkListBoxEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkListBoxEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkListBoxEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkListBoxEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkListBoxEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkListBoxEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkInfoBar properties

/////////////////////////////////////////////////////////////////////////////
// _OlkInfoBar operations

LPDISPATCH _OlkInfoBar::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkInfoBar::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkInfoBar::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkInfoBar::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkInfoBarEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkInfoBarEvents operations

void OlkInfoBarEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkInfoBarEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkInfoBarEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkInfoBarEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkInfoBarEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkContactPhoto properties

/////////////////////////////////////////////////////////////////////////////
// _OlkContactPhoto operations

BOOL _OlkContactPhoto::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkContactPhoto::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkContactPhoto::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkContactPhoto::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkContactPhoto::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkContactPhoto::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkContactPhotoEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkContactPhotoEvents operations

void OlkContactPhotoEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkContactPhotoEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkContactPhotoEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkContactPhotoEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkContactPhotoEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkContactPhotoEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkContactPhotoEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkContactPhotoEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkContactPhotoEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkContactPhotoEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkContactPhotoEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkBusinessCardControl properties

/////////////////////////////////////////////////////////////////////////////
// _OlkBusinessCardControl operations

LPDISPATCH _OlkBusinessCardControl::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkBusinessCardControl::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkBusinessCardControl::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkBusinessCardControl::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkBusinessCardControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkBusinessCardControlEvents operations

void OlkBusinessCardControlEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkBusinessCardControlEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkBusinessCardControlEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkBusinessCardControlEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkBusinessCardControlEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkPageControl properties

/////////////////////////////////////////////////////////////////////////////
// _OlkPageControl operations

long _OlkPageControl::GetPage()
{
	long result;
	InvokeHelper(0xfbd1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkPageControl::SetPage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkPageControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkPageControlEvents operations


/////////////////////////////////////////////////////////////////////////////
// _OlkDateControl properties

/////////////////////////////////////////////////////////////////////////////
// _OlkDateControl operations

BOOL _OlkDateControl::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkDateControl::GetAutoWordSelect()
{
	BOOL result;
	InvokeHelper(0xda, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetAutoWordSelect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xda, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkDateControl::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkDateControl::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _OlkDateControl::GetDate()
{
	DATE result;
	InvokeHelper(0xfbd2, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfbd2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkDateControl::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkDateControl::GetEnterFieldBehavior()
{
	long result;
	InvokeHelper(0xe0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetEnterFieldBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkDateControl::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkDateControl::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkDateControl::GetHideSelection()
{
	BOOL result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetHideSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkDateControl::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkDateControl::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkDateControl::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkDateControl::GetShowNoneButton()
{
	BOOL result;
	InvokeHelper(0xfbd3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetShowNoneButton(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbd3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _OlkDateControl::GetText()
{
	CString result;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkDateControl::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkDateControl::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkDateControl::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

void _OlkDateControl::DropDown()
{
	InvokeHelper(0x3e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OlkDateControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkDateControlEvents operations

void OlkDateControlEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkDateControlEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkDateControlEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkDateControlEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkDateControlEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkDateControlEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkDateControlEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkDateControlEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::DropButtonClick()
{
	InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkDateControlEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkTimeControl properties

/////////////////////////////////////////////////////////////////////////////
// _OlkTimeControl operations

BOOL _OlkTimeControl::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTimeControl::GetAutoWordSelect()
{
	BOOL result;
	InvokeHelper(0xda, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetAutoWordSelect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xda, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkTimeControl::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTimeControl::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTimeControl::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _OlkTimeControl::GetEnterFieldBehavior()
{
	long result;
	InvokeHelper(0xe0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetEnterFieldBehavior(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _OlkTimeControl::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkTimeControl::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkTimeControl::GetHideSelection()
{
	BOOL result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetHideSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTimeControl::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkTimeControl::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTimeControl::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _OlkTimeControl::GetReferenceTime()
{
	DATE result;
	InvokeHelper(0xfbd4, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetReferenceTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfbd4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTimeControl::GetStyle()
{
	long result;
	InvokeHelper(0xfbd5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbd5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _OlkTimeControl::GetTime()
{
	DATE result;
	InvokeHelper(0xfbd2, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfbd2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _OlkTimeControl::GetText()
{
	CString result;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _OlkTimeControl::GetTextAlign()
{
	long result;
	InvokeHelper(0x2714, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetTextAlign(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2714, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkTimeControl::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

DATE _OlkTimeControl::GetIntervalTime()
{
	DATE result;
	InvokeHelper(0xfbdb, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _OlkTimeControl::SetIntervalTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfbdb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _OlkTimeControl::DropDown()
{
	InvokeHelper(0x3e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OlkTimeControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkTimeControlEvents operations

void OlkTimeControlEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeControlEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeControlEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeControlEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkTimeControlEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTimeControlEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkTimeControlEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTimeControlEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::DropButtonClick()
{
	InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeControlEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkCategory properties

/////////////////////////////////////////////////////////////////////////////
// _OlkCategory operations

BOOL _OlkCategory::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkCategory::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCategory::GetBackStyle()
{
	long result;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetBackStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkCategory::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long _OlkCategory::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _OlkCategory::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkCategory::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkCategory::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkCategoryEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkCategoryEvents operations

void OlkCategoryEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCategoryEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCategoryEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCategoryEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCategoryEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkCategoryEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkCategoryEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkCategoryEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCategoryEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkCategoryEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkCategoryEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _OlkFrameHeader properties

/////////////////////////////////////////////////////////////////////////////
// _OlkFrameHeader operations

long _OlkFrameHeader::GetAlignment()
{
	long result;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2c6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _OlkFrameHeader::GetCaption()
{
	CString result;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _OlkFrameHeader::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

unsigned long _OlkFrameHeader::GetForeColor()
{
	unsigned long result;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _OlkFrameHeader::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkFrameHeader::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkFrameHeader::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkFrameHeader::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OlkFrameHeaderEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkFrameHeaderEvents operations


/////////////////////////////////////////////////////////////////////////////
// _OlkSenderPhoto properties

/////////////////////////////////////////////////////////////////////////////
// _OlkSenderPhoto operations

BOOL _OlkSenderPhoto::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkSenderPhoto::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkSenderPhoto::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkSenderPhoto::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkSenderPhoto::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkSenderPhoto::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkSenderPhoto::GetPreferredWidth()
{
	long result;
	InvokeHelper(0xfbe5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _OlkSenderPhoto::GetPreferredHeight()
{
	long result;
	InvokeHelper(0xfbe6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// OlkSenderPhotoEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkSenderPhotoEvents operations

void OlkSenderPhotoEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkSenderPhotoEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkSenderPhotoEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkSenderPhotoEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkSenderPhotoEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkSenderPhotoEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _TimeZone properties

/////////////////////////////////////////////////////////////////////////////
// _TimeZone operations

LPDISPATCH _TimeZone::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimeZone::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimeZone::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimeZone::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _TimeZone::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TimeZone::GetDaylightDesignation()
{
	CString result;
	InvokeHelper(0xfc2b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _TimeZone::GetStandardDesignation()
{
	CString result;
	InvokeHelper(0xfc2c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _TimeZone::GetBias()
{
	long result;
	InvokeHelper(0xfc21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _TimeZone::GetStandardBias()
{
	long result;
	InvokeHelper(0xfc22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _TimeZone::GetDaylightBias()
{
	long result;
	InvokeHelper(0xfc23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

DATE _TimeZone::GetStandardDate()
{
	DATE result;
	InvokeHelper(0xfc24, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

DATE _TimeZone::GetDaylightDate()
{
	DATE result;
	InvokeHelper(0xfc25, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _TimeZone::GetId()
{
	CString result;
	InvokeHelper(0xfc32, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}
/////////////////////////////////////////////////////////////////////////////
// Recipient properties

/////////////////////////////////////////////////////////////////////////////
// Recipient operations

LPDISPATCH Recipient::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipient::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Recipient::GetAddress()
{
	CString result;
	InvokeHelper(0x3003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetAddressEntry()
{
	LPDISPATCH result;
	InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Recipient::SetRefAddressEntry(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x79, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString Recipient::GetAutoResponse()
{
	CString result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Recipient::SetAutoResponse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long Recipient::GetDisplayType()
{
	long result;
	InvokeHelper(0x3900, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Recipient::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Recipient::GetIndex()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Recipient::GetMeetingResponseStatus()
{
	long result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Recipient::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL Recipient::GetResolved()
{
	BOOL result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long Recipient::GetTrackingStatus()
{
	long result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Recipient::SetTrackingStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE Recipient::GetTrackingStatusTime()
{
	DATE result;
	InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void Recipient::SetTrackingStatusTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x77, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Recipient::GetType()
{
	long result;
	InvokeHelper(0xc15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Recipient::SetType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void Recipient::Delete()
{
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString Recipient::FreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat)
{
	CString result;
	static BYTE parms[] =
		VTS_DATE VTS_I4 VTS_VARIANT;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Start, MinPerChar, &CompleteFormat);
	return result;
}

BOOL Recipient::Resolve()
{
	BOOL result;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipient::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// AddressEntry properties

/////////////////////////////////////////////////////////////////////////////
// AddressEntry operations

LPDISPATCH AddressEntry::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressEntry::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntry::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntry::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString AddressEntry::GetAddress()
{
	CString result;
	InvokeHelper(0x3003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void AddressEntry::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long AddressEntry::GetDisplayType()
{
	long result;
	InvokeHelper(0x3900, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString AddressEntry::GetId()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString AddressEntry::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void AddressEntry::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString AddressEntry::GetType()
{
	CString result;
	InvokeHelper(0x3002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void AddressEntry::SetType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void AddressEntry::Delete()
{
	InvokeHelper(0x302, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void AddressEntry::Details(const VARIANT& HWnd)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x301, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &HWnd);
}

CString AddressEntry::GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat)
{
	CString result;
	static BYTE parms[] =
		VTS_DATE VTS_I4 VTS_VARIANT;
	InvokeHelper(0x306, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Start, MinPerChar, &CompleteFormat);
	return result;
}

void AddressEntry::Update(const VARIANT& MakePermanent, const VARIANT& Refresh)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x300, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &MakePermanent, &Refresh);
}

LPDISPATCH AddressEntry::GetContact()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntry::GetExchangeUser()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressEntry::GetAddressEntryUserType()
{
	long result;
	InvokeHelper(0xfaf2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntry::GetExchangeDistributionList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaef, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntry::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// AddressEntries properties

/////////////////////////////////////////////////////////////////////////////
// AddressEntries operations

LPDISPATCH AddressEntries::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressEntries::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressEntries::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH AddressEntries::Add(LPCTSTR Type, const VARIANT& Name, const VARIANT& Address)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, &Name, &Address);
	return result;
}

LPDISPATCH AddressEntries::GetFirst()
{
	LPDISPATCH result;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::GetLast()
{
	LPDISPATCH result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressEntries::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void AddressEntries::Sort(const VARIANT& Property_, const VARIANT& Order)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Property_, &Order);
}

*/
/*
/////////////////////////////////////////////////////////////////////////////
// Actions properties

/////////////////////////////////////////////////////////////////////////////
// Actions operations

LPDISPATCH Actions::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Actions::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Actions::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Actions::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Actions::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Actions::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Actions::Add()
{
	LPDISPATCH result;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Actions::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// Action properties

/////////////////////////////////////////////////////////////////////////////
// Action operations

LPDISPATCH Action::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Action::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Action::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Action::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Action::GetCopyLike()
{
	long result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Action::SetCopyLike(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL Action::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Action::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString Action::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Action::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Action::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Action::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Action::GetPrefix()
{
	CString result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Action::SetPrefix(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long Action::GetReplyStyle()
{
	long result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Action::SetReplyStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Action::GetResponseStyle()
{
	long result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Action::SetResponseStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Action::GetShowOn()
{
	long result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Action::SetShowOn(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void Action::Delete()
{
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Action::Execute()
{
	LPDISPATCH result;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}
*/
/*
/////////////////////////////////////////////////////////////////////////////
// Attachments properties

/////////////////////////////////////////////////////////////////////////////
// Attachments operations

LPDISPATCH Attachments::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachments::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachments::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachments::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Attachments::Add(const VARIANT& Source, const VARIANT& Type, const VARIANT& Position, const VARIANT& DisplayName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Source, &Type, &Position, &DisplayName);
	return result;
}

void Attachments::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// Attachment properties

/////////////////////////////////////////////////////////////////////////////
// Attachment operations

LPDISPATCH Attachment::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachment::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachment::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Attachment::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Attachment::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Attachment::SetDisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString Attachment::GetFileName()
{
	CString result;
	InvokeHelper(0x3704, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Attachment::GetIndex()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString Attachment::GetPathName()
{
	CString result;
	InvokeHelper(0x3708, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Attachment::GetPosition()
{
	long result;
	InvokeHelper(0x72, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Attachment::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x72, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Attachment::GetType()
{
	long result;
	InvokeHelper(0x3705, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Attachment::Delete()
{
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Attachment::SaveAsFile(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

LPDISPATCH Attachment::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Attachment::GetSize()
{
	long result;
	InvokeHelper(0xfbe9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Attachment::GetBlockLevel()
{
	long result;
	InvokeHelper(0xfc18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
*/
/*
/////////////////////////////////////////////////////////////////////////////
// _PropertyAccessor properties

/////////////////////////////////////////////////////////////////////////////
// _PropertyAccessor operations

LPDISPATCH _PropertyAccessor::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _PropertyAccessor::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PropertyAccessor::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PropertyAccessor::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT _PropertyAccessor::GetProperty_(LPCTSTR SchemaName)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfafb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		SchemaName);
	return result;
}

void _PropertyAccessor::SetProperty_(LPCTSTR SchemaName, const VARIANT& Value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xfafc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SchemaName, &Value);
}

VARIANT _PropertyAccessor::GetProperties(const VARIANT& SchemaNames)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfafe, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&SchemaNames);
	return result;
}

VARIANT _PropertyAccessor::SetProperties(const VARIANT& SchemaNames, const VARIANT& Values)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfaff, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&SchemaNames, &Values);
	return result;
}

DATE _PropertyAccessor::UTCToLocalTime(DATE Value)
{
	DATE result;
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfb00, DISPATCH_METHOD, VT_DATE, (void*)&result, parms,
		Value);
	return result;
}

DATE _PropertyAccessor::LocalTimeToUTC(DATE Value)
{
	DATE result;
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfb01, DISPATCH_METHOD, VT_DATE, (void*)&result, parms,
		Value);
	return result;
}

VARIANT _PropertyAccessor::StringToBinary(LPCTSTR Value)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb02, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Value);
	return result;
}

CString _PropertyAccessor::BinaryToString(const VARIANT& Value)
{
	CString result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb03, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		&Value);
	return result;
}

void _PropertyAccessor::DeleteProperty(LPCTSTR SchemaName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb91, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SchemaName);
}

VARIANT _PropertyAccessor::DeleteProperties(VARIANT* SchemaNames)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0xfb92, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		SchemaNames);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// FormDescription properties

/////////////////////////////////////////////////////////////////////////////
// FormDescription operations

LPDISPATCH FormDescription::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long FormDescription::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH FormDescription::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH FormDescription::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString FormDescription::GetCategory()
{
	CString result;
	InvokeHelper(0x3304, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetCategory(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3304, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetCategorySub()
{
	CString result;
	InvokeHelper(0x3305, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetCategorySub(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3305, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetComment()
{
	CString result;
	InvokeHelper(0x3004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetComment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetContactName()
{
	CString result;
	InvokeHelper(0x3303, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetContactName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3303, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetDisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL FormDescription::GetHidden()
{
	BOOL result;
	InvokeHelper(0x3307, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void FormDescription::SetHidden(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3307, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString FormDescription::GetIcon()
{
	CString result;
	InvokeHelper(0xffd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetIcon(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xffd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL FormDescription::GetLocked()
{
	BOOL result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void FormDescription::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString FormDescription::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString FormDescription::GetMiniIcon()
{
	CString result;
	InvokeHelper(0xffc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetMiniIcon(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xffc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetName()
{
	CString result;
	InvokeHelper(0xf01d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf01d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString FormDescription::GetNumber()
{
	CString result;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL FormDescription::GetOneOff()
{
	BOOL result;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void FormDescription::SetOneOff(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString FormDescription::GetScriptText()
{
	CString result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString FormDescription::GetTemplate()
{
	CString result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetTemplate(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL FormDescription::GetUseWordMail()
{
	BOOL result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void FormDescription::SetUseWordMail(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString FormDescription::GetVersion()
{
	CString result;
	InvokeHelper(0x3301, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void FormDescription::SetVersion(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3301, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void FormDescription::PublishForm(long Registry, const VARIANT& Folder)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Registry, &Folder);
}


/////////////////////////////////////////////////////////////////////////////
// _Inspector properties

/////////////////////////////////////////////////////////////////////////////
// _Inspector operations

LPDISPATCH _Inspector::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Inspector::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x2100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::GetCurrentItem()
{
	LPDISPATCH result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Inspector::GetEditorType()
{
	long result;
	InvokeHelper(0x2110, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::GetModifiedFormPages()
{
	LPDISPATCH result;
	InvokeHelper(0x2106, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Inspector::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2103, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

void _Inspector::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x2104, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

void _Inspector::HideFormPage(LPCTSTR PageName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2108, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PageName);
}

BOOL _Inspector::IsWordMail()
{
	BOOL result;
	InvokeHelper(0x2105, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Inspector::SetCurrentFormPage(LPCTSTR PageName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x210c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PageName);
}

void _Inspector::ShowFormPage(LPCTSTR PageName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2109, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PageName);
}

LPDISPATCH _Inspector::GetWordEditor()
{
	LPDISPATCH result;
	InvokeHelper(0x210f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Inspector::GetCaption()
{
	CString result;
	InvokeHelper(0x2111, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Inspector::GetHeight()
{
	long result;
	InvokeHelper(0x2114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Inspector::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Inspector::GetLeft()
{
	long result;
	InvokeHelper(0x2115, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Inspector::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2115, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Inspector::GetTop()
{
	long result;
	InvokeHelper(0x2116, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Inspector::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Inspector::GetWidth()
{
	long result;
	InvokeHelper(0x2117, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Inspector::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2117, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Inspector::GetWindowState()
{
	long result;
	InvokeHelper(0x2112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Inspector::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Inspector::Activate()
{
	InvokeHelper(0x2113, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Inspector::SetControlItemProperty(LPDISPATCH Control, LPCTSTR PropertyName)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0xfac9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Control, PropertyName);
}

LPDISPATCH _Inspector::NewFormRegion()
{
	LPDISPATCH result;
	InvokeHelper(0xfbed, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspector::OpenFormRegion(LPCTSTR Path)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfbff, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Path);
	return result;
}

void _Inspector::SaveFormRegion(LPDISPATCH Page, LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0xfc00, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Page, FileName);
}


/////////////////////////////////////////////////////////////////////////////
// UserProperties properties

/////////////////////////////////////////////////////////////////////////////
// UserProperties operations

LPDISPATCH UserProperties::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long UserProperties::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH UserProperties::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH UserProperties::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long UserProperties::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH UserProperties::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH UserProperties::Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, Type, &AddToFolderFields, &DisplayFormat);
	return result;
}

LPDISPATCH UserProperties::Find(LPCTSTR Name, const VARIANT& Custom)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Custom);
	return result;
}

void UserProperties::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// UserProperty properties

/////////////////////////////////////////////////////////////////////////////
// UserProperty operations

LPDISPATCH UserProperty::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long UserProperty::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH UserProperty::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH UserProperty::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString UserProperty::GetFormula()
{
	CString result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void UserProperty::SetFormula(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString UserProperty::GetName()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long UserProperty::GetType()
{
	long result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString UserProperty::GetValidationFormula()
{
	CString result;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void UserProperty::SetValidationFormula(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString UserProperty::GetValidationText()
{
	CString result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void UserProperty::SetValidationText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

VARIANT UserProperty::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void UserProperty::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

void UserProperty::Delete()
{
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

*/

/*
/////////////////////////////////////////////////////////////////////////////
// _Folders properties

/////////////////////////////////////////////////////////////////////////////
// _Folders operations

LPDISPATCH _Folders::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Folders::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Folders::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Folders::Add(LPCTSTR Name, const VARIANT& Type)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Type);
	return result;
}

LPDISPATCH _Folders::GetFirst()
{
	LPDISPATCH result;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::GetLast()
{
	LPDISPATCH result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Folders::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Folders::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

*/


/*
/////////////////////////////////////////////////////////////////////////////
// Panes properties

/////////////////////////////////////////////////////////////////////////////
// Panes operations

LPDISPATCH Panes::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Panes::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Panes::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Panes::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Panes::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Panes::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

*/
/*
/////////////////////////////////////////////////////////////////////////////
// _NavigationPane properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationPane operations

LPDISPATCH _NavigationPane::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationPane::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationPane::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationPane::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _NavigationPane::GetIsCollapsed()
{
	BOOL result;
	InvokeHelper(0xfbb4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NavigationPane::SetIsCollapsed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbb4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _NavigationPane::GetCurrentModule()
{
	LPDISPATCH result;
	InvokeHelper(0xfbb5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _NavigationPane::SetRefCurrentModule(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbb5, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

long _NavigationPane::GetDisplayedModuleCount()
{
	long result;
	InvokeHelper(0xfbb6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NavigationPane::SetDisplayedModuleCount(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbb6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _NavigationPane::GetModules()
{
	LPDISPATCH result;
	InvokeHelper(0xfbb7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationModule properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationModule operations

LPDISPATCH _NavigationModule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationModule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationModule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationModule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationModule::GetNavigationModuleType()
{
	long result;
	InvokeHelper(0xfbb9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _NavigationModule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _NavigationModule::GetPosition()
{
	long result;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _NavigationModule::SetPosition(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _NavigationModule::GetVisible()
{
	BOOL result;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NavigationModule::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbbb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _NavigationModules properties

/////////////////////////////////////////////////////////////////////////////
// _NavigationModules operations

LPDISPATCH _NavigationModules::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationModules::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationModules::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationModules::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NavigationModules::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NavigationModules::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _NavigationModules::GetNavigationModule(long ModuleType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbb8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ModuleType);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// View properties

/////////////////////////////////////////////////////////////////////////////
// View operations

LPDISPATCH View::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long View::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH View::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH View::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void View::Apply()
{
	InvokeHelper(0x197, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH View::Copy(LPCTSTR Name, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, SaveOption);
	return result;
}

void View::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void View::Reset()
{
	InvokeHelper(0xfa44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void View::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString View::GetLanguage()
{
	CString result;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void View::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL View::GetLockUserChanges()
{
	BOOL result;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void View::SetLockUserChanges(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfa40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString View::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void View::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long View::GetSaveOption()
{
	long result;
	InvokeHelper(0xfa3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL View::GetStandard()
{
	BOOL result;
	InvokeHelper(0xfa3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long View::GetViewType()
{
	long result;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString View::GetXml()
{
	CString result;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void View::SetXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfa3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void View::GoToDate(DATE Date)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfa36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Date);
}

CString View::GetFilter()
{
	CString result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void View::SetFilter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _Views properties

/////////////////////////////////////////////////////////////////////////////
// _Views operations

LPDISPATCH _Views::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Views::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Views::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Views::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Views::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Views::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Views::Add(LPCTSTR Name, long ViewType, long SaveOption)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, ViewType, SaveOption);
	return result;
}

void _Views::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// _Store properties

/////////////////////////////////////////////////////////////////////////////
// _Store operations

LPDISPATCH _Store::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Store::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Store::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Store::GetStoreID()
{
	CString result;
	InvokeHelper(0x3108, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Store::GetExchangeStoreType()
{
	long result;
	InvokeHelper(0xfada, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Store::GetFilePath()
{
	CString result;
	InvokeHelper(0xfadd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Store::GetIsCachedExchange()
{
	BOOL result;
	InvokeHelper(0xfade, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Store::GetIsDataFileStore()
{
	BOOL result;
	InvokeHelper(0xfadf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetRootFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfadb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetSearchFolders()
{
	LPDISPATCH result;
	InvokeHelper(0xfadc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Store::GetIsOpen()
{
	BOOL result;
	InvokeHelper(0xfae0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetRules()
{
	LPDISPATCH result;
	InvokeHelper(0xfb09, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Store::GetSpecialFolder(long FolderType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2108, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FolderType);
	return result;
}

BOOL _Store::GetIsInstantSearchEnabled()
{
	BOOL result;
	InvokeHelper(0xfc14, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Rules properties

/////////////////////////////////////////////////////////////////////////////
// _Rules operations

LPDISPATCH _Rules::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Rules::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rules::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rules::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Rules::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rules::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Rules::Create(LPCTSTR Name, long RuleType)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0xfb0a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, RuleType);
	return result;
}

void _Rules::Save(const VARIANT& ShowProgress)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &ShowProgress);
}

void _Rules::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

BOOL _Rules::GetIsRssRulesProcessingEnabled()
{
	BOOL result;
	InvokeHelper(0xfc15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Rules::SetIsRssRulesProcessingEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfc15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _Rule properties

/////////////////////////////////////////////////////////////////////////////
// _Rule operations

LPDISPATCH _Rule::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Rule::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rule::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rule::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Rule::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Rule::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Rule::GetExecutionOrder()
{
	long result;
	InvokeHelper(0xfb0b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Rule::SetExecutionOrder(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb0b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Rule::GetRuleType()
{
	long result;
	InvokeHelper(0xfb0c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Rule::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Rule::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Rule::GetIsLocalRule()
{
	BOOL result;
	InvokeHelper(0xfb0d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Rule::Execute(const VARIANT& ShowProgress, const VARIANT& Folder, const VARIANT& IncludeSubfolders, const VARIANT& RuleExecuteOption)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xfb0e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &ShowProgress, &Folder, &IncludeSubfolders, &RuleExecuteOption);
}

LPDISPATCH _Rule::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xfb10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rule::GetConditions()
{
	LPDISPATCH result;
	InvokeHelper(0xfb29, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Rule::GetExceptions()
{
	LPDISPATCH result;
	InvokeHelper(0xfb33, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _RuleActions properties

/////////////////////////////////////////////////////////////////////////////
// _RuleActions operations

LPDISPATCH _RuleActions::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleActions::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleActions::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::Item(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH _RuleActions::GetCopyToFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfb12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetDeletePermanently()
{
	LPDISPATCH result;
	InvokeHelper(0xfb13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetDelete()
{
	LPDISPATCH result;
	InvokeHelper(0xf045, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetDesktopAlert()
{
	LPDISPATCH result;
	InvokeHelper(0xfb17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetNotifyDelivery()
{
	LPDISPATCH result;
	InvokeHelper(0xfb16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetNotifyRead()
{
	LPDISPATCH result;
	InvokeHelper(0xfb15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetStop()
{
	LPDISPATCH result;
	InvokeHelper(0xfb14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetMoveToFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfb18, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetCc()
{
	LPDISPATCH result;
	InvokeHelper(0xfb19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetForward()
{
	LPDISPATCH result;
	InvokeHelper(0xfb1a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetForwardAsAttachment()
{
	LPDISPATCH result;
	InvokeHelper(0xfb1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetRedirect()
{
	LPDISPATCH result;
	InvokeHelper(0xfb1c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetAssignToCategory()
{
	LPDISPATCH result;
	InvokeHelper(0xfb22, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetPlaySound()
{
	LPDISPATCH result;
	InvokeHelper(0xfb23, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetMarkAsTask()
{
	LPDISPATCH result;
	InvokeHelper(0xfb26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetNewItemAlert()
{
	LPDISPATCH result;
	InvokeHelper(0xfb28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleActions::GetClearCategories()
{
	LPDISPATCH result;
	InvokeHelper(0xfc12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _RuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _RuleAction operations

LPDISPATCH _RuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _RuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _RuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _RuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _MoveOrCopyRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _MoveOrCopyRuleAction operations

LPDISPATCH _MoveOrCopyRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MoveOrCopyRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MoveOrCopyRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MoveOrCopyRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _MoveOrCopyRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MoveOrCopyRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _MoveOrCopyRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MoveOrCopyRuleAction::GetFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfb11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MoveOrCopyRuleAction::SetFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _SendRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _SendRuleAction operations

LPDISPATCH _SendRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SendRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SendRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SendRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _SendRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SendRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _SendRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SendRuleAction::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Recipients properties

/////////////////////////////////////////////////////////////////////////////
// Recipients operations

LPDISPATCH Recipients::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipients::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Recipients::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Recipients::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Recipients::Add(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

void Recipients::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

BOOL Recipients::ResolveAll()
{
	BOOL result;
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _AssignToCategoryRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _AssignToCategoryRuleAction operations

LPDISPATCH _AssignToCategoryRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AssignToCategoryRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AssignToCategoryRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AssignToCategoryRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _AssignToCategoryRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AssignToCategoryRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _AssignToCategoryRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _AssignToCategoryRuleAction::GetCategories()
{
	VARIANT result;
	InvokeHelper(0xfb21, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _AssignToCategoryRuleAction::SetCategories(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _PlaySoundRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _PlaySoundRuleAction operations

LPDISPATCH _PlaySoundRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _PlaySoundRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PlaySoundRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PlaySoundRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _PlaySoundRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PlaySoundRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _PlaySoundRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _PlaySoundRuleAction::GetFilePath()
{
	CString result;
	InvokeHelper(0xfadd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PlaySoundRuleAction::SetFilePath(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfadd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _MarkAsTaskRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _MarkAsTaskRuleAction operations

LPDISPATCH _MarkAsTaskRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MarkAsTaskRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MarkAsTaskRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MarkAsTaskRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _MarkAsTaskRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MarkAsTaskRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _MarkAsTaskRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MarkAsTaskRuleAction::GetFlagTo()
{
	CString result;
	InvokeHelper(0xfb24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MarkAsTaskRuleAction::SetFlagTo(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _MarkAsTaskRuleAction::GetMarkInterval()
{
	long result;
	InvokeHelper(0xfb25, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MarkAsTaskRuleAction::SetMarkInterval(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _NewItemAlertRuleAction properties

/////////////////////////////////////////////////////////////////////////////
// _NewItemAlertRuleAction operations

LPDISPATCH _NewItemAlertRuleAction::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _NewItemAlertRuleAction::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NewItemAlertRuleAction::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _NewItemAlertRuleAction::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _NewItemAlertRuleAction::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _NewItemAlertRuleAction::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _NewItemAlertRuleAction::GetActionType()
{
	long result;
	InvokeHelper(0xfb0f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _NewItemAlertRuleAction::GetText()
{
	CString result;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _NewItemAlertRuleAction::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _RuleConditions properties

/////////////////////////////////////////////////////////////////////////////
// _RuleConditions operations

LPDISPATCH _RuleConditions::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleConditions::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleConditions::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::Item(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH _RuleConditions::GetCc()
{
	LPDISPATCH result;
	InvokeHelper(0xfb2c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetHasAttachment()
{
	LPDISPATCH result;
	InvokeHelper(0xfb2d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetImportance()
{
	LPDISPATCH result;
	InvokeHelper(0xfb2b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetMeetingInviteOrUpdate()
{
	LPDISPATCH result;
	InvokeHelper(0xfb2e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetNotTo()
{
	LPDISPATCH result;
	InvokeHelper(0xfb2f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetOnlyToMe()
{
	LPDISPATCH result;
	InvokeHelper(0xfb30, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetToMe()
{
	LPDISPATCH result;
	InvokeHelper(0xfb31, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetToOrCc()
{
	LPDISPATCH result;
	InvokeHelper(0xfb32, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetBody()
{
	LPDISPATCH result;
	InvokeHelper(0xfb48, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetBodyOrSubject()
{
	LPDISPATCH result;
	InvokeHelper(0xfb49, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetCategory()
{
	LPDISPATCH result;
	InvokeHelper(0xfb4a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetFormName()
{
	LPDISPATCH result;
	InvokeHelper(0xfb4b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetFrom()
{
	LPDISPATCH result;
	InvokeHelper(0xfb4c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetMessageHeader()
{
	LPDISPATCH result;
	InvokeHelper(0xfb4e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetRecipientAddress()
{
	LPDISPATCH result;
	InvokeHelper(0xfb52, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetSenderAddress()
{
	LPDISPATCH result;
	InvokeHelper(0xfb4f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetSenderInAddressList()
{
	LPDISPATCH result;
	InvokeHelper(0xfb50, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetSubject()
{
	LPDISPATCH result;
	InvokeHelper(0xfb51, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetSentTo()
{
	LPDISPATCH result;
	InvokeHelper(0xfb53, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetOnLocalMachine()
{
	LPDISPATCH result;
	InvokeHelper(0xfb54, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetOnOtherMachine()
{
	LPDISPATCH result;
	InvokeHelper(0xfbd9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetAnyCategory()
{
	LPDISPATCH result;
	InvokeHelper(0xfc13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetFromAnyRSSFeed()
{
	LPDISPATCH result;
	InvokeHelper(0xfc16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleConditions::GetFromRssFeed()
{
	LPDISPATCH result;
	InvokeHelper(0xfc17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _RuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _RuleCondition operations

LPDISPATCH _RuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _RuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _RuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _RuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ImportanceRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _ImportanceRuleCondition operations

LPDISPATCH _ImportanceRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ImportanceRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ImportanceRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ImportanceRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _ImportanceRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ImportanceRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ImportanceRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _ImportanceRuleCondition::GetImportance()
{
	long result;
	InvokeHelper(0xfb2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ImportanceRuleCondition::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _AccountRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _AccountRuleCondition operations

LPDISPATCH _AccountRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AccountRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AccountRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AccountRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _AccountRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AccountRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _AccountRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AccountRuleCondition::GetAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _AccountRuleCondition::SetAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _Account properties

/////////////////////////////////////////////////////////////////////////////
// _Account operations

LPDISPATCH _Account::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Account::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Account::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Account::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Account::GetAccountType()
{
	long result;
	InvokeHelper(0xfad2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Account::GetDisplayName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Account::GetUserName_()
{
	CString result;
	InvokeHelper(0xfad3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Account::GetSmtpAddress()
{
	CString result;
	InvokeHelper(0xfad4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TextRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _TextRuleCondition operations

LPDISPATCH _TextRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TextRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TextRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TextRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _TextRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _TextRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _TextRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _TextRuleCondition::GetText()
{
	VARIANT result;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _TextRuleCondition::SetText(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _CategoryRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _CategoryRuleCondition operations

LPDISPATCH _CategoryRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CategoryRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CategoryRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CategoryRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _CategoryRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CategoryRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _CategoryRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _CategoryRuleCondition::GetCategories()
{
	VARIANT result;
	InvokeHelper(0xfb45, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _CategoryRuleCondition::SetCategories(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _FormNameRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _FormNameRuleCondition operations

LPDISPATCH _FormNameRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _FormNameRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormNameRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FormNameRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _FormNameRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _FormNameRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _FormNameRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _FormNameRuleCondition::GetFormName()
{
	VARIANT result;
	InvokeHelper(0xfb46, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _FormNameRuleCondition::SetFormName(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _ToOrFromRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _ToOrFromRuleCondition operations

LPDISPATCH _ToOrFromRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ToOrFromRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ToOrFromRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ToOrFromRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _ToOrFromRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ToOrFromRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ToOrFromRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ToOrFromRuleCondition::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _AddressRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _AddressRuleCondition operations

LPDISPATCH _AddressRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AddressRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AddressRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AddressRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _AddressRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _AddressRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _AddressRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _AddressRuleCondition::GetAddress()
{
	VARIANT result;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _AddressRuleCondition::SetAddress(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _SenderInAddressListRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _SenderInAddressListRuleCondition operations

LPDISPATCH _SenderInAddressListRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SenderInAddressListRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SenderInAddressListRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SenderInAddressListRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _SenderInAddressListRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SenderInAddressListRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _SenderInAddressListRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SenderInAddressListRuleCondition::GetAddressList()
{
	LPDISPATCH result;
	InvokeHelper(0xfb47, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SenderInAddressListRuleCondition::SetAddressList(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}


/////////////////////////////////////////////////////////////////////////////
// AddressList properties

/////////////////////////////////////////////////////////////////////////////
// AddressList operations

LPDISPATCH AddressList::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressList::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressList::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressList::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressList::GetAddressEntries()
{
	LPDISPATCH result;
	InvokeHelper(0x100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString AddressList::GetId()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long AddressList::GetIndex()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL AddressList::GetIsReadOnly()
{
	BOOL result;
	InvokeHelper(0xf017, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString AddressList::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressList::GetContactsFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfafa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressList::GetAddressListType()
{
	long result;
	InvokeHelper(0xfaed, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long AddressList::GetResolutionOrder()
{
	long result;
	InvokeHelper(0xfaee, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL AddressList::GetIsInitialAddressList()
{
	BOOL result;
	InvokeHelper(0xfaf8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressList::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _FromRssFeedRuleCondition properties

/////////////////////////////////////////////////////////////////////////////
// _FromRssFeedRuleCondition operations

LPDISPATCH _FromRssFeedRuleCondition::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _FromRssFeedRuleCondition::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FromRssFeedRuleCondition::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _FromRssFeedRuleCondition::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _FromRssFeedRuleCondition::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _FromRssFeedRuleCondition::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _FromRssFeedRuleCondition::GetConditionType()
{
	long result;
	InvokeHelper(0xfb2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT _FromRssFeedRuleCondition::GetFromRssFeed()
{
	VARIANT result;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _FromRssFeedRuleCondition::SetFromRssFeed(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}


/////////////////////////////////////////////////////////////////////////////
// _StorageItem properties

/////////////////////////////////////////////////////////////////////////////
// _StorageItem operations

LPDISPATCH _StorageItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _StorageItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _StorageItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _StorageItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

DATE _StorageItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

DATE _StorageItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _StorageItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _StorageItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _StorageItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _StorageItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _StorageItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _StorageItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _StorageItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _StorageItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _StorageItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _StorageItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _StorageItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _StorageItem::GetCreator()
{
	CString result;
	InvokeHelper(0x85bc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _StorageItem::SetCreator(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x85bc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _Table properties

/////////////////////////////////////////////////////////////////////////////
// _Table operations

LPDISPATCH _Table::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Table::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Table::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Table::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Table::FindRow(LPCTSTR Filter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filter);
	return result;
}

LPDISPATCH _Table::FindNextRow()
{
	LPDISPATCH result;
	InvokeHelper(0x63, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT _Table::GetArray(long MaxRows)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbcf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		MaxRows);
	return result;
}

LPDISPATCH _Table::GetNextRow()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Table::GetRowCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void _Table::MoveToStart()
{
	InvokeHelper(0xfb1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Table::Restrict(LPCTSTR Filter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Filter);
	return result;
}

void _Table::Sort(LPCTSTR SortProperty, const VARIANT& Descending)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SortProperty, &Descending);
}

LPDISPATCH _Table::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0xfb93, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Table::GetEndOfTable()
{
	BOOL result;
	InvokeHelper(0xfb1f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Row properties

/////////////////////////////////////////////////////////////////////////////
// _Row operations

LPDISPATCH _Row::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Row::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Row::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Row::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Row::BinaryToString(const VARIANT& Index)
{
	CString result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb03, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		&Index);
	return result;
}

VARIANT _Row::GetValues()
{
	VARIANT result;
	InvokeHelper(0xfbce, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _Row::Item(const VARIANT& Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Index);
	return result;
}

DATE _Row::LocalTimeToUTC(const VARIANT& Index)
{
	DATE result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb01, DISPATCH_METHOD, VT_DATE, (void*)&result, parms,
		&Index);
	return result;
}

DATE _Row::UTCToLocalTime(const VARIANT& Index)
{
	DATE result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfb00, DISPATCH_METHOD, VT_DATE, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Columns properties

/////////////////////////////////////////////////////////////////////////////
// _Columns operations

LPDISPATCH _Columns::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Columns::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Columns::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Columns::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Columns::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Columns::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Columns::Add(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

void _Columns::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

void _Columns::RemoveAll()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _Column properties

/////////////////////////////////////////////////////////////////////////////
// _Column operations

LPDISPATCH _Column::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Column::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Column::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Column::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Column::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _CalendarSharing properties

/////////////////////////////////////////////////////////////////////////////
// _CalendarSharing operations

LPDISPATCH _CalendarSharing::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _CalendarSharing::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarSharing::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _CalendarSharing::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SaveAsICal(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb98, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

LPDISPATCH _CalendarSharing::ForwardAsICal(long MailFormat)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb99, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		MailFormat);
	return result;
}

long _CalendarSharing::GetCalendarDetail()
{
	long result;
	InvokeHelper(0xfb9a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetCalendarDetail(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfb9a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _CalendarSharing::GetEndDate()
{
	DATE result;
	InvokeHelper(0xfb9b, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetEndDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfb9b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _CalendarSharing::GetFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfb9c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _CalendarSharing::GetIncludeAttachments()
{
	BOOL result;
	InvokeHelper(0xfb9d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetIncludeAttachments(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb9d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _CalendarSharing::GetIncludePrivateDetails()
{
	BOOL result;
	InvokeHelper(0xfb9e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetIncludePrivateDetails(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb9e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _CalendarSharing::GetRestrictToWorkingHours()
{
	BOOL result;
	InvokeHelper(0xfb9f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetRestrictToWorkingHours(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfb9f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _CalendarSharing::GetStartDate()
{
	DATE result;
	InvokeHelper(0xfba0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xfba0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _CalendarSharing::GetIncludeWholeCalendar()
{
	BOOL result;
	InvokeHelper(0xfba1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _CalendarSharing::SetIncludeWholeCalendar(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfba1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

*/
/*
/////////////////////////////////////////////////////////////////////////////
// Links properties

/////////////////////////////////////////////////////////////////////////////
// Links operations

LPDISPATCH Links::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Links::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Links::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Links::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Links::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Links::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Links::Add(LPDISPATCH Item)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Item);
	return result;
}

void Links::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// Link properties

/////////////////////////////////////////////////////////////////////////////
// Link operations

LPDISPATCH Link::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Link::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Link::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Link::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Link::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Link::GetType()
{
	long result;
	InvokeHelper(0x2101, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Link::GetItem()
{
	LPDISPATCH result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// ItemProperties properties

/////////////////////////////////////////////////////////////////////////////
// ItemProperties operations

LPDISPATCH ItemProperties::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long ItemProperties::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH ItemProperties::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH ItemProperties::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long ItemProperties::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH ItemProperties::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH ItemProperties::Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, Type, &AddToFolderFields, &DisplayFormat);
	return result;
}

void ItemProperties::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// ItemProperty properties

/////////////////////////////////////////////////////////////////////////////
// ItemProperty operations

LPDISPATCH ItemProperty::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long ItemProperty::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH ItemProperty::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH ItemProperty::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString ItemProperty::GetName()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long ItemProperty::GetType()
{
	long result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT ItemProperty::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void ItemProperty::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

BOOL ItemProperty::GetIsUserProperty()
{
	BOOL result;
	InvokeHelper(0xfa08, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Conflicts properties

/////////////////////////////////////////////////////////////////////////////
// Conflicts operations

LPDISPATCH Conflicts::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Conflicts::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Conflicts::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Conflicts::GetFirst()
{
	LPDISPATCH result;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::GetLast()
{
	LPDISPATCH result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflicts::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Conflict properties

/////////////////////////////////////////////////////////////////////////////
// Conflict operations

LPDISPATCH Conflict::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Conflict::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflict::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflict::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Conflict::GetItem()
{
	LPDISPATCH result;
	InvokeHelper(0xfab8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Conflict::GetName()
{
	CString result;
	InvokeHelper(0xfab9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long Conflict::GetType()
{
	long result;
	InvokeHelper(0xfabc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// ItemEvents properties

/////////////////////////////////////////////////////////////////////////////
// ItemEvents operations

void ItemEvents::Open(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ItemEvents::CustomAction(LPDISPATCH Action, LPDISPATCH Response, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Action, Response, Cancel);
}

void ItemEvents::CustomPropertyChange(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void ItemEvents::Forward(LPDISPATCH Forward, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf468, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Forward, Cancel);
}

void ItemEvents::Close(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ItemEvents::PropertyChange(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void ItemEvents::Read()
{
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ItemEvents::Reply(LPDISPATCH Response, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf466, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Response, Cancel);
}

void ItemEvents::ReplyAll(LPDISPATCH Response, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf467, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Response, Cancel);
}

void ItemEvents::Send(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ItemEvents::Write(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ItemEvents::BeforeCheckNames(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf00a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ItemEvents::AttachmentAdd(LPDISPATCH Attachment)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf00b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment);
}

void ItemEvents::AttachmentRead(LPDISPATCH Attachment)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf00c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment);
}

void ItemEvents::BeforeAttachmentSave(LPDISPATCH Attachment, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf00d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// ItemEvents_10 properties

/////////////////////////////////////////////////////////////////////////////
// ItemEvents_10 operations

void ItemEvents_10::BeforeDelete(LPDISPATCH Item, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfa75, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item, Cancel);
}

void ItemEvents_10::AttachmentRemove(LPDISPATCH Attachment)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfbae, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment);
}

void ItemEvents_10::BeforeAttachmentAdd(LPDISPATCH Attachment, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfbb0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment, Cancel);
}

void ItemEvents_10::BeforeAttachmentPreview(LPDISPATCH Attachment, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfbaf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment, Cancel);
}

void ItemEvents_10::BeforeAttachmentRead(LPDISPATCH Attachment, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfbab, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment, Cancel);
}

void ItemEvents_10::BeforeAttachmentWriteToTempFile(LPDISPATCH Attachment, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfbb2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Attachment, Cancel);
}

void ItemEvents_10::Unload()
{
	InvokeHelper(0xfbad, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ItemEvents_10::BeforeAutoSave(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xfc02, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _UserDefinedProperties properties

/////////////////////////////////////////////////////////////////////////////
// _UserDefinedProperties operations

LPDISPATCH _UserDefinedProperties::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _UserDefinedProperties::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _UserDefinedProperties::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _UserDefinedProperties::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _UserDefinedProperties::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _UserDefinedProperties::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _UserDefinedProperties::Add(LPCTSTR Name, long Type, const VARIANT& DisplayFormat, const VARIANT& Formula)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, Type, &DisplayFormat, &Formula);
	return result;
}

LPDISPATCH _UserDefinedProperties::Find(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

void _UserDefinedProperties::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void _UserDefinedProperties::Refresh()
{
	InvokeHelper(0xfa45, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _UserDefinedProperty properties

/////////////////////////////////////////////////////////////////////////////
// _UserDefinedProperty operations

LPDISPATCH _UserDefinedProperty::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _UserDefinedProperty::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _UserDefinedProperty::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _UserDefinedProperty::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _UserDefinedProperty::GetFormula()
{
	CString result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _UserDefinedProperty::GetName()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _UserDefinedProperty::GetType()
{
	long result;
	InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _UserDefinedProperty::GetDisplayFormat()
{
	long result;
	InvokeHelper(0xfa27, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _UserDefinedProperty::Delete()
{
	InvokeHelper(0xfc0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _ExchangeUser properties

/////////////////////////////////////////////////////////////////////////////
// _ExchangeUser operations

LPDISPATCH _ExchangeUser::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ExchangeUser::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetAddress()
{
	CString result;
	InvokeHelper(0x3003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ExchangeUser::GetDisplayType()
{
	long result;
	InvokeHelper(0x3900, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetId()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetType()
{
	CString result;
	InvokeHelper(0x3002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _ExchangeUser::Delete()
{
	InvokeHelper(0x302, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ExchangeUser::Details(const VARIANT& HWnd)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x301, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &HWnd);
}

CString _ExchangeUser::GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat)
{
	CString result;
	static BYTE parms[] =
		VTS_DATE VTS_I4 VTS_VARIANT;
	InvokeHelper(0x306, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Start, MinPerChar, &CompleteFormat);
	return result;
}

void _ExchangeUser::Update(const VARIANT& MakePermanent, const VARIANT& Refresh)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x300, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &MakePermanent, &Refresh);
}

LPDISPATCH _ExchangeUser::GetContact()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetExchangeUser()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ExchangeUser::GetAddressEntryUserType()
{
	long result;
	InvokeHelper(0xfaf2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetExchangeDistributionList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaef, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetDirectReports()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeUser::GetMemberOfList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetAlias()
{
	CString result;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetAssistantName()
{
	CString result;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetAssistantName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetBusinessTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetBusinessTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetCity()
{
	CString result;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetComments()
{
	CString result;
	InvokeHelper(0x3004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetComments(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetCompanyName()
{
	CString result;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetDepartment()
{
	CString result;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetDepartment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetFirstName()
{
	CString result;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetJobTitle()
{
	CString result;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetJobTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetLastName()
{
	CString result;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetMobileTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetMobileTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetOfficeLocation()
{
	CString result;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetOfficeLocation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetPostalCode()
{
	CString result;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetPrimarySmtpAddress()
{
	CString result;
	InvokeHelper(0xfaf5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetStateOrProvince()
{
	CString result;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetStateOrProvince(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetStreetAddress()
{
	CString result;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetStreetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _ExchangeUser::GetExchangeUserManager()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ExchangeUser::GetYomiCompanyName()
{
	CString result;
	InvokeHelper(0x802e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetYomiCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetYomiFirstName()
{
	CString result;
	InvokeHelper(0x802c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetYomiFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetYomiLastName()
{
	CString result;
	InvokeHelper(0x802d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetYomiLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetYomiDisplayName()
{
	CString result;
	InvokeHelper(0xfbe7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetYomiDisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfbe7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeUser::GetYomiDepartment()
{
	CString result;
	InvokeHelper(0xfbe8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeUser::SetYomiDepartment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfbe8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _ExchangeDistributionList properties

/////////////////////////////////////////////////////////////////////////////
// _ExchangeDistributionList operations

LPDISPATCH _ExchangeDistributionList::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ExchangeDistributionList::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ExchangeDistributionList::GetAddress()
{
	CString result;
	InvokeHelper(0x3003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeDistributionList::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _ExchangeDistributionList::GetDisplayType()
{
	long result;
	InvokeHelper(0x3900, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ExchangeDistributionList::GetId()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ExchangeDistributionList::GetName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeDistributionList::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeDistributionList::GetType()
{
	CString result;
	InvokeHelper(0x3002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeDistributionList::SetType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _ExchangeDistributionList::Delete()
{
	InvokeHelper(0x302, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ExchangeDistributionList::Details(const VARIANT& HWnd)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x301, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &HWnd);
}

CString _ExchangeDistributionList::GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat)
{
	CString result;
	static BYTE parms[] =
		VTS_DATE VTS_I4 VTS_VARIANT;
	InvokeHelper(0x306, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Start, MinPerChar, &CompleteFormat);
	return result;
}

void _ExchangeDistributionList::Update(const VARIANT& MakePermanent, const VARIANT& Refresh)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x300, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &MakePermanent, &Refresh);
}

LPDISPATCH _ExchangeDistributionList::GetContact()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetExchangeUser()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ExchangeDistributionList::GetAddressEntryUserType()
{
	long result;
	InvokeHelper(0xfaf2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetExchangeDistributionList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaef, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetMemberOfList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetExchangeDistributionListMembers()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ExchangeDistributionList::GetAlias()
{
	CString result;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ExchangeDistributionList::GetComments()
{
	CString result;
	InvokeHelper(0x3004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExchangeDistributionList::SetComments(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ExchangeDistributionList::GetPrimarySmtpAddress()
{
	CString result;
	InvokeHelper(0xfaf5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ExchangeDistributionList::GetOwners()
{
	LPDISPATCH result;
	InvokeHelper(0xfaf7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// AddressLists properties

/////////////////////////////////////////////////////////////////////////////
// AddressLists operations

LPDISPATCH AddressLists::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressLists::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressLists::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressLists::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long AddressLists::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH AddressLists::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SyncObjects properties

/////////////////////////////////////////////////////////////////////////////
// SyncObjects operations

LPDISPATCH SyncObjects::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SyncObjects::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SyncObjects::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH SyncObjects::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long SyncObjects::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SyncObjects::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH SyncObjects::GetAppFolders()
{
	LPDISPATCH result;
	InvokeHelper(0xfa4a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _SyncObject properties

/////////////////////////////////////////////////////////////////////////////
// _SyncObject operations

LPDISPATCH _SyncObject::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SyncObject::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SyncObject::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SyncObject::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _SyncObject::GetName()
{
	CString result;
	InvokeHelper(0x2100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SyncObject::Start()
{
	InvokeHelper(0x2101, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _SyncObject::Stop()
{
	InvokeHelper(0x2102, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// SyncObjectEvents properties

/////////////////////////////////////////////////////////////////////////////
// SyncObjectEvents operations

void SyncObjectEvents::SyncStart()
{
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SyncObjectEvents::Progress(long State, LPCTSTR Description, long Value, long Max)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 State, Description, Value, Max);
}

void SyncObjectEvents::OnError(long Code, LPCTSTR Description)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Code, Description);
}

void SyncObjectEvents::SyncEnd()
{
	InvokeHelper(0xf004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _Accounts properties

/////////////////////////////////////////////////////////////////////////////
// _Accounts operations

LPDISPATCH _Accounts::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Accounts::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Accounts::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Accounts::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Accounts::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Accounts::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Stores properties

/////////////////////////////////////////////////////////////////////////////
// _Stores operations

LPDISPATCH _Stores::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Stores::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Stores::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Stores::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Stores::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Stores::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// StoresEvents_12 properties

/////////////////////////////////////////////////////////////////////////////
// StoresEvents_12 operations

void StoresEvents_12::BeforeStoreRemove(LPDISPATCH Store, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfbb1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Store, Cancel);
}

void StoresEvents_12::StoreAdd(LPDISPATCH Store)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Store);
}


/////////////////////////////////////////////////////////////////////////////
// _SelectNamesDialog properties

/////////////////////////////////////////////////////////////////////////////
// _SelectNamesDialog operations

LPDISPATCH _SelectNamesDialog::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SelectNamesDialog::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SelectNamesDialog::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SelectNamesDialog::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _SelectNamesDialog::GetCaption()
{
	CString result;
	InvokeHelper(0xfae2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfae2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SelectNamesDialog::Display()
{
	BOOL result;
	InvokeHelper(0xf021, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SelectNamesDialog::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetRecipients(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf814, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _SelectNamesDialog::GetBccLabel()
{
	CString result;
	InvokeHelper(0xfae3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetBccLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfae3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SelectNamesDialog::GetCcLabel()
{
	CString result;
	InvokeHelper(0xfae4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetCcLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfae4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SelectNamesDialog::GetToLabel()
{
	CString result;
	InvokeHelper(0xfae5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetToLabel(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfae5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SelectNamesDialog::GetAllowMultipleSelection()
{
	BOOL result;
	InvokeHelper(0xfae6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetAllowMultipleSelection(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfae6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SelectNamesDialog::GetForceResolution()
{
	BOOL result;
	InvokeHelper(0xfae7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetForceResolution(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfae7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SelectNamesDialog::GetShowOnlyInitialAddressList()
{
	BOOL result;
	InvokeHelper(0xfae8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetShowOnlyInitialAddressList(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfae8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _SelectNamesDialog::GetNumberOfRecipientSelectors()
{
	long result;
	InvokeHelper(0xfae9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetNumberOfRecipientSelectors(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfae9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _SelectNamesDialog::GetInitialAddressList()
{
	LPDISPATCH result;
	InvokeHelper(0xfaea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SelectNamesDialog::SetInitialAddressList(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfaea, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _SelectNamesDialog::SetDefaultDisplayMode(long defaultMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfaeb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 defaultMode);
}


/////////////////////////////////////////////////////////////////////////////
// _Categories properties

/////////////////////////////////////////////////////////////////////////////
// _Categories operations

LPDISPATCH _Categories::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Categories::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Categories::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Categories::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Categories::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Categories::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Categories::Add(LPCTSTR Name, const VARIANT& Color, const VARIANT& ShortcutKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Color, &ShortcutKey);
	return result;
}

void _Categories::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// _Category properties

/////////////////////////////////////////////////////////////////////////////
// _Category operations

LPDISPATCH _Category::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Category::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Category::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Category::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Category::GetName()
{
	CString result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Category::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Category::GetColor()
{
	long result;
	InvokeHelper(0xfba3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Category::SetColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfba3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Category::GetShortcutKey()
{
	long result;
	InvokeHelper(0xfba4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Category::SetShortcutKey(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfba4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Category::GetCategoryID()
{
	CString result;
	InvokeHelper(0xfbd0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

unsigned long _Category::GetCategoryBorderColor()
{
	unsigned long result;
	InvokeHelper(0xfc1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

unsigned long _Category::GetCategoryGradientTopColor()
{
	unsigned long result;
	InvokeHelper(0xfc1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

unsigned long _Category::GetCategoryGradientBottomColor()
{
	unsigned long result;
	InvokeHelper(0xfc1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _SharingItem properties

/////////////////////////////////////////////////////////////////////////////
// _SharingItem operations

LPDISPATCH _SharingItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _SharingItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _SharingItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SharingItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _SharingItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _SharingItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _SharingItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SharingItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _SharingItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SharingItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _SharingItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SharingItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _SharingItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _SharingItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _SharingItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _SharingItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _SharingItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

void _SharingItem::Allow()
{
	InvokeHelper(0xfbef, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _SharingItem::Deny()
{
	LPDISPATCH result;
	InvokeHelper(0xfbfb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetRemoteName()
{
	CString result;
	InvokeHelper(0xfbea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetRemoteID()
{
	CString result;
	InvokeHelper(0xfbeb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetRemotePath()
{
	CString result;
	InvokeHelper(0xfbec, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetSharingProviderGuid()
{
	CString result;
	InvokeHelper(0xfbee, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::OpenSharedFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xfbf2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetSharingProvider()
{
	long result;
	InvokeHelper(0xfbf5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _SharingItem::GetAllowWriteAccess()
{
	BOOL result;
	InvokeHelper(0xfbf8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetAllowWriteAccess(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfbf8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _SharingItem::GetType()
{
	long result;
	InvokeHelper(0xfbf9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbf9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _SharingItem::GetRequestedFolder()
{
	long result;
	InvokeHelper(0xfbfa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetSendUsingAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetSendUsingAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _SharingItem::GetAlternateRecipientAllowed()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetAlternateRecipientAllowed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SharingItem::GetAutoForwarded()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetAutoForwarded(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _SharingItem::GetBcc()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetBcc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetCc()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetCc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _SharingItem::GetDeferredDeliveryTime()
{
	DATE result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetDeferredDeliveryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _SharingItem::GetDeleteAfterSubmit()
{
	BOOL result;
	InvokeHelper(0xe01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetDeleteAfterSubmit(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _SharingItem::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _SharingItem::GetFlagRequest()
{
	CString result;
	InvokeHelper(0x8530, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetFlagRequest(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8530, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _SharingItem::GetHTMLBody()
{
	CString result;
	InvokeHelper(0xf404, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetHTMLBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf404, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SharingItem::GetOriginatorDeliveryReportRequested()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetOriginatorDeliveryReportRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SharingItem::GetReadReceiptRequested()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReadReceiptRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _SharingItem::GetReceivedByEntryID()
{
	CString result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetReceivedByName()
{
	CString result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetReceivedOnBehalfOfEntryID()
{
	CString result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetReceivedOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _SharingItem::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

BOOL _SharingItem::GetRecipientReassignmentProhibited()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetRecipientReassignmentProhibited(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _SharingItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _SharingItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SharingItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _SharingItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _SharingItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _SharingItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _SharingItem::GetRemoteStatus()
{
	long result;
	InvokeHelper(0x8511, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetRemoteStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8511, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _SharingItem::GetReplyRecipientNames()
{
	CString result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetReplyRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::GetSaveSentMessageFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xf401, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetRefSaveSentMessageFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf401, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _SharingItem::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _SharingItem::GetSent()
{
	BOOL result;
	InvokeHelper(0xf402, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE _SharingItem::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetSentOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetSentOnBehalfOfName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _SharingItem::GetSubmitted()
{
	BOOL result;
	InvokeHelper(0xf403, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetTo()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetTo(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _SharingItem::ClearConversationIndex()
{
	InvokeHelper(0xf822, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _SharingItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _SharingItem::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _SharingItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _SharingItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetBodyFormat()
{
	long result;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetBodyFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _SharingItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _SharingItem::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _SharingItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _SharingItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SharingItem::AddBusinessCard(LPDISPATCH contact)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb96, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 contact);
}

CString _SharingItem::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _SharingItem::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _SharingItem::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _SharingItem::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _SharingItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _SharingItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetSenderEmailAddress()
{
	CString result;
	InvokeHelper(0xc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SharingItem::GetSenderEmailType()
{
	CString result;
	InvokeHelper(0xc1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _SharingItem::GetPermission()
{
	long result;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetPermission(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _SharingItem::GetPermissionService()
{
	long result;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _SharingItem::SetPermissionService(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _SharingItem::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _SharingItem::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _SharingItem::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Explorers properties

/////////////////////////////////////////////////////////////////////////////
// _Explorers operations

LPDISPATCH _Explorers::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Explorers::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorers::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorers::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Explorers::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Explorers::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Explorers::Add(const VARIANT& Folder, long DisplayMode)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Folder, DisplayMode);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents properties

/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents operations

void ExplorerEvents::Activate()
{
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ExplorerEvents::FolderSwitch()
{
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ExplorerEvents::BeforeFolderSwitch(LPDISPATCH NewFolder, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NewFolder, Cancel);
}

void ExplorerEvents::ViewSwitch()
{
	InvokeHelper(0xf004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ExplorerEvents::BeforeViewSwitch(const VARIANT& NewView, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PBOOL;
	InvokeHelper(0xf005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &NewView, Cancel);
}

void ExplorerEvents::Deactivate()
{
	InvokeHelper(0xf006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ExplorerEvents::SelectionChange()
{
	InvokeHelper(0xf007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ExplorerEvents::Close()
{
	InvokeHelper(0xf008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents_10 properties

/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents_10 operations

void ExplorerEvents_10::BeforeItemCopy(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xfa0e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ExplorerEvents_10::BeforeItemCut(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xfa0f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ExplorerEvents_10::BeforeItemPaste(VARIANT* ClipboardContent, LPDISPATCH Target, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfa10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ClipboardContent, Target, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _Inspectors properties

/////////////////////////////////////////////////////////////////////////////
// _Inspectors operations

LPDISPATCH _Inspectors::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Inspectors::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspectors::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspectors::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Inspectors::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Inspectors::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Inspectors::Add(LPDISPATCH Item)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Item);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// InspectorEvents properties

/////////////////////////////////////////////////////////////////////////////
// InspectorEvents operations

void InspectorEvents::Activate()
{
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void InspectorEvents::Deactivate()
{
	InvokeHelper(0xf006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void InspectorEvents::Close()
{
	InvokeHelper(0xf008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// InspectorEvents_10 properties

/////////////////////////////////////////////////////////////////////////////
// InspectorEvents_10 operations

void InspectorEvents_10::PageChange(BSTR* ActivePageName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0xfbf4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ActivePageName);
}


/////////////////////////////////////////////////////////////////////////////
// Search properties

/////////////////////////////////////////////////////////////////////////////
// Search operations

LPDISPATCH Search::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Search::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Search::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Search::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString Search::GetFilter()
{
	CString result;
	InvokeHelper(0xfa64, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL Search::GetIsSynchronous()
{
	BOOL result;
	InvokeHelper(0xfa68, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH Search::GetResults()
{
	LPDISPATCH result;
	InvokeHelper(0xfa69, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Search::GetSearchSubFolders()
{
	BOOL result;
	InvokeHelper(0xfa66, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Search::Stop()
{
	InvokeHelper(0xfa67, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString Search::GetTag()
{
	CString result;
	InvokeHelper(0xfa6d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString Search::GetScope()
{
	CString result;
	InvokeHelper(0xfaa5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH Search::Save(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfaad, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

LPDISPATCH Search::GetTable()
{
	LPDISPATCH result;
	InvokeHelper(0xfb1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Results properties

/////////////////////////////////////////////////////////////////////////////
// _Results operations

LPDISPATCH _Results::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Results::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Results::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _Results::GetFirst()
{
	LPDISPATCH result;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::GetLast()
{
	LPDISPATCH result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::GetNext()
{
	LPDISPATCH result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Results::GetPrevious()
{
	LPDISPATCH result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Results::ResetColumns()
{
	InvokeHelper(0x5d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Results::SetColumns(LPCTSTR Columns)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Columns);
}

void _Results::Sort(LPCTSTR Property_, const VARIANT& Descending)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Property_, &Descending);
}

long _Results::GetDefaultItemType()
{
	long result;
	InvokeHelper(0xfa8f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Results::SetDefaultItemType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa8f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _Reminders properties

/////////////////////////////////////////////////////////////////////////////
// _Reminders operations

LPDISPATCH _Reminders::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Reminders::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminders::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminders::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Reminders::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminders::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

void _Reminders::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// _Reminder properties

/////////////////////////////////////////////////////////////////////////////
// _Reminder operations

LPDISPATCH _Reminder::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Reminder::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminder::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminder::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Reminder::Dismiss()
{
	InvokeHelper(0xfa9a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Reminder::Snooze(const VARIANT& SnoozeTime)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfa9b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &SnoozeTime);
}

CString _Reminder::GetCaption()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Reminder::GetIsVisible()
{
	BOOL result;
	InvokeHelper(0xfa9d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Reminder::GetItem()
{
	LPDISPATCH result;
	InvokeHelper(0xfa9e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

DATE _Reminder::GetNextReminderDate()
{
	DATE result;
	InvokeHelper(0xfa9f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

DATE _Reminder::GetOriginalReminderDate()
{
	DATE result;
	InvokeHelper(0xfaa0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _TimeZones properties

/////////////////////////////////////////////////////////////////////////////
// _TimeZones operations

LPDISPATCH _TimeZones::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimeZones::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimeZones::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimeZones::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _TimeZones::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _TimeZones::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

DATE _TimeZones::ConvertTime(DATE SourceDateTime, LPDISPATCH SourceTimeZone, LPDISPATCH DestinationTimeZone)
{
	DATE result;
	static BYTE parms[] =
		VTS_DATE VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfc26, DISPATCH_METHOD, VT_DATE, (void*)&result, parms,
		SourceDateTime, SourceTimeZone, DestinationTimeZone);
	return result;
}

LPDISPATCH _TimeZones::GetCurrentTimeZone()
{
	LPDISPATCH result;
	InvokeHelper(0xfc2a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _OlkTimeZoneControl properties

/////////////////////////////////////////////////////////////////////////////
// _OlkTimeZoneControl operations

long _OlkTimeZoneControl::GetAppointmentTimeField()
{
	long result;
	InvokeHelper(0xfc2f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetAppointmentTimeField(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfc2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkTimeZoneControl::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _OlkTimeZoneControl::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _OlkTimeZoneControl::GetLocked()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _OlkTimeZoneControl::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _OlkTimeZoneControl::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _OlkTimeZoneControl::GetSelectedTimeZoneIndex()
{
	long result;
	InvokeHelper(0xfc30, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetSelectedTimeZoneIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfc30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT _OlkTimeZoneControl::GetValue()
{
	VARIANT result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _OlkTimeZoneControl::SetValue(const VARIANT& newValue)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 &newValue);
}

void _OlkTimeZoneControl::DropDown()
{
	InvokeHelper(0x3e9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OlkTimeZoneControlEvents properties

/////////////////////////////////////////////////////////////////////////////
// OlkTimeZoneControlEvents operations

void OlkTimeZoneControlEvents::Click()
{
	InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::DoubleClick()
{
	InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::MouseDown(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeZoneControlEvents::MouseMove(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeZoneControlEvents::MouseUp(long Button, long Shift, float X, float Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button, Shift, X, Y);
}

void OlkTimeZoneControlEvents::Enter()
{
	InvokeHelper(0x80018202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::Exit(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018203, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OlkTimeZoneControlEvents::KeyDown(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTimeZoneControlEvents::KeyPress(long* KeyAscii)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyAscii);
}

void OlkTimeZoneControlEvents::KeyUp(long* KeyCode, long Shift)
{
	static BYTE parms[] =
		VTS_PI4 VTS_I4;
	InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 KeyCode, Shift);
}

void OlkTimeZoneControlEvents::Change()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::DropButtonClick()
{
	InvokeHelper(0x7d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::AfterUpdate()
{
	InvokeHelper(0x80018200, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void OlkTimeZoneControlEvents::BeforeUpdate(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x80018201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents properties

/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents operations

void ApplicationEvents::ItemSend(LPDISPATCH Item, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item, Cancel);
}

void ApplicationEvents::NewMail()
{
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ApplicationEvents::Reminder(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ApplicationEvents::OptionsPagesAdd(LPDISPATCH Pages)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Pages);
}

void ApplicationEvents::Startup()
{
	InvokeHelper(0xf006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ApplicationEvents::Quit()
{
	InvokeHelper(0xf007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// PropertyPages properties

/////////////////////////////////////////////////////////////////////////////
// PropertyPages operations

LPDISPATCH PropertyPages::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PropertyPages::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH PropertyPages::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PropertyPages::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PropertyPages::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH PropertyPages::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

void PropertyPages::Add(const VARIANT& Page, LPCTSTR Title)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_BSTR;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Page, Title);
}

void PropertyPages::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// RecurrencePattern properties

/////////////////////////////////////////////////////////////////////////////
// RecurrencePattern operations

LPDISPATCH RecurrencePattern::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long RecurrencePattern::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH RecurrencePattern::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH RecurrencePattern::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long RecurrencePattern::GetDayOfMonth()
{
	long result;
	InvokeHelper(0x1000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetDayOfMonth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long RecurrencePattern::GetDayOfWeekMask()
{
	long result;
	InvokeHelper(0x1001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetDayOfWeekMask(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long RecurrencePattern::GetDuration()
{
	long result;
	InvokeHelper(0x100d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetDuration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE RecurrencePattern::GetEndTime()
{
	DATE result;
	InvokeHelper(0x100c, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetEndTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x100c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH RecurrencePattern::GetExceptions()
{
	LPDISPATCH result;
	InvokeHelper(0x100e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long RecurrencePattern::GetInstance()
{
	long result;
	InvokeHelper(0x1003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetInstance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long RecurrencePattern::GetInterval()
{
	long result;
	InvokeHelper(0x1004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetInterval(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long RecurrencePattern::GetMonthOfYear()
{
	long result;
	InvokeHelper(0x1006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetMonthOfYear(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL RecurrencePattern::GetNoEndDate()
{
	BOOL result;
	InvokeHelper(0x100b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetNoEndDate(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x100b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long RecurrencePattern::GetOccurrences()
{
	long result;
	InvokeHelper(0x1005, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetOccurrences(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE RecurrencePattern::GetPatternEndDate()
{
	DATE result;
	InvokeHelper(0x1002, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetPatternEndDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x1002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE RecurrencePattern::GetPatternStartDate()
{
	DATE result;
	InvokeHelper(0x1008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetPatternStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x1008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long RecurrencePattern::GetRecurrenceType()
{
	long result;
	InvokeHelper(0x1007, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetRecurrenceType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL RecurrencePattern::GetRegenerate()
{
	BOOL result;
	InvokeHelper(0x100a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetRegenerate(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x100a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE RecurrencePattern::GetStartTime()
{
	DATE result;
	InvokeHelper(0x1009, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void RecurrencePattern::SetStartTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x1009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH RecurrencePattern::GetOccurrence(DATE StartDate)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x100f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		StartDate);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Exceptions properties

/////////////////////////////////////////////////////////////////////////////
// Exceptions operations

LPDISPATCH Exceptions::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Exceptions::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exceptions::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exceptions::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Exceptions::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exceptions::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Exception properties

/////////////////////////////////////////////////////////////////////////////
// Exception operations

LPDISPATCH Exception::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Exception::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exception::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exception::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exception::GetAppointmentItem()
{
	LPDISPATCH result;
	InvokeHelper(0x2001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Exception::GetDeleted()
{
	BOOL result;
	InvokeHelper(0x2002, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE Exception::GetOriginalDate()
{
	DATE result;
	InvokeHelper(0x2000, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

LPDISPATCH Exception::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}
*/
/*
/////////////////////////////////////////////////////////////////////////////
// _MeetingItem properties

/////////////////////////////////////////////////////////////////////////////
// _MeetingItem operations

LPDISPATCH _MeetingItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MeetingItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MeetingItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MeetingItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MeetingItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MeetingItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _MeetingItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MeetingItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _MeetingItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _MeetingItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MeetingItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _MeetingItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MeetingItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _MeetingItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _MeetingItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _MeetingItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _MeetingItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MeetingItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _MeetingItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MeetingItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MeetingItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _MeetingItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _MeetingItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MeetingItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MeetingItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

BOOL _MeetingItem::GetAutoForwarded()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetAutoForwarded(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MeetingItem::GetDeferredDeliveryTime()
{
	DATE result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetDeferredDeliveryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _MeetingItem::GetDeleteAfterSubmit()
{
	BOOL result;
	InvokeHelper(0xe01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetDeleteAfterSubmit(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MeetingItem::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _MeetingItem::GetOriginatorDeliveryReportRequested()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetOriginatorDeliveryReportRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MeetingItem::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetReceivedTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xe06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _MeetingItem::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _MeetingItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _MeetingItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _MeetingItem::GetReplyRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetSaveSentMessageFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xf401, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetRefSaveSentMessageFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf401, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _MeetingItem::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MeetingItem::GetSent()
{
	BOOL result;
	InvokeHelper(0xf402, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE _MeetingItem::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

BOOL _MeetingItem::GetSubmitted()
{
	BOOL result;
	InvokeHelper(0xf403, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetAssociatedAppointment(BOOL AddToCalendar)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf760, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		AddToCalendar);
	return result;
}

LPDISPATCH _MeetingItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _MeetingItem::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _MeetingItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MeetingItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MeetingItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _MeetingItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _MeetingItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _MeetingItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _MeetingItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetMeetingWorkspaceURL()
{
	CString result;
	InvokeHelper(0x8209, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _MeetingItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetSenderEmailAddress()
{
	CString result;
	InvokeHelper(0xc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _MeetingItem::GetSenderEmailType()
{
	CString result;
	InvokeHelper(0xc1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _MeetingItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// ExplorersEvents properties

/////////////////////////////////////////////////////////////////////////////
// ExplorersEvents operations

void ExplorersEvents::NewExplorer(LPDISPATCH Explorer)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Explorer);
}


/////////////////////////////////////////////////////////////////////////////
// FoldersEvents properties

/////////////////////////////////////////////////////////////////////////////
// FoldersEvents operations

void FoldersEvents::FolderAdd(LPDISPATCH Folder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Folder);
}

void FoldersEvents::FolderChange(LPDISPATCH Folder)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Folder);
}

void FoldersEvents::FolderRemove()
{
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// InspectorsEvents properties

/////////////////////////////////////////////////////////////////////////////
// InspectorsEvents operations

void InspectorsEvents::NewInspector(LPDISPATCH Inspector)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Inspector);
}


/////////////////////////////////////////////////////////////////////////////
// ItemsEvents properties

/////////////////////////////////////////////////////////////////////////////
// ItemsEvents operations

void ItemsEvents::ItemAdd(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ItemsEvents::ItemChange(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ItemsEvents::ItemRemove()
{
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// NameSpaceEvents properties

/////////////////////////////////////////////////////////////////////////////
// NameSpaceEvents operations

void NameSpaceEvents::OptionsPagesAdd(LPDISPATCH Pages, LPDISPATCH Folder)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xf005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Pages, Folder);
}

void NameSpaceEvents::AutoDiscoverComplete()
{
	InvokeHelper(0xfc2d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroup properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroup operations

LPDISPATCH OutlookBarGroup::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long OutlookBarGroup::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarGroup::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarGroup::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString OutlookBarGroup::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void OutlookBarGroup::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH OutlookBarGroup::GetShortcuts()
{
	LPDISPATCH result;
	InvokeHelper(0x2102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long OutlookBarGroup::GetViewType()
{
	long result;
	InvokeHelper(0x2103, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void OutlookBarGroup::SetViewType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2103, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _OutlookBarShortcuts properties

/////////////////////////////////////////////////////////////////////////////
// _OutlookBarShortcuts operations

LPDISPATCH _OutlookBarShortcuts::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OutlookBarShortcuts::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarShortcuts::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarShortcuts::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OutlookBarShortcuts::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarShortcuts::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _OutlookBarShortcuts::Add(const VARIANT& Target, LPCTSTR Name, const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Target, Name, &Index);
	return result;
}

void _OutlookBarShortcuts::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcut properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcut operations

LPDISPATCH OutlookBarShortcut::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long OutlookBarShortcut::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarShortcut::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarShortcut::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString OutlookBarShortcut::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void OutlookBarShortcut::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

VARIANT OutlookBarShortcut::GetTarget()
{
	VARIANT result;
	InvokeHelper(0x2100, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void OutlookBarShortcut::SetIcon(const VARIANT& Icon)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xfaa2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Icon);
}


/////////////////////////////////////////////////////////////////////////////
// _OutlookBarGroups properties

/////////////////////////////////////////////////////////////////////////////
// _OutlookBarGroups operations

LPDISPATCH _OutlookBarGroups::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OutlookBarGroups::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarGroups::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarGroups::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OutlookBarGroups::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarGroups::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH _OutlookBarGroups::Add(LPCTSTR Name, const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Index);
	return result;
}

void _OutlookBarGroups::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroupsEvents properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroupsEvents operations

void OutlookBarGroupsEvents::GroupAdd(LPDISPATCH NewGroup)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NewGroup);
}

void OutlookBarGroupsEvents::BeforeGroupAdd(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OutlookBarGroupsEvents::BeforeGroupRemove(LPDISPATCH Group, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Group, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _OutlookBarPane properties

/////////////////////////////////////////////////////////////////////////////
// _OutlookBarPane operations

LPDISPATCH _OutlookBarPane::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _OutlookBarPane::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarPane::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarPane::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _OutlookBarPane::GetContents()
{
	LPDISPATCH result;
	InvokeHelper(0x2100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _OutlookBarPane::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _OutlookBarPane::GetVisible()
{
	BOOL result;
	InvokeHelper(0x2103, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _OutlookBarPane::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2103, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarStorage properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarStorage operations

LPDISPATCH OutlookBarStorage::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long OutlookBarStorage::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarStorage::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarStorage::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH OutlookBarStorage::GetGroups()
{
	LPDISPATCH result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarPaneEvents properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarPaneEvents operations

void OutlookBarPaneEvents::BeforeNavigate(LPDISPATCH Shortcut, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Shortcut, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcutsEvents properties

/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcutsEvents operations

void OutlookBarShortcutsEvents::ShortcutAdd(LPDISPATCH NewShortcut)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NewShortcut);
}

void OutlookBarShortcutsEvents::BeforeShortcutAdd(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void OutlookBarShortcutsEvents::BeforeShortcutRemove(LPDISPATCH Shortcut, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Shortcut, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// PropertyPageSite properties

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSite operations

LPDISPATCH PropertyPageSite::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long PropertyPageSite::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH PropertyPageSite::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PropertyPageSite::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void PropertyPageSite::OnStatusChange()
{
	InvokeHelper(0x2100, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Pages properties

/////////////////////////////////////////////////////////////////////////////
// Pages operations

LPDISPATCH Pages::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Pages::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Pages::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Pages::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Pages::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Pages::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

LPDISPATCH Pages::Add(const VARIANT& Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x12c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Name);
	return result;
}

void Pages::Remove(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}


/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_10 properties

/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_10 operations

void ApplicationEvents_10::AdvancedSearchComplete(LPDISPATCH SearchObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa6a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SearchObject);
}

void ApplicationEvents_10::AdvancedSearchStopped(LPDISPATCH SearchObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa6b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SearchObject);
}

void ApplicationEvents_10::MAPILogonComplete()
{
	InvokeHelper(0xfa90, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_11 properties

/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_11 operations

void ApplicationEvents_11::NewMailEx(LPCTSTR EntryIDCollection)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfab5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 EntryIDCollection);
}

void ApplicationEvents_11::FolderContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Folder)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfb42, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandBar, Folder);
}

void ApplicationEvents_11::StoreContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Store)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfb43, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandBar, Store);
}

void ApplicationEvents_11::ShortcutContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Shortcut)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfb44, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandBar, Shortcut);
}

void ApplicationEvents_11::ViewContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH View)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfb40, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandBar, View);
}

void ApplicationEvents_11::ItemContextMenuDisplay(LPDISPATCH CommandBar, LPDISPATCH Selection)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0xfb41, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandBar, Selection);
}

void ApplicationEvents_11::ContextMenuClose(long ContextMenu)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfba6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ContextMenu);
}

void ApplicationEvents_11::ItemLoad(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfba7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ApplicationEvents_11::BeforeFolderSharingDialog(LPDISPATCH FolderToShare, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfc01, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FolderToShare, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// _AttachmentSelection properties

/////////////////////////////////////////////////////////////////////////////
// _AttachmentSelection operations

LPDISPATCH _AttachmentSelection::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AttachmentSelection::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AttachmentSelection::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AttachmentSelection::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _AttachmentSelection::GetCount()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _AttachmentSelection::Item(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// MAPIFolderEvents_12 properties

/////////////////////////////////////////////////////////////////////////////
// MAPIFolderEvents_12 operations

void MAPIFolderEvents_12::BeforeFolderMove(LPDISPATCH MoveTo, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfba8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MoveTo, Cancel);
}

void MAPIFolderEvents_12::BeforeItemMove(LPDISPATCH Item, LPDISPATCH MoveTo, BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
	InvokeHelper(0xfba9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item, MoveTo, Cancel);
}


/////////////////////////////////////////////////////////////////////////////
// ResultsEvents properties

/////////////////////////////////////////////////////////////////////////////
// ResultsEvents operations

void ResultsEvents::ItemAdd(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ResultsEvents::ItemChange(LPDISPATCH Item)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item);
}

void ResultsEvents::ItemRemove()
{
	InvokeHelper(0xf003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _ViewsEvents properties

/////////////////////////////////////////////////////////////////////////////
// _ViewsEvents operations

void _ViewsEvents::ViewAdd(LPDISPATCH View)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x190, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 View);
}

void _ViewsEvents::ViewRemove(LPDISPATCH View)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa47, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 View);
}


/////////////////////////////////////////////////////////////////////////////
// ReminderCollectionEvents properties

/////////////////////////////////////////////////////////////////////////////
// ReminderCollectionEvents operations

void ReminderCollectionEvents::BeforeReminderShow(BOOL* Cancel)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0xfa93, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cancel);
}

void ReminderCollectionEvents::ReminderAdd(LPDISPATCH ReminderObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa94, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ReminderObject);
}

void ReminderCollectionEvents::ReminderChange(LPDISPATCH ReminderObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa95, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ReminderObject);
}

void ReminderCollectionEvents::ReminderFire(LPDISPATCH ReminderObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa96, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ReminderObject);
}

void ReminderCollectionEvents::ReminderRemove()
{
	InvokeHelper(0xfa97, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ReminderCollectionEvents::Snooze(LPDISPATCH ReminderObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa98, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ReminderObject);
}


/////////////////////////////////////////////////////////////////////////////
// _DistListItem properties

/////////////////////////////////////////////////////////////////////////////
// _DistListItem operations

LPDISPATCH _DistListItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DistListItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DistListItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DistListItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DistListItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DistListItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _DistListItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DistListItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _DistListItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DistListItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _DistListItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _DistListItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _DistListItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _DistListItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _DistListItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _DistListItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _DistListItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DistListItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _DistListItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DistListItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DistListItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _DistListItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _DistListItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DistListItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DistListItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

CString _DistListItem::GetDLName()
{
	CString result;
	InvokeHelper(0x8053, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetDLName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8053, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _DistListItem::GetMemberCount()
{
	long result;
	InvokeHelper(0x804b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DistListItem::AddMembers(LPDISPATCH Recipients)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf900, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipients);
}

void _DistListItem::RemoveMembers(LPDISPATCH Recipients)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf901, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipients);
}

LPDISPATCH _DistListItem::GetMember(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf905, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

long _DistListItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DistListItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DistListItem::AddMember(LPDISPATCH Recipient)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipient);
}

void _DistListItem::RemoveMember(LPDISPATCH Recipient)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfa8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipient);
}

LPDISPATCH _DistListItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DistListItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _DistListItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _DistListItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DistListItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _DistListItem::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _DistListItem::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _DistListItem::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _DistListItem::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _DistListItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _DistListItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _DistListItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _DistListItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _DistListItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _DistListItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _DistListItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _DistListItem::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _DistListItem::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _DistListItem::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _DocumentItem properties

/////////////////////////////////////////////////////////////////////////////
// _DocumentItem operations

LPDISPATCH _DocumentItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DocumentItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DocumentItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DocumentItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DocumentItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DocumentItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _DocumentItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DocumentItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _DocumentItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _DocumentItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _DocumentItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _DocumentItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _DocumentItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _DocumentItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _DocumentItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DocumentItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _DocumentItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _DocumentItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DocumentItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _DocumentItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _DocumentItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DocumentItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _DocumentItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _DocumentItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DocumentItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DocumentItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _DocumentItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DocumentItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DocumentItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _DocumentItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _DocumentItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _DocumentItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _DocumentItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _DocumentItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _DocumentItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}
*/
/*
/////////////////////////////////////////////////////////////////////////////
// _PostItem properties

/////////////////////////////////////////////////////////////////////////////
// _PostItem operations

LPDISPATCH _PostItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _PostItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _PostItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _PostItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _PostItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _PostItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _PostItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _PostItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _PostItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _PostItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _PostItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PostItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _PostItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _PostItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _PostItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _PostItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _PostItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PostItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _PostItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _PostItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _PostItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _PostItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _PostItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _PostItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _PostItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _PostItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _PostItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

DATE _PostItem::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _PostItem::GetHTMLBody()
{
	CString result;
	InvokeHelper(0xf404, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetHTMLBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf404, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _PostItem::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _PostItem::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::ClearConversationIndex()
{
	InvokeHelper(0xf822, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _PostItem::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _PostItem::Post()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _PostItem::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _PostItem::GetBodyFormat()
{
	long result;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::SetBodyFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _PostItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _PostItem::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _PostItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _PostItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _PostItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _PostItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetSenderEmailAddress()
{
	CString result;
	InvokeHelper(0xc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetSenderEmailType()
{
	CString result;
	InvokeHelper(0xc1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _PostItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _PostItem::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _PostItem::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _PostItem::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _PostItem::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _PostItem::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _PostItem::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PostItem::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _PostItem::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PostItem::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _PostItem::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _PostItem::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _PostItem::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _PostItem::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _PostItem::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _PostItem::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _PostItem::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _PostItem::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _PostItem::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _RemoteItem properties

/////////////////////////////////////////////////////////////////////////////
// _RemoteItem operations

LPDISPATCH _RemoteItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _RemoteItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _RemoteItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _RemoteItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _RemoteItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _RemoteItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _RemoteItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _RemoteItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _RemoteItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _RemoteItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _RemoteItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _RemoteItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _RemoteItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _RemoteItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _RemoteItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _RemoteItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _RemoteItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _RemoteItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _RemoteItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _RemoteItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _RemoteItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _RemoteItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

BOOL _RemoteItem::GetHasAttachment()
{
	BOOL result;
	InvokeHelper(0x8f07, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _RemoteItem::GetRemoteMessageClass()
{
	CString result;
	InvokeHelper(0x8f02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetTransferSize()
{
	long result;
	InvokeHelper(0x8f05, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetTransferTime()
{
	long result;
	InvokeHelper(0x8f04, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _RemoteItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _RemoteItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _RemoteItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _RemoteItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _RemoteItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _RemoteItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _RemoteItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ReportItem properties

/////////////////////////////////////////////////////////////////////////////
// _ReportItem operations

LPDISPATCH _ReportItem::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0xf000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ReportItem::GetClass()
{
	long result;
	InvokeHelper(0xf00a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetSession()
{
	LPDISPATCH result;
	InvokeHelper(0xf00b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0xf001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetActions()
{
	LPDISPATCH result;
	InvokeHelper(0xf817, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetAttachments()
{
	LPDISPATCH result;
	InvokeHelper(0xf815, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetBillingInformation()
{
	CString result;
	InvokeHelper(0x8535, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetBillingInformation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8535, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ReportItem::GetBody()
{
	CString result;
	InvokeHelper(0x9100, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9100, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ReportItem::GetCategories()
{
	CString result;
	InvokeHelper(0x9001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetCategories(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ReportItem::GetCompanies()
{
	CString result;
	InvokeHelper(0x853b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetCompanies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x853b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ReportItem::GetConversationIndex()
{
	CString result;
	InvokeHelper(0xfac0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetConversationTopic()
{
	CString result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _ReportItem::GetCreationTime()
{
	DATE result;
	InvokeHelper(0x3007, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetEntryID()
{
	CString result;
	InvokeHelper(0xf01e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetFormDescription()
{
	LPDISPATCH result;
	InvokeHelper(0xf095, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetGetInspector()
{
	LPDISPATCH result;
	InvokeHelper(0xf03e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ReportItem::GetImportance()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetImportance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _ReportItem::GetLastModificationTime()
{
	DATE result;
	InvokeHelper(0x3008, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetMessageClass()
{
	CString result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetMessageClass(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _ReportItem::GetMileage()
{
	CString result;
	InvokeHelper(0x8534, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetMileage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8534, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _ReportItem::GetNoAging()
{
	BOOL result;
	InvokeHelper(0x850e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetNoAging(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x850e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _ReportItem::GetOutlookInternalVersion()
{
	long result;
	InvokeHelper(0x8552, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetOutlookVersion()
{
	CString result;
	InvokeHelper(0x8554, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _ReportItem::GetSaved()
{
	BOOL result;
	InvokeHelper(0xf0a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _ReportItem::GetSensitivity()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetSensitivity(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _ReportItem::GetSize()
{
	long result;
	InvokeHelper(0xe08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _ReportItem::GetSubject()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _ReportItem::GetUnRead()
{
	BOOL result;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetUnRead(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf01c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _ReportItem::GetUserProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xf816, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _ReportItem::Close(long SaveMode)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf023, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveMode);
}

LPDISPATCH _ReportItem::Copy()
{
	LPDISPATCH result;
	InvokeHelper(0xf032, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _ReportItem::Delete()
{
	InvokeHelper(0xf04a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ReportItem::Display(const VARIANT& Modal)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xf0a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Modal);
}

LPDISPATCH _ReportItem::Move(LPDISPATCH DestFldr)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf034, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DestFldr);
	return result;
}

void _ReportItem::PrintOut()
{
	InvokeHelper(0xf033, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ReportItem::Save()
{
	InvokeHelper(0xf048, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ReportItem::SaveAs(LPCTSTR Path, const VARIANT& Type)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0xf051, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path, &Type);
}

LPDISPATCH _ReportItem::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ReportItem::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ReportItem::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _ReportItem::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _ReportItem::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _ReportItem::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _ReportItem::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _ReportItem::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _ReportItem::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

*/
