using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows.Forms.VisualStyles;

namespace PertNetworkUIExtension
{
	public delegate void SelectionChangeEventHandler(object sender, NetworkItem item);
	public delegate bool DragDropChangeEventHandler(object sender, NetworkDragEventArgs e);

	[System.ComponentModel.DesignerCategory("")]

	public class NetworkDragEventItem
	{
		public NetworkDragEventItem( /* TODO*/ )
		{
		}
	}

	public class NetworkDragEventArgs : EventArgs
	{
		public NetworkDragEventArgs( /* TODO*/ )
		{
		}

	}

	public partial class NetworkControl : UserControl
	{
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		static extern int SendMessage(IntPtr hWnd, int msg, int wParam = 0, int lParam = 0);

		// --------------------------

		[DllImport("User32.dll")]
		static extern uint GetDoubleClickTime();

		// --------------------------

		[DllImport("User32.dll")]
		static extern int GetSystemMetrics(int index);

		const int SM_CXDOUBLECLK = 36;
		const int SM_CYDOUBLECLK = 37;
		const int SM_CXDRAG = 68;
		const int SM_CYDRAG = 69;

		const int WM_PAINT = 0x000F;

		// Constants ---------------------------------------------------------------------

		protected float ZoomFactor { get; private set; }
		protected bool IsZoomed { get { return (ZoomFactor < 1.0f); } }

		protected enum DropPos
		{
			None,
			On,
			Left,
			Right,
		}

		// Data --------------------------------------------------------------------------

		private DropPos DropSite;
		private Timer DragTimer;
		private int LastDragTick = 0;

		// Font setting must go thru SetFont
		public new Font Font
		{
			get { return base.Font; }
			private set { base.Font = value; }
		}
		protected Font BaseFont { get; private set; }

		private bool IsSavingToImage = false;
		private uint SelectedItemId = 0;

		protected int LabelPadding = 2;

		protected int ItemHeight { get { return ((Font.Height + LabelPadding) * 4); } }
		protected int ItemWidth { get { return (ItemHeight* 3); } }
		protected int ItemVertSpacing { get { return (ItemHeight / 4); } }
		protected int ItemHorzSpacing { get { return (ItemWidth / 4); } }
		protected int GraphBorder { get { return ItemVertSpacing; } }

		public NetworkData Data { get; private set; }

		// Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

		public NetworkControl()
		{
			DropSite = DropPos.None;
			ConnectionColor = Color.Magenta;
			Data = new NetworkData();
			BaseFont = Font;

			InitializeComponent();
		}

		public void RebuildGroups()
		{
			RecalculateGraphSize(Data.RebuildGroups());
		}

		public void RecalculateGraphSize()
		{
			RecalculateGraphSize(Data.Items.CalcMaximumPosition());
		}

		private void RecalculateGraphSize(Point maxPos)
		{
			Rectangle maxItemRect = CalcItemRectangle(maxPos.X, maxPos.Y, false);

			AutoScrollMinSize = new Size(maxItemRect.Right + GraphBorder, maxItemRect.Bottom + GraphBorder);

			HorizontalScroll.SmallChange = ItemHeight;
			VerticalScroll.SmallChange = ItemWidth;

			Invalidate();
		}

		public NetworkItem HitTestItem(Point pos)
		{
			// Brute force for now
			foreach (var group in Data.Groups)
			{
				foreach (var item in group.ItemValues)
				{
					var itemRect = CalcItemRectangle(item);

					if (itemRect.Contains(pos))
						return item;
				}
			}

			// else
			return null;
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			DoPaint(e.Graphics, e.ClipRectangle);
		}

		void DoPaint(Graphics graphics, Rectangle clipRect)
		{
			int iGroup = 0;
			var drawnItems = new HashSet<NetworkItem>();

			foreach (var group in Data.Groups)
			{
				foreach (var item in group.ItemValues)
				{
					if (!drawnItems.Contains(item))
					{
						if (WantDrawItem(item, clipRect))
						{
							OnPaintItem(graphics, item, iGroup, (!IsSavingToImage && (item.UniqueId == SelectedItemId)));
						}
						else
						{
							//int breakpoint = 0;
						}

						var dependencies = group.GetItemDependencies(item);

						foreach (var dependItem in dependencies)
						{
							if (WantDrawConnection(dependItem, item, clipRect))
							{
								var smoothing = graphics.SmoothingMode;
								graphics.SmoothingMode = SmoothingMode.AntiAlias;

								OnPaintConnection(graphics, dependItem, item);

								graphics.SmoothingMode = smoothing;
							}
							else
							{
								//int breakpoint = 0;
							}
						}

						drawnItems.Add(item);
					}
				}

				iGroup++;
			}

		}

