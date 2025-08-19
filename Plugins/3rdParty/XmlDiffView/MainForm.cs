using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Xml;
using Microsoft.XmlDiffPatch;
using System.Windows.Forms.Html;

namespace VisualXmlDiff
{
	/// <summary>
	/// Summary description for VisualXmlDiff.
	/// This class draws the main form and then does comparison of two files.
	/// </summary>
	public class VisualXmlDiff : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem mnuOptions;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private IContainer components;

		public string filename1 = string.Empty;
		public string filename2 = string.Empty;
		string diffFile = null;

		//The main class which is used to compare two files.
		XmlDiff diff = new XmlDiff();
		XmlDiffOptions diffOptions = new XmlDiffOptions();
		bool compareFragments = false;

		public System.Windows.Forms.MenuItem icoOpt;
		private System.Windows.Forms.MenuItem ipiOpt;
		private System.Windows.Forms.MenuItem icOpt;
		private System.Windows.Forms.MenuItem ixdOpt;
		private System.Windows.Forms.MenuItem iwsOpt;
		private System.Windows.Forms.MenuItem idtdOpt;
		private System.Windows.Forms.MenuItem inOpt;
		private System.Windows.Forms.MenuItem ipOpt;
		private System.Windows.Forms.MenuItem fOpt;
		private System.Windows.Forms.MenuItem algAuto;
		private System.Windows.Forms.MenuItem algFast;
		private System.Windows.Forms.MenuItem algOptions;
		private Label label1;
		private Label label2;
		private Button button4;
		private MenuItem about1;
		private System.Windows.Forms.MenuItem algPrecise;

