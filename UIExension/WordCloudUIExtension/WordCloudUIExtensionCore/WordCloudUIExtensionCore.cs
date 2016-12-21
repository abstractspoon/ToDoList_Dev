
///////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using TDLPluginHelpers;
using Gma.CodeCloud.Controls.TextAnalyses;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist;
using Gma.CodeCloud.Controls.TextAnalyses.Blacklist.En;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

///////////////////////////////////////////////////////////////////////////

namespace WordCloudUIExtension
{
	public class WordCloudUIExtensionCore : System.Windows.Forms.Panel, ITDLUIExtension
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

            public IEnumerable<string> GetWords(TDLUIExtension.TaskAttribute attrib)
            {
                var words = new List<string>();
                var delims = new char[] { ',', ' ', '\t', '\r', '\n' };

                words.AddRange(GetAttribValue(attrib).Split(delims, StringSplitOptions.RemoveEmptyEntries));

                return words;
            }

            public string GetAttribValue(TDLUIExtension.TaskAttribute attrib)
            {
                switch (attrib)
                {
				case TDLUIExtension.TaskAttribute.Title: return Title;
				case TDLUIExtension.TaskAttribute.DoneDate: return DoneDate;
    			case TDLUIExtension.TaskAttribute.DueDate: return DueDate;
				case TDLUIExtension.TaskAttribute.StartDate: return StartDate;
				case TDLUIExtension.TaskAttribute.AllocTo: return AllocTo;
				case TDLUIExtension.TaskAttribute.AllocBy: return AllocBy;
				case TDLUIExtension.TaskAttribute.Status: return Status;
				case TDLUIExtension.TaskAttribute.Category: return Category;
				case TDLUIExtension.TaskAttribute.Comments: return Comments;
				case TDLUIExtension.TaskAttribute.CreationDate: return CreationDate;
				case TDLUIExtension.TaskAttribute.CreatedBy: return CreatedBy;
				case TDLUIExtension.TaskAttribute.Version: return Version;
				case TDLUIExtension.TaskAttribute.Tag: return Tags;
                }

                // all else
                return "";
            }
        }

        public class AttributeItem
        {
            public AttributeItem(string name, TDLUIExtension.TaskAttribute attrib)
            {
                Name = name;
                Attrib = attrib;
            }

            public override string ToString()
            {
                return Name;
            }

            public string Name { set; get; }
            public TDLUIExtension.TaskAttribute Attrib { set; get; }
        }

        // -------------------------------------------------------------

        private const int ComboTop = 20;
        private const int ControlTop = 50;
        private const string FontName = "Tahoma";

        // -------------------------------------------------------------

		private Boolean m_taskColorIsBkgnd = false;
		private IntPtr m_hwndParent;
        private TDLUIExtension.TaskAttribute m_Attrib;

		private Dictionary<UInt32, CloudTaskItem> m_Items;
		private Gma.CodeCloud.Controls.CloudControl m_WordCloud;

        private System.Windows.Forms.ComboBox m_AttributeCombo;
        private System.Windows.Forms.Label m_AttributeLabel;
        private System.Windows.Forms.ComboBoxRenderer m_AttributeDisplay;

        // -------------------------------------------------------------

		public WordCloudUIExtensionCore(IntPtr hwndParent)
		{
			m_hwndParent = hwndParent;
			InitializeComponent();
		}

		private void CreateWordCloud()
		{
			this.m_WordCloud = new Gma.CodeCloud.Controls.CloudControl();

            this.m_WordCloud.Font = new System.Drawing.Font(FontName, 8);
            this.m_WordCloud.Location = new System.Drawing.Point(0, ControlTop);
 			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);
			this.m_WordCloud.MaxFontSize = 50;

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
            this.m_AttributeCombo.Items.Add(new AttributeItem("Title", TDLUIExtension.TaskAttribute.Title));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Comments", TDLUIExtension.TaskAttribute.Comments));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Status", TDLUIExtension.TaskAttribute.Status));
            this.m_AttributeCombo.Items.Add(new AttributeItem("Category", TDLUIExtension.TaskAttribute.Category));

            // Add selection change handler
            this.m_AttributeCombo.SelectedIndexChanged += new EventHandler(OnAttributeSelChanged);
        }

		// ITDLUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 dwTaskID)
		{
			return true;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
		}

		public void UpdateTasks(TDLTaskList tasks, 
								TDLUIExtension.UpdateType type, 
								System.Collections.Generic.HashSet<TDLUIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case TDLUIExtension.UpdateType.Delete:
				case TDLUIExtension.UpdateType.Move:
				case TDLUIExtension.UpdateType.All:
					// Rebuild
					break;
					
				case TDLUIExtension.UpdateType.New:
				case TDLUIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			TDLTask task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, attribs))
				task = task.GetNextTask();

            // Update
			UpdateWeightedWords();
		}

		private bool ProcessTaskUpdate(TDLTask task, 
									   TDLUIExtension.UpdateType type,
                                       HashSet<TDLUIExtension.TaskAttribute> attribs)
		{
			if (!task.IsValid())
				return false;

			CloudTaskItem item;

			if (m_Items.TryGetValue(task.GetID(), out item))
			{
				if (attribs.Contains(TDLUIExtension.TaskAttribute.Title))
					item.Title = task.GetTitle();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.DoneDate))
					item.DoneDate = task.GetDoneDateString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.DueDate))
					item.DueDate = task.GetDueDateString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.StartDate))
					item.StartDate = task.GetStartDateString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Priority))
					item.Priority = task.GetPriority().ToString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Color))
					item.Color = task.GetColor().ToString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocTo))
					item.AllocTo = String.Join(", ", task.GetAllocatedTo());

				if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocBy))
					item.AllocBy = task.GetAllocatedBy();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Status))
					item.Status = task.GetStatus();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Category))
					item.Category = String.Join(", ", task.GetCategory());

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Percent))
					item.Percent = task.GetPercentDone().ToString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeEstimate))
				{
					TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
					item.TimeEstimate = (task.GetTimeEstimate(ref units).ToString() + units);
				}

				if (attribs.Contains(TDLUIExtension.TaskAttribute.TimeSpent))
				{
					TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
					item.TimeSpent = (task.GetTimeSpent(ref units).ToString() + units);
				}

				if (attribs.Contains(TDLUIExtension.TaskAttribute.FileReference))
					item.FileReference = String.Join(", ", task.GetFileReference());

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Comments))
					item.Comments = task.GetComments();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.CreationDate))
					item.CreationDate = task.GetCreationDateString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.CreatedBy))
					item.CreatedBy = task.GetCreatedBy();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Risk))
					item.Risk = task.GetRisk().ToString();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Dependency))
					item.Dependency = String.Join(", ", task.GetDependency());

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Version))
					item.Version = task.GetVersion();

				if (attribs.Contains(TDLUIExtension.TaskAttribute.Tag))
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

				TDLTask.TimeUnits units = TDLTask.TimeUnits.Hours;
				item.TimeEstimate = (task.GetTimeEstimate(ref units).ToString() + units);
				item.TimeSpent = (task.GetTimeSpent(ref units).ToString() + units);
			}

			m_Items[task.GetID()] = item;

			// Process children
			TDLTask subtask = task.GetFirstSubtask();

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

		public bool WantEditUpdate(TDLUIExtension.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case TDLUIExtension.TaskAttribute.Title:
				case TDLUIExtension.TaskAttribute.DoneDate:
				case TDLUIExtension.TaskAttribute.DueDate:
				case TDLUIExtension.TaskAttribute.StartDate:
				case TDLUIExtension.TaskAttribute.Priority:
				case TDLUIExtension.TaskAttribute.Color:
				case TDLUIExtension.TaskAttribute.AllocTo:
				case TDLUIExtension.TaskAttribute.AllocBy:
				case TDLUIExtension.TaskAttribute.Status:
				case TDLUIExtension.TaskAttribute.Category:
				case TDLUIExtension.TaskAttribute.Percent:
				case TDLUIExtension.TaskAttribute.TimeEstimate:
				case TDLUIExtension.TaskAttribute.TimeSpent:
				case TDLUIExtension.TaskAttribute.FileReference:
				case TDLUIExtension.TaskAttribute.Comments:
				case TDLUIExtension.TaskAttribute.Flag:
				case TDLUIExtension.TaskAttribute.CreationDate:
				case TDLUIExtension.TaskAttribute.CreatedBy:
				case TDLUIExtension.TaskAttribute.Risk:		
				case TDLUIExtension.TaskAttribute.ExternalId:	
				case TDLUIExtension.TaskAttribute.Cost:		
				case TDLUIExtension.TaskAttribute.Dependency:	
				case TDLUIExtension.TaskAttribute.Recurrence:	
				case TDLUIExtension.TaskAttribute.Version:		
				case TDLUIExtension.TaskAttribute.Position:
				case TDLUIExtension.TaskAttribute.Tag:
					return true;				  
			}									  
												  
			// all else
			return false;
		}
	   
		public bool WantSortUpdate(TDLUIExtension.TaskAttribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref TDLTask task)
		{
			return false;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}
	   
		public bool DoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			return false;
		}

		public bool CanDoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            return false;
		}

		public TDLUIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
		{
//             System.Drawing.Point pt = m_WordCloud.PointToClient(new System.Drawing.Point(xPos, yPos));
//             Calendar.Appointment appointment = m_WordCloud.GetAppointmentAt(pt.X, pt.Y);
// 
//             if (appointment != null)
//             {
//                 return TDLUIExtension.HitResult.Task;
//             }
//             else if (m_WordCloud.GetTrueRectangle().Contains(pt))
//             {
//                 return TDLUIExtension.HitResult.Tasklist;
//             }

            // else
			return TDLUIExtension.HitResult.Nowhere;
		}

		public void SetUITheme(TDLTheme theme)
		{
//             m_WordCloud.Invalidate(true);

            this.BackColor = theme.GetAppDrawingColor(TDLTheme.AppColor.AppBackDark);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(TDLPreferences prefs, String key)
		{
		}

		public void LoadPreferences(TDLPreferences prefs, String key, bool appOnly)
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

            TDLWin32.RemoveBorder(m_WordCloud.Handle);
            TDLWin32.RemoveClientEdge(m_WordCloud.Handle);
            Invalidate();
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
