using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	class AttributeComboBox : ComboBox
    {
        protected class AttributeItem
        {
            public AttributeItem(string name, Task.Attribute attrib)
            {
                Name = name;
                Attrib = attrib;
            }

            public override string ToString()
            {
                return Name;
            }

            public string Name { set; get; }
            public Task.Attribute Attrib { set; get; }
        }

        // ---------------------------------------------------------------

		public bool Initialise(Translator trans)
        {
			if (Items.Count == 0) // once only
			{
				AddItem(trans, "Title",			Task.Attribute.Title);
				AddItem(trans, "Comments",		Task.Attribute.Comments);
				AddItem(trans, "Status",		Task.Attribute.Status);
				AddItem(trans, "Category",		Task.Attribute.Category);
				AddItem(trans, "Completion Date", Task.Attribute.DoneDate);
				AddItem(trans, "Due Date",		Task.Attribute.DueDate);
				AddItem(trans, "Start Date",	Task.Attribute.StartDate);
				AddItem(trans, "Allocated To",	Task.Attribute.AllocatedTo);
				AddItem(trans, "Allocated By",	Task.Attribute.AllocatedBy);
				AddItem(trans, "Creation Date", Task.Attribute.CreationDate);
				AddItem(trans, "Created By",	Task.Attribute.CreatedBy);
				AddItem(trans, "Version",		Task.Attribute.Version);
				AddItem(trans, "Tags",			Task.Attribute.Tags);
			}

            return true;
        }

		private bool AddItem(Translator trans, string name, Task.Attribute attrib)
		{
            return (Items.Add(new AttributeItem(trans.Translate(name), attrib)) != -1);
		}

        public Task.Attribute GetSelectedAttribute()
        {
            AttributeItem selItem = (AttributeItem)SelectedItem;

            if (selItem == null)
                return Task.Attribute.Unknown;

            return selItem.Attrib;
        }

        public bool SetSelectedAttribute(Task.Attribute attrib)
        {
            var item = FindAttribute(attrib);

            if (item == null)
                return false;

            // else
            SelectedItem = item;
            return true;
        }

        protected AttributeItem FindAttribute(Task.Attribute attrib)
        {
            foreach (AttributeItem item in Items)
            {
                if (item.Attrib == attrib)
                    return item;
            }

            // else
            return null;
        }

        public static bool IsSupportedAttribute(Task.Attribute attrib)
        {
            switch (attrib)
            {
                case Task.Attribute.Title:
                case Task.Attribute.DoneDate:
                case Task.Attribute.DueDate:
                case Task.Attribute.StartDate:
                case Task.Attribute.AllocatedTo:
                case Task.Attribute.AllocatedBy:
                case Task.Attribute.Status:
                case Task.Attribute.Category:
                case Task.Attribute.Comments:
                case Task.Attribute.CreationDate:
                case Task.Attribute.CreatedBy:
                case Task.Attribute.Version:
                case Task.Attribute.Tags:
                    return true;
            }

            // all else
            return false;
        }
    }
}

