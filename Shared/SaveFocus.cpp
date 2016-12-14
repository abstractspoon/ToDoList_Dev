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
}

CSaveFocus::~CSaveFocus()
{
	if (::IsWindow(m_hwnd) && ::IsWindowEnabled(m_hwnd))
	{
		::SetFocus(m_hwnd);

		if (m_bEdit)
		{
			CHARRANGE crSel;
			::SendMessage(m_hwnd, EM_GETSEL, (WPARAM)&crSel.cpMin, (LPARAM)&crSel.cpMax);

			if (crSel.cpMin != m_crSel.cpMin || crSel.cpMax != m_crSel.cpMax)
				::SendMessage(m_hwnd, EM_SETSEL, (WPARAM)m_crSel.cpMin, (LPARAM)m_crSel.cpMax);
		}
	}
}
