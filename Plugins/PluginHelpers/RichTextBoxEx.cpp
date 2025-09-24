
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
#include <shared\Rtf2HtmlConverter.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::IO;

using namespace Abstractspoon::Tdl::PluginHelpers;

using namespace IIControls;
using namespace Itenso::Solutions::Community::Rtf2Html;

////////////////////////////////////////////////////////////////////////////////////////////////

const int TabWidth = 48;

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
	case EM_PASTESPECIAL:
		if (PasteEvent(this, Clipboard::GetDataObject()))
			return;
		break;

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

void RichTextBoxEx::Indent()
{
	// Match behaviour of core app -> insert leading tab
	// keeping what's selected still selected
	if (String::IsNullOrEmpty(SelectedText) || (SelectedText->IndexOf('\n') == -1))
	{
		int selStart = SelectionStart;
		int selLen = SelectionLength;

		// Move selection to start of text
		SelectionLength = 0;

		// Insert a tab
		SelectedText = "\t";

		// Restore selection
		SelectionStart = (selStart + 1);
		SelectionLength = selLen;
	}
	else  // Paragraph indent
	{
		SelectionIndent += TabWidth;
	}
}

void RichTextBoxEx::Outdent()
{
	if (String::IsNullOrEmpty(SelectedText) || (SelectedText->IndexOf('\n') == -1))
	{
		int selStart = SelectionStart;
		int selLen = SelectionLength;

		// Move selection back a character
		SelectionStart--;
		SelectionLength = 1;

		// Remove a tab
		if (SelectedText == "\t")
			SelectedText = "";

		// Restore selection
		SelectionLength = selLen;
	}
	else  // Paragraph indent
	{
		SelectionIndent = Math::Max(0, (SelectionIndent - TabWidth));
	}
}

String^ RichTextBoxEx::RtfToHtml(String^ rtf, bool useMSWord)
{
	return RtfToHtml(rtf, "", useMSWord);
}


#undef GetTempPath

String^ RichTextBoxEx::RtfToHtml(String^ rtf, String^ imageFolder, bool useMSWord)
{
	if (useMSWord)
	{
		CRtfHtmlConverter converter(TRUE);

		// Convert Rtf to multibyte
		MarshalledString msRtf(rtf);

		CString sRtf(msRtf), sHtml;
		Misc::EncodeAsMultiByte(sRtf, CP_UTF8);

		if (converter.ConvertRtfToHtml((LPCSTR)(LPCWSTR)sRtf, NULL, sHtml, MS(imageFolder)))
			return gcnew String(sHtml);

		return nullptr;
	}

	// else use 'Intenso' components directly
	// to avoid unnecessary temporary string copies
	auto tempRtfPath = Path::ChangeExtension(Path::GetTempFileName(), "rtf");

	// Make sure format is '\rtf1\'
	if (rtf->StartsWith("{\\rtf\\"))
		rtf = rtf->Replace("{\\rtf\\", "{\\rtf1\\");

	File::WriteAllText(tempRtfPath, rtf, System::Text::Encoding::ASCII);

	auto rtf2Html = gcnew Program(tempRtfPath,
								  Path::GetTempPath(),
								  imageFolder,
								  "/IT:png",			// image file format
								  "/DS:content",		// return only body of html
								  String::Empty,		// unused
								  String::Empty,		// unused
								  String::Empty,		// unused 
								  String::Empty,		// unused 
								  String::Empty,		// unused 
								  String::Empty);		// unused

	if (!rtf2Html->Execute())
		return nullptr;

	return File::ReadAllText(Path::ChangeExtension(tempRtfPath, "html"));
}

#define GetTempPath GetTempPathW