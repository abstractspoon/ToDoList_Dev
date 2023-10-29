/*****************************************************************************
 * 
 * ReoScript - .NET Script Language Engine
 * 
 * https://github.com/unvell/ReoScript
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * This software released under MIT license.
 * Copyright (c) 2012-2019 Jingwood, unvell.com, all rights reserved.
 *
 * 
 * This control base on FastColoredTextBox 
 * http://www.codeproject.com/Articles/161871/Fast-Colored-TextBox-for-syntax-highlighting
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * License: GNU Lesser General Public License (LGPLv3)
 *
 * Email: pavel_torgashov@mail.ru.
 *
 * Copyright (C) Pavel Torgashov, 2011-2013. 
 *  
 ****************************************************************************/

using System;
using System.Windows.Forms;
using System.Threading;

using FastColoredTextBoxNS;

namespace unvell.ReoScript.Editor
{
	public partial class FctbConsoleControl : UserControl
	{
		public FctbConsoleControl()
		{
			InitializeComponent();

			fctb.LineReceived += (s, e) => LineReceived?.Invoke(s, e);
		}

		public void OutLine(string text)
		{
			if (fctb.IsDisposed) return;

			fctb.Invoke((MethodInvoker)(() =>
			{
				fctb.Appending = true;
				if (!text.EndsWith("\n")) text += "\n";
				fctb.AppendText(text);
				fctb.GoEnd();
				fctb.Appending = false;
			}));
		}

		private void clearToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Clear();
		}

		public void Clear()
		{
			fctb.Clear();
		}

		public event EventHandler<LineReceivedEventArgs> LineReceived;
	}

	public class MyConsoleTextBox : FastColoredTextBox
	{
		public bool Appending { get; set; }

		public MyConsoleTextBox()
		{

		}

		public string LastInputted { get; set; }

		private Place StartReadPlace { get; set; }

		public override void OnTextChanging(ref string text)
		{
			if (Selection.Start < StartReadPlace || Selection.End < StartReadPlace)
				GoEnd();//move caret to entering position

			if (Selection.Start == StartReadPlace || Selection.End == StartReadPlace)
			{
				if (text == "\b") //backspace
				{
					text = ""; //cancel deleting of last char of readonly text
					return;
				}
			}

			if (!Appending)
			{
				if (text != null && text.Contains("\n"))
				{
					text = text.Substring(0, text.IndexOf('\n') + 1);


					LastInputted = this.GetLineText(this.LinesCount - 1);
					if (LineReceived != null) LineReceived(this, new LineReceivedEventArgs(LastInputted));
				}
			}

			base.OnTextChanging(ref text);
		}

		public event EventHandler<LineReceivedEventArgs> LineReceived;

		protected override void OnKeyDown(KeyEventArgs e)
		{
			base.OnKeyDown(e);

			if (e.KeyCode == Keys.Up)
			{
				Appending = true;
				AppendText(LastInputted);
				Appending = false;
				GoEnd();
			}
			else if (e.KeyCode == Keys.Escape)
			{
				GoEnd();
			}
		}
	}

	public class LineReceivedEventArgs : EventArgs
	{
		public string Text { get; set; }
		public LineReceivedEventArgs(string text)
		{
			this.Text = text;
		}
	}
	/// <summary>
	/// Console emulator.
	/// </summary>
	public class ConsoleTextBox : FastColoredTextBox
	{
		private volatile bool isReadLineMode;
		private volatile bool isUpdating;
		private Place StartReadPlace { get; set; }

		/// <summary>
		/// Control is waiting for line entering. 
		/// </summary>
		public bool IsReadLineMode
		{
			get { return isReadLineMode; }
			set { isReadLineMode = value; }
		}

		/// <summary>
		/// Append line to end of text.
		/// </summary>
		/// <param name="text"></param>
		public void WriteLine(string text)
		{
			IsReadLineMode = false;
			isUpdating = true;
			try
			{
				AppendText(text);
				GoEnd();
			}
			finally
			{
				isUpdating = false;
				ClearUndo();
			}
		}

		/// <summary>
		/// Wait for line entering.
		/// Set IsReadLineMode to false for break of waiting.
		/// </summary>
		/// <returns></returns>
		public string ReadLine()
		{
			GoEnd();
			StartReadPlace = Range.End;
			IsReadLineMode = true;
			try
			{
				while (IsReadLineMode)
				{
					Application.DoEvents();
					Thread.Sleep(5);
				}
			}
			finally
			{
				IsReadLineMode = false;
				ClearUndo();
			}

			return new Range(this, StartReadPlace, Range.End).Text.TrimEnd('\r', '\n');
		}

		public override void OnTextChanging(ref string text)
		{
			if (!IsReadLineMode && !isUpdating)
			{
				text = ""; //cancel changing
				return;
			}

			if (IsReadLineMode)
			{
				if (Selection.Start < StartReadPlace || Selection.End < StartReadPlace)
					GoEnd();//move caret to entering position

				if (Selection.Start == StartReadPlace || Selection.End == StartReadPlace)
					if (text == "\b") //backspace
					{
						text = ""; //cancel deleting of last char of readonly text
						return;
					}

				if (text != null && text.Contains("\n"))
				{
					text = text.Substring(0, text.IndexOf('\n') + 1);
					IsReadLineMode = false;
				}
			}

			base.OnTextChanging(ref text);
		}
	}
}
