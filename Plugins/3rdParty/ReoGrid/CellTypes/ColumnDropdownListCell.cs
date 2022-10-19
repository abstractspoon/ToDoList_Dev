/*****************************************************************************
 * 
 * ReoGrid - .NET Spreadsheet Control
 * 
 * http://reogrid.net/
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * Author: Jing <lujing at unvell.com>
 *
 * Copyright (c) 2012-2016 Jing <lujing at unvell.com>
 * Copyright (c) 2012-2016 unvell.com, all rights reserved.
 * 
 ****************************************************************************/

using System;
using System.Collections.Generic;
using System.Reflection;
using System.Linq;

using System.Windows.Forms;
using RGFloat = System.Single;
using RGImage = System.Drawing.Image;

namespace unvell.ReoGrid.CellTypes
{

	/// <summary>
	/// Represents dropdown list cell for entire column.
	/// </summary>
	public class ColumnDropdownListCell : DropdownCell
	{
		/// <summary>
		/// Listbox component instance.
		/// </summary>
		protected static ListBox listBox;

		/// <summary>
		/// Push down the dropdown panel.
		/// </summary>
		public override void PushDown()
		{
			if (ColumnDropdownListCell.listBox == null)
			{
				ColumnDropdownListCell.listBox = new ListBox
				{
					BorderStyle = System.Windows.Forms.BorderStyle.None,
				};

				base.DropdownControl = ColumnDropdownListCell.listBox;
			}

			ColumnDropdownListCell.listBox.Click += listBox_Click;

			base.PushDown();
		}

		/// <summary>
		/// Push up the dropdown panel.
		/// </summary>
		public override void PullUp()
		{
			if (ColumnDropdownListCell.listBox != null)
			{
				ColumnDropdownListCell.listBox.Click -= listBox_Click;
			}

			base.PullUp();
		}

		void listBox_Click(object sender, EventArgs e)
		{
			SelectedItem = ColumnDropdownListCell.listBox.SelectedItem;

			this.PullUp();
		}
	}

}
