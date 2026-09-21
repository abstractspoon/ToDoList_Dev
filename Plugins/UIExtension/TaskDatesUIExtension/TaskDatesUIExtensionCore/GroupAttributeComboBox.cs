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
		private TaskAttributeItem m_NoneAttrib;

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

		public void Initialise(IEnumerable<Task.Attribute> attribIds, Translator trans)
		{
			if (Items.Count == 0)
			{
				m_NoneAttrib = new TaskAttributeItem()
				{
					AttributeId = Task.Attribute.Unknown,
					Label = trans.Translate("<none>", Translator.Type.ComboBox)
				};

				Items.Add(m_NoneAttrib);

				// The supplied list
				foreach (var attribId in attribIds)
				{
					Items.Add(new TaskAttributeItem()
					{
						AttributeId = attribId,
						Label = TaskList.GetAttributeName(attribId, trans)
					});
				}

				SelectedItem = m_NoneAttrib;
			}
		}

		public Task.Attribute SelectedGroupId
		{
			get
			{
				var selAttrib = (SelectedItem as TaskAttributeItem);

				if (selAttrib == null)
					return Task.Attribute.Unknown;

				return selAttrib.AttributeId;
			}

			set
			{
				if (value != Task.Attribute.Unknown)
				{
					foreach (var obj in Items)
					{
						if ((obj as TaskAttributeItem).AttributeId == value)
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

		public TaskAttributeItem SelectedGroup
		{
			get
			{
				var selAttrib = (SelectedItem as TaskAttributeItem);

				if (selAttrib?.AttributeId != Task.Attribute.Unknown)
					return selAttrib;

				// else
				return null;
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
