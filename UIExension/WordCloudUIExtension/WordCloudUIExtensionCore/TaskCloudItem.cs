using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;

namespace WordCloudUIExtension
{
	public class CloudTaskItem
	{
		public CloudTaskItem(UInt32 id)
		{
			Id = id;
			m_WordAttribute = UIExtension.TaskAttribute.Unknown;
		}

		public override String ToString()
		{
			return Title;
		}
        
		public readonly UInt32 Id;
		public string Title;
		public string DoneDate;
		public string DueDate;
		public string StartDate;
		public string AllocBy;
		public string Status;
		public string Comments;
		public string CreationDate;
		public string CreatedBy;
		public string Version;
		public List<string> AllocTo;
		public List<string> Category;
		public List<string> Tags;
		public Boolean HasIcon;
		public Boolean IsParent;
        public Boolean IsLocked;
        public Boolean IsGoodAsDone;
        public Boolean HasSomeSubtasksDone;

        private System.Drawing.Color taskTextColor = System.Drawing.Color.Empty;
            
        public System.Drawing.Color TextColor
        {
            get { return taskTextColor; }
            set { taskTextColor = value; }
        }

        public System.Drawing.Color GetTextColor(Boolean isSelected, Boolean taskColorIsBkgnd)
        {
            if (!taskTextColor.IsEmpty)
            {
                if (isSelected)
                    return DrawingColor.SetLuminance(TextColor, 0.3f);

                if (taskColorIsBkgnd && !IsDone(true))
                    return DrawingColor.GetBestTextColor(TextColor);
            }
            else
            {
                return System.Drawing.SystemColors.WindowText;
            }

            return TextColor;
        }

        public System.Drawing.Color GetBackColor(Boolean isSelected, Boolean taskColorIsBkgnd)
        {
            if (!taskTextColor.IsEmpty)
            {
                if (!isSelected && taskColorIsBkgnd && !IsDone(true))
                    return TextColor;
            }

            // else
            return System.Drawing.Color.Empty;
        }

        public Boolean IsDone(Boolean includeGoodAsDone)
        {
            if (includeGoodAsDone && IsGoodAsDone)
                return true;

            // else
            return (DoneDate != String.Empty);
        }
        
		private List<string> m_Words;
		private UIExtension.TaskAttribute m_WordAttribute;

		static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.', '[', ']' };

		public void ProcessTaskUpdate(Task task, UIExtension.UpdateType type,
									  HashSet<UIExtension.TaskAttribute> attribs, Boolean newTask)
		{
            IsParent = task.IsParent();
            IsLocked = task.IsLocked();
            IsGoodAsDone = task.IsGoodAsDone();
            HasSomeSubtasksDone = task.HasSomeSubtasksDone();

			if (newTask)
			{
				Title = task.GetTitle();
				DoneDate = SplitDate(task.GetDoneDateString());
				DueDate = SplitDate(task.GetDueDateString());
				StartDate = SplitDate(task.GetStartDateString());
				AllocTo = task.GetAllocatedTo();
				AllocBy = task.GetAllocatedBy();
				Status = task.GetStatus();
				Category = task.GetCategory();
				Comments = task.GetComments();
				CreationDate = SplitDate(task.GetCreationDateString());
				CreatedBy = task.GetCreatedBy();
				Version = task.GetVersion();
				Tags = task.GetTag();
				TextColor = task.GetTextDrawingColor();

				HasIcon = (task.GetIcon().Length > 0);
			}
			else
			{
				if (attribs.Contains(UIExtension.TaskAttribute.Title))
					Title = task.GetTitle();

				if (attribs.Contains(UIExtension.TaskAttribute.DoneDate))
					DoneDate = SplitDate(task.GetDoneDateString());

				if (attribs.Contains(UIExtension.TaskAttribute.DueDate))
					DueDate = SplitDate(task.GetDueDateString());

				if (attribs.Contains(UIExtension.TaskAttribute.StartDate))
					StartDate = SplitDate(task.GetStartDateString());

				if (attribs.Contains(UIExtension.TaskAttribute.AllocTo))
					AllocTo = task.GetAllocatedTo();

				if (attribs.Contains(UIExtension.TaskAttribute.AllocBy))
					AllocBy = task.GetAllocatedBy();

				if (attribs.Contains(UIExtension.TaskAttribute.Status))
					Status = task.GetStatus();

				if (attribs.Contains(UIExtension.TaskAttribute.Category))
					Category = task.GetCategory();

				if (attribs.Contains(UIExtension.TaskAttribute.Comments))
					Comments = task.GetComments();

				if (attribs.Contains(UIExtension.TaskAttribute.CreationDate))
					CreationDate = SplitDate(task.GetCreationDateString());

				if (attribs.Contains(UIExtension.TaskAttribute.CreatedBy))
					CreatedBy = task.GetCreatedBy();

				if (attribs.Contains(UIExtension.TaskAttribute.Version))
					Version = task.GetVersion();

				if (attribs.Contains(UIExtension.TaskAttribute.Tag))
					Tags = task.GetTag();

				if (attribs.Contains(UIExtension.TaskAttribute.Icon))
					HasIcon = (task.GetIcon().Length > 0);

				if (attribs.Contains(UIExtension.TaskAttribute.Color))
					TextColor = task.GetTextDrawingColor();
			}
		}

