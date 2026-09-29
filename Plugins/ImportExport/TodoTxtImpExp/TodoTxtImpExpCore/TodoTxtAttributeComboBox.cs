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
		public const string ProjectsCustAttribId = "CUST_TDT_PROJECTS";
		public const string ContextsCustAttribId = "CUST_TDT_CONTEXTS";

		// ----------------------------------------------------------

		public TodoTxtAttributeComboBox()
		{
			InitializeComponent();
		}

		public TodoTxtAttributeComboBox(IContainer container)
		{
			container.Add(this);

			InitializeComponent();
		}

		public void Populate(bool importing, TaskList tasks, Translator trans)
		{
			Items.Clear();

			foreach (var attrib in tasks.GetAvailableAttributes(trans))
			{
				switch (attrib.AttributeId)
				{
				case Task.Attribute.Category:
				case Task.Attribute.Tags:
					Items.Add(attrib);
					break;

				case Task.Attribute.CustomAttribute:
					if (attrib.CustomAttributeType == CustomAttributeDefinition.Attribute.String)
					{
						// TODO
						Items.Add(attrib);
					}
					break;
				}
			}
		}

		public bool SelectedAttributeIsTags		{ get { return (SelectedAttributeId == Task.Attribute.Tags); } }
		public bool SelectedAttributeIsCategory { get { return (SelectedAttributeId == Task.Attribute.Category); } }
		public bool SelectedAttributeIsCustom	{ get { return (SelectedAttributeId == Task.Attribute.CustomAttribute); } }

		public string SelectedAttributeCustomId
		{
			get
			{
				if (!SelectedAttributeIsCustom)
					return string.Empty;

				// else 
				return SelectedAttribute?.CustomAttributeId;
			}
		}

		// ----------------------------------------------------------------

		TaskAttributeItem SelectedAttribute { get { return (SelectedItem as TaskAttributeItem); } }
		Task.Attribute SelectedAttributeId { get { return (SelectedAttribute?.AttributeId ?? Task.Attribute.Unknown); } }
	}
}
