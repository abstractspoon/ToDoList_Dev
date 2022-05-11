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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace unvell.ReoGrid.WinForm
{
	/// <summary>
	/// Represents the sheet rename dialog
	/// </summary>
	public partial class RenameSheetDialog : Form
	{
		/// <summary>
		/// Name of sheet
		/// </summary>
		public string SheetName { get; set; }

		/// <summary>
		/// Create dialog
		/// </summary>
		public RenameSheetDialog()
		{
			InitializeComponent();

			this.newName.KeyDown += (s, e) =>
			{
				if (e.KeyCode == Keys.Enter)
				{
					btnOK.PerformClick();
				}
			};
		}

		/// <summary>
		/// Event when dialog was loaded
		/// </summary>
		/// <param name="e"></param>
		protected override void OnLoad(EventArgs e)
		{
			base.OnLoad(e);

			this.oldName.Text = this.SheetName;
			this.newName.Text = this.SheetName;
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			this.SheetName = this.newName.Text;

			Close();
		}

	}
}

