﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Command.Handling
{
	public class CommandHandling
	{
		public static bool RemoveCommand(String commandId, ToolStripItemCollection items)
		{
			ToolStripItem cmd = GetCommandItem(commandId, ref items);

			if (cmd != null)
			{
				// Prevent sequential visible separators
				int iItem = items.IndexOf(cmd);

				if (iItem != -1)
				{
				    ToolStripItem nextCmd = null, prevCmd = null;

					if (iItem > 0)
						prevCmd = items[iItem - 1];

					if (iItem < (items.Count - 1))
						nextCmd = items[iItem + 1];

					if ((nextCmd is ToolStripSeparator) &&
						((prevCmd == null) || (prevCmd is ToolStripSeparator)))
					{
                        items.Remove(nextCmd);
					}
					else if ((prevCmd is ToolStripSeparator) && (nextCmd == null))
					{
                        items.Remove(prevCmd);
					}

                    items.Remove(cmd);
                }

                return true;
			}

			return false;
		}

		public static bool ProcessMenuShortcut(Keys keyPress, ToolStripItemCollection items)
		{
			ToolStripMenuItem menu = GetMenuItem(keyPress, items);

			if (menu != null)
			{
				if (menu.Enabled)
					menu.PerformClick();

				return true;
			}

			return false;
		}

		public static Keys GetMenuShortcutFromVirtualKey(UInt32 virtualKeyCode)
		{
			Keys keyPress = (Keys)virtualKeyCode;

			if ((keyPress == Keys.ControlKey) ||
				(keyPress == Keys.ShiftKey) ||
				(keyPress == Keys.Menu))
			{
				return Keys.None;
			}

			if ((Control.ModifierKeys & Keys.Control) == Keys.Control)
				keyPress |= Keys.Control;

			if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift)
				keyPress |= Keys.Shift;

			if ((Control.ModifierKeys & Keys.Alt) == Keys.Alt)
				keyPress |= Keys.Alt;

			return keyPress;
		}

		public static bool HasMenuShortcut(UInt32 virtualKeyCode, ToolStripItemCollection items)
		{
			Keys keyPress = GetMenuShortcutFromVirtualKey(virtualKeyCode);

			if (keyPress == Keys.None)
				return false;

			return (GetMenuItem(keyPress, items) != null);
		}

		public static bool ProcessMenuShortcut(UInt32 virtualKeyCode, ToolStripItemCollection items)
		{
			Keys keyPress = GetMenuShortcutFromVirtualKey(virtualKeyCode);

			if (keyPress == Keys.None)
				return false;

			return ProcessMenuShortcut(keyPress, items);
		}

		public static bool EnableCommand(String commandId, bool enable, ToolStripItemCollection items)
		{
			ToolStripMenuItem menu = GetMenuItem(commandId, items);

			if (menu == null)
				return false;

			menu.Enabled = enable;
			return true;
		}

		public static Keys GetMenuShortcut(String commandId, ToolStripItemCollection items)
		{
			ToolStripMenuItem menu = GetMenuItem(commandId, items);

			if (menu != null)
				return menu.ShortcutKeys;

			return Keys.None;
		}

		public static bool SetMenuShortcut(String commandId, Keys keyPress, ToolStripItemCollection items)
		{
			ToolStripMenuItem menu = GetMenuItem(commandId, items);

			if (menu != null)
			{
				menu.ShortcutKeys = keyPress;
				return true;
			}

			return false;
		}

		public static ToolStripMenuItem GetMenuItem(String commandId, ToolStripItemCollection items)
		{
			ToolStripItem cmd = GetCommandItem(commandId, ref items);

			if (cmd != null)
				return (cmd as ToolStripMenuItem);

			// else
			return null;
		}

		public static void ClearChecked(ToolStripItemCollection items)
		{
			foreach (ToolStripItem item in items)
			{
				ToolStripMenuItem menu = (item as ToolStripMenuItem);

				if (menu != null)
				{
					menu.Checked = false;

					// sub menu items
					ClearChecked(menu.DropDownItems); // RECURSIVE CALL
				}
			}
		}

		public static ToolStripItem GetCommandItem(String commandId, ref ToolStripItemCollection items)
		{
			foreach (ToolStripItem item in items)
			{
				if (item.Name == commandId)
					return item;

				// else sub menu items
				ToolStripMenuItem menu = (item as ToolStripMenuItem);

				if (menu != null)
				{
                    var menuItems = menu.DropDownItems;
					var menuItem = GetCommandItem(commandId, ref menuItems); // RECURSIVE CALL

					if (menuItem != null)
                    {
                        items = menuItems;
                        return menuItem;
                    }
                }
			}

			return null;
		}

		public static ToolStripMenuItem GetMenuItem(Keys keyPress, ToolStripItemCollection items)
		{
			foreach (ToolStripItem item in items)
			{
				ToolStripMenuItem menu = (item as ToolStripMenuItem);

				if (menu != null)
				{
					if (menu.ShortcutKeys == keyPress)
						return menu;

					// sub menu items
					if (menu.HasDropDownItems)
					{
						menu = GetMenuItem(keyPress, menu.DropDownItems); // RECURSIVE CALL

						if (menu != null)
							return menu;
					}
				}
			}

			return null;
		}

		public static bool SetMenuItemShortcut(String commandId, ToolStripItemCollection items, Keys shortcut)
		{
			ToolStripMenuItem item = CommandHandling.GetMenuItem(commandId, items);

			if (item == null)
				return false;

			item.ShortcutKeys = shortcut;
			return true;
		}
	}
}
