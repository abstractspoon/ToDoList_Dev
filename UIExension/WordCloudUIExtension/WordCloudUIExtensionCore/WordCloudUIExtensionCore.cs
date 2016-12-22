
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
	public class WordCloudUIExtensionCore : System.Windows.Forms.Panel, IUIExtension
	{
        // Helper classes
        public class CloudTaskItem
        {
            public string Title;
            public string DoneDate;
            public string DueDate;
            public string StartDate;
            public string Priority;
            public string Color;
            public string AllocTo;
            public string AllocBy;
            public string Status;
            public string Category;
            public string Percent;
            public string TimeEstimate;
            public string TimeSpent;
            public string FileReference;
            public string Comments;
            public string CreationDate;
            public string CreatedBy;
            public string Risk;
            public string ExternalId;
            public string Tags;
            public string Dependency;
            public string Recurrence;
            public string Version;

            public IEnumerable<string> GetWords(UIExtension.TaskAttribute attrib)
            {
                var words = new List<string>();
                var delims = new char[] { ',', ' ', '\t', '\r', '\n' };

                words.AddRange(GetAttribValue(attrib).Split(delims, StringSplitOptions.RemoveEmptyEntries));

                return words;
            }

            public string GetAttribValue(UIExtension.TaskAttribute attrib)
            {
                switch (attrib)
                {
				case UIExtension.TaskAttribute.Title: return Title;
				case UIExtension.TaskAttribute.DoneDate: return DoneDate;
    			case UIExtension.TaskAttribute.DueDate: return DueDate;
				case UIExtension.TaskAttribute.StartDate: return StartDate;
				case UIExtension.TaskAttribute.AllocTo: return AllocTo;
				case UIExtension.TaskAttribute.AllocBy: return AllocBy;
				case UIExtension.TaskAttribute.Status: return Status;
				case UIExtension.TaskAttribute.Category: return Category;
				case UIExtension.TaskAttribute.Comments: return Comments;
				case UIExtension.TaskAttribute.CreationDate: return CreationDate;
				case UIExtension.TaskAttribute.CreatedBy: return CreatedBy;
				case UIExtension.TaskAttribute.Version: return Version;
				case UIExtension.TaskAttribute.Tag: return Tags;
                }

                // all else
                return "";
            }
        }

        public class AttributeItem
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

        public class TdlGraphicEngine : Gma.CodeCloud.Controls.GdiGraphicEngine
        {
            public TdlGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
                : base(graphics, fontFamily, fontStyle, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight)
            {
		    }

            public override void DrawEmphasized(Gma.CodeCloud.Controls.Geometry.LayoutItem layoutItem)
            {
                //base.DrawEmphasized(layoutItem);

                Font font = GetFont(layoutItem.Word.Occurrences);
                Color color = ColorUtil.LighterDrawing(GetPresudoRandomColorFromPalette(layoutItem), 0.2f);

                // Draw offset shadow
                Point pointShadow = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
                int offset = (int)(2 * font.Size / MaxFontSize) + 1;
                pointShadow.Offset(offset, offset);
                System.Windows.Forms.TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, pointShadow, Color.LightGray);

                Point pointText = new Point((int)layoutItem.Rectangle.X, (int)layoutItem.Rectangle.Y);
                System.Windows.Forms.TextRenderer.DrawText(m_Graphics, layoutItem.Word.Text, font, pointText, color);
            }
        }

        public class TdlCloudControl : Gma.CodeCloud.Controls.CloudControl
        {
            public TdlCloudControl()
            {
                DoubleBuffered = true;
            }

            protected override Gma.CodeCloud.Controls.Geometry.IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
            {
                return new TdlGraphicEngine(graphics, this.Font.FontFamily, FontStyle.Regular, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight);
            }
           
        }
        
        // -------------------------------------------------------------

        private const int ComboTop = 20;
        private const int ControlTop = 50;
        private const string FontName = "Tahoma";

        // -------------------------------------------------------------

		private Boolean m_taskColorIsBkgnd = false;
		private IntPtr m_hwndParent;
        private UIExtension.TaskAttribute m_Attrib;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private TdlCloudControl m_WordCloud;

        private System.Windows.Forms.ComboBox m_AttributeCombo;
        private System.Windows.Forms.Label m_AttributeLabel;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent)
		{
			m_hwndParent = hwndParent;
			InitializeComponent();
		}

		private void CreateWordCloud()
		{
			this.m_WordCloud = new TdlCloudControl();

            this.m_WordCloud.Font = new System.Drawing.Font(FontName, 8);
            this.m_WordCloud.Location = new System.Drawing.Point(0, ControlTop);
 			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);

			this.Controls.Add(m_WordCloud);
		}

        private void CreateAttributeCombo()
        {
            // Label
            this.m_AttributeLabel = new System.Windows.Forms.Label();

            this.m_AttributeLabel.Font = new System.Drawing.Font(FontName, 8);
            this.m_AttributeLabel.Location = new System.Drawing.Point(-2, 0);
            this.m_AttributeLabel.Size = new System.Drawing.Size(80, 16);
            this.m_AttributeLabel.Text = "Attribute";
            this.m_AttributeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

            this.Controls.Add(m_AttributeLabel);
            
            // Combo
            this.m_AttributeCombo = new System.Windows.Forms.ComboBox();

            this.m_AttributeCombo.Font = new System.Drawing.Font(FontName, 8);
            this.m_AttributeCombo.Location = new System.Drawing.Point(0, ComboTop);
            this.m_AttributeCombo.Size = new System.Drawing.Size(200, 16);
            this.m_AttributeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

            this.Controls.Add(m_AttributeCombo);

            // Add attributes to combo
            this.m_AttributeCombo.Items.Add(new AttributeItem("Title", UIExtension.TaskAttribute.Title));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Comments", UIExtension.TaskAttribute.Comments));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Status", UIExtension.TaskAttribute.Status));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Category", UIExtension.TaskAttribute.Category));

            // Add selection change handler
            this.m_AttributeCombo.SelectedIndexChanged += new EventHandler(OnAttributeSelChanged);
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

			if (m_Items.TryGetValue(task.GetID(), out item))
			{
				if (attribs.Contains(UIExtension.TaskAttribute.Title))
					item.Title = task.GetTitle();

				if (attribs.Contains(UIExtension.TaskAttribute.DoneDate))
					item.DoneDate = task.GetDoneDateString();

				if (attribs.Contains(UIExtension.TaskAttribute.DueDate))
					item.DueDate = task.GetDueDateString();

				if (attribs.Contains(UIExtension.TaskAttribute.StartDate))
					item.StartDate = task.GetStartDateString();

				if (attribs.Contains(UIExtension.TaskAttribute.Priority))
					item.Priority = task.GetPriority().ToString();

				if (attribs.Contains(UIExtension.TaskAttribute.Color))
					item.Color = task.GetColor().ToString();

				if (attribs.Contains(UIExtension.TaskAttribute.AllocTo))
					item.AllocTo = String.Join(", ", task.GetAllocatedTo());

				if (attribs.Contains(UIExtension.TaskAttribute.AllocBy))
					item.AllocBy = task.GetAllocatedBy();

				if (attribs.Contains(UIExtension.TaskAttribute.Status))
					item.Status = task.GetStatus();

				if (attribs.Contains(UIExtension.TaskAttribute.Category))
					item.Category = String.Join(", ", task.GetCategory());

				if (attribs.Contains(UIExtension.TaskAttribute.Percent))
					item.Percent = task.GetPercentDone().ToString();

				if (attribs.Contains(UIExtension.TaskAttribute.TimeEstimate))
				{
					Task.TimeUnits units = Task.TimeUnits.Hours;
					item.TimeEstimate = (task.GetTimeEstimate(ref units).ToString() + units);
				}

				if (attribs.Contains(UIExtension.TaskAttribute.TimeSpent))
				{
					Task.TimeUnits units = Task.TimeUnits.Hours;
					item.TimeSpent = (task.GetTimeSpent(ref units).ToString() + units);
				}

				if (attribs.Contains(UIExtension.TaskAttribute.FileReference))
					item.FileReference = String.Join(", ", task.GetFileReference());

				if (attribs.Contains(UIExtension.TaskAttribute.Comments))
					item.Comments = task.GetComments();

				if (attribs.Contains(UIExtension.TaskAttribute.CreationDate))
					item.CreationDate = task.GetCreationDateString();

				if (attribs.Contains(UIExtension.TaskAttribute.CreatedBy))
					item.CreatedBy = task.GetCreatedBy();

				if (attribs.Contains(UIExtension.TaskAttribute.Risk))
					item.Risk = task.GetRisk().ToString();

				if (attribs.Contains(UIExtension.TaskAttribute.Dependency))
					item.Dependency = String.Join(", ", task.GetDependency());

				if (attribs.Contains(UIExtension.TaskAttribute.Version))
					item.Version = task.GetVersion();

				if (attribs.Contains(UIExtension.TaskAttribute.Tag))
					item.Tags = String.Join(", ", task.GetTag());
			}
			else
			{
				item = new CloudTaskItem();

				item.Title = task.GetTitle();
				item.DoneDate = task.GetDoneDateString();
				item.DueDate = task.GetDueDateString();
				item.StartDate = task.GetStartDateString();
				item.Priority = task.GetPriority().ToString();
				item.Color = task.GetColor().ToString();
				item.AllocTo = String.Join(", ", task.GetAllocatedTo());
				item.AllocBy = task.GetAllocatedBy();
				item.Status = task.GetStatus();
				item.Category = String.Join(", ", task.GetCategory());
				item.Percent = task.GetPercentDone().ToString();
				item.FileReference = String.Join(", ", task.GetFileReference());
				item.Comments = task.GetComments();
				item.CreationDate = task.GetCreationDateString();
				item.CreatedBy = task.GetCreatedBy();
				item.Risk = task.GetRisk().ToString();
				item.Dependency = String.Join(", ", task.GetDependency());
				item.Version = task.GetVersion();
				item.Tags = String.Join(", ", task.GetTag());

				Task.TimeUnits units = Task.TimeUnits.Hours;
				item.TimeEstimate = (task.GetTimeEstimate(ref units).ToString() + units);
				item.TimeSpent = (task.GetTimeSpent(ref units).ToString() + units);
			}

			m_Items[task.GetID()] = item;

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
			return excludeEnglishCommonWords
					   ? (IBlacklist)new CommonWords()
					   : new NullBlacklist();
		}

		public bool WantEditUpdate(UIExtension.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case UIExtension.TaskAttribute.Title:
				case UIExtension.TaskAttribute.DoneDate:
				case UIExtension.TaskAttribute.DueDate:
				case UIExtension.TaskAttribute.StartDate:
				case UIExtension.TaskAttribute.Priority:
				case UIExtension.TaskAttribute.Color:
				case UIExtension.TaskAttribute.AllocTo:
				case UIExtension.TaskAttribute.AllocBy:
				case UIExtension.TaskAttribute.Status:
				case UIExtension.TaskAttribute.Category:
				case UIExtension.TaskAttribute.Percent:
				case UIExtension.TaskAttribute.TimeEstimate:
				case UIExtension.TaskAttribute.TimeSpent:
				case UIExtension.TaskAttribute.FileReference:
				case UIExtension.TaskAttribute.Comments:
				case UIExtension.TaskAttribute.Flag:
				case UIExtension.TaskAttribute.CreationDate:
				case UIExtension.TaskAttribute.CreatedBy:
				case UIExtension.TaskAttribute.Risk:		
				case UIExtension.TaskAttribute.ExternalId:	
				case UIExtension.TaskAttribute.Cost:		
				case UIExtension.TaskAttribute.Dependency:	
				case UIExtension.TaskAttribute.Recurrence:	
				case UIExtension.TaskAttribute.Version:		
				case UIExtension.TaskAttribute.Position:
				case UIExtension.TaskAttribute.Tag:
					return true;				  
			}									  
												  
			// all else
			return false;
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
//             System.Drawing.Point pt = m_WordCloud.PointToClient(new System.Drawing.Point(xPos, yPos));
//             Calendar.Appointment appointment = m_WordCloud.GetAppointmentAt(pt.X, pt.Y);
// 
//             if (appointment != null)
//             {
//                 return UIExtension.HitResult.Task;
//             }
//             else if (m_WordCloud.GetTrueRectangle().Contains(pt))
//             {
//                 return UIExtension.HitResult.Tasklist;
//             }

            // else
			return UIExtension.HitResult.Nowhere;
		}

		public void SetUITheme(UITheme theme)
		{
//             m_WordCloud.Invalidate(true);

            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackDark);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
			if (appOnly)
			{
				bool taskColorIsBkgnd = (prefs.GetProfileInt("Preferences", "ColorTaskBackground", 0) != 0);

				if (taskColorIsBkgnd != m_taskColorIsBkgnd)
				{
					m_taskColorIsBkgnd = taskColorIsBkgnd;
					Invalidate();
				}
			}
		}
		
		// PRIVATE ------------------------------------------------------------------------------

		private void InitializeComponent()
		{
			this.BackColor = System.Drawing.Color.White;
			this.m_Items = new Dictionary<UInt32, CloudTaskItem>();

			CreateWordCloud();
            CreateAttributeCombo();
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
            m_WordCloud.Invalidate();
        }

        private void OnAttributeSelChanged(object sender, EventArgs args)
        {
            var comboBox = (System.Windows.Forms.ComboBox)sender;
            AttributeItem selItem = (AttributeItem)comboBox.SelectedItem;

            m_Attrib = selItem.Attrib;
            UpdateWeightedWords();
        }
    }
}
