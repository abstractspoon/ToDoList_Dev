﻿using System;
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

	//////////////////////////////////////////////////////////////////////////////////

	public class NetworkItemHitTestResult
	{
		public NetworkPath Path;
		public NetworkItem FromItem, ToItem;
		public int Segment = -1;

		public bool IsValid
		{
			get
			{
				if (FromItem == null)
					return false;

				if (FromItem == ToItem)
					return false;

				if ((ToItem == null) && (Segment != -1))
					return false;

				if ((ToItem != null) && (Segment == -1))
					return false;

				return true;
			}
		}

		public bool Matches(NetworkItemHitTestResult other)
		{
			if (other == null)
				return false;

			return ((Path == other.Path) &&
					(FromItem == other.FromItem) &&
					(ToItem == other.ToItem) &&
					(Segment == other.Segment));
		}

		public static bool Match(NetworkItemHitTestResult pos1, NetworkItemHitTestResult pos2)
		{
			if ((pos1 == null) && (pos2 == null))
				return true;

			if (pos1 == null)
				return false;

			return pos1.Matches(pos2);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	public class NetworkDragEventArgs : EventArgs
	{
		public NetworkDragEventArgs(NetworkItem draggedItem, NetworkItemHitTestResult dropPos, bool copy)
		{
			DraggedItem = draggedItem;
			DropPos = dropPos;
			Copy = copy;
		}

		public bool IsValid
		{
			get
			{
				if ((DraggedItem == null) || (DropPos == null))
					return false;

				if (!DropPos.IsValid)
					return false;

				if ((DropPos.FromItem == DraggedItem) || (DropPos.ToItem == DraggedItem))
					return false;

				return true;
			}
		}

		public NetworkItem DraggedItem;
		public NetworkItemHitTestResult DropPos;
		public bool Copy;
	}

	//////////////////////////////////////////////////////////////////////////////////

	class LayoutHelper
	{
		public LayoutHelper(ScrollableControl ctrl, int itemLineCount)
		{
			m_Ctrl = ctrl;
			Font = ctrl.Font;

			using (var graphics = ctrl.CreateGraphics())
				m_DpiFactor = graphics.DpiX / 96.0;

			SetItemLineCount(itemLineCount);
		}

		private ScrollableControl m_Ctrl;
		private double m_DpiFactor = 1.0;

		public int ItemLineCount { get; private set; }
		public bool SetItemLineCount(int count)
		{
			count = Math.Min(Math.Max(count, 1), 10);

			if (count != ItemLineCount)
			{
				ItemLineCount = count;
				return true;
			}

			return false;
		}
		
		public Font Font;
		public bool IsSavingToImage = false;

		public int ItemHeight { get { return ((Font.Height + LabelPadding) * ItemLineCount); } }
		public int ItemWidth { get { return (ItemHeight * 3); } }
		public int ItemVertSpacing { get { return (ItemHeight / 4); } }
		public int ItemHorzSpacing { get { return (ItemWidth / 4); } }

		public int RowHeight { get { return (ItemHeight + ItemVertSpacing); } }
		public int ColumnWidth { get { return (ItemWidth + ItemHorzSpacing); } }

		public int GraphBorder { get { return ItemVertSpacing; } }
		public int LabelPadding { get { return (int)(2 * m_DpiFactor); } }
		public double HitTestTolerance { get { return (10 * m_DpiFactor); } }

		// ------------------------------

		public Rectangle CalcItemRectangle(NetworkItem item)
		{
			return CalcItemRectangle(item.Position.X, item.Position.Y, !IsSavingToImage);
		}

		public Rectangle CalcPathBounds(NetworkPath path)
		{
			if (path.Count == 0)
				return Rectangle.Empty;

			var pathRect = CalcItemRectangle(path.Items[0]);

			for (int i = 1; i < path.Count; i++)
				pathRect = Rectangle.Union(pathRect, CalcItemRectangle(path.Items[i]));

			return pathRect;
		}

		public Rectangle CalcItemRectangle(int x, int y, bool scrolled)
		{
			Rectangle itemRect = new Rectangle(GraphBorder, GraphBorder, 0, 0);

			itemRect.X += ((ColumnWidth) * x);
			itemRect.Y += ((RowHeight) * y);

			itemRect.Width = ItemWidth;
			itemRect.Height = ItemHeight;

			if (scrolled)
			{
				var scrollPos = ScrollPos;
				itemRect.Offset(-scrollPos.X, -scrollPos.Y);
			}

			return itemRect;
		}

		private Point ScrollPos
		{
			get
			{
				if (IsSavingToImage)
					return Point.Empty;

				return new Point(m_Ctrl.HorizontalScroll.Value, m_Ctrl.VerticalScroll.Value);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////

	public partial class NetworkControl : UserControl
	{
		// Win32 Imports -----------------------------------------------------------------

		[DllImport("User32.dll")]
		static extern uint GetDoubleClickTime();

		// --------------------------

		[DllImport("User32.dll")]
		static extern int GetSystemMetrics(int index);

		const int SM_CXDOUBLECLK = 36;
		const int SM_CYDOUBLECLK = 37;
		const int SM_CXDRAG = 68;
		const int SM_CYDRAG = 69;

		// Constants ---------------------------------------------------------------------

		protected enum SelectionState
		{
			None,
			Selected,
			DropHighlighted
		}

		// Data --------------------------------------------------------------------------

		// Font setting must go thru SetFont
		public new Font Font
		{
			get { return base.Font; }
			private set
			{
				base.Font = value;
				Layout.Font = value;
			}
		}
		protected Font BaseFont { get; private set; }
		protected NetworkData Data { get; private set; }
		protected NetworkItemHitTestResult DropPos { get; private set; }

		protected float ZoomFactor { get; private set; }
		protected bool IsZoomed { get { return (ZoomFactor < 1.0f); } }

		protected int ItemHeight { get { return Layout.ItemHeight; } }
		protected int ItemWidth { get { return Layout.ItemWidth; } }
		protected int ItemVertSpacing { get { return Layout.ItemVertSpacing; } }
		protected int ItemHorzSpacing { get { return Layout.ItemHorzSpacing; } }
		protected int RowHeight { get { return Layout.RowHeight; } }
		protected int ColumnWidth { get { return Layout.ColumnWidth; } }
		protected int GraphBorder { get { return Layout.GraphBorder; } }
		protected int LabelPadding { get { return Layout.LabelPadding; } }

		protected int ItemLineCount
		{
			get { return Layout.ItemLineCount; }
			set
			{
				if (Layout.SetItemLineCount(value))
					RecalculateGraphSize();
			}
		}

		private uint SelectedItemId = 0;
		private new LayoutHelper Layout;
		private Timer DragTimer;

		// Public ------------------------------------------------------------------------

		public event SelectionChangeEventHandler SelectionChange;
		public event DragDropChangeEventHandler DragDropChange;

		public NetworkControl()
		{
			ConnectionColor = Color.Magenta;
			Data = new NetworkData();
			BaseFont = Font;
			Layout = new LayoutHelper(this, 2);

			DragTimer = new Timer();
			DragTimer.Interval = (int)GetDoubleClickTime();
			DragTimer.Tick += new EventHandler(OnDragTimer);

			InitializeComponent();
		}

		public void RebuildPaths()
		{
			RecalculateGraphSize(Data.RebuildPaths());
		}

		public void RecalculateGraphSize()
		{
			RecalculateGraphSize(Data.Paths.MaxPos);
		}

		private void RecalculateGraphSize(Point maxPos)
		{
			Rectangle maxItemRect = Layout.CalcItemRectangle(maxPos.X, maxPos.Y, false);

			AutoScrollMinSize = new Size(maxItemRect.Right + Layout.GraphBorder, maxItemRect.Bottom + Layout.GraphBorder);

			HorizontalScroll.SmallChange = Layout.ItemHeight;
			VerticalScroll.SmallChange = Layout.ItemWidth;

			Invalidate();
		}

		public NetworkItem HitTestItem(Point pos)
		{
			// Brute force for now
			foreach (var path in Data.Paths)
			{
				Rectangle pathRect = Rectangle.Empty;

				foreach (var item in path.Items)
				{
					var itemRect = Layout.CalcItemRectangle(item);

					// We can stop once we are beyond the rightmost visible items
					if (itemRect.Left > ClientRectangle.Right)
						break;

					if (itemRect.Contains(pos))
						return item;

					pathRect = Rectangle.Union(pathRect, itemRect);
				}

				// We can stop when we've passed the bottom of the visible items
				if (pathRect.Bottom > ClientRectangle.Bottom)
					break;
			}

			// else
			return null;
		}

		protected NetworkItemHitTestResult DragHitTest(DragEventArgs e)
		{
			var hitTest = DragHitTest(PointToClient(new Point(e.X, e.Y)));

			if ((hitTest != null) && !GetNetworkDragEventArgs(e, hitTest).IsValid)
				hitTest.Segment = -1;

			return hitTest;
		}

		protected NetworkItemHitTestResult DragHitTest(Point pos)
		{
			// Do the least amount of work possible
			var closestHit = new NetworkItemHitTestResult();

			foreach (var path in Data.Paths)
			{
				if (!ClientRectangle.IntersectsWith(Layout.CalcPathBounds(path)))
					continue;

				var prevItemRect = Layout.CalcItemRectangle(path.Items[0]);

				if (prevItemRect.Contains(pos))
				{
					closestHit.Path = path;
					closestHit.FromItem = path.Items[0];

					break;
				}

				for (int i = 1; i < path.Count; i++)
				{
					var itemRect = Layout.CalcItemRectangle(path.Items[i]);

					if (itemRect.Contains(pos))
					{
						closestHit.Path = path;
						closestHit.FromItem = path.Items[i];

						break;
					}

					if (Rectangle.Union(prevItemRect, itemRect).Contains(pos))
					{
						var points = GetConnectionPoints(path.Items[i - 1], path.Items[i]);
						int segment = Geometry2D.HitTest(points, pos, Layout.HitTestTolerance);

						if (segment != -1)
						{
							closestHit.Path = path;
							closestHit.FromItem = path.Items[i - 1];
							closestHit.ToItem = path.Items[i];
							closestHit.Segment = segment;

							break;
						}
					}

					prevItemRect = itemRect;
				}

				if (closestHit.IsValid)
					break;
			}

			return (closestHit.IsValid ? closestHit : null);
		}

		protected Rectangle CalcItemRectangle(NetworkItem item)
		{
			return Layout.CalcItemRectangle(item);
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			DoPaint(e.Graphics, e.ClipRectangle);
		}

		protected SelectionState GetItemSelectionState(NetworkItem item)
		{
			if (Layout.IsSavingToImage)
				return SelectionState.None;

			if (item.UniqueId == SelectedItemId)
				return SelectionState.Selected;

			if ((DropPos != null) && DropPos.IsValid && (DropPos.Segment == -1) && (DropPos.FromItem == item))
				return SelectionState.DropHighlighted;

			return SelectionState.None;
		}

		protected virtual void DoPaint(Graphics graphics, Rectangle clipRect)
		{
			var drawnItems = new HashSet<NetworkItem>();

			foreach (var path in Data.Paths)
			{
				if (!clipRect.IntersectsWith(Layout.CalcPathBounds(path)))
					continue;

				NetworkItem prevItem = null;

				foreach (var item in path.Items)
				{
					if (!drawnItems.Contains(item))
					{
						if (WantDrawItem(item, clipRect))
						{
							OnPaintItem(graphics, item, path, GetItemSelectionState(item));
						}
						else
						{
							//int breakpoint = 0;
						}

						drawnItems.Add(item);
					}

					if (prevItem != null)
					{
						if (WantDrawConnection(item, prevItem, clipRect))
						{
							graphics.SmoothingMode = SmoothingMode.AntiAlias;

							OnPaintConnection(graphics, prevItem, item, path);
						}
						else
						{
							//int breakpoint = 0;
						}
					}

					prevItem = item;
				}
			}

			// Draw the current drop pos
			if ((DropPos != null) && (DropPos.Segment != -1))
			{
				var points = GetConnectionPoints(DropPos.FromItem, DropPos.ToItem);
				DrawDragInsertionPoint(graphics, points, DropPos.Segment);
			}
		}

		protected bool WantDrawItem(NetworkItem item, Rectangle clipRect)
		{
			if (Layout.IsSavingToImage)
				return true;

			return clipRect.IntersectsWith(Layout.CalcItemRectangle(item));
		}

		protected bool WantDrawConnection(NetworkItem fromItem, NetworkItem toItem, Rectangle clipRect)
		{
			if (Layout.IsSavingToImage)
				return true;

			var fromRect = Layout.CalcItemRectangle(fromItem);
			var toRect = Layout.CalcItemRectangle(toItem);

			return clipRect.IntersectsWith(Rectangle.Union(fromRect, toRect));
		}

		virtual protected void OnPaintItem(Graphics graphics, NetworkItem item, NetworkPath path, SelectionState state)
		{
			var itemRect = Layout.CalcItemRectangle(item);
			graphics.DrawRectangle(Pens.Black, itemRect);

			int iPath = Data.Paths.IndexOf(path) + 1;
			var itemText = String.Format("{0} (id: {1}, p: {2})", item.Title, item.UniqueId, iPath);

			switch (state)
			{
			case SelectionState.DropHighlighted:
				graphics.DrawRectangle(SystemPens.Highlight, itemRect);
				graphics.DrawString(itemText, Font, Brushes.Blue, itemRect);
				break;

			case SelectionState.Selected:
				graphics.FillRectangle(SystemBrushes.Highlight, itemRect);
				graphics.DrawString(itemText, Font, SystemBrushes.HighlightText, itemRect);
				break;

			case SelectionState.None:
				graphics.DrawString(itemText, Font, Brushes.Blue, itemRect);
				break;
			}
		}

		virtual protected void OnPaintConnection(Graphics graphics, NetworkItem fromItem, NetworkItem toItem, NetworkPath path)
		{
			graphics.DrawLines(Pens.Blue, GetConnectionPoints(fromItem, toItem));
		}

		virtual protected Point[] GetConnectionPoints(NetworkItem fromItem, NetworkItem toItem)
		{
			var fromRect = Layout.CalcItemRectangle(fromItem);
			var toRect = Layout.CalcItemRectangle(toItem);

			Point[] points = new Point[2];

			points[0].X = fromRect.Right;
			points[0].Y = (fromRect.Top + (fromRect.Height / 2));
			points[1].X = toRect.Left;
			points[1].Y = (toRect.Top + (toRect.Height / 2));

			return points;
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
			Update();

			return true;
		}

		public Rectangle GetSelectedItemRect()
		{
			var selItem = SelectedItem;

			return ((selItem == null) ? 
						new Rectangle(0, 0, 0, 0) : 
						Layout.CalcItemRectangle(selItem));
		}

		public Bitmap SaveToImage()
		{
			Layout.IsSavingToImage = true;

			// Total size of the graph
			Rectangle graphRect = new Rectangle(Point.Empty, AutoScrollMinSize);

			var image = new Bitmap(graphRect.Width, graphRect.Height, PixelFormat.Format32bppRgb);

			using (Graphics graphics = Graphics.FromImage(image))
			{
				graphics.Clear(SystemColors.Window);
				DoPaint(graphics, graphRect);
			}

			Layout.IsSavingToImage = false;

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

			if (!ReadOnly && (e.Button == MouseButtons.Left))
			{
				DragTimer.Tag = e;
				DragTimer.Start();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			base.OnMouseUp(e);

			DragTimer.Stop();
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

					Layout.Font = Font;
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

		protected void Invalidate(NetworkItemHitTestResult hitTest)
		{
			if ((hitTest != null) && hitTest.IsValid)
			{
				if (hitTest.Segment == -1)
				{
					Invalidate(Layout.CalcItemRectangle(hitTest.FromItem));
				}
				else
				{
					Invalidate(Rectangle.Union(Layout.CalcItemRectangle(hitTest.FromItem),
												Layout.CalcItemRectangle(hitTest.ToItem)));
				}
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

		private bool CheckStartDragging(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			// Check for drag movement
			Point ptOrg = (DragTimer.Tag as MouseEventArgs).Location;

			if (GetDragRect(ptOrg).Contains(cursor))
				return false;

			NetworkItem hit = HitTestItem(ptOrg);

			if (IsAcceptableDragSource(hit))
			{
				DoDragDrop(hit.UniqueId, DragDropEffects.Copy | DragDropEffects.Move);
				return true;
			}

			return false;
		}

		virtual protected bool IsAcceptableDropTarget(NetworkDragEventArgs e)
		{
			return e.IsValid;
		}

		virtual protected bool IsAcceptableDragSource(NetworkItem item)
		{
			return (item != null);
		}

		private Rectangle GetDragRect(Point cursor)
		{
			Debug.Assert(!ReadOnly);

			var rect = new Rectangle(cursor.X, cursor.Y, 0, 0);
			rect.Inflate(GetSystemMetrics(SM_CXDRAG) / 2, GetSystemMetrics(SM_CYDRAG) / 2);

			return rect;
		}

		protected override void OnDragLeave(EventArgs e)
		{
			base.OnDragLeave(e);

			DoDragCleanUp();
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			Debug.Assert(!ReadOnly);

			base.OnQueryContinueDrag(e);

			if (e.EscapePressed)
				e.Action = DragAction.Cancel;
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			base.OnDragOver(e);

			var hitTest = DragHitTest(e);

			if (NetworkItemHitTestResult.Match(hitTest, DropPos))
				return;

			if (hitTest != null)
			{
				if ((DropPos == null) || !DropPos.Matches(hitTest))
				{
					Invalidate(DropPos);
					Invalidate(hitTest);

					DropPos = hitTest;
				}
			}
			else
			{
				Invalidate(DropPos);

				DropPos = null;
			}

			NetworkDragEventArgs args = GetNetworkDragEventArgs(e);

			if (IsAcceptableDropTarget(args))
				e.Effect = (args.Copy ? DragDropEffects.Copy : DragDropEffects.Move);
			else
				e.Effect = DragDropEffects.None;

			Update();
		}

		NetworkDragEventArgs GetNetworkDragEventArgs(DragEventArgs e)
		{
			return GetNetworkDragEventArgs(e, DropPos);
		}
		
		NetworkDragEventArgs GetNetworkDragEventArgs(DragEventArgs e, NetworkItemHitTestResult hitTest)
		{
			uint dragId = (uint)e.Data.GetData(typeof(uint));

			return new NetworkDragEventArgs(Data.GetItem(dragId), 
											hitTest, 
											((e.KeyState & 8) == 8));
		}
		
		protected override void OnDragDrop(DragEventArgs e)
		{
			base.OnDragDrop(e);

			var args = GetNetworkDragEventArgs(e);

			if (args.IsValid)
				DoDrop(args);

			DoDragCleanUp();
		}

		protected virtual void DoDragCleanUp()
		{
			if (DropPos != null)
			{
				Invalidate(DropPos);
				DropPos = null;

				Update();
			}
		}

		protected void DrawDragInsertionPoint(Graphics graphics, Point[] points, int segment)
		{
			Point ptMid = Geometry2D.SegmentMidPoint(points, DropPos.Segment);
			Rectangle circle = new Rectangle(ptMid.X - 3, ptMid.Y - 3, 6, 6);
			var state = graphics.Save();

			graphics.SmoothingMode = SmoothingMode.AntiAlias;
			graphics.FillEllipse(new SolidBrush(Color.FromArgb(0x4f, 0x4f, 0x4f)), circle);
			graphics.Restore(state);
		}

		protected virtual bool DoDrop(NetworkDragEventArgs e)
		{
			if (!e.IsValid)
				return false;

			uint sourceId = e.DraggedItem.UniqueId;
			uint targetId = e.DropPos.FromItem.UniqueId;

			if (e.Copy)
			{
				// Add the target item to the source item's dependency list
				if (e.DraggedItem.DependencyUniqueIds.IndexOf(targetId) != -1)
					return false;

				e.DraggedItem.DependencyUniqueIds.Add(targetId);
			}
			else // move
			{
				if (e.DropPos.Segment == -1)
				{
					e.DraggedItem.DependencyUniqueIds.Clear();
					e.DraggedItem.DependencyUniqueIds.Add(targetId);
				}
				else
				{
					// 1. Redirect the dragged item's dependents onto 
					// the first of the dragged item's own dependencies
					var allDependents = new NetworkDependents();
					allDependents.Build(Data.Items);

					var srcDependents = allDependents.GetDependents(sourceId);

					if (srcDependents?.Count > 0)
					{
						uint firstDependencyId = (e.DraggedItem.HasDependencies ? e.DraggedItem.DependencyUniqueIds[0] : 0);

						foreach (var id in srcDependents)
						{
							var localDependent = Data.GetItem(id);

							if (localDependent != null)
							{
								localDependent.DependencyUniqueIds.Remove(sourceId);

								if (firstDependencyId != 0)
									localDependent.DependencyUniqueIds.Add(firstDependencyId);
							}
						}
					}

					// 2. Replace all the source item's dependencies with the target id
					e.DraggedItem.DependencyUniqueIds.Clear();
					e.DraggedItem.DependencyUniqueIds.Add(targetId);

					// 3. Replace e.DropPos.ToItem's dependency on the target id with the source id
					e.DropPos.ToItem.DependencyUniqueIds.Remove(targetId);
					e.DropPos.ToItem.DependencyUniqueIds.Add(sourceId);
				}
			}

			RebuildPaths();
			Invalidate();

			return true;
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

		protected void EnsureItemVisible(NetworkItem item)
		{
			if (item == null)
				return;

			Rectangle itemRect = Layout.CalcItemRectangle(item);

			if (ClientRectangle.Contains(itemRect))
				return;

			if (HorizontalScroll.Visible)
			{
				int xOffset = 0;

				if (itemRect.Left < ClientRectangle.Left)
				{
					xOffset = (itemRect.Left - ClientRectangle.Left - (Layout.ItemHorzSpacing / 2));
				}
				else if (itemRect.Right > ClientRectangle.Right)
				{
					xOffset = (itemRect.Right - ClientRectangle.Right + (Layout.ItemHorzSpacing / 2));
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
					yOffset = (itemRect.Top - ClientRectangle.Top - (Layout.ItemVertSpacing / 2));
				}
				else if (itemRect.Bottom > ClientRectangle.Bottom)
				{
					yOffset = (itemRect.Bottom - ClientRectangle.Bottom + (Layout.ItemVertSpacing / 2));
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
			get { return Math.Max(1, (VerticalScroll.LargeChange / Layout.RowHeight)); }
		}

		protected bool HandleCursorKey(Keys key)
		{
			NetworkItem selItem = SelectedItem;

			if (selItem == null)
				return false;

			var direction = NetworkMatrix.Direction.Left;
			int increment = 0;

			switch (key)
			{
			case Keys.Left:
				direction = NetworkMatrix.Direction.Left;
				increment = 1;
				break;

			case Keys.Right:
				direction = NetworkMatrix.Direction.Right;
				increment = 1;
				break;

			case Keys.Up:
				direction = NetworkMatrix.Direction.Up;
				increment = 1;
				break;

			case Keys.Down:
				direction = NetworkMatrix.Direction.Down;
				increment = 1;
				break;

			case Keys.PageUp:
				direction = NetworkMatrix.Direction.Up;
				increment = PageSize;
				break;

			case Keys.PageDown:
				direction = NetworkMatrix.Direction.Down;
				increment = PageSize;
				break;

			case Keys.Home:
				direction = NetworkMatrix.Direction.Up;
				increment = Data.Matrix.Size.Height;
				break;

			case Keys.End:
				direction = NetworkMatrix.Direction.Down;
				increment = Data.Matrix.Size.Height;
				break;
			}

			if (increment != 0) // sanity check
			{
				NetworkItem nextItem = Data.Matrix.GetNextNearestItem(selItem.Position, direction, increment);

				if ((nextItem != null) && (nextItem != selItem))
				{
					SelectedItemId = nextItem.UniqueId;

					EnsureItemVisible(SelectedItem);
					Invalidate();

					return true;
				}
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