		public VisualXmlDiff()
		{
			Application.EnableVisualStyles();

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		private static void ShowUsage()
		{
			Console.WriteLine("xmldiffview [filename1] [filename2]");
		}
		[STAThread]
		public static void Main(string[] args)
		{
			if ( args.Length > 0 )
			{
				if ( args.Length != 2 )
				{
					ShowUsage();
					return;
				}
				VisualXmlDiff vxd = new VisualXmlDiff();
				vxd.filename1 = args[0];
				vxd.filename2 = args[1];
				Application.Run( vxd );
			}
			else
			{
				VisualXmlDiff vxd = new VisualXmlDiff();

                Application.EnableVisualStyles();
				Application.Run(vxd);
			}
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
			this.mnuOptions = new System.Windows.Forms.MenuItem();
			this.icoOpt = new System.Windows.Forms.MenuItem();
			this.ipiOpt = new System.Windows.Forms.MenuItem();
			this.icOpt = new System.Windows.Forms.MenuItem();
			this.ixdOpt = new System.Windows.Forms.MenuItem();
			this.iwsOpt = new System.Windows.Forms.MenuItem();
			this.idtdOpt = new System.Windows.Forms.MenuItem();
			this.inOpt = new System.Windows.Forms.MenuItem();
			this.ipOpt = new System.Windows.Forms.MenuItem();
			this.fOpt = new System.Windows.Forms.MenuItem();
			this.algOptions = new System.Windows.Forms.MenuItem();
			this.algAuto = new System.Windows.Forms.MenuItem();
			this.algPrecise = new System.Windows.Forms.MenuItem();
			this.algFast = new System.Windows.Forms.MenuItem();
			this.button3 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.button4 = new System.Windows.Forms.Button();
			this.about1 = new System.Windows.Forms.MenuItem();
			this.SuspendLayout();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.mnuOptions,
            this.about1});
			// 
			// mnuOptions
			// 
			this.mnuOptions.Index = 0;
			this.mnuOptions.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.icoOpt,
            this.ipiOpt,
            this.icOpt,
            this.ixdOpt,
            this.iwsOpt,
            this.idtdOpt,
            this.inOpt,
            this.ipOpt,
            this.fOpt,
            this.algOptions});
			this.mnuOptions.Text = "Options";
			// 
			// icoOpt
			// 
			this.icoOpt.Checked = true;
			this.icoOpt.Index = 0;
			this.icoOpt.Text = "Ignore Child Order";
			this.icoOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// ipiOpt
			// 
			this.ipiOpt.Index = 1;
			this.ipiOpt.Text = "Ignore Processing Instructions";
			this.ipiOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// icOpt
			// 
			this.icOpt.Checked = true;
			this.icOpt.Index = 2;
			this.icOpt.Text = "Ignore Comments";
			this.icOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// ixdOpt
			// 
			this.ixdOpt.Checked = true;
			this.ixdOpt.Index = 3;
			this.ixdOpt.Text = "Ignore Xml Declaration";
			this.ixdOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// iwsOpt
			// 
			this.iwsOpt.Checked = true;
			this.iwsOpt.Index = 4;
			this.iwsOpt.Text = "Ignore Whitespaces";
			this.iwsOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// idtdOpt
			// 
			this.idtdOpt.Checked = true;
			this.idtdOpt.Index = 5;
			this.idtdOpt.Text = "Ignore Document Type Declaration (DTD)";
			this.idtdOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// inOpt
			// 
			this.inOpt.Index = 6;
			this.inOpt.Text = "Ignore Namespaces";
			this.inOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// ipOpt
			// 
			this.ipOpt.Index = 7;
			this.ipOpt.Text = "Ignore Prefixes";
			this.ipOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// fOpt
			// 
			this.fOpt.Index = 8;
			this.fOpt.Text = "Compare Fragments";
			this.fOpt.Click += new System.EventHandler(this.diffOptions_Click);
			// 
			// algOptions
			// 
			this.algOptions.Index = 9;
			this.algOptions.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.algAuto,
            this.algPrecise,
            this.algFast});
			this.algOptions.Text = "Algorithm";
			// 
			// algAuto
			// 
			this.algAuto.Checked = true;
			this.algAuto.Index = 0;
			this.algAuto.RadioCheck = true;
			this.algAuto.Text = "Auto";
			this.algAuto.Click += new System.EventHandler(this.algOptions_Click);
			// 
			// algPrecise
			// 
			this.algPrecise.Index = 1;
			this.algPrecise.RadioCheck = true;
			this.algPrecise.Text = "Greedy";
			this.algPrecise.Click += new System.EventHandler(this.algOptions_Click);
			// 
			// algFast
			// 
			this.algFast.Index = 2;
			this.algFast.RadioCheck = true;
			this.algFast.Text = "Fast";
			this.algFast.Click += new System.EventHandler(this.algOptions_Click);
			// 
			// button3
			// 
			this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.button3.Location = new System.Drawing.Point(180, 70);
			this.button3.Margin = new System.Windows.Forms.Padding(2);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(74, 23);
			this.button3.TabIndex = 7;
			this.button3.Text = "Compare";
			this.button3.Click += new System.EventHandler(this.Compare_Click);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(307, 33);
			this.button2.Margin = new System.Windows.Forms.Padding(2);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(27, 23);
			this.button2.TabIndex = 6;
			this.button2.Text = "...";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button1
			// 
			this.button1.AutoSize = true;
			this.button1.Location = new System.Drawing.Point(307, 8);
			this.button1.Margin = new System.Windows.Forms.Padding(2);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(27, 23);
			this.button1.TabIndex = 3;
			this.button1.Text = "...";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// textBox1
			// 
			this.textBox1.AllowDrop = true;
			this.textBox1.Location = new System.Drawing.Point(44, 10);
			this.textBox1.Margin = new System.Windows.Forms.Padding(2);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(259, 20);
			this.textBox1.TabIndex = 2;
			this.textBox1.DragDrop += new System.Windows.Forms.DragEventHandler(this.textBox1_DragDrop);
			this.textBox1.DragEnter += new System.Windows.Forms.DragEventHandler(this.textBox1_DragEnter);
			// 
			// textBox2
			// 
			this.textBox2.AllowDrop = true;
			this.textBox2.Location = new System.Drawing.Point(44, 35);
			this.textBox2.Margin = new System.Windows.Forms.Padding(2);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(259, 20);
			this.textBox2.TabIndex = 5;
			this.textBox2.DragDrop += new System.Windows.Forms.DragEventHandler(this.textBox2_DragDrop);
			this.textBox2.DragEnter += new System.Windows.Forms.DragEventHandler(this.textBox2_DragEnter);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(7, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(32, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "File 1";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(7, 38);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(32, 13);
			this.label2.TabIndex = 4;
			this.label2.Text = "File 2";
			// 
			// button4
			// 
			this.button4.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.button4.Location = new System.Drawing.Point(259, 70);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(75, 23);
			this.button4.TabIndex = 8;
			this.button4.Text = "Cancel";
			this.button4.UseVisualStyleBackColor = true;
			this.button4.Click += new System.EventHandler(this.button4_Click);
			// 
			// about1
			// 
			this.about1.Index = 1;
			this.about1.Text = "About";
			this.about1.Click += new System.EventHandler(this.about1_Click);
			// 
			// VisualXmlDiff
			// 
			this.AcceptButton = this.button3;
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.CancelButton = this.button4;
			this.ClientSize = new System.Drawing.Size(344, 100);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button4);
			this.Icon = global::VisualXmlDiff.Resource.XmlDiffView;
			this.Margin = new System.Windows.Forms.Padding(2);
			this.Menu = this.mainMenu1;
			this.Name = "VisualXmlDiff";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Xml File Comparison";
			this.Load += new System.EventHandler(this.VisualXmlDiff_Load);
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
			this.ResumeLayout(false);
			this.PerformLayout();

		}
		#endregion

		private void menuItem14_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		string GetInitialBrowseDirectory(string filename, string otherFilename)
		{
			if (!string.IsNullOrWhiteSpace(filename))
				return Path.GetDirectoryName(filename);

			if (!string.IsNullOrWhiteSpace(otherFilename))
				return Path.GetDirectoryName(otherFilename);

			return string.Empty;
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog ofd1 = new OpenFileDialog()
			{
				FileName = textBox1.Text,
				InitialDirectory = GetInitialBrowseDirectory(textBox1.Text, textBox2.Text)
			};

			if (ofd1.ShowDialog() == DialogResult.OK)
				textBox1.Text = ofd1.FileName;
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog ofd1 = new OpenFileDialog()
			{
				FileName = textBox2.Text,
				InitialDirectory = GetInitialBrowseDirectory(textBox2.Text, textBox1.Text)
			};

			if (ofd1.ShowDialog() == DialogResult.OK)
				textBox2.Text = ofd1.FileName;
		}

		
		/// <summary>
		/// This method basically is invoked when user clicks the 
		/// Compare button.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Compare_Click(object sender, System.EventArgs e)
		{
			//Check if file 1 is safe and valid.
			if ( textBox1.Text == null || textBox1.Text == string.Empty )
			{
				MessageBox.Show("File 1 not specified", "Error");
				return;
			}
			
			if ( !File.Exists ( textBox1.Text ) )
			{
				MessageBox.Show("File 1 doesn't exist", "Error");
				return;
			}

			//Check if file 2 is safe and valid.
			if ( textBox2.Text == null || textBox2.Text == string.Empty )
			{
				MessageBox.Show("File 2 not specified", "Error");
				return;
			}

			if ( !File.Exists ( textBox2.Text ) )
			{
				MessageBox.Show("File 2 doesn't exist", "Error");
				return;
			}

			//If you need code to compare two directories, this would 
			//be the place to write that code.

			//Call DoCompare which will compare two files.
			Cursor = Cursors.WaitCursor;

			DoCompare( textBox1.Text, textBox2.Text );

			Cursor = Cursors.Default;
		}

		public void DoCompare(string file1, string file2)
		{
			Random r = new Random(); 
			//to randomize the output files and hence allow 
			//us to generate multiple files for the same pair 
			//of comparisons.
			
			string startupPath = Application.StartupPath;
			//output diff file.
			diffFile = startupPath + Path.DirectorySeparatorChar + "vxd.out"; 
			XmlTextWriter tw = new XmlTextWriter( new StreamWriter( diffFile ) );
			tw.Formatting = Formatting.Indented;

			//This method sets the diff.Options property.
			SetDiffOptions();

			bool isEqual = false;
			
			//Now compare the two files.
			try
			{
				isEqual = diff.Compare( file1, file2, compareFragments, tw);
			}
			catch ( XmlException xe )
			{
				MessageBox.Show( "An exception occurred while comparing\n" + xe.StackTrace, "Error" );
			}
			finally
			{
				tw.Close();
			}

			if (  isEqual )
			{
				//This means the files were identical for given options.
				MessageBox.Show ( "Files Identical for the given options", "Information");
				return; //dont need to show the differences.
			}

			//Files were not equal, so construct XmlDiffView.
			XmlDiffView dv = new XmlDiffView();
			
			//Load the original file again and the diff file.
			XmlTextReader orig = new XmlTextReader( file1 );
			XmlTextReader diffGram = new XmlTextReader( diffFile );
			dv.Load( orig, 
				diffGram );

			//Wrap the HTML file with necessary html and 
			//body tags and prepare it before passing it to the GetHtml method.

//			string tempFile = startupPath + Path.DirectorySeparatorChar + "diff" + r.Next() + ".htm";
            string tempFile = Path.GetTempPath() + "diff" + r.Next() + ".htm";
			StreamWriter sw1 = new StreamWriter( tempFile, false, System.Text.Encoding.Unicode );


            sw1.Write("<html><style> table { table-layout: fixed; font-family:verdana; font-size:12; } </style>");
            sw1.Write("<body><table width='100%'>");
            //Write Legend.
			sw1.Write("<tr><td colspan='2' align='center'><b>Legend:</b> <font style='background-color: yellow'" + 
				" color='black'>added</font>&nbsp;&nbsp;<font style='background-color: red'"+
				" color='black'>removed</font>&nbsp;&nbsp;<font style='background-color: "+
				"lightgreen' color='black'>changed</font>&nbsp;&nbsp;"+
				"<font style='background-color: red' color='blue'>moved from</font>"+
				"&nbsp;&nbsp;<font style='background-color: yellow' color='blue'>moved to"+
				"</font>&nbsp;&nbsp;<font style='background-color: white' color='#AAAAAA'>"+
				"ignored</font></td></tr>");


			sw1.Write("<tr><td><b> File Name : ");
			sw1.Write(textBox1.Text);
			sw1.Write("</b></td><td><b> File Name : ");
			sw1.Write(textBox2.Text);
			sw1.Write("</b></td></tr>");

			//This gets the differences but just has the 
			//rows and columns of an HTML table
			dv.GetHtml( sw1 );
			
			//Finish wrapping up the generated HTML and complete the file.
			sw1.Write("</table></body></html>");
			
			//HouseKeeping...close everything we dont want to lock.
			sw1.Close();
			dv = null;
			orig.Close();
			diffGram.Close();
			File.Delete ( diffFile );

			//Open the IE Control window and pass it the HTML file we created.
			Browser b = new Browser(tempFile);
			b.Show(); //Display it!

			//Done!
		}

		/// <summary>
		/// This method reads the diff options set on the 
		/// menu and configures the XmlDiffOptions object.
		/// </summary>
		private void SetDiffOptions()
		{
			//Reset to None and refresh the options from the menuoptions
			//else eventually all options may get set and the menu changes will
			// not be reflected.
			diffOptions = XmlDiffOptions.None;


			//Read the options settings and OR the XmlDiffOptions enumeration.
			if (ipiOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnorePI;

			if (icoOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreChildOrder;

			if (icOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreComments;
			
			if (idtdOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreDtd;
			
			if (inOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreNamespaces;
			
			if (ipOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnorePrefixes;
			
			if (iwsOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreWhitespace;
			
			if (ixdOpt.Checked)
				diffOptions = diffOptions | XmlDiffOptions.IgnoreXmlDecl;

			if (fOpt.Checked)
				compareFragments = true;

			//Default algorithm is Auto.
			diff.Algorithm = XmlDiffAlgorithm.Auto;

			if ( algFast.Checked )
				diff.Algorithm = XmlDiffAlgorithm.Fast;
			
			if ( algPrecise.Checked )
				diff.Algorithm = XmlDiffAlgorithm.Precise;

			diff.Options = diffOptions;
		}

		//Handler for Diff Options for Ignore Settings.
		private void diffOptions_Click(object sender, System.EventArgs e)
		{
			MenuItem item = (MenuItem) sender;
			item.Checked = !item.Checked;
		}

		//Handler for Diff Options for Algorithm setting.
		private void algOptions_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.Menu.MenuItemCollection items = algOptions.MenuItems;
			foreach (MenuItem i in items)
				i.Checked = false;
			MenuItem item = (MenuItem) sender;
			item.Checked = true;
		}

		private void VisualXmlDiff_Load(object sender, System.EventArgs e)
		{
			textBox1.Text = filename1;
			textBox2.Text = filename2;

			if ( filename1 != string.Empty && filename2 != string.Empty )
				DoCompare( filename1, filename2 );
		}

		private void textBox1_DragEnter(object sender, DragEventArgs e)
		{
			var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

			if (filePaths.Length <= 2)
				e.Effect = DragDropEffects.Copy;
		}

		private void textBox2_DragEnter(object sender, DragEventArgs e)
		{
			var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

			if (filePaths.Length <= 2)
				e.Effect = DragDropEffects.Copy;
		}

		private void textBox1_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

				if (filePaths.Length == 1)
				{
					textBox1.Text = filePaths[0];
				}
				else
				{
					textBox1.Text = filePaths[0];
					textBox2.Text = filePaths[1];
				}
			}
		}

		private void textBox2_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

				if (filePaths.Length == 1)
				{
					textBox2.Text = filePaths[0];
				}
				else
				{
					textBox1.Text = filePaths[0];
					textBox2.Text = filePaths[1];
				}
			}
		}

		private void button4_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void MainForm_DragEnter(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

				if (filePaths.Length == 2)
					e.Effect = DragDropEffects.Copy;
			}
		}

		private void MainForm_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				var filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));

				textBox1.Text = filePaths[0];
				textBox2.Text = filePaths[1];
			}
		}

		private void about1_Click(object sender, EventArgs e)
		{
			MessageBox.Show("XmlDiffView\n\nMicrosoft Corp. Copyright 2004", "About");
		}
	}
}
