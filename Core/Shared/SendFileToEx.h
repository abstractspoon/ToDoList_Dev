#ifndef __SENDFILETOEX_H__
#define __SENDFILETOEX_H__

#include "..\3rdParty\SendFileTo.h"

class CSendFileToEx
{
public:
	static bool SendMail(HWND hWndParent, 
						const CString& strTo, 
						const CString& strSubject, 
						const CString& strBody = _T(""), 
						const CString& strAttachmentFileName = _T(""))
	{
		LPSTR lpszName = NULL, lpszPathName = NULL, lpszSubject = NULL, lpszNoteText = NULL;
		bool bCleanup = false;

		// Passing the body as a NULL pointer is an undocumented
		// way of indicating to the the mail client it should render
		// any attachment in the body.

		lpszName = Misc::WideToMultiByte(strTo);
		lpszSubject = Misc::WideToMultiByte(strSubject);
		lpszNoteText = (strBody.IsEmpty() ? NULL : Misc::WideToMultiByte(strBody));
		lpszPathName = Misc::WideToMultiByte(strAttachmentFileName);

		bool bResult = CSendFileTo::SendMail(hWndParent, lpszName, lpszSubject, lpszNoteText, lpszPathName);

		delete lpszName;
		delete lpszPathName;
		delete lpszSubject;
		delete lpszNoteText;

		return bResult;
	}
};

#endif