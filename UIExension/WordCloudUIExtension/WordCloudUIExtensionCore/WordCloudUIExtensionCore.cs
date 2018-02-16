
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
using Abstractspoon.Tdl.PluginHelpers.ColorUtil;

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]

	public class WordCloudUIExtensionCore : Panel, IUIExtension
	{
        private const int LabelTop = 2;
		private const int ComboTop = 20;
        private const int LabelHeight = (ComboTop - LabelTop);
        private const int ComboHeight = 16;
        private const int ControlTop = 49;
		private const int ComboWidth = 200;
		private const int ComboSpacing = 6;
		private const int MatchListDefaultWidth = 200;
        private const string FontName = "Tahoma";

        // -------------------------------------------------------------

		private IntPtr m_HwndParent;
        private UIExtension.TaskAttribute m_Attrib;
		private Translator m_Trans;

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

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent, Translator trans)
		{
			m_HwndParent = hwndParent;
			m_Trans = trans;
			m_Attrib = UIExtension.TaskAttribute.Title;
            m_ExcludedWords = new CommonWords(); // English by default

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);

			m_Splitting = false;
			m_InitialSplitPos = -1;
			m_SplitterColor = Color.Gray;
			m_SplitterWidth = 6;

			InitializeComponent();
		}

		// IUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 taskId)
		{
			if (m_Attrib == UIExtension.TaskAttribute.Unknown)
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
					// If not,  find the item's word with the most occurrence
					var matches = m_WordCloud.WeightedWords.Where(a => words.Any(x => x.Equals(a.Text, StringComparison.CurrentCultureIgnoreCase))).SortByOccurences();

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

		public void UpdateTasks(TaskList tasks, 
								UIExtension.UpdateType type, 
								System.Collections.Generic.HashSet<UIExtension.TaskAttribute> attribs)
		{
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

			while (task.IsValid() && ProcessTaskUpdate(task, type, attribs, changedTaskIds))
				task = task.GetNextTask();

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
				m_TaskMatchesList.UpdateMatchItemsText(changedTaskIds);

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
                                       HashSet<UIExtension.TaskAttribute> attribs,
									   HashSet<UInt32> taskIds)
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

			if (taskIds != null)
				taskIds.Add(item.Id);

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, attribs, taskIds))
				subtask = subtask.GetNextTask();

			return true;
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskId)
		{
			taskId = 0;

			switch (getTask)
			{
				case UIExtension.GetTask.GetNextTask:
					taskId = m_TaskMatchesList.GetNextSelectedMatchId();
					break;

				case UIExtension.GetTask.GetPrevTask:
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

		public bool WantTaskUpdate(UIExtension.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case UIExtension.TaskAttribute.Title:
				case UIExtension.TaskAttribute.Icon:
				case UIExtension.TaskAttribute.Color:
					return true;
			}

			// all else
			return AttributeComboBox.IsSupportedAttribute(attrib);
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

		public void SetUITheme(UITheme theme)
		{
            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);
			m_SplitterColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);

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
			prefs.WriteProfileInt(key, "SplitterPosFromRight", (ClientRectangle.Right - SplitterCentre().X));
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            { 
                // private settings
				var attrib = (UIExtension.TaskAttribute)prefs.GetProfileInt(key, "AttribToTrack", (int)UIExtension.TaskAttribute.Title);

				if (!AttributeComboBox.IsSupportedAttribute(attrib))
					attrib = UIExtension.TaskAttribute.Title;

				m_AttributeCombo.SetSelectedAttribute(attrib);

				string scheme = prefs.GetProfileString(key, "ColorScheme", "");

				if (!m_ColorsCombo.SetSelectedScheme(scheme))
					m_ColorsCombo.SelectedIndex = 0;

				m_InitialSplitPos = prefs.GetProfileInt(key, "SplitterPosFromRight", MatchListDefaultWidth);
            }

			ShowSplitterBar(prefs.GetProfileInt("Preferences", "HidePaneSplitBar", 0) == 0);

            string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string language = prefs.GetProfileString("Preferences", "LanguageFile", "");

            m_UserIgnoreFilePath = Path.Combine(appPath, "WordCloud.Ignore.txt");

            m_LangIgnoreFilePath = Path.Combine(appPath, "Resources\\Translations", language);
            m_LangIgnoreFilePath = Path.ChangeExtension(m_LangIgnoreFilePath, "WordCloud.Ignore.txt");

            if (prefs.GetProfileInt("Preferences", "SpecifyTreeFont", 0) != 0)
            {
                m_WordCloud.SetFont(prefs.GetProfileString("Preferences", "TreeFont", FontName),
                                    prefs.GetProfileInt("Preferences", "FontSize", 10));
            }
            else
            {
                m_WordCloud.SetFont(FontName, 10);
            }

            UpdateBlacklist();
        }

		void ShowSplitterBar(bool show = true)
		{
			int newSplitterWidth = (show ? 6 : 1);

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

			int numWords = blacklist.Append(m_UserIgnoreFilePath);
			numWords += blacklist.Append(m_LangIgnoreFilePath);

			m_ExcludedWords = blacklist;
			UpdateWeightedWords(true);
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

		protected override void OnPaint(PaintEventArgs e)
        {
			// border around Word cloud and Task match list
            Rectangle border = new Rectangle(Location.X, ControlTop, Size.Width, (Size.Height - ControlTop));

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

			Rectangle baseRect = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);

			// Adjust for border
			baseRect.Inflate(-1, -1);
			baseRect.Y = (ControlTop + 1);
			baseRect.Height -= (ControlTop + 1);

			Rectangle wordCloudRect = new Rectangle(baseRect.Location, baseRect.Size);
			Rectangle taskMatchesRect = new Rectangle(baseRect.Location, baseRect.Size);
			Rectangle splitterRect = SplitterRect();

			taskMatchesRect.X = splitterRect.Right;
			taskMatchesRect.Width = (baseRect.Right - splitterRect.Right);

			m_TaskMatchesList.Location = taskMatchesRect.Location;
			m_TaskMatchesList.Size = taskMatchesRect.Size;

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
			if ((e.Button == MouseButtons.Right) && m_WordCloud.HasItemUnderMouse())
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
				string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
				string ignoreFile = Path.Combine(appPath, "WordCloud.Ignore.txt");

				File.AppendAllText(ignoreFile, item.Tag.ToString() + Environment.NewLine);
				UpdateBlacklist();
			}
		}

		private void OnWordSelectionChanged(object sender)
		{
			RebuldMatchList();
		}

		private String FixupWordCloudSelection(CloudTaskItem selMatch = null)
		{
			String selWord = m_WordCloud.SelectedWord;

			if (selMatch == null)
				selMatch = m_TaskMatchesList.GetSelectedMatch();

			if (selMatch != null)
			{
				var words = selMatch.GetWords(m_Attrib, m_ExcludedWords, false);

				if (words.Any())
				{
					var matches = m_WordCloud.Match(words, false);

					if (matches.Any())
						selWord = matches.First().Text;
				}
			}

			if (selWord == null)
				selWord = m_WordCloud.WeightedWords.SortByOccurences().First().Text;

			m_WordCloud.SelectedWord = selWord;

			return selWord;
		}

		private void OnTaskMatchesSelChanged(object sender, EventArgs args)
		{
			UInt32 selTaskId = m_TaskMatchesList.GetSelectedMatchId();

			if (selTaskId != 0)
				NotifyParentSelChange(selTaskId);
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
				m_WordCloud.Width = (newSplitLeft - 1); // account for border

				int newSplitRight = (newSplitLeft + m_SplitterWidth);

				m_TaskMatchesList.Width = (m_TaskMatchesList.Right - newSplitRight);
				m_TaskMatchesList.Left = newSplitRight;

				Invalidate(false);
				Update();
			}
		}

		protected Rectangle SplitterRect()
		{
			// Splitter pos measured from RHS
			Rectangle splitterRect = new Rectangle(ClientRectangle.Location, ClientRectangle.Size);
			splitterRect.Inflate(-1, 0);

			// Initialise split pos first time only
			if (!ClientRectangle.IsEmpty && (m_InitialSplitPos > 0))
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

	}
}
