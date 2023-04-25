using System;
using System.Diagnostics;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////

namespace EvidenceBoardUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class EvidenceBoardLinkVisibilityComboBox : CustomComboBox.CheckedComboBox
	{
		class EvidenceBoardLinkVisibilityItem : TDLNodeControl.LinkType
		{
			public EvidenceBoardLinkVisibilityItem(Translator trans, string label, EvidenceBoardLinkType type)
				:
				base(label, type)
			{
				m_Trans = trans;
			}

			public override string ToString()
			{
				if ((m_Trans != null) && (Type == EvidenceBoardLinkType.User))
					return string.Format(m_Trans.Translate("{0} (Custom)"), Name);

				return Name;
			}

			private Translator m_Trans;
		}

		// ----------------------------------------------------------------

		Translator m_Trans;
		List<string> m_UserTypes = new List<string>();

		struct LinkVisibilityItem
		{
			public EvidenceBoardLinkType Type;
			public string Name;
			public bool Visible;
		}
		List<LinkVisibilityItem> m_LinkVisibility;

		// ----------------------------------------------------------------

		public EvidenceBoardLinkVisibilityComboBox(Translator trans)
		{
			m_Trans = trans;
			None = trans.Translate("<none>");

			UserLinkTypes = null;
			Sorted = true;

			DropDownClosed += (s, e) => { m_LinkVisibility = null; };

		}

		public IEnumerable<string> UserLinkTypes
		{
			get
			{
				return m_UserTypes;
			}

			set
			{
				// Rebuild the combo, preserving the selection
				Items.Clear();

				int index = Items.Add(new EvidenceBoardLinkVisibilityItem(m_Trans, "Dependencies", EvidenceBoardLinkType.Dependency));
				ListBox.SetItemChecked(index, IsTypeVisible(EvidenceBoardLinkType.Dependency));

				index = Items.Add(new EvidenceBoardLinkVisibilityItem(m_Trans, "Parent/Child", EvidenceBoardLinkType.ParentChild));
				ListBox.SetItemChecked(index, IsTypeVisible(EvidenceBoardLinkType.ParentChild));

				if (value != null)
				{
					foreach (var name in value)
					{
						var visItem = new EvidenceBoardLinkVisibilityItem(m_Trans, name, EvidenceBoardLinkType.User);
						index = Items.Add(visItem);

						// if the new item is a new user type then enable it
						// else use the restore the previous state
						ListBox.SetItemChecked(index, IsTypeVisible(visItem.Type, visItem.Name));
					}

					m_LinkVisibility = null;
				}

				m_UserTypes.Clear();

				if (value != null)
					m_UserTypes.AddRange(value);
			}
		}

		List<LinkVisibilityItem> LinkVisibility
		{
			get
			{
				if (m_LinkVisibility == null)
				{
					m_LinkVisibility = new List<LinkVisibilityItem>();

					for (int index = 0; index < Items.Count; index++)
					{
						var item = (EvidenceBoardLinkVisibilityItem)Items[index];

						m_LinkVisibility.Add(new LinkVisibilityItem()
						{
							Name = item.Name,
							Type = item.Type,
							Visible = ListBox.GetItemChecked(index)
						});
					}
				}

				return m_LinkVisibility;
			}
		}

		bool IsTypeVisible(EvidenceBoardLinkType type, string name = "")
		{
			var index = LinkVisibility.FindIndex(x => ((x.Type == type) && (x.Name == name)));

			return ((index == -1) ? true : LinkVisibility[index].Visible);
		}

		public List<TDLNodeControl.LinkType> SelectedOptions
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
			// Then only hide what was explicitly hidden before
			var prevVisibility = prefs.GetProfileString(key, "LinkTypeVisibility", "_");

			if (prevVisibility == "_")
			{
				CheckAll();
				m_LinkVisibility = null;
			}
			else
			{
				m_LinkVisibility = new List<LinkVisibilityItem>();

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
						(type != (int)EvidenceBoardLinkType.User))
					{
						Debug.Assert(false);
						continue;
					}

					if ((type == (int)EvidenceBoardLinkType.User) && 
						string.IsNullOrWhiteSpace(parts[1]))
					{
						Debug.Assert(false);
						continue;
					}

					int visible = 1;
					int.TryParse(parts[2], out visible);
					
					int find = FindItemIndex((EvidenceBoardLinkType)type, parts[1]);

					if (find != -1)
						ListBox.SetItemChecked(find, (visible != 0));

					m_LinkVisibility.Add(new LinkVisibilityItem() {
						Name = parts[1],
						Type = (EvidenceBoardLinkType)type,
						Visible = (visible != 0) });
				}
			}

			return SelectedOptions;
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
