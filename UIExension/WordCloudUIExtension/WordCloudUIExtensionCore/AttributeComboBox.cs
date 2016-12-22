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

        public bool Initialise()
        {
            // Add attributes to combo
            this.Items.Add(new AttributeItem("Title",           UIExtension.TaskAttribute.Title));
            this.Items.Add(new AttributeItem("Comments",        UIExtension.TaskAttribute.Comments));
            this.Items.Add(new AttributeItem("Status",          UIExtension.TaskAttribute.Status));
            this.Items.Add(new AttributeItem("Category",        UIExtension.TaskAttribute.Category));
			this.Items.Add(new AttributeItem("Completion date", UIExtension.TaskAttribute.DoneDate));
    		this.Items.Add(new AttributeItem("Due date",        UIExtension.TaskAttribute.DueDate));
			this.Items.Add(new AttributeItem("Start date",      UIExtension.TaskAttribute.StartDate));
			this.Items.Add(new AttributeItem("Allocated to",    UIExtension.TaskAttribute.AllocTo));
			this.Items.Add(new AttributeItem("Allocated by",    UIExtension.TaskAttribute.AllocBy));
			this.Items.Add(new AttributeItem("Creation date",   UIExtension.TaskAttribute.CreationDate));
			this.Items.Add(new AttributeItem("Created by",      UIExtension.TaskAttribute.CreatedBy));
			this.Items.Add(new AttributeItem("Version",         UIExtension.TaskAttribute.Version));
			this.Items.Add(new AttributeItem("Tags",             UIExtension.TaskAttribute.Tag));

            SetSelAttribute(UIExtension.TaskAttribute.Title);

            return true;
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

