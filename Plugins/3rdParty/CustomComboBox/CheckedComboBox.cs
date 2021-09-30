using System;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Drawing;
using System.Collections.Generic;

namespace CustomComboBox
{
    public class CheckedComboBox : CustomComboBox
    {
		private CheckedListBox m_ListBox;
		private Point m_LastScreenMouseMove;
		private int[] m_PrevCheckedIndices;

        public CheckedComboBox()
        {
			None = "<none>";

            InitializeComponent();
        }

		private void InitializeComponent()
		{
			m_ListBox = new CheckedListBox();

			m_ListBox.BorderStyle = BorderStyle.None;
			m_ListBox.CheckOnClick = true;
			m_ListBox.IntegralHeight = true;
			m_ListBox.ItemCheck += new ItemCheckEventHandler(OnListItemCheck);
			m_ListBox.MouseMove += new MouseEventHandler(OnListMouseMove);
			m_ListBox.KeyDown += new KeyEventHandler(OnListKeyDown);

			AllowResizeDropDown = false;
			ControlSize = new Size(1, 1);
			DrawMode = DrawMode.OwnerDrawFixed;
			DropDownControl = m_ListBox;
			DropDownSizeMode = CustomComboBox.SizeMode.UseDropDownSize;
			DropDownStyle = ComboBoxStyle.DropDownList;
			FormattingEnabled = true;

			DrawItem += new DrawItemEventHandler(OnDrawComboItem);
			DropDown += new System.EventHandler(OnListDropDown);
			DropDownClosed += new System.EventHandler(OnListCloseUp);
		}

		public String None
		{
			get; set;
		}

		public new Control DropDownControl
		{
			get { return base.DropDownControl; }
			private set { base.DropDownControl = value; }
		}

		protected CheckedListBox ListBox
		{
			get { return m_ListBox; }
		}

		public new ListBox.ObjectCollection Items
		{
			get { return m_ListBox.Items; }
		}

		public CheckedListBox.CheckedItemCollection CheckedItems
		{
			get { return m_ListBox.CheckedItems; }
		}

		protected override void OnFontChanged(EventArgs e)
		{
			base.OnFontChanged(e);

			m_ListBox.Font = this.Font;
		}

		public override Size CalcRequiredDropDownSize()
		{
			var size = base.CalcRequiredDropDownSize();

			if ((DropDownControl != null) && (Items.Count > 0))
			{
				int checkSize = 0;

				if (Application.RenderWithVisualStyles)
				{
					using (Graphics graphics = m_ListBox.CreateGraphics())
						checkSize = (int)(CheckBoxRenderer.GetGlyphSize(graphics, CheckBoxState.CheckedNormal)).Width;
				}
				else
				{
					checkSize = 16; // TODO
				}
				checkSize += 4;

				size.Height = Items.Count * Math.Max(m_ListBox.ItemHeight, checkSize);

				for (int index = 0; index < Items.Count; index++)
				{
					var text = Items[index].ToString();

					if (!string.IsNullOrEmpty(text))
					{
						int itemWidth = TextRenderer.MeasureText(text, m_ListBox.Font).Width;
						size.Width = Math.Max(itemWidth + checkSize, size.Width);
					}
				}
			}

			return size;
		}

		void OnListDropDown(object sender, System.EventArgs e)
		{
			m_LastScreenMouseMove = Cursor.Position;

			m_PrevCheckedIndices = new int[m_ListBox.CheckedIndices.Count];
			m_ListBox.CheckedIndices.CopyTo(m_PrevCheckedIndices, 0);
		}

		void OnListCloseUp(object sender, System.EventArgs e)
		{
			if (Cancelled)
			{
				for (int index = 0; index < Items.Count; index++)
				{
					m_ListBox.SetItemChecked(index, Array.Exists(m_PrevCheckedIndices, item => item == index));
				}
			}

			m_PrevCheckedIndices = null;
		}

		private void OnListItemCheck(object sender, ItemCheckEventArgs e)
		{
			Invalidate();
		}

		private void OnListKeyDown(object sender, KeyEventArgs e)
		{
			if ((e.KeyCode == Keys.Enter) && IsDroppedDown)
			{
				e.Handled = true;
				HideDropDown();
			}
		}

		private void OnListMouseMove(object sender, MouseEventArgs e)
		{
			if (m_LastScreenMouseMove != Cursor.Position)
			{
				int itemHit = m_ListBox.IndexFromPoint(e.Location);

				if (itemHit != -1)
					m_ListBox.SelectedIndex = itemHit;

				m_LastScreenMouseMove = m_ListBox.PointToScreen(e.Location);
			}
		}

		private string FormatItems(string separator)
		{
			String items = "";

			foreach (var checkedItem in m_ListBox.CheckedItems)
			{
				if (items != "")
					items = items + separator;

				items = items + checkedItem.ToString();
			}

			return items;
		}

		private void OnDrawComboItem(object sender, DrawItemEventArgs e)
		{
			if (e.Index == -1)
			{
				String text = FormatItems(", ");
				Color color = SystemColors.WindowText;

				if (text == "")
				{
					text = None;
					color = SystemColors.ControlDarkDark;
				}

				TextRenderer.DrawText(e.Graphics, text, e.Font, new Point(e.Bounds.Top, e.Bounds.Left), color);
			}
		}
	}
}
