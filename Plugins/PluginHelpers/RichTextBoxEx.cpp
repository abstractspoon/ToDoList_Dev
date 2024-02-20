﻿
#include "stdafx.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "PluginHelpers.h"
#include "RichTextBoxEx.h"
#include "UIExtension.h"
#include "ContentControl.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>
#include <shared\GraphicsMisc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace IIControls;

////////////////////////////////////////////////////////////////////////////////////////////////

CreateParams^ RichTextBoxEx::CreateParams::get()
{
	auto cp = RichTextBox::CreateParams::get();
	cp->ClassName = L"RichEdit50W";

	return cp;
}

HWND RichTextBoxEx::HWnd()
{
	return Win32::GetHwnd(Handle);
}

void RichTextBoxEx::WndProc(Message% m)
{
	const int WM_REFLECT = (WM_USER + 0x1C00);

	if (m_LinkTip != nullptr)
		m_LinkTip->ProcessMessage(m);

	switch (m.Msg)
	{
	case WM_NOTIFY + WM_REFLECT:
		{
			NMHDR* pNMHDR = (NMHDR*)(int)m.LParam;

			if (pNMHDR->code == EN_LINK)
			{
				ENLINK* pENL = (ENLINK*)pNMHDR;
				m_CurrentLink = GetTextRange(pENL->chrg);

				switch (pENL->msg)
				{
				case WM_SETCURSOR:
					{
						m_LinkTipVOffset = ToolStripEx::GetActualCursorHeight(Cursors::Arrow); // default

						if (SelectionContainsMessagePos())
						{
							GraphicsMisc::SetStandardCursor(IDC_ARROW);
						}
						else if (Misc::IsKeyPressed(VK_CONTROL))
						{
							GraphicsMisc::SetHandCursor();
						}
						else
						{
							GraphicsMisc::SetStandardCursor(IDC_IBEAM);
							m_LinkTipVOffset /= 2;
						}
						m.Result = (IntPtr)1; // No further processing
					}
					return;

				case WM_LBUTTONDOWN:
					if (!Misc::IsKeyPressed(VK_CONTROL))
					{
						// If pENL->lParam is zero then this is a synthesized 
						// message in response to the <enter> key being pressed
						// when the caret is within a hyperlink
						Drawing::Point ptClient((int)pENL->lParam);

						if (pENL->lParam == 0)
						{
							// Insert a newline at the caret position
							SelectedText = "\r\n";
							return;
						}
						else if (!SelectionContainsPos(ptClient))
						{
							if (!Focused)
								Focus();

 							SelectionLength = 0;
 							SelectionStart = GetCharIndexFromPosition(ptClient);
							return;
						}
					}
					break; // else default behaviour
				}
			}
		}
		break;

	case WM_SETCURSOR:
		{
			m_CurrentLink = String::Empty;
			DefWndProc(m);

			if (!String::IsNullOrEmpty(m_CurrentLink))
			{
				// Initialise label tip first time
				if ((NeedLinkTooltip != nullptr) && (m_LinkTip == nullptr))
					m_LinkTip = gcnew LabelTip(this);

				// Handled above
				m.Result = (IntPtr)1;
				return;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			// Move the Caret to the click location unless
			// the position is already over some selected text
			Drawing::Point ptClient((int)m.LParam);

			if (!SelectionContainsPos(ptClient))
			{
				Focus();

				SelectionLength = 0;
				SelectionStart = GetCharIndexFromPosition(ptClient);
			}
		}
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_MOUSELEAVE:
		m_CurrentLink = String::Empty;
		break;
	}

	RichTextBox::WndProc(m);
}

void RichTextBoxEx::OnLinkClicked(LinkClickedEventArgs^ e)
{
	Debug::Assert(Misc::IsKeyPressed(VK_CONTROL) != FALSE);

	ContentControlWnd::GoToLink(e->LinkText, Parent->Handle, Handle);
}

bool RichTextBoxEx::SelectionContainsPos(Point ptClient)
{
	if (SelectionLength == 0)
		return false;

	int charAtPos = GetCharIndexFromPosition(ptClient);

	if (charAtPos <= SelectionStart)
		return false;

	if (charAtPos >= (SelectionStart + SelectionLength))
		return false;

	return true;
}

bool RichTextBoxEx::SelectionContainsMessagePos()
{
	return SelectionContainsPos(PointToClient(MousePosition));
}

String^ RichTextBoxEx::GetTextRange(const CHARRANGE& cr)
{
	int nLength = int(cr.cpMax - cr.cpMin + 1);

	LPTSTR szChar = new TCHAR[nLength];
	ZeroMemory(szChar, nLength * sizeof(TCHAR));

	TEXTRANGE tr;
	tr.chrg = cr;
	tr.lpstrText = szChar;
	::SendMessage(HWnd(), EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	CString sText(szChar);
	delete[] szChar;

	return gcnew String(sText);
}

LabelTipInfo^ RichTextBoxEx::ToolHitTest(Drawing::Point ptScreen)
{
	if (!String::IsNullOrEmpty(m_CurrentLink) && (NeedLinkTooltip != nullptr))
	{
		auto e = gcnew NeedLinkTooltipEventArgs(m_CurrentLink);
		NeedLinkTooltip(this, e);

		if (!String::IsNullOrEmpty(e->tooltip))
		{
			auto ttInfo = gcnew LabelTipInfo();

			ttInfo->Font = Parent->Font;
			ttInfo->Text = e->tooltip;
			ttInfo->Id = e->tooltip->GetHashCode();

			ttInfo->Rect.Location = PointToClient(ptScreen);
			ttInfo->Rect.Offset(0, m_LinkTipVOffset);

			return ttInfo;
		}
	}
	
	return nullptr;
}
