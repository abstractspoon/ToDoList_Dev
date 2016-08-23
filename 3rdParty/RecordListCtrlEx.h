// ==========================================================================
// CRecordListCtrl.h
// 
// Author : Vitali Halershtein
//          vitali2001by@yahoo.co.uk
//
// Last Modified : 7.09.2003
// by            : Vitali Halershtein
// Copyright  :
// This software is released into the public domain. You are free to use it in any way you like.
// You should save Author text in the header files. If you modify it or extend it, please consider 
// posting new code here for everyone to share. This software is provided "as is" with no 
// expressed or implied warranty. I accept no liability for any damage or loss of business
// that this software may cause. 
// ==========================================================================

#ifndef RECORDLISTCTRL_H_
#define RECORDLISTCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxdb.h>
//=======================================================
// This is include to CListCtrlEx from:
//#include "\\ListCtrlEx\\Inc\ListCtrlEx.h" // type correct path here
// Do uncomment corresponding code fragments
// ======================================================

class CRecordListCtrl : public CListCtrl//CListCtrlEx
{
public:
    DECLARE_DYNCREATE(CRecordListCtrl);
	CRecordListCtrl();
	virtual ~CRecordListCtrl();
	
public:
    BOOL Create(CRect& rc,CWnd* pParentWnd, UINT nID );
//     CString ParseSQL(CString aSQL);
	int LoadListCtrl(CString aDSN, CString aSQL);
    
protected:
	//{{AFX_MSG(CRecordListCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif 
