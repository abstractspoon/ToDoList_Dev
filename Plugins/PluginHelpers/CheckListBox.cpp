// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CheckListBox.h"
#include "Win32.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace System::Windows::Forms;

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

void CheckListBox::WndProc(Message% m)
{
	switch (m.Msg)
	{
	case WM_LBUTTONDOWN:
		{
			auto pt = Win32::GetPoint(m.LParam);
			int i = IndexFromPoint(pt);

			if ((i != -1) && !SelectedIndices->Contains(i))
			{
				auto checkRect = GetItemRectangle(i);
				checkRect.Width = checkRect.Height;

				if (checkRect.Contains(pt))
				{
					SelectedIndices->Clear();
					SelectedIndices->Add(i);
 	
					SetItemChecked(i, !GetItemChecked(i));
					m.Result = IntPtr::Zero;

					return;
				}
			}
		}
		break;
	}

	CheckedListBox::WndProc(m);
}
