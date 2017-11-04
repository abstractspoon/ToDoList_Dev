
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
        private const int LabelTop = 2;
		private const int ComboTop = 20;
        private const int LabelHeight = (ComboTop - LabelTop);
        private const int ComboHeight = 20;
        private const int ControlTop = 49;
		private const int MatchesWidth = 200;
        private const string FontName = "Tahoma";

        // -------------------------------------------------------------

		[FlagsAttribute]
		private enum IgnoreSelChanges
		{
			None		= 0x0,
			WordChange	= 0x1,
			MatchChange	= 0x2,
			All			= 0xf,
		}

		private bool WantIgnoreSelChange(IgnoreSelChanges change)
		{
			return ((m_IgnoreSelChanges & change) == change);
		}

        // -------------------------------------------------------------

		private IntPtr m_HwndParent;
        private UIExtension.TaskAttribute m_Attrib;
		private Translator m_Trans;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private TdlCloudControl m_WordCloud;
        private IBlacklist m_ExcludedWords;
		private IgnoreSelChanges m_IgnoreSelChanges;

        private StyleComboBox m_StylesCombo;
		private System.Windows.Forms.Label m_StylesLabel;
		private AttributeComboBox m_AttributeCombo;
        private System.Windows.Forms.Label m_AttributeLabel;
        private ColorSchemeComboBox m_ColorsCombo;
        private System.Windows.Forms.Label m_ColorsLabel;
		private TaskMatchesListView m_TaskMatchesList;

		private System.Drawing.Font m_ControlsFont;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_Attrib = UIExtension.TaskAttribute.Title;
            m_ExcludedWords = new CommonWords(); // English by default

			m_ControlsFont = new System.Drawing.Font(FontName, 8);
			m_IgnoreSelChanges = IgnoreSelChanges.None;

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

			m_IgnoreSelChanges = IgnoreSelChanges.MatchChange;

			if ((m_WordCloud.SelectedItem == null) ||
				!words.Exists(x => x.Equals(m_WordCloud.SelectedItem, StringComparison.CurrentCultureIgnoreCase)))
			{
				var matches = m_WordCloud.WeightedWords.Where(a => words.Any(x => x.Equals(a.Text, StringComparison.CurrentCultureIgnoreCase))).SortByOccurences();

				if (!matches.Any())
					return false;

				var bestMatch = matches.First();

				m_WordCloud.SelectedItem = bestMatch.Text;
			}
			m_IgnoreSelChanges = IgnoreSelChanges.None;

 			m_TaskMatchesList.SetSelectedMatchId(item.Id);

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

			var selStyle = m_StylesCombo.SelectedItem as StyleComboBox.StyleItem;

			if (selStyle != null)
			{
				if (selStyle.Sorted)
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
			System.Drawing.Point ptClient = m_TaskMatchesList.PointToClient(new System.Drawing.Point(xPos, yPos));
			ListViewHitTestInfo lvHit = m_TaskMatchesList.HitTest(ptClient);

			if (lvHit.Item != null)
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
			CreateTaskMatchesListView();

			Invalidate(true);
		}


		private void CreateWordCloud()
		{
			this.m_WordCloud = new TdlCloudControl(this.Handle, m_Trans, FontName);

			this.m_WordCloud.Location = new System.Drawing.Point(0, ControlTop);
			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);

			this.Controls.Add(m_WordCloud);

			this.m_WordCloud.SelectionChange += new SelectionChangeEventHandler(OnWordSelectionChanged);

		}

		private void CreateTaskMatchesListView()
		{
			this.m_TaskMatchesList = new TaskMatchesListView(m_HwndParent);
			this.m_TaskMatchesList.Initialise(m_Trans);

			this.m_TaskMatchesList.Font = m_ControlsFont;
			this.m_TaskMatchesList.Location = new System.Drawing.Point(0, ComboTop);
			this.m_TaskMatchesList.Size = new System.Drawing.Size(200, 500);

			this.Controls.Add(m_TaskMatchesList);

			// Add message handlers
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
			this.m_StylesCombo = new StyleComboBox();
			this.m_StylesCombo.Initialise(m_Trans);

			this.m_StylesCombo.Font = m_ControlsFont;
			this.m_StylesCombo.Location = new System.Drawing.Point(440, ComboTop);
			this.m_StylesCombo.Size = new System.Drawing.Size(200, 16);
			this.m_StylesCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			this.m_StylesCombo.SetSelectedStyle(Gma.CodeCloud.Controls.LayoutType.Spiral);
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

            System.Drawing.Rectangle wordCloudRect = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			System.Drawing.Rectangle taskMatchesRect = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);

			taskMatchesRect.X = (taskMatchesRect.Right - MatchesWidth);
			taskMatchesRect.Y = ControlTop;
			taskMatchesRect.Width = MatchesWidth;
			taskMatchesRect.Height -= ControlTop;

			m_TaskMatchesList.Location = taskMatchesRect.Location;
			m_TaskMatchesList.Size = taskMatchesRect.Size;
			
			wordCloudRect.Width -= (MatchesWidth + 3);
            wordCloudRect.Y = ControlTop;
            wordCloudRect.Height -= ControlTop;
			wordCloudRect.Inflate(-1, -1); // for border

            m_WordCloud.Location = wordCloudRect.Location;
            m_WordCloud.Size = wordCloudRect.Size;

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
            var selStyle = m_StylesCombo.GetSelectedStyle();

			m_WordCloud.LayoutType = selStyle;
			UpdateWeightedWords();
        }

		private void OnWordSelectionChanged(object sender)
		{
			if (WantIgnoreSelChange(IgnoreSelChanges.WordChange))
				return;

			// else
			m_IgnoreSelChanges |= IgnoreSelChanges.MatchChange;

            string selWord = m_WordCloud.SelectedItem;

			if (selWord != null)
			{
				CloudTaskItem selItem = null;
				UInt32 selItemId = m_TaskMatchesList.GetSelectedMatchId();

				// Build a list of task items containing this value
				m_TaskMatchesList.ClearMatches();

				foreach (var item in m_Items)
				{
					if (item.Value.AttributeHasValue(m_Attrib, selWord))
					{
						m_TaskMatchesList.AddMatch(item.Value);

						if (item.Value.Id == selItemId)
							selItem = item.Value;
					}
				}

				string headerText = m_Trans.Translate("Task Matches");

				if (m_TaskMatchesList.Items.Count > 0)
				{
					// See if our currently selected task is in the new match list
					selItemId = m_TaskMatchesList.SetSelectedMatchId(selItemId);

					if ((selItem == null) || (selItemId != selItem.Id))
						NotifyParentSelChange(selItemId);

					m_TaskMatchesList.Columns[0].Text = String.Format("{0} ({1})", headerText, m_TaskMatchesList.Items.Count);
					m_TaskMatchesList.Invalidate();
				}
				else
				{
					m_TaskMatchesList.Columns[0].Text = headerText;
				}
			}

			m_IgnoreSelChanges &= ~IgnoreSelChanges.MatchChange;
		}

		private void OnTaskMatchesSelChanged(object sender, EventArgs args)
		{
			if (WantIgnoreSelChange(IgnoreSelChanges.MatchChange))
				return;

			// else
			m_IgnoreSelChanges |= IgnoreSelChanges.WordChange;

			UInt32 selTaskId = m_TaskMatchesList.GetSelectedMatchId();

			if (selTaskId != 0)
				NotifyParentSelChange(selTaskId);

			m_IgnoreSelChanges &= ~IgnoreSelChanges.WordChange;
		}

		private void NotifyParentSelChange(UInt32 taskId)
		{
			var parent = new UIExtension.ParentNotify(m_HwndParent);

			parent.NotifySelChange(taskId);
		}
	}
}
