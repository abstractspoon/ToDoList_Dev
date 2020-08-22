// soundedit.cpp : implementation file
//

#include "stdafx.h"
#include "soundedit.h"
#include "filemisc.h"

#pragma warning(disable: 4201)
#include <Mmsystem.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// for PlaySound
#pragma comment(lib, "winmm.lib")

/////////////////////////////////////////////////////////////////////////////

#define ID_PLAYSOUNDBTN 0xfff0
#define NO_SOUND _T("None")

/////////////////////////////////////////////////////////////////////////////
// CSoundEdit

CString CSoundEdit::s_sFilter = _T("");
CString CSoundEdit::s_sPlayBtnTip = _T("Play");

/////////////////////////////////////////////////////////////////////////////

CSoundEdit::CSoundEdit() : CFileEdit(FES_COMBOSTYLEBTN)
{
	SetFilter(s_sFilter);
	AddButton(ID_PLAYSOUNDBTN, 0x38, s_sPlayBtnTip, CALC_BTNWIDTH, _T("Marlett"));
	SetButtonTip(FEBTN_BROWSE, s_sBrowseBtnTip);
}

CSoundEdit::~CSoundEdit()
{
}


BEGIN_MESSAGE_MAP(CSoundEdit, CFileEdit)
	//{{AFX_MSG_MAP(CSoundEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundEdit message handlers

void CSoundEdit::OnBtnClick(UINT nID)
{
	if (nID == ID_PLAYSOUNDBTN)
	{
		CString sFilePath;
		GetWindowText(sFilePath);

		if (!sFilePath.IsEmpty())
			PlaySound(sFilePath, NULL, (SND_FILENAME | SND_ASYNC));

		return;
	}

	// else
	CFileEdit::OnBtnClick(nID);
}

CString CSoundEdit::GetWindowsSound(LPCTSTR szSoundFile)
{
	CString sSoundPath = FileMisc::GetWindowsFolder() + _T("\\media\\") + szSoundFile;

	if (FileMisc::GetExtension(sSoundPath, FALSE).IsEmpty())
		sSoundPath += _T(".wav");

	return sSoundPath;
}
