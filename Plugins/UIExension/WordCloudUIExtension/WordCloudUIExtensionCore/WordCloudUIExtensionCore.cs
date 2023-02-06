
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;

using Gma.CodeCloud.Controls;
using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

using Abstractspoon.Tdl.PluginHelpers;
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	public class WordCloudUIExtensionCore : Panel, IUIExtension
	{
        private const int ComboHeight = 16;
		private const int ComboWidth = 200;
		private const int ComboSpacing = 6;
		private const int MatchListDefaultWidth = 200;
        private const string FontName = "Tahoma";

        private static int MinSplitWidth = DPIScaling.Scale(25 * 2);
		private static int LabelTop = DPIScaling.Scale(2); 
		private static int ComboTop = DPIScaling.Scale(20);
        private static int LabelHeight = (ComboTop - LabelTop);

        // -------------------------------------------------------------

		private IntPtr m_HwndParent;
        private Task.Attribute m_Attrib;
		private Translator m_Trans;
		private String m_TypeId, m_UiName;

		private bool m_Splitting;
		private Color m_SplitterColor;
		private int m_InitialSplitPos;
		private int m_SplitterWidth;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private TdlCloudControl m_WordCloud;
        private IBlacklist m_ExcludedWords;

        private StyleComboBox m_StylesCombo;
		private Label m_StylesLabel;
		private AttributeComboBox m_AttributeCombo;
        private Label m_AttributeLabel;
        private ColorSchemeComboBox m_ColorsCombo;
        private Label m_ColorsLabel;
		private TaskMatchesListView m_TaskMatchesList;

		private Font m_ControlsFont;
		private String m_UserIgnoreFilePath, m_LangIgnoreFilePath;
		private Timer m_CommentsTimer;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_Attrib = Task.Attribute.Title;
            m_ExcludedWords = new CommonWords(); // English by default

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_CommentsTimer = new Timer();
			m_CommentsTimer.Interval = 2000;
			m_CommentsTimer.Tick += new EventHandler(OnUpdateTimer);

			m_Splitting = false;
			m_InitialSplitPos = -1;
			m_SplitterColor = Color.Gray;
            m_SplitterWidth = DPIScaling.Scale(6);

			InitializeComponent();
		}


		// IUIExtension ------------------------------------------------------------------

		public override Boolean Focused
        {
            get { return (base.Focused || m_WordCloud.Focused || m_TaskMatchesList.Focused); }
        }

		public bool SelectTask(UInt32 taskId)
		{
			if (m_Attrib == Task.Attribute.Unknown)
				return false;

			if (!m_TaskMatchesList.HasMatchId(taskId))
			{
				// Get the Cloud items having this id
				CloudTaskItem item;

				if (!m_Items.TryGetValue(taskId, out item))
					return false; // ??

				// Get the item's words
				var words = item.GetWords(m_Attrib, m_ExcludedWords, false);

				if (!words.Any())
					return false;

				// See if the currently selected word is in the item's words
				String selWord = m_WordCloud.SelectedWord;

				if ((selWord == null) ||
					!words.Exists(x => x.Equals(selWord, StringComparison.CurrentCultureIgnoreCase)))
				{
					// If not, find the item's word with the most occurrence
					var matches = m_WordCloud.WeightedWords.Where(a => words.Any(x => x.Equals(a.Text, StringComparison.CurrentCultureIgnoreCase))).SortByOccurrences();

					if (!matches.Any())
						return false; // ??

					// This should update the match list
					m_WordCloud.SelectedWord = matches.First().Text;
				}

				RebuldMatchList();
			}

			m_TaskMatchesList.SetSelectedMatchId(taskId);
			return true;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			m_CommentsTimer.Stop();
			m_CommentsTimer.Tag = null;

			HashSet<UInt32> changedTaskIds = null;

			switch (type)
			{
				case UIExtension.UpdateType.Delete:
				case UIExtension.UpdateType.All:
					// Rebuild
                    m_Items.Clear();
					break;
					
				case UIExtension.UpdateType.New:
				case UIExtension.UpdateType.Edit:
					// In-place update
					changedTaskIds = new HashSet<UInt32>();
					break;
			}
            
			Task task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, changedTaskIds))
				task = task.GetNextTask();

			// Comments is the only attribute that gets delivered as a 
			// 'stream of consciousness' as each character gets typed
			// so to avoid excessive processing we use a timer to delay
			// the update until there is a break in the typing
			if (tasks.IsAttributeAvailable(m_Attrib))
			{
				if (changedTaskIds == null)
				{
					UpdateDisplay(null); // immediate
				}
				else if (changedTaskIds.Count > 0)
				{
					if (m_Attrib != Task.Attribute.Comments)
					{
						UpdateDisplay(null); // immediate
					}
					else // comments
					{
						// Delayed update
						m_CommentsTimer.Tag = changedTaskIds;
						m_CommentsTimer.Start();
					}
				}
			}
		}

		private void OnUpdateTimer(object sender, EventArgs e)
		{
			m_CommentsTimer.Stop();

			// Sanity check because tracked attrib could feasibly have changed
			if (m_Attrib == Task.Attribute.Comments)
				UpdateDisplay(m_CommentsTimer.Tag as HashSet<UInt32>);
		}

		private void UpdateDisplay(HashSet<UInt32> changedTaskIds)
		{
			UpdateWeightedWords(true);
			UpdateMatchList(changedTaskIds);
		}

		void UpdateMatchList(HashSet<UInt32> changedTaskIds = null)
		{
			if (m_WordCloud.WeightedWords.Count() == 0)
			{
				m_TaskMatchesList.ClearMatches();
			}
			else if (changedTaskIds != null)
			{
				m_TaskMatchesList.UpdateMatchItems(changedTaskIds);

				// If the selected match also changed then we may
				// may need to change the currently selected word
				if (changedTaskIds.Contains(m_TaskMatchesList.GetSelectedMatchId()))
					FixupWordCloudSelection();
			}
			else
			{
				// Pick a word that ensures the currently selected
				// item in the match list will be preserved
				var selMatch = m_TaskMatchesList.GetSelectedMatch();
				var selWord = FixupWordCloudSelection(selMatch);
				
				RebuldMatchList();

				if (selMatch != null)
					m_TaskMatchesList.SetSelectedMatchId(selMatch.Id);
			}

            m_TaskMatchesList.EnsureSelectionVisible();
		}

		private bool ProcessTaskUpdate(Task task, 
									   UIExtension.UpdateType type,
									   HashSet<UInt32> taskIds)
		{
			if (!task.IsValid())
				return false;

			// Ignore reference tasks
			if (task.GetReferenceID() != 0)
				return true;

			CloudTaskItem item;

			if (m_Items.TryGetValue(task.GetID(), out item))
			{
				item.ProcessTaskUpdate(task, type, false);
			}
			else
			{
				item = new CloudTaskItem(task.GetID());
				item.ProcessTaskUpdate(task, type, true);

				m_Items[item.Id] = item;
			}


			if (taskIds != null)
				taskIds.Add(item.Id);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, taskIds))
				subtask = subtask.GetNextTask();

			return true;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskId)
		{
			taskId = 0;

			switch (getTask)
			{
				case UIExtension.GetTask.GetNextTask:
				case UIExtension.GetTask.GetNextVisibleTask:
					taskId = m_TaskMatchesList.GetNextSelectedMatchId();
					break;

				case UIExtension.GetTask.GetPrevTask:
				case UIExtension.GetTask.GetPrevVisibleTask:
					taskId = m_TaskMatchesList.GetPrevSelectedMatchId();
					break;
			}

			return (taskId != 0);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            if (text == String.Empty)
                return false;

            if (!findReplace)
            {
                var words = CloudTaskItem.ToWords(text);

                if (!words.Any())
                    return false;

                if (!m_WordCloud.SelectedWordMatches(words, caseSensitive, wholeWord))
                {
                    var matches = m_WordCloud.Match(words, false);

                    if (matches.Any())
                    {
                        m_WordCloud.SelectedWord = matches.First().Text;
                        return true;
                    }

                    return false;
                }
            }

			// Then on the match list
			if (m_TaskMatchesList.SelectMatch(text, selectTask, caseSensitive, wholeWord, findReplace))
			{
				return true;
			}
	
			// all else
			return false;
		}

		protected void UpdateWeightedWords(bool force)
		{
			var words = new List<string>();

			foreach (var item in m_Items)
			{
				var taskWords = item.Value.GetWords(m_Attrib, m_ExcludedWords, force);
				words.AddRange(taskWords);
			}

            m_WordCloud.WeightedWords = words.CountOccurences();
		}

        public Bitmap SaveToImage()
        {
            return m_WordCloud.SaveToImage();
        }

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			switch (attrib)
			{
				case Task.Attribute.Title:
				case Task.Attribute.Icon:
				case Task.Attribute.Color:
					return true;
			}

			// all else
			return AttributeComboBox.IsSupportedAttribute(attrib);
		}
	   
		public bool WantSortUpdate(Task.Attribute attrib)
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
	   
		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			Rectangle editRect = m_TaskMatchesList.GetSelectedMatchEditRect();

			if (!editRect.IsEmpty)
			{
				// Convert to screen coords
				editRect = m_TaskMatchesList.RectangleToScreen(editRect);

				left = editRect.Left;
				top = editRect.Top;
				right = editRect.Right;
				bottom = editRect.Bottom;

				return true;
			}

			// else
			return false;
		}

		public UIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
		{
			Point ptClient = m_TaskMatchesList.PointToClient(new Point(xPos, yPos));
			ListViewHitTestInfo lvHit = m_TaskMatchesList.HitTest(ptClient);

			if (lvHit.Item != null)
				return UIExtension.HitResult.Task;

            // else
			return UIExtension.HitResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xPos, Int32 yPos)
		{
			Point ptClient = m_TaskMatchesList.PointToClient(new Point(xPos, yPos));
			ListViewHitTestInfo lvHit = m_TaskMatchesList.HitTest(ptClient);

			if (lvHit.Item != null)
			{
				var task = (lvHit.Item.Tag as CloudTaskItem);

				if (task != null)
					return task.Id;
			}

            // else
			return 0;
		}

		public void SetUITheme(UITheme theme)
		{
            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_SplitterColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

			Color labelColor = theme.GetAppDrawingColor(UITheme.AppColor.AppText);

			m_AttributeLabel.ForeColor = labelColor;
			m_ColorsLabel.ForeColor = labelColor;
			m_StylesLabel.ForeColor = labelColor;
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
			m_WordCloud.SetFont(faceName, pointSize);

			m_TaskMatchesList.Font = new Font(faceName, pointSize);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileInt(key, "AttribToTrack", (int)m_Attrib);
			prefs.WriteProfileString(key, "ColorScheme", m_ColorsCombo.GetSelectedSchemeAsString());
			prefs.WriteProfileInt(key, "SplitterPosFromRight", (ClientRectangle.Right - SplitterCentre().X));
			prefs.WriteProfileInt(key, "LayoutStyle", (int)m_StylesCombo.GetSelectedStyle());
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            { 
                // private settings
				var attrib = (Task.Attribute)prefs.GetProfileInt(key, "AttribToTrack", (int)Task.Attribute.Title);

				if (!AttributeComboBox.IsSupportedAttribute(attrib))
					attrib = Task.Attribute.Title;

				m_AttributeCombo.SetSelectedAttribute(attrib);

				string scheme = prefs.GetProfileString(key, "ColorScheme", "");

				if (!m_ColorsCombo.SetSelectedScheme(scheme))
					m_ColorsCombo.SelectedIndex = 0;

				m_InitialSplitPos = prefs.GetProfileInt(key, "SplitterPosFromRight", MatchListDefaultWidth);
                m_InitialSplitPos = Math.Max(m_InitialSplitPos, MinSplitWidth);

				var style = (LayoutType)prefs.GetProfileInt(key, "LayoutStyle", (int)LayoutType.Spiral);

				if (!m_StylesCombo.SetSelectedStyle(style))
					m_StylesCombo.SelectedIndex = 0;
            }

            // App settings
			ShowSplitterBar(!prefs.GetProfileBool("Preferences", "HidePaneSplitBar", false));

            string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string language = prefs.GetProfileString("Preferences", "LanguageFile", "");

            m_UserIgnoreFilePath = Path.Combine(appPath, "WordCloud.Ignore.txt");

            try
            {
                var file = File.OpenWrite(m_UserIgnoreFilePath);
                file.Close();
            }
            catch (UnauthorizedAccessException)
            {
                var folderPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
                m_UserIgnoreFilePath = Path.Combine(folderPath, "Abstractspoon\\ToDoList\\WordCloud.Ignore.txt");

                try
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(m_UserIgnoreFilePath));

                    var file = File.OpenWrite(m_UserIgnoreFilePath);
                    file.Close();
                }
                catch (UnauthorizedAccessException)
                {
                    m_UserIgnoreFilePath = null;
                }
            }

            m_LangIgnoreFilePath = Path.Combine(appPath, "Resources\\Translations", language);
            m_LangIgnoreFilePath = Path.ChangeExtension(m_LangIgnoreFilePath, "WordCloud.Ignore.txt");

            m_TaskMatchesList.TaskColorIsBackground = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
            m_TaskMatchesList.ShowParentsAsFolders = prefs.GetProfileBool("Preferences", "ShowParentsAsFolders", false);
            m_TaskMatchesList.ShowCompletionCheckboxes = prefs.GetProfileBool("Preferences", "AllowCheckboxAgainstTreeItem", false);
			m_TaskMatchesList.ShowLabelTips = !prefs.GetProfileBool("Preferences", "ShowInfoTips", false);

            UpdateBlacklist();
        }

		void ShowSplitterBar(bool show = true)
		{
            int newSplitterWidth = (show ? DPIScaling.Scale(6) : 1);

			if (newSplitterWidth != m_SplitterWidth)
			{
				m_SplitterWidth = newSplitterWidth;
				OnSizeChanged(new EventArgs());
			}
		}
		
		// PRIVATE ------------------------------------------------------------------------------

		private void InitializeComponent()
		{
			this.BackColor = Color.White;
			m_Items = new Dictionary<UInt32, CloudTaskItem>();

			CreateWordCloud();
			CreateTaskMatchesListView();
            CreateAttributeCombo();
			CreateColorSchemeCombo();
			CreateStyleCombo();

			Invalidate(true);
		}

		private void UpdateBlacklist()
		{
			CommonBlacklist blacklist = new CommonBlacklist();

            try
            {
                int numWords = blacklist.Append(m_UserIgnoreFilePath);
                numWords += blacklist.Append(m_LangIgnoreFilePath);

                m_ExcludedWords = blacklist;
                UpdateWeightedWords(true);
            }
            catch (Exception)
            {
            }
		}

		private void CreateWordCloud()
		{
			m_WordCloud = new TdlCloudControl(this.Handle, m_Trans);

			m_WordCloud.Location = new Point(0, ControlTop);
			m_WordCloud.Size = new Size(100, 100);
			m_WordCloud.Cursor = Cursors.Default;
			m_WordCloud.LayoutType = Gma.CodeCloud.Controls.LayoutType.Spiral;

			this.Controls.Add(m_WordCloud);

			m_WordCloud.SelectionChange += new SelectionChangeEventHandler(OnWordSelectionChanged);
			m_WordCloud.MouseClick += new MouseEventHandler(OnWordCloudMouseClick);

            m_WordCloud.SetFont(FontName, 10); // default
		}

		private void CreateTaskMatchesListView()
		{
			m_TaskMatchesList = new TaskMatchesListView(m_HwndParent);
			m_TaskMatchesList.Initialise(m_Trans);

			m_TaskMatchesList.Font = m_ControlsFont;
			m_TaskMatchesList.Location = new Point(0, ComboTop);
			m_TaskMatchesList.Size = new Size(MatchListDefaultWidth, 100);
			m_TaskMatchesList.Cursor = Cursors.Default;
			m_TaskMatchesList.BorderStyle = BorderStyle.None;

			this.Controls.Add(m_TaskMatchesList);

			// Add message handlers
			m_TaskMatchesList.SelectedIndexChanged += new EventHandler(OnTaskMatchesSelChanged);
            m_TaskMatchesList.EditTaskDone += new EditTaskCompletionEventHandler(OnTaskMatchesEditTaskDone);
            m_TaskMatchesList.EditTaskIcon += new EditTaskIconEventHandler(OnTaskMatchesEditTaskIcon);
            m_TaskMatchesList.EditTaskLabel += new EditTaskLabelEventHandler(OnTaskMatchesEditTaskLabel);
		}

		private void CreateAttributeCombo()
		{
			// Label
			m_AttributeLabel = InitialiseLabel(new Label(), "&Attribute to 'track'", null);

			// Combo
			m_AttributeCombo = (AttributeComboBox)InitialiseCombo(new AttributeComboBox(), null);
			m_AttributeCombo.Initialise(m_Trans);

			// Add selection change handler
			m_AttributeCombo.SelectedIndexChanged += new EventHandler(OnAttributeSelChanged);
		}

		private void CreateColorSchemeCombo()
		{
			// Label
			m_ColorsLabel = InitialiseLabel(new Label(), "&Colour Scheme", m_AttributeLabel);

			// Combo
			m_ColorsCombo = (ColorSchemeComboBox)InitialiseCombo(new ColorSchemeComboBox(), m_AttributeCombo);

			// Add selection change handler
			m_ColorsCombo.SelectedIndexChanged += new EventHandler(OnColorSchemeSelChanged);
		}

		private void CreateStyleCombo()
		{
			// Label
			m_StylesLabel = InitialiseLabel(new Label(), "&Style", m_ColorsLabel);

			// Combo
			m_StylesCombo = (StyleComboBox)InitialiseCombo(new StyleComboBox(), m_ColorsCombo);

			m_StylesCombo.Initialise(m_Trans);
			m_StylesCombo.SetSelectedStyle(Gma.CodeCloud.Controls.LayoutType.Spiral);

			// Add selection change handler
			m_StylesCombo.SelectedIndexChanged += new EventHandler(OnStyleComboSelChanged);
		}

		protected Label InitialiseLabel(Label label, String labelText, Label prevLabel)
		{
			if (prevLabel == null)
				label.Location = new Point(-2, LabelTop);
			else
				label.Location = new Point(prevLabel.Right + ComboSpacing, LabelTop);

			label.Size = new Size(ComboWidth, LabelHeight);
			label.Text = m_Trans.Translate(labelText);
			label.TextAlign = ContentAlignment.MiddleLeft;
			label.Font = m_ControlsFont;

			this.Controls.Add(label);
			return label;
		}

		protected ComboBox InitialiseCombo(ComboBox combo, ComboBox prevCombo)
		{
			if (prevCombo == null)
				combo.Location = new Point(0, ComboTop);
			else
				combo.Location = new Point(prevCombo.Right + ComboSpacing, ComboTop);
			
			combo.Size = new Size(ComboWidth, ComboHeight);
			combo.DropDownStyle = ComboBoxStyle.DropDownList;
			combo.DropDownHeight = 400;
			combo.Font = m_ControlsFont;

			this.Controls.Add(combo);
			return combo;
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			m_TaskMatchesList.Focus();
		}

		protected override void OnPaintBackground(PaintEventArgs e)
		{
			// Exclude main controls first
			e.Graphics.ExcludeClip(RectangleToClient(m_TaskMatchesList.RectangleToScreen(m_TaskMatchesList.ClientRectangle)));
			e.Graphics.ExcludeClip(RectangleToClient(m_WordCloud.RectangleToScreen(m_WordCloud.ClientRectangle)));

			base.OnPaintBackground(e);
		}

		protected override void OnPaint(PaintEventArgs e)
        {
			// border around Word cloud and Task match list
            Rectangle border = new Rectangle(ClientRectangle.Left, ControlTop, ClientRectangle.Width, (ClientRectangle.Height - ControlTop));

            ControlPaint.DrawBorder(e.Graphics, border, Color.DarkGray, ButtonBorderStyle.Solid);

			// Splitter bar
			Rectangle splitterRect = SplitterRect();
			Color fillColor = m_SplitterColor;
			Color borderColor = DrawingColor.AdjustLighting(m_SplitterColor, -0.2f, true);

			if (m_SplitterWidth == 1)
				fillColor = borderColor;

			using (var brush = new SolidBrush(fillColor))
				e.Graphics.FillRectangle(brush, splitterRect);

			// draw drag marker (2 x 20)
			if (m_SplitterWidth > 2)
			{
				ControlPaint.DrawBorder(e.Graphics, splitterRect, borderColor, ButtonBorderStyle.Solid);

				Point splitCentre = SplitterCentre();
				Rectangle markerRect = new Rectangle((splitCentre.X - 1), (splitCentre.Y - 10), 2, 20);

				// use the splitter colour luminance to decide whether
				// to draw the marker lighter or darker
				Color markerColor;

				if (m_SplitterColor.GetBrightness() > 0.5f)
					markerColor = DrawingColor.AdjustLighting(m_SplitterColor, -0.3f, true);
				else
					markerColor = DrawingColor.AdjustLighting(m_SplitterColor, 0.3f, true);

				using (var brush = new SolidBrush(markerColor))
					e.Graphics.FillRectangle(brush, markerRect);
			}
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

			if (ClientRectangle.IsEmpty)
				return;

			Rectangle baseRect = ClientRectangle;

			// Adjust for border
			baseRect.Inflate(-1, -1);
			baseRect.Y = (ControlTop + 1);
			baseRect.Height -= (ControlTop + 1);

			Rectangle wordCloudRect = baseRect, matchListRect = baseRect;
			Rectangle splitterRect = SplitterRect();

			// Always make sure the splitter is visible
			splitterRect.X = Math.Max(splitterRect.X, MinSplitWidth);
			splitterRect.X = Math.Min(splitterRect.X, ClientRectangle.Right - MinSplitWidth);
			
			matchListRect.X = splitterRect.Right;
			matchListRect.Width = (baseRect.Right - splitterRect.Right);

			m_TaskMatchesList.Bounds = matchListRect;

			wordCloudRect.Width = (splitterRect.Left - baseRect.Left);

			m_WordCloud.Location = wordCloudRect.Location;
			m_WordCloud.Size = wordCloudRect.Size;

			Invalidate(true);
		}

        private void OnAttributeSelChanged(object sender, EventArgs args)
        {
            m_Attrib = m_AttributeCombo.GetSelectedAttribute();

            UpdateWeightedWords(false);
			UpdateMatchList();
        }

		private void OnColorSchemeSelChanged(object sender, EventArgs args)
		{
			m_WordCloud.Palette = m_ColorsCombo.GetSelectedScheme();
		}

		private void OnStyleComboSelChanged(object sender, EventArgs args)
		{
			m_WordCloud.LayoutType = m_StylesCombo.GetSelectedStyle();

			UpdateWeightedWords(false);
        }

		private void RebuldMatchList()
		{
            string selWord = m_WordCloud.SelectedWord;

			if (selWord != null)
			{
				CloudTaskItem selItem = null;
				UInt32 selItemId = m_TaskMatchesList.GetSelectedMatchId();

				// Build a list of task items containing this value
				m_TaskMatchesList.BeginUpdate();
				m_TaskMatchesList.ClearMatches();

				foreach (var item in m_Items)
				{
					if (item.Value.AttributeHasValue(m_Attrib, selWord, m_ExcludedWords))
					{
						m_TaskMatchesList.AddMatch(item.Value);

						if (item.Value.Id == selItemId)
							selItem = item.Value;
					}
				}
				m_TaskMatchesList.EndUpdate();

				string headerText = m_Trans.Translate("Task Matches");

				if (m_TaskMatchesList.Items.Count > 0)
				{
					// See if our currently selected task is in the new match list
					selItemId = m_TaskMatchesList.SetSelectedMatchId(selItemId);

					if ((selItem == null) || (selItemId != selItem.Id))
						NotifyParentSelChange(selItemId);

					m_TaskMatchesList.Columns[0].Text = String.Format("{0} ({1})", headerText, m_TaskMatchesList.Items.Count);
				}
				else
				{
					m_TaskMatchesList.Columns[0].Text = headerText;
				}
			}
		}

		private void OnWordCloudMouseClick(object sender, MouseEventArgs e)
		{
			if (!String.IsNullOrEmpty(m_UserIgnoreFilePath) && (e.Button == MouseButtons.Right) && m_WordCloud.HasItemUnderMouse())
			{
				var menu = new ContextMenuStrip();

				string format = m_Trans.Translate("&Ignore '{0}'");
				string menuText = string.Format(format, m_WordCloud.SelectedWord);
				var item = menu.Items.Add(menuText);

				item.Click += OnWordCloudIgnoreWord;
				item.Tag = m_WordCloud.SelectedWord;
				
				menu.Show(m_WordCloud, e.Location);
			}
		}

		private void OnWordCloudIgnoreWord(object sender, EventArgs e)
		{
			var item = sender as ToolStripItem;

			if (item != null)
			{
				// Look for user-defined 'Ignore' file
                try
                {
                    File.AppendAllText(m_UserIgnoreFilePath, item.Tag.ToString() + Environment.NewLine);
				    UpdateBlacklist();
                }
                catch (Exception /*exception*/)
                {
                }
                finally
                {
                }
			}
		}

		private void OnWordSelectionChanged(object sender)
		{
			RebuldMatchList();
		}

		private String FixupWordCloudSelection(CloudTaskItem selMatch = null)
		{
            // Check if the currently selected word is still
            // present in the currently selected match
			String selWord = m_WordCloud.SelectedWord;

			if (selMatch == null)
				selMatch = m_TaskMatchesList.GetSelectedMatch();

			if ((selMatch != null) && (selWord != null))
			{
				var words = selMatch.GetWords(m_Attrib, m_ExcludedWords, false);

                // If not, find the first match between the item's text
                // and the wordcloud contents
                if (words.Any() && !words.Contains(selWord, StringComparer.InvariantCultureIgnoreCase))
                {
                    var matches = m_WordCloud.Match(words, false);

                    if (matches.Any())
                        selWord = matches.First().Text;
                }
			}

			if (selWord == null)
				selWord = m_WordCloud.WeightedWords.SortByOccurrences().First().Text;

			m_WordCloud.SelectedWord = selWord;

			return selWord;
		}

		private void OnTaskMatchesSelChanged(object sender, EventArgs args)
		{
			UInt32 selTaskId = m_TaskMatchesList.GetSelectedMatchId();

			if (selTaskId != 0)
				NotifyParentSelChange(selTaskId);
		}

        private Boolean OnTaskMatchesEditTaskDone(object sender, UInt32 taskId, bool completed)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyMod(Task.Attribute.DoneDate,
                                    (completed ? DateTime.Now : DateTime.MinValue));
        }

        private Boolean OnTaskMatchesEditTaskIcon(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditIcon();
        }

        private Boolean OnTaskMatchesEditTaskLabel(object sender, UInt32 taskId)
        {
            var notify = new UIExtension.ParentNotify(m_HwndParent);

            return notify.NotifyEditLabel();
        }

		private void NotifyParentSelChange(UInt32 taskId)
		{
			var parent = new UIExtension.ParentNotify(m_HwndParent);

			parent.NotifySelChange(taskId);
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			if (PtInSplitter(e.Location))
			{
				m_Splitting = true;
				Capture = true;

				return;
			}

			base.OnMouseDown(e);
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			if (m_Splitting)
			{
				m_Splitting = false;
				Capture = false;
			}

			base.OnMouseUp(e);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			if (m_Splitting)
			{
				UpdateSplitPos(e.X);
			}
			else if (PtInSplitter(e.Location))
			{
				Cursor = Cursors.VSplit;
			}
			else
			{
				Cursor = Cursors.Default;
			}

			base.OnMouseMove(e);
		}

		protected void UpdateSplitPos(int horzSplitCentre)
		{
			int newSplitLeft = (horzSplitCentre - (m_SplitterWidth / 2));

			newSplitLeft = Math.Max(1, newSplitLeft);
			newSplitLeft = Math.Min(ClientRectangle.Width - m_SplitterWidth - 1, newSplitLeft);

			if ((newSplitLeft - 1) != m_WordCloud.Width)
			{
                int newSplitRight = (newSplitLeft + m_SplitterWidth);
                int newTaskMatchesWidth = (m_TaskMatchesList.Right - newSplitRight);

                if (newTaskMatchesWidth < MinSplitWidth)
                {
                    newTaskMatchesWidth = MinSplitWidth;
                    newSplitRight = (m_TaskMatchesList.Right - newTaskMatchesWidth);
                    newSplitLeft = (newSplitRight - m_SplitterWidth);
                }

                if (m_TaskMatchesList.Width != newTaskMatchesWidth)
                {
                    m_WordCloud.Width = (newSplitLeft - 1); // account for border

                    m_TaskMatchesList.Width = newTaskMatchesWidth;
				    m_TaskMatchesList.Left = newSplitRight;

				    Invalidate(false);
				    Update();
                }
			}
		}

		protected Rectangle SplitterRect()
		{
			// Note: Splitter pos is measured from RHS
			Rectangle splitterRect = ClientRectangle;
			splitterRect.Inflate(-1, 0);

			// Initialise split pos first time only
			if ((ClientRectangle.Width > 0) && (m_InitialSplitPos > 0))
			{
				splitterRect.X = (ClientRectangle.Right - m_InitialSplitPos - (m_SplitterWidth / 2));
				splitterRect.Width = m_SplitterWidth;

				m_InitialSplitPos = -1;
			}
			else
			{
				splitterRect.X = (splitterRect.Right - m_TaskMatchesList.Width - m_SplitterWidth);
				splitterRect.Width = m_SplitterWidth;
			}

			splitterRect.Y = ControlTop;
			splitterRect.Height = (splitterRect.Height - ControlTop);

			return splitterRect;
		}

		protected Point SplitterCentre()
		{
			// Splitter pos measured from RHS
			Rectangle splitterRect = SplitterRect();
			Point centrePt = new Point(splitterRect.Left + (m_SplitterWidth / 2),
										splitterRect.Top + (splitterRect.Height / 2));

			return centrePt;
		}

		protected bool PtInSplitter(Point pt)
		{
			return SplitterRect().Contains(pt);
		}

        protected int ControlTop
        {
            get 
            { 
                if (m_AttributeCombo != null)
                    return m_AttributeCombo.Bounds.Bottom + DPIScaling.Scale(4); 

                // else
                return 0;
            }
        }
	}
}
