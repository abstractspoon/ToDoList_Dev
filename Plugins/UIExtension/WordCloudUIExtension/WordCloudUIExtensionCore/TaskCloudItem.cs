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
	public class CloudTaskItem : ITaskBase
	{
		public CloudTaskItem(uint id)
		{
			Id = id;
			m_WordAttribute = Task.Attribute.Unknown;
		}

		public override String ToString()
		{
			return Title;
		}
        
		// ITaskBase --------------------------------------
		public uint Id				{ get; private set; }
		public String Title			{ get; set; }
		public bool HasIcon			{ get; set; }
		public bool IsLocked		{ get; set; }
        public bool IsDone			{ get; set; }
        public bool IsGoodAsDone	{ get; set; }
		public bool IsParent		{ get; set; }

		public String Position		{ get { return String.Empty; } } // not relevant
		public int Depth			{ get { return 0; } } // not relevant

		// Local attributes -------------------------------
		public String DoneDate;
		public String DueDate;
		public String StartDate;
		public String AllocBy;
		public String Status;
		public String Comments;
		public String CreationDate;
		public String CreatedBy;
		public String Version;
		public List<String> AllocTo;
		public List<String> Category;
		public List<String> Tags;
        public bool HasSomeSubtasksDone;

        private System.Drawing.Color taskTextColor = System.Drawing.Color.Empty;
            
        public System.Drawing.Color TextColor
        {
            get { return taskTextColor; }
            set { taskTextColor = value; }
        }

        public System.Drawing.Color GetTextColor(bool isSelected, bool taskColorIsBkgnd)
        {
			if (isSelected)
			{
				if (SystemInformation.HighContrast)
					return SystemColors.HighlightText;

				// else
				return UIExtension.SelectionRect.GetTextColor(UIExtension.SelectionRect.Style.Selected, taskTextColor);
			}

            if (!taskTextColor.IsEmpty)
                return (taskColorIsBkgnd ? DrawingColor.GetBestTextColor(TextColor, true) : taskTextColor);

			// else
			return System.Drawing.SystemColors.WindowText;
        }

        public System.Drawing.Color GetBackColor(bool taskColorIsBkgnd)
        {
            if (!taskTextColor.IsEmpty && taskColorIsBkgnd)
                return TextColor;

            // else
            return System.Drawing.Color.Empty;
        }

        public bool SetDone(bool done)
        {
			if (done == IsDone)
				return false;

            // else
			if (done)
				DoneDate = DateTime.Now.ToShortDateString();
			else
				DoneDate = String.Empty;

            return true;
        }
        
		private List<String> m_Words;
		private Task.Attribute m_WordAttribute;

		static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.', '[', ']' };

		public void ProcessTaskUpdate(Task task, UIExtension.UpdateType type, bool newTask)
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

		static String SplitDate(String date)
		{
			String[] parts = date.Split(new char[] { ' ' }, 2);

			if (parts.Length > 0)
				return parts[0];

			// else
			return "";
		}

		public bool Matches(String words, bool caseSensitive, bool wholeWord, bool titleOnly)
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
                    bool match = true;

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

		public static List<String> ToWords(String text, IBlacklist exclusions)
		{
			var words = ToWords(text, 2);

			if (exclusions != null)
				words.RemoveAll(p => (exclusions.Countains(p)));

			return words;
		}

		public static List<String> ToWords(String text, int minWordLength = 0)
		{
			List<String> words = text.Split(WordDelims, StringSplitOptions.RemoveEmptyEntries).ToList();

			words = words.Select(p => p.Trim(WordTrim)).ToList();
			words = words.Distinct(StringComparer.CurrentCultureIgnoreCase).ToList();

			words.RemoveAll(p => (p.Length < minWordLength));

			return words;
		}

		public List<String> GetWords(Task.Attribute attrib, IBlacklist exclusions, bool force)
		{
			if (force || (attrib != m_WordAttribute))
			{
				m_Words = GetAttributeValues(attrib, exclusions);
				m_WordAttribute = attrib;
			}

			return m_Words;
		}

		public List<String> GetAttributeValues(Task.Attribute attrib, IBlacklist exclusions)
		{
			var values = new List<String>();

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

		public bool AttributeHasValue(Task.Attribute attrib, String value, IBlacklist exclusions)
		{
			var words = GetWords(attrib, exclusions, false);

			return (words.Find(x => x.Equals(value, StringComparison.CurrentCultureIgnoreCase)) != null);
		}
	}
}

