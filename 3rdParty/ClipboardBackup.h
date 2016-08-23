// ClipboardBackup.h: interface for the CClipboardBackup class.
//
// Copyright 2006 (c) RegExLab.com
//
// Author: Ê·ÊÙÎ° (sswater shi)
//
// 2006/05/17 01:21:10
//

#if !defined(AFX_CLIPBOARDBACKUP_H__B2363083_E96C_4F7E_8D2D_39A0C3A7046C__INCLUDED_)
#define AFX_CLIPBOARDBACKUP_H__B2363083_E96C_4F7E_8D2D_39A0C3A7046C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxTempl.h>

/*
#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12
#define CF_UNICODETEXT      13
#define CF_ENHMETAFILE      14

#if(WINVER >= 0x0400)
#define CF_HDROP            15
#define CF_LOCALE           16
#define CF_MAX              17
#endif // WINVER >= 0x0400

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E
*/

class CClipboardBackup
{
public:
	BOOL Backup();
	BOOL Restore();
	BOOL AddData(LPCTSTR szData, UINT nFormat);

public:
	CClipboardBackup(HWND hWnd);
	~CClipboardBackup();

private:
	BOOL m_bFilled;
	HWND m_hWnd;
	
	struct ClipboardData
	{
		UINT   m_nFormat;
		TCHAR  m_szFormatName[100];
		HANDLE m_hData;
	};

	CList <ClipboardData, ClipboardData&> m_lstData;
};

#endif // !defined(AFX_CLIPBOARDBACKUP_H__B2363083_E96C_4F7E_8D2D_39A0C3A7046C__INCLUDED_)
