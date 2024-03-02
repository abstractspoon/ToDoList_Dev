using System;
using System.Diagnostics;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////

namespace EvidenceBoardUIExtension
{
	class EvidenceBoardLinkVisibilityComboBox : CustomComboBox.CheckedComboBox
	{
		class EvidenceBoardLinkVisibilityItem : TDLNodeControl.LinkType
		{
			public EvidenceBoardLinkVisibilityItem(string label, EvidenceBoardLinkType type)
				:
				base(label, type)
			{
				Label = label;
			}

			public override string ToString()
			{
				return Label;
			}

			public string Label;
		}

		// ----------------------------------------------------------------

		HashSet<string> m_UserTypes = new HashSet<string>();

		class LinkTypeVisibility : TDLNodeControl.LinkType
		{
			public LinkTypeVisibility(string label, EvidenceBoardLinkType type)
				:
				base(label, type)
			{
			}

			public LinkTypeVisibility(TDLNodeControl.LinkType other)
				:
				base(other.Name, other.Type)
			{
			}

			public bool Visible = true;
		}
		List<LinkTypeVisibility> m_PrevLinkVisibility;

		// ----------------------------------------------------------------

		public EvidenceBoardLinkVisibilityComboBox()
		{
			Items.Add(new EvidenceBoardLinkVisibilityItem("Untyped", EvidenceBoardLinkType.None));
			Items.Add(new EvidenceBoardLinkVisibilityItem("Dependency", EvidenceBoardLinkType.Dependency));
			Items.Add(new EvidenceBoardLinkVisibilityItem("Parent/Child", EvidenceBoardLinkType.ParentChild));

			UserLinkTypes = null;
			Sorted = true;

			DropDownClosed += (s, e) => { m_PrevLinkVisibility = null; };
		}

		public void Translate(Translator trans)
		{
			None = trans.Translate(None, Translator.Type.ComboBox);

			foreach (var item in Items)
			{
				var link = (item as EvidenceBoardLinkVisibilityItem);

				if (link.Type != EvidenceBoardLinkType.User)
					link.Label = trans.Translate(link.Label, Translator.Type.ComboBox);
			}
		}

		public IEnumerable<string> UserLinkTypes
		{
			get
			{
				return m_UserTypes;
			}

			set
			{
				if ((value != null) && m_UserTypes.SetEquals(value))
					return;

				// Cache the current selection
				var prevVisibility = LinkVisibility;

				// Remove all user types
				int iItem = Items.Count;

				while (iItem-- > 0)
				{
					var link = (Items[iItem] as EvidenceBoardLinkVisibilityItem);

					if (link.Type == EvidenceBoardLinkType.User)
						Items.RemoveAt(iItem);
				}

				// Re-add
				if (value != null)
				{
					foreach (var name in value)
					{
						if (!String.IsNullOrWhiteSpace(name))
							Items.Add(new EvidenceBoardLinkVisibilityItem(name, EvidenceBoardLinkType.User));
					}
				}

				// Restore the selection
				for (int index = 0; index < Items.Count; index++)
				{
					var item = (EvidenceBoardLinkVisibilityItem)Items[index];
					ListBox.SetItemChecked(index, IsTypeVisible(prevVisibility, item.Type, item.Name));
				}

				m_PrevLinkVisibility = null;

				if (value != null)
					m_UserTypes = new HashSet<string>(value);
				else
					m_UserTypes.Clear();
			}
		}

		private List<LinkTypeVisibility> LinkVisibility
		{
			get
			{
				if (m_PrevLinkVisibility != null)
					return m_PrevLinkVisibility;

				// else
				var linkVis = new List<LinkTypeVisibility>();

				for (int index = 0; index < Items.Count; index++)
				{
					var item = (EvidenceBoardLinkVisibilityItem)Items[index];

					linkVis.Add(new LinkTypeVisibility(item)
					{
						Visible = ListBox.GetItemChecked(index)
					});
				}

				return linkVis;
			}
		}

		private bool IsTypeVisible(List<LinkTypeVisibility> vis, EvidenceBoardLinkType type, string name = "")
		{
			int index = vis.FindIndex(x => ((x.Type == type) && (x.Name == name)));

			return ((index == -1) ? true : vis[index].Visible);
		}

		public List<TDLNodeControl.LinkType> SelectedLinkTypes
		{
			get
			{
				var selOptions = new List<TDLNodeControl.LinkType>();

				foreach (var option in LinkVisibility)
				{
					if (option.Visible)
						selOptions.Add(new TDLNodeControl.LinkType(option.Name, option.Type));
				}

				return selOptions;
			}
		}

		int FindItemIndex(EvidenceBoardLinkType type, string name)
		{
			for (int index = 0; index < Items.Count; index++)
			{
				var item = (EvidenceBoardLinkVisibilityItem)Items[index];

				if ((type == item.Type) && (name == item.Name))
					return index;
			}

			return -1;
		}

		public List<TDLNodeControl.LinkType> LoadPreferences(Preferences prefs, String key)
		{
			var prevVisibility = prefs.GetProfileString(key, "LinkTypeVisibility", "_");

			if (prevVisibility == "_")
			{
				CheckAll();
				m_PrevLinkVisibility = null;
			}
			else
			{
				m_PrevLinkVisibility = new List<LinkTypeVisibility>();

				var options = prevVisibility.Split(new[] { '|' }, StringSplitOptions.RemoveEmptyEntries );

				foreach (var option in options)
				{
					int type = -1;
					string name = string.Empty;

					var parts = option.Split(':');

					if (parts.Length != 3)
					{
						Debug.Assert(false);
						continue;
					}

					if (!int.TryParse(parts[0], out type))
					{
						Debug.Assert(false);
						continue;
					}

					if ((type != (int)EvidenceBoardLinkType.Dependency) && 
						(type != (int)EvidenceBoardLinkType.ParentChild) &&
						(type != (int)EvidenceBoardLinkType.None) &&
						(type != (int)EvidenceBoardLinkType.User))
					{
						Debug.Assert(false);
						continue;
					}

					int visible = 1;
					int.TryParse(parts[2], out visible);
					
					int find = FindItemIndex((EvidenceBoardLinkType)type, parts[1]);

					if (find != -1)
						ListBox.SetItemChecked(find, (visible != 0));

					m_PrevLinkVisibility.Add(new LinkTypeVisibility(parts[1], (EvidenceBoardLinkType)type)
					{
						Visible = (visible != 0)
					});
				}
			}

			return SelectedLinkTypes;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			string options = string.Empty;

			for (int index = 0; index < Items.Count; index++)
			{
				bool visible = ListBox.GetItemChecked(index);

				var item = (EvidenceBoardLinkVisibilityItem)Items[index];
				var option = string.Format("{0}:{1}:{2}|", (int)item.Type, item.Name, (visible ? 1 : 0));

				options = options + option;
			}
			
			prefs.WriteProfileString(key, "LinkTypeVisibility", options);
		}
	}

	// ----------------------------------------------------------------------------
}