		bool WantDrawItem(NetworkItem item, Rectangle clipRect)
		{
			if (IsSavingToImage)
				return true;

			return clipRect.IntersectsWith(CalcItemRectangle(item));
		}

		bool WantDrawConnection(NetworkItem fromItem, NetworkItem toItem, Rectangle clipRect)
		{
			if (IsSavingToImage)
				return true;

			var fromRect = CalcItemRectangle(fromItem);
			var toRect = CalcItemRectangle(toItem);

			return clipRect.IntersectsWith(Rectangle.Union(fromRect, toRect));
		}

		virtual protected void OnPaintItem(Graphics graphics, NetworkItem item, int iGroup, bool selected)
		{
			var itemRect = CalcItemRectangle(item);
			var itemText = String.Format("{0} (id: {1}, grp: {2})", item.Title, item.UniqueId, iGroup);

			graphics.DrawRectangle(Pens.Black, itemRect);

			if (selected)
			{
				graphics.FillRectangle(SystemBrushes.Highlight, itemRect);
				graphics.DrawString(itemText, Font, SystemBrushes.HighlightText, itemRect);
			}
			else
			{
				graphics.DrawString(itemText, Font, Brushes.Blue, itemRect);
			}
		}

		virtual protected void OnPaintConnection(Graphics graphics, NetworkItem fromItem, NetworkItem toItem)
		{
			var fromRect = CalcItemRectangle(fromItem);
			var toRect = CalcItemRectangle(toItem);

			graphics.DrawLine(Pens.Blue, 
								fromRect.Right, 
								(fromRect.Top + (fromRect.Height / 2)), 
								toRect.Left, 
								(toRect.Top + (toRect.Height / 2)));
		}

		protected Rectangle CalcItemRectangle(NetworkItem item)
		{
			return CalcItemRectangle(item.Position.X, item.Position.Y, !IsSavingToImage);
		}

		private Rectangle CalcItemRectangle(int x, int y, bool scrolled)
		{
			Rectangle itemRect = new Rectangle(GraphBorder, GraphBorder, 0, 0);

			itemRect.X += ((ItemWidth + ItemHorzSpacing) * x);
			itemRect.Y += ((ItemHeight + ItemVertSpacing) * y);

			itemRect.Width = ItemWidth;
			itemRect.Height = ItemHeight;

			// Offset rect by scroll pos
			if (scrolled)
				itemRect.Offset(-HorizontalScroll.Value, -VerticalScroll.Value);

			return itemRect;
		}
				
		public bool SetFont(String fontName, int fontSize)
		{
			if ((BaseFont.Name == fontName) && (BaseFont.Size == fontSize))
				return false;

			ZoomFactor = 1.0f;
			BaseFont = new Font(fontName, fontSize, FontStyle.Regular);
			Font = BaseFont;

			RecalculateGraphSize();
			return true;
		}

		public Color ConnectionColor;
		public bool ReadOnly;

		public bool SetSelectedItem(uint uniqueID)
		{
			var item = Data.GetItem(uniqueID);

			if (item == null)
				return false;

			SelectedItemId = uniqueID;

			EnsureItemVisible(item);
			Invalidate();

			return true;
		}

		public Rectangle GetSelectedItemRect()
		{
			var selItem = SelectedItem;

			return ((selItem == null) ? new Rectangle(0, 0, 0, 0) : CalcItemRectangle(selItem));
		}

		public Bitmap SaveToImage()
		{
			IsSavingToImage = true;

			// Total size of the graph
			Rectangle graphRect = new Rectangle(Point.Empty, AutoScrollMinSize);

			var image = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

			using (Graphics graphics = Graphics.FromImage(image))
			{
				graphics.Clear(SystemColors.Window);
				DoPaint(graphics, Rectangle.Empty);
			}

			IsSavingToImage = false;

			return image;
		}

		// Message Handlers -----------------------------------------------------------

		protected override void OnMouseDoubleClick(MouseEventArgs e)
		{
			// TODO
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			base.OnMouseDown(e);

			if (e.Button != MouseButtons.Left)
				return;

			var item = HitTestItem(e.Location);

			if (item == null)
				return;

			if (item.UniqueId != SelectedItemId)
			{
				SetSelectedItem(item.UniqueId);

				SelectionChange?.Invoke(this, SelectedItem);
			}
		}

