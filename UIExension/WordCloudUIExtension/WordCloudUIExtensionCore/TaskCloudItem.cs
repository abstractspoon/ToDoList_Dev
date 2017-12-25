using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Abstractspoon.Tdl.PluginHelpers;
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
		public bool HasIcon;
		public bool IsParent;
		public System.Drawing.Color TextColor;

		private List<string> m_Words;
		private UIExtension.TaskAttribute m_WordAttribute;

		static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.' };


		public void ProcessTaskUpdate(Task task, UIExtension.UpdateType type,
									  HashSet<UIExtension.TaskAttribute> attribs, bool newTask)
		{
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
				IsParent = task.GetFirstSubtask().IsValid();
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

		public bool Matches(IEnumerable<String> words, bool caseSensitive, bool wholeWord)
		{
			var attribs = new List<UIExtension.TaskAttribute>();

			attribs.Add(UIExtension.TaskAttribute.Title);
			attribs.Add(UIExtension.TaskAttribute.Comments);	

            StringComparison compare = (caseSensitive ? StringComparison.CurrentCulture : StringComparison.CurrentCultureIgnoreCase);

			foreach (var attrib in attribs)
			{
				var attribWords = GetAttributeValues(attrib, null);

				foreach (var word in words)
				{
					if (wholeWord)
					{
						if (attribWords.Any(x => x.Equals(word, compare)))
							return true;
					}
					else
					{
						if (attribWords.Any(x => x.IndexOf(word, compare) >= 0))
							return true;
					}
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

		public List<string> GetWords(UIExtension.TaskAttribute attrib, IBlacklist exclusions, bool force)
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

		public bool AttributeHasValue(UIExtension.TaskAttribute attrib, String value, IBlacklist exclusions)
		{
			var words = GetWords(attrib, exclusions, false);

			return (words.Find(x => x.Equals(value, StringComparison.CurrentCultureIgnoreCase)) != null);
		}
	}
}

