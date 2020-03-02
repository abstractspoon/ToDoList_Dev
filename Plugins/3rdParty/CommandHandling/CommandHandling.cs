using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Command.Handling
{
	public class CommandHandling
	{
		public static bool HideCommand(String commandId, ToolStripItemCollection items)
		{
			ToolStripItem cmd = GetCommandItem(commandId, items);

			if (cmd != null)
			{
				cmd.Visible = false;
				cmd.Enabled = false;

				// Prevent sequential visible separators
				ToolStripItem nextVisCmd = null;
				ToolStripItem prevVisCmd = null;

				int iItem = items.IndexOf(cmd);

				// Handle nested menus
				// ie. cmd may not be an immediate child of items
				if ((iItem == -1) && (cmd.OwnerItem is ToolStripMenuItem))
				{
					items = (cmd.OwnerItem as ToolStripMenuItem).DropDownItems;
					iItem = items.IndexOf(cmd);
				}

				if (iItem != -1)
				{
					int iNext = iItem, iPrev = iItem;

					while (iPrev-- > 0)
					{
						if (items[iPrev].Visible)
						{
							prevVisCmd = items[iPrev];
							break;
						}
					}

					while (++iNext < items.Count)
					{
						if (items[iNext].Visible)
						{
							nextVisCmd = items[iNext];
							break;
						}
					}

					if ((nextVisCmd is ToolStripSeparator) &&
						((prevVisCmd == null) || (prevVisCmd is ToolStripSeparator)))
					{
						nextVisCmd.Visible = false;
						nextVisCmd.Enabled = false;
					}
					else if ((prevVisCmd is ToolStripSeparator) && (nextVisCmd == null))
					{
						prevVisCmd.Visible = false;
						prevVisCmd.Enabled = false;
					}
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
			ToolStripItem cmd = GetCommandItem(commandId, items);

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

		public static ToolStripItem GetCommandItem(String commandId, ToolStripItemCollection items)
		{
			foreach (ToolStripItem item in items)
			{
				if (item.Name == commandId)
					return item;

				// else sub menu items
				ToolStripMenuItem menu = (item as ToolStripMenuItem);

				if (menu != null)
				{
					var menuItem = GetCommandItem(commandId, menu.DropDownItems); // RECURSIVE CALL

					if (menuItem != null)
						return menuItem;
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
	}
}
