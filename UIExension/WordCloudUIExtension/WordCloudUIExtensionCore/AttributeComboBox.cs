using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
    class AttributeComboBox : System.Windows.Forms.ComboBox
    {
        protected class AttributeItem
        {
            public AttributeItem(string name, UIExtension.TaskAttribute attrib)
            {
                Name = name;
                Attrib = attrib;
            }

            public override string ToString()
            {
                return Name;
            }

            public string Name { set; get; }
            public UIExtension.TaskAttribute Attrib { set; get; }
        }

        // ---------------------------------------------------------------

		public bool Initialise(Translator trans)
        {
			if (Items.Count == 0) // once only
			{
				AddItem(trans, "Title", UIExtension.TaskAttribute.Title);
				AddItem(trans, "Comments", UIExtension.TaskAttribute.Comments);
				AddItem(trans, "Status", UIExtension.TaskAttribute.Status);
				AddItem(trans, "Category", UIExtension.TaskAttribute.Category);
				AddItem(trans, "Completion Date", UIExtension.TaskAttribute.DoneDate);
				AddItem(trans, "Due Date", UIExtension.TaskAttribute.DueDate);
				AddItem(trans, "Start Date", UIExtension.TaskAttribute.StartDate);
				AddItem(trans, "Allocated To", UIExtension.TaskAttribute.AllocTo);
				AddItem(trans, "Allocated By", UIExtension.TaskAttribute.AllocBy);
				AddItem(trans, "Creation Date", UIExtension.TaskAttribute.CreationDate);
				AddItem(trans, "Created By", UIExtension.TaskAttribute.CreatedBy);
				AddItem(trans, "Version", UIExtension.TaskAttribute.Version);
				AddItem(trans, "Tags", UIExtension.TaskAttribute.Tag);

				SetSelAttribute(UIExtension.TaskAttribute.Title);
			}

            return true;
        }

		private bool AddItem(Translator trans, string name, UIExtension.TaskAttribute attrib)
		{
            return (Items.Add(new AttributeItem(trans.Translate(name), attrib)) != -1);
		}

        public UIExtension.TaskAttribute GetSelAttribute()
        {
            AttributeItem selItem = (AttributeItem)SelectedItem;

            if (selItem == null)
                return UIExtension.TaskAttribute.Unknown;

            return selItem.Attrib;
        }

        public bool SetSelAttribute(UIExtension.TaskAttribute attrib)
        {
            var item = FindAttribute(attrib);

            if (item == null)
                return false;

            // else
            SelectedItem = item;
            return true;
        }

        protected AttributeItem FindAttribute(UIExtension.TaskAttribute attrib)
        {
            foreach (AttributeItem item in Items)
            {
                if (item.Attrib == attrib)
                    return item;
            }

            // else
            return null;
        }

        public static bool IsSupportedAttribute(UIExtension.TaskAttribute attrib)
        {
            switch (attrib)
            {
                case UIExtension.TaskAttribute.Title:
                case UIExtension.TaskAttribute.DoneDate:
                case UIExtension.TaskAttribute.DueDate:
                case UIExtension.TaskAttribute.StartDate:
                case UIExtension.TaskAttribute.AllocTo:
                case UIExtension.TaskAttribute.AllocBy:
                case UIExtension.TaskAttribute.Status:
                case UIExtension.TaskAttribute.Category:
                case UIExtension.TaskAttribute.Comments:
                case UIExtension.TaskAttribute.CreationDate:
                case UIExtension.TaskAttribute.CreatedBy:
                case UIExtension.TaskAttribute.Version:
                case UIExtension.TaskAttribute.Tag:
                    return true;
            }

            // all else
            return false;
        }
    }
}