		protected override void OnMouseWheel(MouseEventArgs e)
		{
			if ((ModifierKeys & Keys.Control) == Keys.Control)
			{
				float newFactor = ZoomFactor;

				if (e.Delta > 0)
				{
					newFactor += 0.1f;
					newFactor = Math.Min(newFactor, 1.0f);
				}
				else
				{
					newFactor -= 0.1f;
					newFactor = Math.Max(newFactor, 0.4f);
				}

				if (newFactor != ZoomFactor)
				{
					// Convert mouse pos to relative coordinates
					float relX = ((e.Location.X + HorizontalScroll.Value) / (float)HorizontalScroll.Maximum);
					float relY = ((e.Location.Y + VerticalScroll.Value) / (float)VerticalScroll.Maximum);

					ZoomFactor = newFactor;

					if (ZoomFactor == 1.0)
						Font = BaseFont;
					else
						Font = new Font(BaseFont.FontFamily, BaseFont.Size * ZoomFactor, BaseFont.Style);

					RecalculateGraphSize();

					// Scroll the view to keep the mouse located in the 
					// same relative position as before
					if (HorizontalScroll.Visible)
					{
						int newX = (int)(relX * HorizontalScroll.Maximum) - e.Location.X;

						HorizontalScroll.Value = Validate(newX, HorizontalScroll);
					}

					if (VerticalScroll.Visible)
					{
						int newY = (int)(relY * VerticalScroll.Maximum) - e.Location.Y;

						VerticalScroll.Value = Validate(newY, VerticalScroll);
					}

					PerformLayout();
				}
			}
			else
			{
				// Default scroll
				base.OnMouseWheel(e);
			}
		}

		static int Validate(int pos, ScrollProperties scroll)
		{
			return Math.Max(scroll.Minimum, Math.Min(pos, scroll.Maximum));
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);

			if ((e.Button == MouseButtons.Left) && DragTimer.Enabled)
			{
				Debug.Assert(!ReadOnly);

				if (CheckStartDragging(e.Location))
					DragTimer.Stop();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			DragTimer.Stop();

			base.OnMouseUp(e);

			if (e.Button == MouseButtons.Left)
			{
				// TODO
			}
		}

		protected void OnDragTimer(object sender, EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			DragTimer.Stop();

			bool mouseDown = ((MouseButtons & MouseButtons.Left) == MouseButtons.Left);

			if (mouseDown)
				CheckStartDragging(MousePosition);
		}
	
		protected override void OnDragOver(DragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			// TODO
		}

		protected override void OnDragDrop(DragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			// TODO
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
			{
				// TODO
			}
		}

