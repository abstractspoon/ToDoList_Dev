#ifndef __SENDFILETO_H__
#define __SENDFILETO_H__

#include <mapi.h>

#ifndef ULONG_PTR
#define ULONG_PTR ULONG 
#endif

class CSendFileTo
{
public:
	static bool SendMail(HWND hWndParent, 
						const LPCSTR strTo, 
						const LPCSTR strSubject, 
						const LPCSTR strBody=NULL, 
						const LPCSTR strAttachmentFileName=NULL)
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

		recipient.lpszName = (LPSTR)strTo;
		fileDesc.lpszPathName = (LPSTR)strAttachmentFileName;
		message.lpszSubject = (LPSTR)strSubject;
		message.lpszNoteText = (LPSTR)strBody;

		message.nFileCount = (!strAttachmentFileName || !strAttachmentFileName[0]) ? 0 : 1;
		message.lpFiles = (!strAttachmentFileName || !strAttachmentFileName[0]) ? NULL : &fileDesc;

		message.nRecipCount = (!strTo || !strTo[0]) ? 0 : 1;
		message.lpRecips = (!strTo || !strTo[0]) ? NULL : &recipient;
		
		int nError = fnSendMail(0, (ULONG_PTR)hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

		if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
			return false;
		
		return true;
	}
};

#endif