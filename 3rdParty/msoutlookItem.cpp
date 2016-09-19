// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msoutlookItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OutlookAPI;


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

CString _Item::GetAccount()
{
	CString result;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetAccount(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a00, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetAnniversary()
{
	DATE result;
	InvokeHelper(0x3a41, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetAnniversary(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x3a41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _Item::GetAssistantName()
{
	CString result;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetAssistantName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetAssistantTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetAssistantTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetBirthday()
{
	DATE result;
	InvokeHelper(0x3a42, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetBirthday(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x3a42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _Item::GetBusiness2TelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusiness2TelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddress()
{
	CString result;
	InvokeHelper(0x801b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressCity()
{
	CString result;
	InvokeHelper(0x8046, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8046, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressCountry()
{
	CString result;
	InvokeHelper(0x8049, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressPostalCode()
{
	CString result;
	InvokeHelper(0x8048, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8048, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x804a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x804a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressState()
{
	CString result;
	InvokeHelper(0x8047, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessAddressStreet()
{
	CString result;
	InvokeHelper(0x8045, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8045, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessFaxNumber()
{
	CString result;
	InvokeHelper(0x3a24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessHomePage()
{
	CString result;
	InvokeHelper(0x3a51, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessHomePage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a51, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetBusinessTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a08, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCallbackTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCallbackTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCarTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCarTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetChildren()
{
	CString result;
	InvokeHelper(0x800c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetChildren(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x800c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCompanyAndFullName()
{
	CString result;
	InvokeHelper(0x8018, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetCompanyLastFirstNoSpace()
{
	CString result;
	InvokeHelper(0x8032, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetCompanyLastFirstSpaceOnly()
{
	CString result;
	InvokeHelper(0x8033, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetCompanyMainTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a57, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCompanyMainTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCompanyName()
{
	CString result;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetComputerNetworkName()
{
	CString result;
	InvokeHelper(0x3a49, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetComputerNetworkName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCustomerID()
{
	CString result;
	InvokeHelper(0x3a4a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCustomerID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetDepartment()
{
	CString result;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetDepartment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail1Address()
{
	CString result;
	InvokeHelper(0x8083, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail1Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8083, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail1AddressType()
{
	CString result;
	InvokeHelper(0x8082, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail1AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8082, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail1DisplayName()
{
	CString result;
	InvokeHelper(0x8080, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetEmail1EntryID()
{
	CString result;
	InvokeHelper(0x8085, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetEmail2Address()
{
	CString result;
	InvokeHelper(0x8093, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail2Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8093, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail2AddressType()
{
	CString result;
	InvokeHelper(0x8092, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail2AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8092, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail2DisplayName()
{
	CString result;
	InvokeHelper(0x8090, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetEmail2EntryID()
{
	CString result;
	InvokeHelper(0x8095, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetEmail3Address()
{
	CString result;
	InvokeHelper(0x80a3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail3Address(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail3AddressType()
{
	CString result;
	InvokeHelper(0x80a2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetEmail3AddressType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetEmail3DisplayName()
{
	CString result;
	InvokeHelper(0x80a0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetEmail3EntryID()
{
	CString result;
	InvokeHelper(0x80a5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetFileAs()
{
	CString result;
	InvokeHelper(0x8005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetFileAs(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetFirstName()
{
	CString result;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetFTPSite()
{
	CString result;
	InvokeHelper(0x3a4c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetFTPSite(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetFullName()
{
	CString result;
	InvokeHelper(0x3001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetFullName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetFullNameAndCompany()
{
	CString result;
	InvokeHelper(0x8019, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Item::GetGender()
{
	long result;
	InvokeHelper(0x3a4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetGender(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3a4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetGovernmentIDNumber()
{
	CString result;
	InvokeHelper(0x3a07, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetGovernmentIDNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a07, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHobby()
{
	CString result;
	InvokeHelper(0x3a43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHobby(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHome2TelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHome2TelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddress()
{
	CString result;
	InvokeHelper(0x801a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressCity()
{
	CString result;
	InvokeHelper(0x3a59, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressCountry()
{
	CString result;
	InvokeHelper(0x3a5a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a5b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a5e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressState()
{
	CString result;
	InvokeHelper(0x3a5c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeAddressStreet()
{
	CString result;
	InvokeHelper(0x3a5d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeFaxNumber()
{
	CString result;
	InvokeHelper(0x3a25, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHomeTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a09, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHomeTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a09, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetInitials()
{
	CString result;
	InvokeHelper(0x3a0a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetInitials(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetInternetFreeBusyAddress()
{
	CString result;
	InvokeHelper(0x80d8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetInternetFreeBusyAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80d8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetISDNNumber()
{
	CString result;
	InvokeHelper(0x3a2d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetISDNNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetJobTitle()
{
	CString result;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetJobTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetJournal()
{
	BOOL result;
	InvokeHelper(0x8025, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetJournal(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8025, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Item::GetLanguage()
{
	CString result;
	InvokeHelper(0x3a0c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetLanguage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetLastFirstAndSuffix()
{
	CString result;
	InvokeHelper(0x8036, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastFirstNoSpace()
{
	CString result;
	InvokeHelper(0x8030, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastFirstNoSpaceCompany()
{
	CString result;
	InvokeHelper(0x8034, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastFirstSpaceOnly()
{
	CString result;
	InvokeHelper(0x8031, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastFirstSpaceOnlyCompany()
{
	CString result;
	InvokeHelper(0x8035, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastName()
{
	CString result;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetLastNameAndFirstName()
{
	CString result;
	InvokeHelper(0x8017, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetMailingAddress()
{
	CString result;
	InvokeHelper(0x3a15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressCity()
{
	CString result;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressCountry()
{
	CString result;
	InvokeHelper(0x3a26, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a2b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressState()
{
	CString result;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMailingAddressStreet()
{
	CString result;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMailingAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetManagerName()
{
	CString result;
	InvokeHelper(0x3a4e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetManagerName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMiddleName()
{
	CString result;
	InvokeHelper(0x3a44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMiddleName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetMobileTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetMobileTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetNetMeetingAlias()
{
	CString result;
	InvokeHelper(0x805f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetNetMeetingAlias(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x805f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetNetMeetingServer()
{
	CString result;
	InvokeHelper(0x8060, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetNetMeetingServer(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8060, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetNickName()
{
	CString result;
	InvokeHelper(0x3a4f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetNickName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOfficeLocation()
{
	CString result;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOfficeLocation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOrganizationalIDNumber()
{
	CString result;
	InvokeHelper(0x3a10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOrganizationalIDNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddress()
{
	CString result;
	InvokeHelper(0x801c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x801c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressCity()
{
	CString result;
	InvokeHelper(0x3a5f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressCity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressCountry()
{
	CString result;
	InvokeHelper(0x3a60, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressCountry(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressPostalCode()
{
	CString result;
	InvokeHelper(0x3a61, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressPostalCode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressPostOfficeBox()
{
	CString result;
	InvokeHelper(0x3a64, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressPostOfficeBox(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressState()
{
	CString result;
	InvokeHelper(0x3a62, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressState(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherAddressStreet()
{
	CString result;
	InvokeHelper(0x3a63, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherAddressStreet(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherFaxNumber()
{
	CString result;
	InvokeHelper(0x3a23, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherFaxNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOtherTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOtherTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetPagerNumber()
{
	CString result;
	InvokeHelper(0x3a21, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetPagerNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetPersonalHomePage()
{
	CString result;
	InvokeHelper(0x3a50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetPersonalHomePage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetPrimaryTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetPrimaryTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetProfession()
{
	CString result;
	InvokeHelper(0x3a46, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetProfession(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetRadioTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetRadioTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetReferredBy()
{
	CString result;
	InvokeHelper(0x3a47, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetReferredBy(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Item::GetSelectedMailingAddress()
{
	long result;
	InvokeHelper(0x8022, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetSelectedMailingAddress(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetSpouse()
{
	CString result;
	InvokeHelper(0x3a48, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetSpouse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetSuffix()
{
	CString result;
	InvokeHelper(0x3a05, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetSuffix(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetTelexNumber()
{
	CString result;
	InvokeHelper(0x3a2c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetTelexNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetTitle()
{
	CString result;
	InvokeHelper(0x3a45, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetTitle(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetTTYTDDTelephoneNumber()
{
	CString result;
	InvokeHelper(0x3a4b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetTTYTDDTelephoneNumber(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3a4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetUser1()
{
	CString result;
	InvokeHelper(0x804f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetUser1(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x804f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetUser2()
{
	CString result;
	InvokeHelper(0x8050, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetUser2(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8050, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetUser3()
{
	CString result;
	InvokeHelper(0x8051, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetUser3(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8051, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetUser4()
{
	CString result;
	InvokeHelper(0x8052, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetUser4(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8052, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetWebPage()
{
	CString result;
	InvokeHelper(0x802b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetWebPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetYomiCompanyName()
{
	CString result;
	InvokeHelper(0x802e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetYomiCompanyName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetYomiFirstName()
{
	CString result;
	InvokeHelper(0x802c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetYomiFirstName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetYomiLastName()
{
	CString result;
	InvokeHelper(0x802d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetYomiLastName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x802d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Item::ForwardAsVcard()
{
	LPDISPATCH result;
	InvokeHelper(0xf8a1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetLinks()
{
	LPDISPATCH result;
	InvokeHelper(0xf405, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetItemProperties()
{
	LPDISPATCH result;
	InvokeHelper(0xfa09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Item::GetLastFirstNoSpaceAndSuffix()
{
	CString result;
	InvokeHelper(0x8038, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Item::GetDownloadState()
{
	long result;
	InvokeHelper(0xfa4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::ShowCategoriesDialog()
{
	InvokeHelper(0xfa0b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _Item::GetIMAddress()
{
	CString result;
	InvokeHelper(0x8062, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetIMAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8062, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Item::GetMarkForDownload()
{
	long result;
	InvokeHelper(0x8571, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetMarkForDownload(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8571, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Item::SetEmail1DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8080, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Item::SetEmail2DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8090, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Item::SetEmail3DisplayName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetIsConflict()
{
	BOOL result;
	InvokeHelper(0xfaa4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Item::GetAutoResolvedWinner()
{
	BOOL result;
	InvokeHelper(0xfaba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetConflicts()
{
	LPDISPATCH result;
	InvokeHelper(0xfabb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::AddPicture(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfabd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

void _Item::RemovePicture()
{
	InvokeHelper(0xfabe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Item::GetHasPicture()
{
	BOOL result;
	InvokeHelper(0xfabf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetPropertyAccessor()
{
	LPDISPATCH result;
	InvokeHelper(0xfafd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::ForwardAsBusinessCard()
{
	LPDISPATCH result;
	InvokeHelper(0xfb94, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::ShowBusinessCardEditor()
{
	InvokeHelper(0xfb95, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::SaveBusinessCardImage(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfb97, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

void _Item::ShowCheckPhoneDialog(long PhoneNumber)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PhoneNumber);
}

CString _Item::GetTaskSubject()
{
	CString result;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetTaskSubject(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetTaskDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetTaskDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetTaskStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetTaskStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetTaskCompletedDate()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetTaskCompletedDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetToDoTaskOrdinal()
{
	DATE result;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetToDoTaskOrdinal(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x85a0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Item::GetReminderOverrideDefault()
{
	BOOL result;
	InvokeHelper(0x851c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetReminderOverrideDefault(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetReminderPlaySound()
{
	BOOL result;
	InvokeHelper(0x851e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetReminderPlaySound(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x851e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetReminderSet()
{
	BOOL result;
	InvokeHelper(0x8503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetReminderSet(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Item::GetReminderSoundFile()
{
	CString result;
	InvokeHelper(0x851f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetReminderSoundFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x851f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetReminderTime()
{
	DATE result;
	InvokeHelper(0x8502, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetReminderTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _Item::MarkAsTask(long MarkInterval)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfbfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MarkInterval);
}

void _Item::ClearTaskFlag()
{
	InvokeHelper(0xfc09, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Item::GetIsMarkedAsTask()
{
	BOOL result;
	InvokeHelper(0xfc0a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Item::GetBusinessCardLayoutXml()
{
	CString result;
	InvokeHelper(0xfc0d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBusinessCardLayoutXml(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc0d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Item::ResetBusinessCard()
{
	InvokeHelper(0xfc0e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::AddBusinessCardLogoPicture(LPCTSTR Path)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xfc0f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Path);
}

long _Item::GetBusinessCardType()
{
	long result;
	InvokeHelper(0xfc10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}




/////////////////////////////////////////////////////////////////////////////
// _Item operations

BOOL _Item::GetAlternateRecipientAllowed()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetAlternateRecipientAllowed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetAutoForwarded()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetAutoForwarded(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Item::GetBcc()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetBcc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetCc()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCc(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetDeferredDeliveryTime()
{
	DATE result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetDeferredDeliveryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Item::GetDeleteAfterSubmit()
{
	BOOL result;
	InvokeHelper(0xe01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetDeleteAfterSubmit(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _Item::GetExpiryTime()
{
	DATE result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetExpiryTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _Item::GetFlagRequest()
{
	CString result;
	InvokeHelper(0x8530, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetFlagRequest(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8530, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetHTMLBody()
{
	CString result;
	InvokeHelper(0xf404, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetHTMLBody(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf404, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetOriginatorDeliveryReportRequested()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetOriginatorDeliveryReportRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetReadReceiptRequested()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetReadReceiptRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Item::GetReceivedByEntryID()
{
	CString result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetReceivedByName()
{
	CString result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetReceivedOnBehalfOfEntryID()
{
	CString result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetReceivedOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _Item::GetReceivedTime()
{
	DATE result;
	InvokeHelper(0xe06, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

BOOL _Item::GetRecipientReassignmentProhibited()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetRecipientReassignmentProhibited(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Item::GetRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf814, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Item::GetRemoteStatus()
{
	long result;
	InvokeHelper(0x8511, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetRemoteStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8511, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetReplyRecipientNames()
{
	CString result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetReplyRecipients()
{
	LPDISPATCH result;
	InvokeHelper(0xf013, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetSaveSentMessageFolder()
{
	LPDISPATCH result;
	InvokeHelper(0xf401, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::SetRefSaveSentMessageFolder(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf401, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _Item::GetSenderName()
{
	CString result;
	InvokeHelper(0xc1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Item::GetSent()
{
	BOOL result;
	InvokeHelper(0xf402, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

DATE _Item::GetSentOn()
{
	DATE result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

CString _Item::GetSentOnBehalfOfName()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetSentOnBehalfOfName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetSubmitted()
{
	BOOL result;
	InvokeHelper(0xf403, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Item::GetTo()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetTo(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetVotingOptions()
{
	CString result;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetVotingOptions(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf01b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetVotingResponse()
{
	CString result;
	InvokeHelper(0x8524, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetVotingResponse(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8524, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Item::ClearConversationIndex()
{
	InvokeHelper(0xf822, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Item::Forward()
{
	LPDISPATCH result;
	InvokeHelper(0xf813, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::Reply()
{
	LPDISPATCH result;
	InvokeHelper(0xf810, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::ReplyAll()
{
	LPDISPATCH result;
	InvokeHelper(0xf811, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::Send()
{
	InvokeHelper(0xf075, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Item::GetBodyFormat()
{
	long result;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetBodyFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfa49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetInternetCodepage()
{
	long result;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetInternetCodepage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3fde, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetSenderEmailAddress()
{
	CString result;
	InvokeHelper(0xc1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetSenderEmailType()
{
	CString result;
	InvokeHelper(0xc1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Item::GetPermission()
{
	long result;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetPermission(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfac6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetPermissionService()
{
	long result;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetPermissionService(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfacb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Item::AddBusinessCard(LPDISPATCH contact)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfb96, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 contact);
}

LPDISPATCH _Item::GetSendUsingAccount()
{
	LPDISPATCH result;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::SetSendUsingAccount(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfad1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

/////////////////////////////////////////////////////////////////////////////
// _Item operations

BOOL _Item::GetAllDayEvent()
{
	BOOL result;
	InvokeHelper(0x8215, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetAllDayEvent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8215, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Item::GetBusyStatus()
{
	long result;
	InvokeHelper(0x8205, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetBusyStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8205, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetDuration()
{
	long result;
	InvokeHelper(0x8213, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetDuration(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8213, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

DATE _Item::GetEnd()
{
	DATE result;
	InvokeHelper(0x820e, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetEnd(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x820e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Item::GetIsRecurring()
{
	BOOL result;
	InvokeHelper(0x8223, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Item::GetLocation()
{
	CString result;
	InvokeHelper(0x8208, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetLocation(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8208, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Item::GetMeetingStatus()
{
	long result;
	InvokeHelper(0x8217, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetMeetingStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8217, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetOptionalAttendees()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOptionalAttendees(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetOrganizer()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Item::GetRecurrenceState()
{
	long result;
	InvokeHelper(0xf545, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

DATE _Item::GetReplyTime()
{
	DATE result;
	InvokeHelper(0x8220, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetReplyTime(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8220, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString _Item::GetRequiredAttendees()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetRequiredAttendees(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetResources()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetResources(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetResponseRequested()
{
	BOOL result;
	InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetResponseRequested(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Item::GetResponseStatus()
{
	long result;
	InvokeHelper(0x8218, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

DATE _Item::GetStart()
{
	DATE result;
	InvokeHelper(0x820d, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetStart(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x820d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _Item::ClearRecurrencePattern()
{
	InvokeHelper(0xf0a5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Item::ForwardAsVcal()
{
	LPDISPATCH result;
	InvokeHelper(0xf547, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::GetRecurrencePattern()
{
	LPDISPATCH result;
	InvokeHelper(0xf0a4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf502, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Response, &fNoUI, &fAdditionalTextDialog);
	return result;
}

CString _Item::GetMeetingWorkspaceURL()
{
	CString result;
	InvokeHelper(0x8209, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Item::GetGlobalAppointmentID()
{
	CString result;
	InvokeHelper(0xfbfd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Item::GetForceUpdateToAllAttendees()
{
	BOOL result;
	InvokeHelper(0xf53a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetForceUpdateToAllAttendees(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf53a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _Item::GetStartUTC()
{
	DATE result;
	InvokeHelper(0x8268, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetStartUTC(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8268, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetEndUTC()
{
	DATE result;
	InvokeHelper(0x8269, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetEndUTC(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8269, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetStartInStartTimeZone()
{
	DATE result;
	InvokeHelper(0x8266, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetStartInStartTimeZone(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8266, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

DATE _Item::GetEndInEndTimeZone()
{
	DATE result;
	InvokeHelper(0x8267, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetEndInEndTimeZone(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8267, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Item::GetStartTimeZone()
{
	LPDISPATCH result;
	InvokeHelper(0xfc27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::SetStartTimeZone(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfc27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Item::GetEndTimeZone()
{
	LPDISPATCH result;
	InvokeHelper(0xfc28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::SetEndTimeZone(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfc28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

/////////////////////////////////////////////////////////////////////////

CString _Item::GetContactNames()
{
	CString result;
	InvokeHelper(0xe04, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetContactNames(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetDocPosted()
{
	BOOL result;
	InvokeHelper(0x8711, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetDocPosted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8711, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetDocPrinted()
{
	BOOL result;
	InvokeHelper(0x870e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetDocPrinted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x870e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetDocRouted()
{
	BOOL result;
	InvokeHelper(0x8710, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetDocRouted(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8710, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Item::GetDocSaved()
{
	BOOL result;
	InvokeHelper(0x870f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetDocSaved(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x870f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Item::GetType()
{
	CString result;
	InvokeHelper(0x8700, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetType(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8700, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Item::StartTimer()
{
	InvokeHelper(0xf725, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::StopTimer()
{
	InvokeHelper(0xf726, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// _Item operations


void _Item::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetLeft()
{
	long result;
	InvokeHelper(0x8b04, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetTop()
{
	long result;
	InvokeHelper(0x8b05, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetWidth()
{
	long result;
	InvokeHelper(0x8b02, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetActualWork()
{
	long result;
	InvokeHelper(0x8110, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetActualWork(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8110, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetCardData()
{
	CString result;
	InvokeHelper(0x812b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetCardData(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x812b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetComplete()
{
	BOOL result;
	InvokeHelper(0x811c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetComplete(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x811c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

DATE _Item::GetDateCompleted()
{
	DATE result;
	InvokeHelper(0x810f, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetDateCompleted(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x810f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Item::GetDelegationState()
{
	long result;
	InvokeHelper(0x812a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Item::GetDelegator()
{
	CString result;
	InvokeHelper(0x8121, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

DATE _Item::GetDueDate()
{
	DATE result;
	InvokeHelper(0x8105, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetDueDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Item::GetOrdinal()
{
	long result;
	InvokeHelper(0x8123, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetOrdinal(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8123, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetOwner()
{
	CString result;
	InvokeHelper(0x811f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetOwner(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x811f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long _Item::GetOwnership()
{
	long result;
	InvokeHelper(0x8129, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Item::GetPercentComplete()
{
	long result;
	InvokeHelper(0xf61f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetPercentComplete(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf61f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Item::GetResponseState()
{
	long result;
	InvokeHelper(0xf623, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString _Item::GetRole()
{
	CString result;
	InvokeHelper(0x8127, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetRole(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8127, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetSchedulePlusPriority()
{
	CString result;
	InvokeHelper(0x812f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetSchedulePlusPriority(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x812f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

DATE _Item::GetStartDate()
{
	DATE result;
	InvokeHelper(0x8104, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
	return result;
}

void _Item::SetStartDate(DATE newValue)
{
	static BYTE parms[] =
		VTS_DATE;
	InvokeHelper(0x8104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Item::GetStatus()
{
	long result;
	InvokeHelper(0x8101, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetStatus(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8101, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Item::GetStatusOnCompletionRecipients()
{
	CString result;
	InvokeHelper(0xe02, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetStatusOnCompletionRecipients(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Item::GetStatusUpdateRecipients()
{
	CString result;
	InvokeHelper(0xe03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Item::SetStatusUpdateRecipients(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Item::GetTeamTask()
{
	BOOL result;
	InvokeHelper(0x8103, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Item::SetTeamTask(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8103, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Item::GetTotalWork()
{
	long result;
	InvokeHelper(0x8111, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Item::SetTotalWork(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8111, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Item::Assign()
{
	LPDISPATCH result;
	InvokeHelper(0xf620, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Item::CancelResponseState()
{
	InvokeHelper(0xf622, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Item::MarkComplete()
{
	InvokeHelper(0xf60d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Item::SkipRecurrence()
{
	BOOL result;
	InvokeHelper(0xf624, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Item::StatusReport()
{
	LPDISPATCH result;
	InvokeHelper(0xf612, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

