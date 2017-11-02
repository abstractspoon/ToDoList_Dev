
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;

using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

using Abstractspoon.Tdl.PluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	public class WordCloudUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
	{
        // Helper classes
		private class StyleComboItem
		{
			public StyleComboItem(String text, Gma.CodeCloud.Controls.LayoutType type, bool sorted)
			{
				m_Text = text;
				m_Type = type;
				m_Sorted = sorted;
			}

			public String Text 
			{ 
				get { return m_Text; } 
			}
			
			public Gma.CodeCloud.Controls.LayoutType Type
			{
				get { return m_Type; }
			}

			public bool Sorted
			{
				get { return m_Sorted; }
			}
			
			public override String ToString()
			{
				return Text;
			}
			
			private String m_Text;
			private Gma.CodeCloud.Controls.LayoutType m_Type;
			private bool m_Sorted;
		}

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

			private List<string> m_Words;
			private UIExtension.TaskAttribute m_WordAttribute;
			
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
					HasIcon = (task.GetIcon().Length > 0);
					IsParent = (task.GetFirstSubtask() != null);
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
				}
			}

			static string SplitDate(string date) 
			{
				string[] parts = date.Split(new char[] {' '}, 2);

				if (parts.Length > 0)
					return parts[0];

				// else
				return "";
			}

            public List<string> GetWords(UIExtension.TaskAttribute attrib)
            {
				if (attrib != m_WordAttribute)
				{
					var values = GetAttributeValues(attrib);

					// Split title and comments only into individual words removing duplicates
					if ((attrib == UIExtension.TaskAttribute.Title) ||
						(attrib == UIExtension.TaskAttribute.Comments))
					{
						if (values.Count > 0)
						{
							List<string> parts = values[0].Split(WordDelims, StringSplitOptions.RemoveEmptyEntries).Select(p => p.Trim(WordTrim)).ToList();

							values = parts.Distinct(StringComparer.CurrentCultureIgnoreCase).ToList();
						}
					}

					values.RemoveAll(p => (p.Length < 2));
					m_Words = values;
				}

				return m_Words;
            }

            public List<string> GetAttributeValues(UIExtension.TaskAttribute attrib)
            {
                var values = new List<string>();

                switch (attrib)
                {
					case UIExtension.TaskAttribute.Title:			values.Add(Title);			break;
					case UIExtension.TaskAttribute.DoneDate:		values.Add(DoneDate);		break;
					case UIExtension.TaskAttribute.DueDate:			values.Add(DueDate);		break;
					case UIExtension.TaskAttribute.StartDate:		values.Add(StartDate);		break;
					case UIExtension.TaskAttribute.AllocBy:			values.Add(AllocBy);		break;
					case UIExtension.TaskAttribute.Status:			values.Add(Status);			break;
					case UIExtension.TaskAttribute.Comments:		values.Add(Comments);		break;
					case UIExtension.TaskAttribute.CreationDate:	values.Add(CreationDate);	break;
					case UIExtension.TaskAttribute.CreatedBy:		values.Add(CreatedBy);		break;
					case UIExtension.TaskAttribute.Version:			values.Add(Version);		break;

					case UIExtension.TaskAttribute.AllocTo:			values = AllocTo;			break;
					case UIExtension.TaskAttribute.Category:		values = Category;			break;
					case UIExtension.TaskAttribute.Tag:				values = Tags;				break;
				}

                return values;
            }

			public bool AttributeHasValue(UIExtension.TaskAttribute attrib, String value)
			{
				GetWords(attrib);

 				return (m_Words.Find(x => x.Equals(value, StringComparison.CurrentCultureIgnoreCase)) != null);
			}
        }

        // -------------------------------------------------------------

        private const int LabelTop = 2;
		private const int ComboTop = 20;
        private const int LabelHeight = (ComboTop - LabelTop);
        private const int ComboHeight = 20;
        private const int ControlTop = 49;
		private const int MatchesWidth = 200;
        private const string FontName = "Tahoma";

		private static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		private static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.' };

        // -------------------------------------------------------------

		private IntPtr m_hwndParent;
        private UIExtension.TaskAttribute m_Attrib;
		private Translator m_Trans;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private TdlCloudControl m_WordCloud;
        private IBlacklist m_ExcludedWords;
		private bool m_WordSelChanging;

        private ComboBox m_StylesCombo;
		private System.Windows.Forms.Label m_StylesLabel;
		private AttributeComboBox m_AttributeCombo;
        private System.Windows.Forms.Label m_AttributeLabel;
        private ColorSchemeComboBox m_ColorsCombo;
        private System.Windows.Forms.Label m_ColorsLabel;

		private System.Windows.Forms.Label m_TaskMatchesLabel;
		private ListBox m_TaskMatchesList;
		private bool m_TaskMatchesHaveIcons;
		
		private System.Drawing.Font m_ControlsFont;
		private UIExtension.TaskIcon m_TaskIcons;
		private UIExtension.SelectionRect m_SelectionRect;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent, Translator trans)
		{
			m_hwndParent = hwndParent;
			m_Trans = trans;
			m_Attrib = UIExtension.TaskAttribute.Title;
            m_ExcludedWords = new CommonWords(); // English by default

			m_ControlsFont = new System.Drawing.Font(FontName, 8);
			m_TaskIcons = new UIExtension.TaskIcon(hwndParent);
			m_SelectionRect = new UIExtension.SelectionRect();

			m_WordSelChanging = false;

			InitializeComponent();
		}

		// IUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 dwTaskID)
		{
			if (m_Attrib == UIExtension.TaskAttribute.Unknown)
				return false;

			CloudTaskItem item;

			if (!m_Items.TryGetValue(dwTaskID, out item))
				return false;

			var words = item.GetWords(m_Attrib);

			if (!words.Any())
				return false;

			if (!words.Exists(x => x.Equals(m_WordCloud.SelectedItem, StringComparison.CurrentCultureIgnoreCase)))
			{
				var matches = m_WordCloud.WeightedWords.Where(a => words.Any(x => x.Equals(a.Text, StringComparison.CurrentCultureIgnoreCase))).SortByOccurences();

				if (!matches.Any())
					return false;

				var bestMatch = matches.First();

				m_WordCloud.SelectedItem = bestMatch.Text;
			}

			// Our task should be in the list...
			m_TaskMatchesList.SelectedItem = item;
			
			return true;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
		}

		public void UpdateTasks(TaskList tasks, 
								UIExtension.UpdateType type, 
								System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.Move:
				case UIExtension.UpdateType.All:
					// Rebuild
					break;
					
				case UIExtension.UpdateType.New:
				case UIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, attribs))
				task = task.GetNextTask();

            // Update
			UpdateWeightedWords();
		}

		private bool ProcessTaskUpdate(Task task, 
									   UIExtension.UpdateType type,
                                       HashSet<UIExtension.TaskAttribute> attribs)
		{
			if (!task.IsValid())
				return false;

			CloudTaskItem item;
			bool newTask = !m_Items.TryGetValue(task.GetID(), out item);

			if (newTask)
			{
				item = new CloudTaskItem(task.GetID());
				m_Items[item.Id] = item;
			}

			item.ProcessTaskUpdate(task, type, attribs, newTask);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, attribs))
				subtask = subtask.GetNextTask();

			return true;
		}

		protected void UpdateWeightedWords()
		{
			var words = new List<string>();

			foreach (var item in m_Items)
			{
				var taskWords = item.Value.GetWords(m_Attrib);
				words.AddRange(taskWords);
			}

			StyleComboItem selItem = (StyleComboItem)m_StylesCombo.SelectedItem;

			if (selItem != null)
			{
				if (selItem.Sorted)
					this.m_WordCloud.WeightedWords = words.Filter(m_ExcludedWords).CountOccurences().SortByText();
				else
					this.m_WordCloud.WeightedWords = words.Filter(m_ExcludedWords).CountOccurences().SortByOccurences();
			}
		}

		public bool WantEditUpdate(UIExtension.TaskAttribute attrib)
		{
			if (AttributeComboBox.IsSupportedAttribute(attrib))
				return true;

			// else
			return (attrib == UIExtension.TaskAttribute.Icon);
		}
	   
		public bool WantSortUpdate(UIExtension.TaskAttribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
			return false;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}
	   
		public bool DoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			switch (cmd)
			{
				case UIExtension.AppCommand.SelectTask:
					return SelectTask(dwExtra);
			}

			// else
			return false;
		}

		public bool CanDoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			switch (cmd)
			{
				case UIExtension.AppCommand.SelectTask:
					return true;
			}

			// else
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            return false;
		}

		public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
		{
            System.Drawing.Point pt = m_WordCloud.PointToClient(new System.Drawing.Point(xPos, yPos));
            Gma.CodeCloud.Controls.Geometry.LayoutItem foundItem;

            if (m_WordCloud.TryGetItemAtLocation(pt, out foundItem))
                return UIExtension.HitResult.Task;

            // else
			return UIExtension.HitResult.Nowhere;
		}

		public void SetUITheme(UITheme theme)
		{
            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_AttributeLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
			m_ColorsLabel.ForeColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "AttribToTrack", (int)m_Attrib);
			prefs.WriteProfileString(key, "ColorScheme", m_ColorsCombo.GetSelectedSchemeAsString());
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (appOnly)
            {
				CommonBlacklist blacklist = new CommonBlacklist();

                // Look for user-defined 'Ignore' file
                string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string ignoreFile = Path.Combine(appPath, "WordCloud.Ignore.txt");

				int numWords = blacklist.Append(ignoreFile);

                // Also look for localised ignore file
                string language = prefs.GetProfileString("Preferences", "LanguageFile", "");
                    
                ignoreFile = Path.Combine(appPath, language);
                ignoreFile = Path.ChangeExtension(ignoreFile, "WordCloud.Ignore.txt");

				numWords += blacklist.Append(ignoreFile);

				if (numWords > 0)
				{
                    m_ExcludedWords = blacklist;
                    UpdateWeightedWords();
                }
            }
            else // private settings
			{
				var attrib = (UIExtension.TaskAttribute)prefs.GetProfileInt(key, "AttribToTrack", (int)UIExtension.TaskAttribute.Title);

				if (!AttributeComboBox.IsSupportedAttribute(attrib))
					attrib = UIExtension.TaskAttribute.Title;

				m_AttributeCombo.SetSelectedAttribute(attrib);

				string scheme = prefs.GetProfileString(key, "ColorScheme", "");

				if (!m_ColorsCombo.SetSelectedScheme(scheme))
					m_ColorsCombo.SelectedIndex = 0;
			}
		}
		
		// PRIVATE ------------------------------------------------------------------------------

		private void InitializeComponent()
		{
			this.BackColor = System.Drawing.Color.White;
			this.m_Items = new Dictionary<UInt32, CloudTaskItem>();

			CreateWordCloud();
            CreateAttributeCombo();
			CreateColorSchemeCombo();
			CreateStyleCombo();
			CreateTaskMatchesListbox();

			Invalidate(true);
		}


		private void CreateWordCloud()
		{
			this.m_WordCloud = new TdlCloudControl(this.Handle, m_Trans, FontName);

			this.m_WordCloud.Location = new System.Drawing.Point(0, ControlTop);
			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);
		//	this.m_WordCloud.BorderStyle = BorderStyle.None;

			this.Controls.Add(m_WordCloud);

			this.m_WordCloud.SelectionChange += new SelectionChangeEventHandler(OnWordSelectionChanged);

		}

		private void CreateTaskMatchesListbox()
		{
			// Label
			this.m_TaskMatchesLabel = new System.Windows.Forms.Label();

			this.m_TaskMatchesLabel.Font = m_ControlsFont;
			this.m_TaskMatchesLabel.Location = new System.Drawing.Point(-2, LabelTop);
			this.m_TaskMatchesLabel.Size = new System.Drawing.Size(200, 16);
			this.m_TaskMatchesLabel.Text = m_Trans.Translate("&Task Matches");
			this.m_TaskMatchesLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			//this.m_TaskMatchesLabel.FlatStyle = FlatStyle.Standard;

			this.Controls.Add(m_TaskMatchesLabel);

			// Combo
			this.m_TaskMatchesList = new ListBox();

			this.m_TaskMatchesList.DrawMode = DrawMode.OwnerDrawFixed;
			this.m_TaskMatchesList.Font = m_ControlsFont;
			this.m_TaskMatchesList.Location = new System.Drawing.Point(0, ComboTop);
			this.m_TaskMatchesList.Size = new System.Drawing.Size(200, 500);
			this.m_TaskMatchesList.ItemHeight = m_ControlsFont.Height + 5;
			this.m_TaskMatchesList.IntegralHeight = false;
			this.m_TaskMatchesList.Sorted = true;
			
			this.Controls.Add(m_TaskMatchesList);

			// Add message handlers
			this.m_TaskMatchesList.DrawItem += new DrawItemEventHandler(OnDrawTaskMatchesItem);
			this.m_TaskMatchesList.SelectedIndexChanged += new EventHandler(OnTaskMatchesSelChanged);
		}

		private void CreateAttributeCombo()
		{
			// Label
			this.m_AttributeLabel = new System.Windows.Forms.Label();

            this.m_AttributeLabel.Font = m_ControlsFont;
			this.m_AttributeLabel.Location = new System.Drawing.Point(-2, LabelTop);
			this.m_AttributeLabel.Size = new System.Drawing.Size(200, 16);
			this.m_AttributeLabel.Text = m_Trans.Translate("&Attribute to 'track'");
			this.m_AttributeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_AttributeLabel);

			// Combo
			this.m_AttributeCombo = new AttributeComboBox();

            this.m_AttributeCombo.Font = m_ControlsFont;
			this.m_AttributeCombo.Location = new System.Drawing.Point(0, ComboTop);
			this.m_AttributeCombo.Size = new System.Drawing.Size(200, 16);
			this.m_AttributeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_AttributeCombo.Initialise(m_Trans);

			this.Controls.Add(m_AttributeCombo);

			// Add selection change handler
			this.m_AttributeCombo.SelectedIndexChanged += new EventHandler(OnAttributeSelChanged);
		}

		private void CreateColorSchemeCombo()
		{
			// Label
			this.m_ColorsLabel = new System.Windows.Forms.Label();

			this.m_ColorsLabel.Font = m_ControlsFont;
			this.m_ColorsLabel.Location = new System.Drawing.Point(218, LabelTop);
			this.m_ColorsLabel.Size = new System.Drawing.Size(200, 16);
			this.m_ColorsLabel.Text = m_Trans.Translate("&Colour Scheme");
			this.m_ColorsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_ColorsLabel);

			// Combo
			this.m_ColorsCombo = new ColorSchemeComboBox();

            this.m_ColorsCombo.Font = m_ControlsFont;
			this.m_ColorsCombo.Location = new System.Drawing.Point(220, ComboTop);
			this.m_ColorsCombo.Size = new System.Drawing.Size(200, 16);
			this.m_ColorsCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			this.Controls.Add(m_ColorsCombo);

			// Add selection change handler
			this.m_ColorsCombo.SelectedIndexChanged += new EventHandler(OnColorSchemeSelChanged);
		}

		private void CreateStyleCombo()
		{
			// Label
			this.m_StylesLabel = new System.Windows.Forms.Label();

			this.m_StylesLabel.Font = m_ControlsFont;
			this.m_StylesLabel.Location = new System.Drawing.Point(438, LabelTop);
			this.m_StylesLabel.Size = new System.Drawing.Size(200, 16);
			this.m_StylesLabel.Text = m_Trans.Translate("&Style");
			this.m_StylesLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_StylesLabel);

			// Combo
			this.m_StylesCombo = new ComboBox();

			this.m_StylesCombo.Font = m_ControlsFont;
			this.m_StylesCombo.Location = new System.Drawing.Point(440, ComboTop);
			this.m_StylesCombo.Size = new System.Drawing.Size(200, 16);
			this.m_StylesCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			this.m_StylesCombo.Items.Add(new StyleComboItem(m_Trans.Translate("Spiral"), Gma.CodeCloud.Controls.LayoutType.Spiral, false));
			this.m_StylesCombo.Items.Add(new StyleComboItem(m_Trans.Translate("Typewriter"), Gma.CodeCloud.Controls.LayoutType.Typewriter, true));

			this.m_StylesCombo.SelectedIndex = 0;
			this.m_WordCloud.LayoutType = Gma.CodeCloud.Controls.LayoutType.Spiral;

			this.Controls.Add(m_StylesCombo);

			// Add selection change handler
			this.m_StylesCombo.SelectedIndexChanged += new EventHandler(OnStyleComboSelChanged);
		}

		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            System.Drawing.Rectangle border = new System.Drawing.Rectangle(m_WordCloud.Location, m_WordCloud.Size);
			border.Inflate(1, 1);

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, border, System.Drawing.Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            System.Drawing.Rectangle WordCloud = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			System.Drawing.Rectangle TaskMatchesLabel = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			System.Drawing.Rectangle TaskMatchesListbox = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);

			TaskMatchesLabel.X = (TaskMatchesLabel.Right - MatchesWidth);
			TaskMatchesLabel.Y = (ControlTop - LabelHeight - LabelTop);
			TaskMatchesLabel.Width = MatchesWidth;
			TaskMatchesLabel.Height = LabelHeight;

			m_TaskMatchesLabel.Location = TaskMatchesLabel.Location;
			m_TaskMatchesLabel.Size = TaskMatchesLabel.Size;
			
			TaskMatchesListbox.X = TaskMatchesLabel.X;
			TaskMatchesListbox.Width = TaskMatchesLabel.Width;
			TaskMatchesListbox.Y = ControlTop;
			TaskMatchesListbox.Height -= ControlTop;

			m_TaskMatchesList.Location = TaskMatchesListbox.Location;
			m_TaskMatchesList.Size = TaskMatchesListbox.Size;
			
			WordCloud.Width -= (MatchesWidth + 3);
            WordCloud.Y = ControlTop;
            WordCloud.Height -= ControlTop;
			WordCloud.Inflate(-1, -1); // for border

            m_WordCloud.Location = WordCloud.Location;
            m_WordCloud.Size = WordCloud.Size;

            Win32.RemoveBorder(m_WordCloud.Handle);
            Win32.RemoveClientEdge(m_WordCloud.Handle);

			Invalidate(true);
        }

        private void OnAttributeSelChanged(object sender, EventArgs args)
        {
            m_Attrib = m_AttributeCombo.GetSelectedAttribute();

            UpdateWeightedWords();
        }

		private void OnColorSchemeSelChanged(object sender, EventArgs args)
		{
			m_WordCloud.Palette = m_ColorsCombo.GetSelectedScheme();
		}

		private void OnStyleComboSelChanged(object sender, EventArgs args)
		{
            StyleComboItem selItem = (StyleComboItem)m_StylesCombo.SelectedItem;

			if (selItem != null)
			{
				m_WordCloud.LayoutType = selItem.Type;
				UpdateWeightedWords();
			}
        }

		private void OnWordSelectionChanged(object sender)
		{
			m_WordSelChanging = true;

            string selItem = m_WordCloud.SelectedItem;

			if (selItem != null)
			{
				// Build a list of task items containing this value
				m_TaskMatchesList.Items.Clear();
				m_TaskMatchesHaveIcons = false;

				foreach (var item in m_Items)
				{
					if (item.Value.AttributeHasValue(m_Attrib, selItem))
					{
						m_TaskMatchesList.Items.Add(item.Value);

						if (m_TaskIcons != null)
							m_TaskMatchesHaveIcons |= item.Value.HasIcon;
					}
				}

				if (m_TaskMatchesList.Items.Count > 0)
				{
					m_TaskMatchesList.SelectedIndex = 0;
					m_TaskMatchesLabel.Text = String.Format(m_Trans.Translate("&Task Matches ({0})"), m_TaskMatchesList.Items.Count);
				}
				else
				{
					m_TaskMatchesLabel.Text = m_Trans.Translate("&Task Matches");
				}
			}

			m_WordSelChanging = false;
		}

		private void OnDrawTaskMatchesItem(object sender, DrawItemEventArgs e)
		{
			// Get the ListBox and the item.
			if ((e.Index < 0) || (e.Index >= m_TaskMatchesList.Items.Count))
				return;

			var item = (m_TaskMatchesList.Items[e.Index] as CloudTaskItem);

			if (item == null)
				return;

			// Draw the background.
			bool selected = ((e.State & DrawItemState.Selected) == DrawItemState.Selected);

			if (selected)
				m_SelectionRect.Draw(m_TaskMatchesList.Handle, e.Graphics, e.Bounds.X, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
			else
				e.DrawBackground();

			// Draw the icon
			if (m_TaskMatchesHaveIcons && item.HasIcon)
			{
				if ((item.IsParent || item.HasIcon) && m_TaskIcons.Get(item.Id))
				{
					Point ptIcon = new Point(e.Bounds.Left + 2, e.Bounds.Top/* + 2*/);
					m_TaskIcons.Draw(e.Graphics, ptIcon.X, ptIcon.Y);
				}
			}

			// Draw the text
			string text = item.ToString();
			StringFormat stringFormat = new StringFormat();
			stringFormat.Alignment = StringAlignment.Near;
			stringFormat.LineAlignment = StringAlignment.Center;
			stringFormat.FormatFlags = StringFormatFlags.NoWrap;
			stringFormat.Trimming = StringTrimming.EllipsisCharacter;

			// Adjust for icon
			Rectangle textRect = new Rectangle(e.Bounds.Location, e.Bounds.Size);

			if (m_TaskMatchesHaveIcons)
			{
				textRect.X += 18;
				textRect.Width -= 18;
			}
			textRect.Y++;

			e.Graphics.DrawString(text, 
				                  this.Font, 
								  SystemBrushes.WindowText, 
								  textRect,
								  stringFormat);
		}

		private void OnTaskMatchesSelChanged(object sender, EventArgs args)
		{
			if (!m_WordSelChanging)
			{
				var selItem = m_TaskMatchesList.SelectedItem as CloudTaskItem;

				if (selItem != null)
				{
					var parent = new UIExtension.ParentNotify(m_hwndParent);

					parent.NotifySelChange(selItem.Id);
				}
			}
		}
    }
}
