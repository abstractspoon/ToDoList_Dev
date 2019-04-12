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
	ToolStripItem^ cmd = GetCommandItem(commandId, items);

	if (cmd != nullptr)
	{
		cmd->Visible = false;
		cmd->Enabled = false;

		return true;
	}

	return false;
}

bool CommandHandling::ProcessMenuShortcut(Keys keyPress, ToolStripItemCollection^ items)
{
	ToolStripMenuItem^ menu = GetMenuItem(keyPress, items);

	if (menu != nullptr)
	{
		if (menu->Enabled)
			menu->PerformClick();

		return true;
	}

	return false;
}

System::Windows::Forms::Keys CommandHandling::GetMenuShortcut(String^ commandId, System::Windows::Forms::ToolStripItemCollection^ items)
{
	ToolStripMenuItem^ menu = GetMenuItem(commandId, items);

	if (menu != nullptr)
		return menu->ShortcutKeys;

	return System::Windows::Forms::Keys::None;
}

bool CommandHandling::SetMenuShortcut(String^ commandId, Keys keyPress, ToolStripItemCollection^ items)
{
	ToolStripMenuItem^ menu = GetMenuItem(commandId, items);

	if (menu != nullptr)
	{ 
		menu->ShortcutKeys = keyPress;
		return true;
	}

	return false;
}

ToolStripMenuItem^ CommandHandling::GetMenuItem(String^ commandId, ToolStripItemCollection^ items)
{
	ToolStripItem^ cmd = GetCommandItem(commandId, items);

	if (cmd != nullptr)
		return dynamic_cast<ToolStripMenuItem^>(cmd);

	// else
	return nullptr;
}

ToolStripItem^ CommandHandling::GetCommandItem(String^ commandId, ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		ToolStripItem^ cmd = items[nItem];

		if (cmd->Name == commandId)
			return cmd;

		// else sub menu items
		ToolStripMenuItem^ menu = dynamic_cast<ToolStripMenuItem^>(cmd);

		if ((menu != nullptr) && menu->HasDropDownItems)
		{
			cmd = GetCommandItem(commandId, menu->DropDownItems); // RECURSIVE CALL

			if (cmd != nullptr)
				return cmd;
		}
	}

	return nullptr;
}

ToolStripMenuItem^ CommandHandling::GetMenuItem(Keys keyPress, ToolStripItemCollection^ items)
{
	int nItem = items->Count;

	while (nItem--)
	{
		ToolStripMenuItem^ menu = dynamic_cast<ToolStripMenuItem^>(items[nItem]);

		if (menu != nullptr)
		{
			if (menu->ShortcutKeys == keyPress)
				return menu;

			// sub menu items
			if (menu->HasDropDownItems)
			{
				menu = GetMenuItem(keyPress, menu->DropDownItems); // RECURSIVE CALL

				if (menu != nullptr)
					return menu;
			}
		}
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

