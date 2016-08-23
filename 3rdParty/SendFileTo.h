#ifndef __SENDFILETO_H__
#define __SENDFILETO_H__

#include "..\shared\misc.h"

#include <mapi.h>

#ifndef ULONG_PTR
#define ULONG_PTR ULONG 
#endif

class CSendFileTo
{
public:
	static bool SendMail(HWND hWndParent, 
						const CString& strTo=_T(""), 
						const CString& strSubject=_T(""), 
						const CString& strBody=_T(""), 
						const CString& strAttachmentFileName=_T(""))
	{
		if (!hWndParent || !::IsWindow(hWndParent))
			return false;
		
		HINSTANCE hMAPI = ::LoadLibrary(_T("MAPI32.DLL"));
		if (!hMAPI)
			return false;
		
		typedef ULONG (PASCAL *PFNSENDMAIL)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
		PFNSENDMAIL fnSendMail = (PFNSENDMAIL)GetProcAddress(hMAPI, "MAPISendMail");
		
		if (!fnSendMail)
			return false;
		
		MapiFileDesc fileDesc = { 0 };
		MapiMessage message = { 0 };
		MapiRecipDesc recipient = { 0, MAPI_TO, 0 };

		fileDesc.nPosition = (ULONG)-1;

#ifdef _UNICODE
		recipient.lpszName = (LPSTR)(LPCTSTR)Misc::WideToMultiByte(strTo);
		fileDesc.lpszPathName = (LPSTR)(LPCTSTR)Misc::WideToMultiByte(strAttachmentFileName);
		message.lpszSubject = (LPSTR)(LPCTSTR)Misc::WideToMultiByte(strSubject);
		message.lpszNoteText = (LPSTR)(LPCTSTR)Misc::WideToMultiByte(strBody);
#else
		recipient.lpszName = (LPSTR)(LPCTSTR)strTo;
		fileDesc.lpszPathName = (LPSTR)(LPCTSTR)strAttachmentFileName;
		message.lpszSubject = (LPSTR)(LPCTSTR)strSubject;
		message.lpszNoteText = (LPSTR)(LPCTSTR)strBody;
#endif		

		message.nFileCount = strAttachmentFileName.IsEmpty() ? 0 : 1;
		message.lpFiles = strAttachmentFileName.IsEmpty() ? NULL : &fileDesc;

		message.nRecipCount = strTo.IsEmpty() ? 0 : 1;
		message.lpRecips = strTo.IsEmpty() ? NULL : &recipient;
		
		int nError = fnSendMail(0, (ULONG_PTR)hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

#ifdef _UNICODE
		// cleanup
		delete recipient.lpszName;
		delete fileDesc.lpszPathName;
		delete message.lpszSubject;
		delete message.lpszNoteText;
#endif		
		
		if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
			return false;
		
		return true;
	}
};

#endif