		static string SplitDate(string date)
		{
			string[] parts = date.Split(new char[] { ' ' }, 2);

			if (parts.Length > 0)
				return parts[0];

			// else
			return "";
		}

		public Boolean Matches(String words, Boolean caseSensitive, Boolean wholeWord, Boolean titleOnly)
		{
			var searchIn = new List<String> { Title };

            if (!titleOnly)
			    searchIn.Add(Comments);	

            StringComparison compare = (caseSensitive ? StringComparison.CurrentCulture : StringComparison.CurrentCultureIgnoreCase);

			foreach (var search in searchIn)
			{
				int find = search.IndexOf(words, compare);

                while (find >= 0)
                {
                    Boolean match = true;

                    if (wholeWord)
                    {
                        char prevChar = ' ', nextChar = ' '; // know delimiters

                        // check for leading trailing delimiters
                        if (find > 0)
                            prevChar = search[find - 1];

                        if ((find + words.Length) < search.Length)
                            nextChar = search[find + words.Length];
                        
                        match = (WordDelims.Contains(prevChar) || WordTrim.Contains(prevChar) ||
                                 WordDelims.Contains(nextChar) || WordTrim.Contains(nextChar));
					}

                    if (match)
                        return true;

                    // else
                    find = search.IndexOf(words, find + 1, compare);
				}
			}

			return false;
		}

		public static List<string> ToWords(String text, IBlacklist exclusions)
		{
			var words = ToWords(text, 2);

			if (exclusions != null)
				words.RemoveAll(p => (exclusions.Countains(p)));

			return words;
		}

		public static List<string> ToWords(String text, int minWordLength = 0)
		{
			List<string> words = text.Split(WordDelims, StringSplitOptions.RemoveEmptyEntries).ToList();

			words = words.Select(p => p.Trim(WordTrim)).ToList();
			words = words.Distinct(StringComparer.CurrentCultureIgnoreCase).ToList();

			words.RemoveAll(p => (p.Length < minWordLength));

			return words;
		}

		public List<string> GetWords(UIExtension.TaskAttribute attrib, IBlacklist exclusions, Boolean force)
		{
			if (force || (attrib != m_WordAttribute))
			{
				m_Words = GetAttributeValues(attrib, exclusions);
				m_WordAttribute = attrib;
			}

			return m_Words;
		}

		public List<string> GetAttributeValues(UIExtension.TaskAttribute attrib, IBlacklist exclusions)
		{
			var values = new List<string>();

			switch (attrib)
			{
				case UIExtension.TaskAttribute.Title:			values = ToWords(Title);	break;
				case UIExtension.TaskAttribute.Comments:		values = ToWords(Comments); break;

				case UIExtension.TaskAttribute.AllocTo:			values = AllocTo;			break;
				case UIExtension.TaskAttribute.Category:		values = Category;			break;
				case UIExtension.TaskAttribute.Tag:				values = Tags;				break;

				case UIExtension.TaskAttribute.DoneDate:		values.Add(DoneDate);		break;
				case UIExtension.TaskAttribute.DueDate:			values.Add(DueDate);		break;
				case UIExtension.TaskAttribute.StartDate:		values.Add(StartDate);		break;
				case UIExtension.TaskAttribute.AllocBy:			values.Add(AllocBy);		break;
				case UIExtension.TaskAttribute.Status:			values.Add(Status);			break;
				case UIExtension.TaskAttribute.CreationDate:	values.Add(CreationDate);	break;
				case UIExtension.TaskAttribute.CreatedBy:		values.Add(CreatedBy);		break;
				case UIExtension.TaskAttribute.Version:			values.Add(Version);		break;
			}

			values.RemoveAll(p => (p.Length < 2));

			if (exclusions != null)
				values.RemoveAll(p => (exclusions.Countains(p)));
			
			return values;
		}

		public Boolean AttributeHasValue(UIExtension.TaskAttribute attrib, String value, IBlacklist exclusions)
		{
			var words = GetWords(attrib, exclusions, false);

			return (words.Find(x => x.Equals(value, StringComparison.CurrentCultureIgnoreCase)) != null);
		}
	}
}

