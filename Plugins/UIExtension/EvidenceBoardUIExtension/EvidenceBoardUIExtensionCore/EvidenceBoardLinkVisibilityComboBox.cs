using System;
using System.Diagnostics;
using System.Collections.Generic;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////

namespace EvidenceBoardUIExtension
{
	class EvidenceBoardLinkVisibilityComboBox : CheckComboBox
	{
		Translator m_Trans;
 		List<ICheckComboBoxItem> m_PrevLinkVisibility;
		HashSet<string> m_UserTypes = new HashSet<string>();

		// -----------------------------------

		public EvidenceBoardLinkVisibilityComboBox(Translator trans)
		{
			m_Trans = trans;

			Prompt = trans.Translate("<none>", Translator.Type.ComboBox);
			UserLinkTypes = null;

			AddDefaultLinks();
		}

		private void AddDefaultLinks()
		{
			AddItem(new LinkComboItem("Untyped", EvidenceBoardLinkType.None, m_Trans), true);
			AddItem(new LinkComboItem("Dependency", EvidenceBoardLinkType.Dependency, m_Trans), true);
			AddItem(new LinkComboItem("Parent/Child", EvidenceBoardLinkType.ParentChild, m_Trans), true);
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

				// Cache the previous items and states
				var prevItems = new List<ICheckComboBoxItem>(CheckedItems);
				var prevCheckedItems = (m_PrevLinkVisibility ?? new List<ICheckComboBoxItem>(CheckedItems));

				// Rebuild the combo
				RemoveAllItems();
				AddDefaultLinks();

				m_UserTypes.Clear();

				if (value != null)
				{
					int userId = (int)EvidenceBoardLinkType.User;

					foreach (var name in value)
					{
						if (!String.IsNullOrWhiteSpace(name))
						{
							AddItem(new LinkComboItem(name, (EvidenceBoardLinkType)userId++, null), true);
							m_UserTypes.Add(name);
						}
					}
				}

				// Restore the check states
				foreach (var item in Items)
				{
					bool isChecked = (!prevItems.Contains(item) || prevCheckedItems.Contains(item));
					SetItemChecked(item, isChecked);
				}

				m_PrevLinkVisibility = null;
			}
		}

		public List<TDLNodeControl.LinkType> SelectedLinkTypes
		{
			get
			{
				var selLinks = new List<TDLNodeControl.LinkType>();

				foreach (var item in CheckedItems)
				{
					var link = (item as LinkComboItem);
					var type = (link.IsUserType ? EvidenceBoardLinkType.User : link.Type);

					selLinks.Add(new TDLNodeControl.LinkType(link.Name, type));
				}

				return selLinks;
			}
		}

		public List<TDLNodeControl.LinkType> LoadPreferences(Preferences prefs, String key)
		{
			var prevVisibility = prefs.GetProfileString(key, "LinkTypeVisibility", "_");

			if (prevVisibility == "_")
			{
				m_PrevLinkVisibility = null;
			}
			else
			{
				m_PrevLinkVisibility = new List<ICheckComboBoxItem>();

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

					if (type < 0)
					{
						Debug.Assert(false);
						continue;
					}

					int visible = 1;
					int.TryParse(parts[2], out visible);
					
					if (visible != 0)
						m_PrevLinkVisibility.Add(new LinkComboItem(parts[1], (EvidenceBoardLinkType)type, null));
				}
			}

			return SelectedLinkTypes;
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			string options = string.Empty;

			foreach (var item in Items)
			{
				var linkItem = (item as LinkComboItem);
				var type = (linkItem.IsUserType ? EvidenceBoardLinkType.User : linkItem.Type);

				bool isChecked = IsItemChecked(item);
				var option = string.Format("{0}:{1}:{2}|", (int)type, linkItem.Name, (isChecked ? 1 : 0));

				options = options + option;
			}
			
			prefs.WriteProfileString(key, "LinkTypeVisibility", options);
		}
	}

	///////////////////////////////////////////////////////

	class LinkComboItem : TDLNodeControl.LinkType, ICheckComboBoxItem
	{
		private string m_Label;

		// ------------------------

		public LinkComboItem(string name, EvidenceBoardLinkType type, Translator trans)
			:
			base(name, type)
		{
			if (trans == null)
				m_Label = name;
			else
				m_Label = trans.Translate(name, Translator.Type.ComboBox);
		}

		public String Label { get { return m_Label; } }
		public int UniqueId { get { return (int)Type; } }
		public bool IsUserType { get { return (Type >= EvidenceBoardLinkType.User); } }

		public override bool Equals(object obj)
		{
			var link = (obj as TDLNodeControl.LinkType);

			if (link != null)
			{
				switch (link.Type)
				{
				case EvidenceBoardLinkType.None:
				case EvidenceBoardLinkType.Dependency:
				case EvidenceBoardLinkType.ParentChild:
					return (link.Type == this.Type);
				}

				// All else
				Debug.Assert(link.Type >= EvidenceBoardLinkType.User);
				return (link.Name == this.Name);
			}

			return false;
		}

	}

}
