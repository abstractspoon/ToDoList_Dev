// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "CommandHandling.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;
using namespace System;
using namespace Windows::Forms;

////////////////////////////////////////////////////////////////////////////////////////////////

bool CommandHandling::HideCommand(String^ commandId, ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		ToolStripItem^ cmd = items[nItem];

		if (cmd->Name == commandId)
		{
			cmd->Visible = false;
			cmd->Enabled = false;

			return true;
		}
		else
		{
			ToolStripMenuItem^ menu = dynamic_cast<ToolStripMenuItem^>(cmd);

			if ((menu != nullptr) && menu->HasDropDownItems)
			{
				if (HideCommand(commandId, menu->DropDownItems))
					return true;
			}
		}
	}

	return false;
}

bool CommandHandling::ProcessMenuShortcut(Keys keyPress, ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		ToolStripMenuItem^ menu = dynamic_cast<ToolStripMenuItem^>(items[nItem]);

		if ((menu != nullptr) && menu->Enabled)
		{
			if (menu->ShortcutKeys == keyPress)
			{
				// do command
				menu->PerformClick();
				return true;
			}
			else  if (menu->HasDropDownItems)
			{
				if (ProcessMenuShortcut(keyPress, menu->DropDownItems))
					return true;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

