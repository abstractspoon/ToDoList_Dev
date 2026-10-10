using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////

namespace TaskDatesUIExtension
{
	public partial class GroupAttributeComboBox : ComboBox
	{
		class ComboItem
		{
			public TaskAttributeItem Attribute { get; private set; }
			private string m_Text;

			public ComboItem(TaskAttributeItem attrib, Translator trans)
			{
				Attribute = attrib;

				switch (attrib.AttributeId)
				{
				case Task.Attribute.Unknown:
					m_Text = string.Format(trans.Translate("<none>", Translator.Type.ComboBox));
					break;

				case Task.Attribute.CustomAttribute:
					m_Text = string.Format(trans.Translate("{0} (Custom)", Translator.Type.ComboBox), attrib.Label);
					break;

				default:
					m_Text = attrib.Label;
					break;
				}
			}

			public override string ToString() { return m_Text; }
		}

		// ----------------------------------------------

		private ComboItem m_NoneAttrib;

		// ----------------------------------------------

		public GroupAttributeComboBox()
		{
			InitializeComponent();
		}

		public GroupAttributeComboBox(IContainer container)
		{
			container.Add(this);

			InitializeComponent();
		}

		public void Initialise(IEnumerable<TaskAttributeItem> attribs, Translator trans)
		{
			if (m_NoneAttrib == null)
			{
				var noneAttrib = new TaskAttributeItem() { AttributeId = Task.Attribute.Unknown };
				m_NoneAttrib = new ComboItem(noneAttrib, trans);
			}

			object selItem = (SelectedItem ?? m_NoneAttrib);

			Items.Clear();
			Items.Add(m_NoneAttrib);

			if (attribs?.Count() > 0)
			{
				foreach (var attrib in attribs)
					Items.Add(new ComboItem(attrib, trans));
			}

			if (!Items.Contains(selItem))
				selItem = m_NoneAttrib;

			SelectedItem = selItem;
		}

		public string SelectedGroupByKey
		{
			get	{ return SelectedGroupBy?.AttributeKey(); }

			set
			{
				if (!string.IsNullOrEmpty(value))
				{
					foreach (var obj in Items)
					{
						if ((obj as ComboItem)?.Attribute.AttributeKey() == value)
						{
							SelectedItem = obj;
							return;
						}
					}
				}

				// All else
				SelectedItem = m_NoneAttrib;
			}
		}

		public TaskAttributeItem SelectedGroupBy
		{
			get
			{
				var selItem = (SelectedItem as ComboItem);

				if ((selItem == null) || (selItem.Attribute.AttributeId == Task.Attribute.Unknown))
					return null;

				// else
				return selItem.Attribute;
			}

			set
			{
				if (value == null)
					SelectedItem = m_NoneAttrib;
				else
					SelectedItem = value;
			}
		}
	}
}
