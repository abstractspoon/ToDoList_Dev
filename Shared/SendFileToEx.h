#ifndef __SENDFILETOEX_H__
#define __SENDFILETOEX_H__

#include "..\3rdParty\SendFileTo.h"

class CSendFileToEx
{
public:
	static bool SendMail(HWND hWndParent, 
						const CString& strTo=_T(""), 
						const CString& strSubject=_T(""), 
						const CString& strBody=_T(""), 
						const CString& strAttachmentFileName=_T(""))
	{
		bool bResult = false;

#ifdef _UNICODE
		LPCSTR lpszName = (LPCSTR)Misc::WideToMultiByte(strTo);
		LPCSTR lpszPathName = (LPCSTR)Misc::WideToMultiByte(strAttachmentFileName);
		LPCSTR lpszSubject = (LPCSTR)Misc::WideToMultiByte(strSubject);
		LPCSTR lpszNoteText = (LPCSTR)Misc::WideToMultiByte(strBody);

		bResult = CSendFileTo::SendMail(hWndParent, lpszName, lpszSubject, lpszNoteText, lpszPathName);

		delete lpszName;
		delete lpszPathName;
		delete lpszSubject;
		delete lpszNoteText;
#else
		bResult = CSendFileTo::SendMail(hWndParent, strTo, strSubject, strBody, strAttachmentFileName);
#endif		

		return bResult;
	}
};

#endif