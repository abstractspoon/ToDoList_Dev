
#include "stdafx.h"
#include "UITheme.h"
#include "Win32.h"
#include "DPIScaling.h"
#include "Translator.h"
#include "FormsUtil.h"
#include "PluginHelpers.h"
#include "RichTextBoxEx.h"
#include "UIExtension.h"

#include <shared\Clipboard.h>
#include <shared\Misc.h>

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Drawing;
using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

CreateParams^ RichTextBoxEx::CreateParams::get()
{
	auto cp = RichTextBox::CreateParams::get();
	cp->ClassName = L"RichEdit50W";

	return cp;
}

void RichTextBoxEx::WndProc(Message% m)
{
	const int WM_REFLECT = (WM_USER + 0x1C00);

	switch (m.Msg)
	{
	case WM_NOTIFY + WM_REFLECT:
		{
			NMHDR* pNMHDR = (NMHDR*)(int)m.LParam;

			if (pNMHDR->code == EN_LINK)
			{
				ENLINK* pENL = (ENLINK*)pNMHDR;

				m_ContextUrl = GetTextRange(pENL->chrg);
				m.Result = (IntPtr)1;

				return;
			}
		}
		break;

	case WM_SETCURSOR:
		{
// 			m_ShowHandCursor = false;
// 			DefWndProc(m);
// 
// 			if (m_ShowHandCursor)
// 			{
// 				Cursor = UIExtension::HandCursor();
// 			}
// 			else if (SelectionContainsMessagePos())
// 			{
// 				Cursor = Cursors::Arrow;
// 			}
// 			else
// 			{
// 				Cursor = Cursors::IBeam;
// 			}
// 
// 			m.Result = (IntPtr)1;
// 			return;
		}
		break;
	}

	RichTextBox::WndProc(m);
}

bool RichTextBoxEx::SelectionContainsMessagePos()
{
	Point pos = PointToClient(MousePosition);

	int selStart = SelectionStart;
	int selEnd = SelectionStart + SelectionLength;

	int charAtPos = GetCharIndexFromPosition(pos);

	return ((charAtPos < selStart) || (charAtPos > selEnd));
}

String^ RichTextBoxEx::GetTextRange(const CHARRANGE& cr)
{
	int nLength = int(cr.cpMax - cr.cpMin + 1);

	// create an ANSI buffer 
	LPTSTR szChar = new TCHAR[nLength];
	ZeroMemory(szChar, nLength * sizeof(TCHAR));

	TEXTRANGE tr;
	tr.chrg = cr;
	tr.lpstrText = szChar;
	::SendMessage(Win32::GetHwnd(Handle), EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	CString sText(szChar);
	delete[] szChar;

	return gcnew String(sText);
}
