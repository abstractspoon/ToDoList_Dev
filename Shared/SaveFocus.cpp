// SaveFocus.cpp: implementation of the CSaveFocus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveFocus.h"
#include "Winclasses.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveFocus::CSaveFocus() : m_hwnd(NULL), m_bEdit(FALSE)
{
	m_hwnd = ::GetFocus();
	m_bEdit = CWinClasses::IsEditControl(m_hwnd);

	if (m_bEdit)
		::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&m_crSel.cpMin, (LPARAM)&m_crSel.cpMax);
	else
		m_crSel.cpMax = m_crSel.cpMin = 0;

#ifdef _DEBUG
// 	if (m_hwnd)
// 	{
// 		CString sFocus;
// 		CWnd::FromHandle(m_hwnd)->GetWindowText(sFocus);
// 		TRACE(_T("CSaveFocus(%s = %s)\n"), CWinClasses::GetClassEx(m_hwnd), sFocus.Left(100));
// 	}
#endif

}

CSaveFocus::~CSaveFocus()
{
	RestoreFocus();
}

BOOL CSaveFocus::RestoreFocus()
{
	if (!(::IsWindow(m_hwnd) && ::IsWindowEnabled(m_hwnd) && ::IsWindowVisible(m_hwnd)))
	{
		m_hwnd = NULL;
		return FALSE;
	}
	
	if (::GetFocus() != m_hwnd)
	{
#ifdef _DEBUG
// 		CString sFocus;
// 		CWnd::FromHandle(m_hwnd)->GetWindowText(sFocus);
// 		TRACE(_T("~CSaveFocus(%s = %s)\n"), CWinClasses::GetClassEx(m_hwnd), sFocus.Left(100));
#endif

		::SetFocus(m_hwnd);

		if (m_bEdit)
		{
			CHARRANGE crSel;
			::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&crSel.cpMin, (LPARAM)&crSel.cpMax);

			if (crSel.cpMin != m_crSel.cpMin || crSel.cpMax != m_crSel.cpMax)
				::SendMessage(m_hwnd, EM_SETSEL, (WPARAM)m_crSel.cpMin, (LPARAM)m_crSel.cpMax);
		}
	}

	m_hwnd = NULL;
	return TRUE;
}
