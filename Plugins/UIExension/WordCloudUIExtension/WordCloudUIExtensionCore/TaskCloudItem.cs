using System;
using System.Drawing;
using System.Windows.Forms;
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
			m_WordAttribute = Task.Attribute.Unknown;
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
				{
					if (SystemInformation.HighContrast)
						return SystemColors.HighlightText;

					// else
					return DrawingColor.SetLuminance(TextColor, 0.3f);
				}

                if (taskColorIsBkgnd && !IsDone(true))
                    return DrawingColor.GetBestTextColor(TextColor);
            }
            else
            {
                return System.Drawing.SystemColors.WindowText;
            }

            return TextColor;
        }

        public System.Drawing.Color GetBackColor(Boolean taskColorIsBkgnd)
        {
            if (!taskTextColor.IsEmpty)
            {
                if (taskColorIsBkgnd && !IsDone(true))
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

        public Boolean SetDone(Boolean done)
        {
			if (done == IsDone(false))
				return false;

            // else
			if (done)
				DoneDate = DateTime.Now.ToShortDateString();
			else
				DoneDate = String.Empty;

            return true;
        }
        
		private List<string> m_Words;
		private Task.Attribute m_WordAttribute;

		static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.', '[', ']' };

		public void ProcessTaskUpdate(Task task, UIExtension.UpdateType type, Boolean newTask)
		{
            IsParent = task.IsParent();
            IsLocked = task.IsLocked(true);
            IsGoodAsDone = task.IsGoodAsDone();
            HasSomeSubtasksDone = task.HasSomeSubtasksDone();

			if (newTask)
			{
				Title = task.GetTitle();
				DoneDate = SplitDate(task.GetDoneDateString());
				DueDate = SplitDate(task.GetDueDateString(false));
				StartDate = SplitDate(task.GetStartDateString(false));
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
				if (task.IsAttributeAvailable(Task.Attribute.Title))
					Title = task.GetTitle();

				if (task.IsAttributeAvailable(Task.Attribute.DoneDate))
					DoneDate = SplitDate(task.GetDoneDateString());

				if (task.IsAttributeAvailable(Task.Attribute.DueDate))
					DueDate = SplitDate(task.GetDueDateString(false));

				if (task.IsAttributeAvailable(Task.Attribute.StartDate))
					StartDate = SplitDate(task.GetStartDateString(false));

				if (task.IsAttributeAvailable(Task.Attribute.AllocatedTo))
					AllocTo = task.GetAllocatedTo();

				if (task.IsAttributeAvailable(Task.Attribute.AllocatedBy))
					AllocBy = task.GetAllocatedBy();

				if (task.IsAttributeAvailable(Task.Attribute.Status))
					Status = task.GetStatus();

				if (task.IsAttributeAvailable(Task.Attribute.Category))
					Category = task.GetCategory();

				if (task.IsAttributeAvailable(Task.Attribute.Comments))
					Comments = task.GetComments();

				if (task.IsAttributeAvailable(Task.Attribute.CreationDate))
					CreationDate = SplitDate(task.GetCreationDateString());

				if (task.IsAttributeAvailable(Task.Attribute.CreatedBy))
					CreatedBy = task.GetCreatedBy();

				if (task.IsAttributeAvailable(Task.Attribute.Version))
					Version = task.GetVersion();

				if (task.IsAttributeAvailable(Task.Attribute.Tags))
					Tags = task.GetTag();

				if (task.IsAttributeAvailable(Task.Attribute.Icon))
					HasIcon = (task.GetIcon().Length > 0);

				if (task.IsAttributeAvailable(Task.Attribute.Color))
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

		public List<string> GetWords(Task.Attribute attrib, IBlacklist exclusions, Boolean force)
		{
			if (force || (attrib != m_WordAttribute))
			{
				m_Words = GetAttributeValues(attrib, exclusions);
				m_WordAttribute = attrib;
			}

			return m_Words;
		}

		public List<string> GetAttributeValues(Task.Attribute attrib, IBlacklist exclusions)
		{
			var values = new List<string>();

			switch (attrib)
			{
				case Task.Attribute.Title:			values = ToWords(Title);	break;
				case Task.Attribute.Comments:		values = ToWords(Comments); break;

				case Task.Attribute.AllocatedTo:	values = AllocTo;			break;
				case Task.Attribute.Category:		values = Category;			break;
				case Task.Attribute.Tags:			values = Tags;				break;

				case Task.Attribute.DoneDate:		values.Add(DoneDate);		break;
				case Task.Attribute.DueDate:		values.Add(DueDate);		break;
				case Task.Attribute.StartDate:		values.Add(StartDate);		break;
				case Task.Attribute.AllocatedBy:	values.Add(AllocBy);		break;
				case Task.Attribute.Status:			values.Add(Status);			break;
				case Task.Attribute.CreationDate:	values.Add(CreationDate);	break;
				case Task.Attribute.CreatedBy:		values.Add(CreatedBy);		break;
				case Task.Attribute.Version:		values.Add(Version);		break;
			}

			values.RemoveAll(p => (p.Length < 2));

			if (exclusions != null)
				values.RemoveAll(p => (exclusions.Countains(p)));
			
			return values;
		}

		public Boolean AttributeHasValue(Task.Attribute attrib, String value, IBlacklist exclusions)
		{
			var words = GetWords(attrib, exclusions, false);

			return (words.Find(x => x.Equals(value, StringComparison.CurrentCultureIgnoreCase)) != null);
		}
	}
}

