#if !defined(AFX_SOUNDEDIT_H__08AF667E_8517_45FD_BBCD_0AA50DC6E4E6__INCLUDED_)
#define AFX_SOUNDEDIT_H__08AF667E_8517_45FD_BBCD_0AA50DC6E4E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// soundedit.h : header file
//

#include "FileEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSoundEdit window

class CSoundEdit : public CFileEdit
{
// Construction
public:
	CSoundEdit();
	virtual ~CSoundEdit();

	static void SetDefaultFilter(LPCTSTR szFilter) { s_sFilter = szFilter; }
	static void SetDefaultPlayButtonTip(LPCTSTR szTip) { s_sPlayBtnTip = szTip; }

	static CString GetWindowsSound(LPCTSTR szSoundFile);
	
// Implementation
protected:
	static CString s_sFilter;
	static CString s_sPlayBtnTip;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSoundEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnBtnClick(UINT nID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDEDIT_H__08AF667E_8517_45FD_BBCD_0AA50DC6E4E6__INCLUDED_)
