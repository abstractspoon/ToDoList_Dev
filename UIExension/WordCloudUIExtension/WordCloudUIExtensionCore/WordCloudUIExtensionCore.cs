
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
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
        public class CloudTaskItem
        {
			public CloudTaskItem(UInt32 id)
			{
				Id = id;
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

            public IEnumerable<string> GetWords(UIExtension.TaskAttribute attrib)
            {
				var values = GetAttributeValues(attrib);

				// Split title and comments only into individual words
				if ((attrib == UIExtension.TaskAttribute.Title) ||
					(attrib == UIExtension.TaskAttribute.Comments))
				{
					if (values.Count > 0)
					{
						string[] parts = values[0].Split(WordDelims, StringSplitOptions.RemoveEmptyEntries);

						values.Clear();

						foreach (var p in parts)
							values.Add(p.Trim(WordTrim));
					}
				}

				values.RemoveAll(p => (p.Length < 2));
				return values;
            }

            public List<string> GetAttributeValues(UIExtension.TaskAttribute attrib)
            {
                var values = new List<string>();

                switch (attrib)
                {
					case UIExtension.TaskAttribute.Title:		values.Add(Title);			break;
					case UIExtension.TaskAttribute.DoneDate:	values.Add(DoneDate);		break;
					case UIExtension.TaskAttribute.DueDate:		values.Add(DueDate);		break;
					case UIExtension.TaskAttribute.StartDate:	values.Add(StartDate);		break;
					case UIExtension.TaskAttribute.AllocBy:		values.Add(AllocBy);		break;
					case UIExtension.TaskAttribute.Status:		values.Add(Status);			break;
					case UIExtension.TaskAttribute.Comments:	values.Add(Comments);		break;
					case UIExtension.TaskAttribute.CreationDate: values.Add(CreationDate);	break;
					case UIExtension.TaskAttribute.CreatedBy:	values.Add(CreatedBy);		break;
					case UIExtension.TaskAttribute.Version:		values.Add(Version);		break;

					case UIExtension.TaskAttribute.AllocTo:		values = AllocTo;			break;
					case UIExtension.TaskAttribute.Category:	values = Category;			break;
					case UIExtension.TaskAttribute.Tag:			values = Tags;				break;
				}

                return values;
            }
        }

        public class TdlGraphicEngine : Gma.CodeCloud.Controls.GdiGraphicEngine
        {
            public TdlGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
                : base(graphics, fontFamily, fontStyle, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight)
            {
		    }

            public override void DrawEmphasized(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
            {
                Font font = GetFont(layoutItem.Word.Occurrences);
				Color color = GetPresudoRandomColorFromPalette(layoutItem);

                // Draw offset shadow
				Color shadowColor = ColorUtil.LighterDrawing(color, 0.7f);
                Point pointShadow = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
                int offset = (int)(2 * font.Size / MaxFontSize) + 1;
                pointShadow.Offset(offset, offset);
                System.Windows.Forms.TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, pointShadow, shadowColor);

				// Then text
                Color textColor = ColorUtil.DarkerDrawing(color, 0.2f);
                Point pointText = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
                System.Windows.Forms.TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, pointText, textColor);
            }
        }

        public class TdlCloudControl : Gma.CodeCloud.Controls.CloudControl
        {
            public TdlCloudControl()
            {
                DoubleBuffered = true;
                MaxFontSize = 30;
				Font = new System.Drawing.Font(FontName, 10);
            }

            protected override Gma.CodeCloud.Controls.Geometry.IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
            {
                return new TdlGraphicEngine(graphics, this.Font.FontFamily, FontStyle.Regular, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight);
            }
           
        }
        
        // -------------------------------------------------------------

        private const int LabelTop = 2;
		private const int ComboTop = 20;
        private const int ControlTop = 49;
        private const string FontName = "Tahoma";

		private static readonly char[] WordDelims = { ',', ' ', '\t', '\r', '\n' };
		private static readonly char[] WordTrim = { '\'', '\"', '{', '}', '(', ')', ':', ';', '.' };

        // -------------------------------------------------------------

		private IntPtr m_hwndParent;
        private UIExtension.TaskAttribute m_Attrib;
		private Translator m_trans;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private TdlCloudControl m_WordCloud;

        private AttributeComboBox m_AttributeCombo;
        private System.Windows.Forms.Label m_AttributeLabel;
        private ColorSchemeComboBox m_ColorsCombo;
        private System.Windows.Forms.Label m_ColorsLabel;

		private System.Drawing.Font m_ControlsFont;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent, Translator trans)
		{
			m_hwndParent = hwndParent;
			m_trans = trans;
			m_Attrib = UIExtension.TaskAttribute.Title;

			m_ControlsFont = new System.Drawing.Font(FontName, 8);

			InitializeComponent();
		}

		// IUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 dwTaskID)
		{
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

			IBlacklist blacklist = CreateBlacklist(true);
			this.m_WordCloud.WeightedWords = words.Filter(blacklist).CountOccurences().SortByOccurences();
		}

		public static IBlacklist CreateBlacklist(bool excludeEnglishCommonWords)
		{
			return (excludeEnglishCommonWords ? (IBlacklist)new CommonWords() : new NullBlacklist());
		}

		public bool WantEditUpdate(UIExtension.TaskAttribute attrib)
		{
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
	   
		public bool DoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			return false;
		}

		public bool CanDoAppCommand(UIExtension.AppCommand cmd, UInt32 dwExtra)
		{
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
			if (!appOnly)
			{
				var attrib = (UIExtension.TaskAttribute)prefs.GetProfileInt(key, "AttribToTrack", (int)UIExtension.TaskAttribute.Title);

				if (!AttributeComboBox.IsSupportedAttribute(attrib))
					attrib = UIExtension.TaskAttribute.Title;

				m_AttributeCombo.SetSelectedAttribute(attrib);

				var scheme = prefs.GetProfileString(key, "ColorScheme", "");

				if (scheme.Length > 0)
					m_ColorsCombo.SetSelectedScheme(scheme);
				else
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

			Invalidate(true);
		}


		private void CreateWordCloud()
		{
			this.m_WordCloud = new TdlCloudControl();

			this.m_WordCloud.Location = new System.Drawing.Point(0, ControlTop);
			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);

			this.Controls.Add(m_WordCloud);
		}

		private void CreateAttributeCombo()
		{
			// Label
			this.m_AttributeLabel = new System.Windows.Forms.Label();

			this.m_AttributeLabel.Font = new System.Drawing.Font(FontName, 8);
			this.m_AttributeLabel.Location = new System.Drawing.Point(-2, LabelTop);
			this.m_AttributeLabel.Size = new System.Drawing.Size(200, 16);
			this.m_AttributeLabel.Text = m_trans.Translate("&Attribute to 'track'");
			this.m_AttributeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_AttributeLabel);

			// Combo
			this.m_AttributeCombo = new AttributeComboBox();

			this.m_AttributeCombo.Font = new System.Drawing.Font(FontName, 8);
			this.m_AttributeCombo.Location = new System.Drawing.Point(0, ComboTop);
			this.m_AttributeCombo.Size = new System.Drawing.Size(200, 16);
			this.m_AttributeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.m_AttributeCombo.Initialise(m_trans);

			this.Controls.Add(m_AttributeCombo);

			// Add selection change handler
			this.m_AttributeCombo.SelectedIndexChanged += new EventHandler(OnAttributeSelChanged);
		}

		private void CreateColorSchemeCombo()
		{
			// Label
			this.m_ColorsLabel = new System.Windows.Forms.Label();

			this.m_ColorsLabel.Font = new System.Drawing.Font(FontName, 8);
			this.m_ColorsLabel.Location = new System.Drawing.Point(218, LabelTop);
			this.m_ColorsLabel.Size = new System.Drawing.Size(200, 16);
			this.m_ColorsLabel.Text = m_trans.Translate("&Colour Scheme");
			this.m_ColorsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

			this.Controls.Add(m_ColorsLabel);

			// Combo
			this.m_ColorsCombo = new ColorSchemeComboBox();

			this.m_ColorsCombo.Font = new System.Drawing.Font(FontName, 8);
			this.m_ColorsCombo.Location = new System.Drawing.Point(220, ComboTop);
			this.m_ColorsCombo.Size = new System.Drawing.Size(200, 16);
			this.m_ColorsCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

			this.Controls.Add(m_ColorsCombo);

			// Add selection change handler
			this.m_ColorsCombo.SelectedIndexChanged += new EventHandler(OnColorSchemeSelChanged);
		}

		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            System.Drawing.Rectangle Border = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
            Border.Y = ControlTop;
            Border.Height -= ControlTop;

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, Border, System.Drawing.Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            System.Drawing.Rectangle WordCloud = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);

            WordCloud.Y = ControlTop;
            WordCloud.Height -= ControlTop;
            WordCloud.Inflate(-1, -1);

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
    }
}
