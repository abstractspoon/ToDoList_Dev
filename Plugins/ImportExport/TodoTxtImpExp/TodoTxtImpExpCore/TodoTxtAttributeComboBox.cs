using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////

namespace TodoTxtImpExp
{
	public partial class TodoTxtAttributeComboBox : ComboBox
	{
		public TodoTxtAttributeComboBox()
		{
			InitializeComponent();
		}

		public TodoTxtAttributeComboBox(IContainer container)
		{
			container.Add(this);

			InitializeComponent();
		}

		public void Populate(IEnumerable<TaskAttributeItem> availAttribs,
							 IEnumerable<CustomAttributeDefinition> customAttribs, Translator trans)
		{
			BeginUpdate();
			Items.Clear();

			var comboAttribs = new List<TaskAttributeItem>();

			// Built-in attributes
			foreach (var attrib in availAttribs)
			{
				switch (attrib.AttributeId)
				{
				case Task.Attribute.Tags:
				case Task.Attribute.Category:
					Items.Add(new TaskAttributeItem() { AttributeId = attrib.AttributeId, Label = attrib.Label });
					break;
				}
			}

			// Custom string-list attributes
			foreach (var custAttrib in customAttribs)
			{
				Debug.Assert(custAttrib.AttributeType == CustomAttributeDefinition.Attribute.String);

				if (custAttrib.ListType != CustomAttributeDefinition.List.None)
				{
					Items.Add(new TaskAttributeItem()
					{
						AttributeId = Task.Attribute.CustomAttribute,
						CustomAttributeId = custAttrib.Id,
						Label = string.Format(trans.Translate("{0} (Custom)", Translator.Type.Text), custAttrib.Label)
					});
				}
			}

			EndUpdate();
		}

		public Task.Attribute SelectedAttributeId { get { return (SelectedAttribute?.AttributeId ?? Task.Attribute.Unknown); } }

		public string SelectedAttributeCustomId
		{
			get
			{
				if (SelectedAttributeId != Task.Attribute.CustomAttribute)
					return string.Empty;

				// else 
				return SelectedAttribute?.CustomAttributeId;
			}
		}

		public bool SelectAttribute(Task.Attribute attribId, string custAttribId = "")
		{
			foreach (var item in Items)
			{
				var taskAttrib = (item as TaskAttributeItem);

				if (attribId == taskAttrib.AttributeId)
				{
					if ((attribId != Task.Attribute.CustomAttribute) ||
						(custAttribId == taskAttrib.CustomAttributeId))
					{
						SelectedItem = item;
						return true;
					}
				}
			}

			return false;
		}

		// ----------------------------------------------------------------

		TaskAttributeItem SelectedAttribute { get { return (SelectedItem as TaskAttributeItem); } }
	}
}
