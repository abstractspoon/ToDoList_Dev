using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace EisenhowerUIExtension
{
	public partial class EisenhowerPaneListView : TaskListView
	{
		const int XColIndex = 1;
		const int YColIndex = 2;

		// --------------------------------------------------------

		private bool m_Selected;

		// --------------------------------------------------------

		public EisenhowerPaneListView()
		{
		}

		public bool Selected
		{
			get { return m_Selected; }

			set
			{
				if (m_Selected != value)
				{
					m_Selected = value;

					if (!m_Selected)
						SelectedIndices.Clear();

					Invalidate();
				}
			}
		}

		public uint HitTestTask(Point screenPos)
		{
			Point ptClient = PointToClient(screenPos);
			var lvHit = HitTest(ptClient);

			if (lvHit.Item != null)
				return GetTaskId(lvHit.Item.Index);

			// else
			return 0;
		}

		public uint GetTaskId(UIExtension.GetTask getTask)
		{
			return GetTaskIdEx(getTask, m_Selected);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
			return SelectTaskEx(text, selectTask, caseSensitive, wholeWord, findReplace);
		}

		public string XAttribTitle
		{
			set { Columns[XColIndex].Text = value; }
		}

		public string YAttribTitle
		{
			set { Columns[YColIndex].Text = value; }
		}

		public uint FirstTaskId  { get { return base.GetTaskId(0); } }
		public uint LastTaskId   { get { return base.GetTaskId(LastIndex); } }

		public uint FirstSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[0]) : 0); } }
		public uint LastSelectedTaskId	{ get { return (HasSelection ? GetTaskId(SelectedIndices[LastIndex]) : 0); } }

		protected override void OnDrawItem(DrawListViewItemEventArgs e)
		{
			base.OnDrawItem(e);
		}

		public void DrawDragImage(Graphics graphics, Size size)
		{
			var format = new StringFormat(StringFormatFlags.NoClip | StringFormatFlags.FitBlackBox | StringFormatFlags.NoWrap);

			format.LineAlignment = StringAlignment.Center;
			format.Trimming = StringTrimming.None;
			format.Alignment = StringAlignment.Near;

			var labelRect = Rectangle.Empty;
			labelRect.Height = Items[0].Bounds.Height;

			foreach (int index in SelectedIndices)
			{
				var task = GetTask(index);

				if ((task != null) && !task.IsLocked)
				{
					labelRect.X = LabelPadding;
					labelRect.Width = CalcLabelDragImageWidth(task, graphics);

					// Icon
					if (ItemsHaveIcons)
					{
						if (task.HasIcon && m_TaskIcons.Get(task.Id))
							m_TaskIcons.Draw(graphics, labelRect.Left, labelRect.Top);

						labelRect.X += TextIconOffset;
						labelRect.Width -= TextIconOffset;
					}

					// Selection
					UIExtension.SelectionRect.Draw(Handle,
													 graphics,
													 labelRect.X,
													 labelRect.Y,
													 labelRect.Width,
													 labelRect.Height,
													 false); // opaque
					// text
					graphics.DrawString(task.Title, 
										GetFont(task, true), 
										SystemBrushes.WindowText, 
										labelRect, 
										format);

					// Next item
					labelRect.Y = labelRect.Bottom;
				}
			}
		}

		public Size GetDragImageSize()
		{
			Size imageSize = Size.Empty;

			if (HasSelection)
			{
				var graphics = Graphics.FromHwnd(Handle);
				var itemHeight = Items[0].Bounds.Height;
				
				foreach (int index in SelectedIndices)
				{
					var task = GetTask(index);

					int width = CalcLabelDragImageWidth(task, graphics);
					imageSize.Width = Math.Max(imageSize.Width, width);

					if (width > 0)
						imageSize.Height += itemHeight;
				}
			}

			return imageSize;
		}

		// --------------------------------------------------------
		// Message handlers

		protected override bool IsItemSelected(ListViewItem lvItem)
		{
			return (m_Selected && base.IsItemSelected(lvItem));
		}

		public int CalcLabelDragImageWidth(ITaskBase task, Graphics graphics)
		{
			if ((task == null) || task.IsLocked)
				return 0;

			int labelWidth = (int)graphics.MeasureString(task.Title, GetFont(task, true)).Width;

			if (ItemsHaveIcons)
				labelWidth += TextIconOffset;

			return labelWidth + (2 * LabelPadding);
		}

	}
}