		protected override void OnDragLeave(EventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnDragLeave(e);

			// TODO

			Invalidate();
		}
		
		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);
		}

		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			Invalidate(GetSelectedItemRect());
		}

		protected override void OnLostFocus(EventArgs e)
		{
			base.OnLostFocus(e);

			Invalidate(GetSelectedItemRect());
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (HandleCursorKey(e.KeyCode))
				return;

// 			switch (e.KeyCode)
// 			{
// 			}

			// else
			base.OnKeyDown(e);
		}

		// Internals -----------------------------------------------------------

		private bool CheckStartDragging(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			// TODO

			return false;
		}
		
		virtual protected bool IsAcceptableDropTarget(Object draggedItemData, Object dropTargetItemData, DropPos dropPos, bool copy)
		{
			Debug.Assert(!ReadOnly);

			return true;
		}

		virtual protected bool IsAcceptableDragSource(Object itemData)
		{
			Debug.Assert(!ReadOnly);

			return (itemData != null);
		}

		private Rectangle GetDoubleClickRect(Point cursor)
		{
			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDOUBLECLK) / 2, GetSystemMetrics(SM_CYDOUBLECLK) / 2);

			return rect;
		}

		private Rectangle GetDragRect(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDRAG) / 2, GetSystemMetrics(SM_CYDRAG) / 2);

			return rect;
		}

		private void DoDrop(/* TODO */)
		{
			Debug.Assert(!ReadOnly);

			// TODO
		}

		virtual protected bool DoDrop(NetworkDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			if (DragDropChange != null)
				return DragDropChange(this, e);

			// else
			return true;
		}

		protected void EnsureItemVisible(NetworkItem item)
		{
			if (item == null)
				return;

			Rectangle itemRect = CalcItemRectangle(item);

			if (ClientRectangle.Contains(itemRect))
				return;

			if (HorizontalScroll.Visible)
			{
				int xOffset = 0;

				if (itemRect.Left < ClientRectangle.Left)
				{
					xOffset = (itemRect.Left - ClientRectangle.Left - (ItemHorzSpacing / 2));
				}
				else if (itemRect.Right > ClientRectangle.Right)
				{
					xOffset = (itemRect.Right - ClientRectangle.Right + (ItemHorzSpacing / 2));
				}

				if (xOffset != 0)
				{
					int scrollX = (HorizontalScroll.Value + xOffset);
					HorizontalScroll.Value = Validate(scrollX, HorizontalScroll);
				}
			}

			if (VerticalScroll.Visible)
			{
				int yOffset = 0;

				if (itemRect.Top < ClientRectangle.Top)
				{
					yOffset = (itemRect.Top - ClientRectangle.Top - (ItemVertSpacing / 2));
				}
				else if (itemRect.Bottom > ClientRectangle.Bottom)
				{
					yOffset = (itemRect.Bottom - ClientRectangle.Bottom + (ItemVertSpacing / 2));
				}

				if (yOffset != 0)
				{
					int scrollY = (VerticalScroll.Value + yOffset);
					VerticalScroll.Value = Validate(scrollY, VerticalScroll);
				}
			}

			PerformLayout();
			Invalidate();
		}

		protected void EnableSelectionNotifications(bool enable)
		{
			// TODO
		}

		protected int PageSize
		{
			get { return Math.Max(1, (VerticalScroll.LargeChange / (ItemHeight + ItemVertSpacing))); }
		}

		protected bool HandleCursorKey(Keys key)
		{
			NetworkItem selItem = SelectedItem;

			if (selItem == null)
				return false;

			uint nextItemId = 0;

			switch (key)
			{
			case Keys.Left:
				if (selItem.Position.X > 0)
				{
					// Try for an item at the same vertical position
					var leftItems = Data.Items.GetHorizontalItems(selItem.Position.Y, 0, selItem.Position.X - 1);

					// else try for the first of this item's dependencies
					if (leftItems.Count == 0)
						leftItems = Data.Items.GetItemDependencies(selItem);

					if (leftItems.Count > 0)
						nextItemId = leftItems.Last().UniqueId;
				}
				break;

			case Keys.Right:
				{
					// Try for an item at the same vertical position
					var rightItems = Data.Items.GetHorizontalItems(selItem.Position.Y, selItem.Position.X + 1);

					// else try for the first of this item's dependents
					if (rightItems.Count == 0)
						rightItems = Data.Items.GetItemDependents(selItem);

					if (rightItems.Count > 0)
						nextItemId = rightItems[0].UniqueId;
				}
				break;

			case Keys.Up:
				if (selItem.Position.Y > 0)
				{
					// Try for an item at the same horizontal position
					var upperItems = Data.Items.GetVerticalItems(selItem.Position.X, 0, selItem.Position.Y - 1);

					if (upperItems.Count > 0)
						nextItemId = upperItems.Last().UniqueId;
				}
				break;

			case Keys.Down:
				{
					// Try for an item at the same horizontal position
					var lowerItems = Data.Items.GetVerticalItems(selItem.Position.X, selItem.Position.Y + 1);

					if (lowerItems.Count > 0)
						nextItemId = lowerItems[0].UniqueId;
				}
				break;

			case Keys.PageUp:
				if (selItem.Position.Y > 0)
				{
					// Try for an item at the same horizontal position
					var upperItems = Data.Items.GetVerticalItems(selItem.Position.X, 0, selItem.Position.Y - 1);

					if (upperItems.Count > PageSize)
					{
						nextItemId = upperItems[selItem.Position.Y - PageSize].UniqueId;
					}
					else if (upperItems.Count > 0)
					{
						nextItemId = upperItems[0].UniqueId;
					}
				}
				break;

			case Keys.PageDown:
				{
					// Try for an item at the same horizontal position
					var lowerItems = Data.Items.GetVerticalItems(selItem.Position.X, selItem.Position.Y + 1);

					if (lowerItems.Count > PageSize)
					{
						nextItemId = lowerItems[PageSize].UniqueId;
					}
					else if (lowerItems.Count > 0)
					{
						nextItemId = lowerItems.Last().UniqueId;
					}
				}
				break;

			case Keys.Home:
				if (selItem.Position.Y > 0)
				{
					// Try for an item at the same horizontal position
					var upperItems = Data.Items.GetVerticalItems(selItem.Position.X, 0, selItem.Position.Y - 1);

					if (upperItems.Count > 0)
						nextItemId = upperItems[0].UniqueId;
				}
				break;

			case Keys.End:
				{
					// Try for an item at the same horizontal position
					var lowerItems = Data.Items.GetVerticalItems(selItem.Position.X, selItem.Position.Y + 1);

					if (lowerItems.Count > 0)
						nextItemId = lowerItems.Last().UniqueId;
				}
				break;
			}

			if (nextItemId != 0)
			{
				SelectedItemId = nextItemId;

				EnsureItemVisible(SelectedItem);
				Invalidate();

				return true;
			}

			return false;
		}

		protected bool IsEmpty()
		{
			return (Data.Items.Count == 0);
		}

		protected NetworkItem SelectedItem
		{
			get
			{
				return Data.GetItem(SelectedItemId);
			}
		}

		protected NetworkItem HitTestPositions(Point point)
		{
			// TODO

			return null;
		}

	}

}
