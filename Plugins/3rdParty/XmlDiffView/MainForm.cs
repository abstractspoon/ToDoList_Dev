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
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.MenuItem mnuOptions;
		private System.Windows.Forms.MenuItem menuItem14;
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
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem algAuto;
		private System.Windows.Forms.MenuItem algFast;
		private System.Windows.Forms.MenuItem algOptions;
		private System.Windows.Forms.MenuItem algPrecise;

		public VisualXmlDiff()
		{

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
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.menuItem14 = new System.Windows.Forms.MenuItem();
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
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.button3 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.mnuFile,
            this.mnuOptions,
            this.menuItem1});
			// 
			// mnuFile
			// 
			this.mnuFile.Index = 0;
			this.mnuFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem14});
			this.mnuFile.Text = "File";
			// 
			// menuItem14
			// 
			this.menuItem14.Index = 0;
			this.menuItem14.Text = "Exit";
			this.menuItem14.Click += new System.EventHandler(this.menuItem14_Click);
			// 
			// mnuOptions
			// 
			this.mnuOptions.Index = 1;
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
			this.mnuOptions.Text = "Diff Options";
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
			// menuItem1
			// 
			this.menuItem1.Index = 2;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem2});
			this.menuItem1.Text = "Help";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 0;
			this.menuItem2.Text = "About";
			this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
			// 
			// button3
			// 
			this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.button3.Location = new System.Drawing.Point(351, 109);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(157, 35);
			this.button3.TabIndex = 6;
			this.button3.Text = "Compare";
			this.button3.Click += new System.EventHandler(this.Compare_Click);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(13, 58);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(166, 36);
			this.button2.TabIndex = 5;
			this.button2.Text = "Compare With ...";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(13, 12);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(166, 35);
			this.button1.TabIndex = 4;
			this.button1.Text = "Original File  ...";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(192, 15);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(640, 26);
			this.textBox1.TabIndex = 8;
			// 
			// textBox2
			// 
			this.textBox2.Location = new System.Drawing.Point(192, 61);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(640, 26);
			this.textBox2.TabIndex = 9;
			// 
			// VisualXmlDiff
			// 
			this.AcceptButton = this.button3;
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(850, 156);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.Menu = this.mainMenu1;
			this.Name = "VisualXmlDiff";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "XmlDiff";
			this.Load += new System.EventHandler(this.VisualXmlDiff_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}
		#endregion

		private void menuItem14_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog ofd1 = new OpenFileDialog();
			ofd1.ShowDialog();
			textBox1.Text = ofd1.FileName;
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog ofd1 = new OpenFileDialog();
			ofd1.ShowDialog();
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
				MessageBox.Show("File 1 not selected, please select");
				return;
			}
			
			if ( !File.Exists ( textBox1.Text ) )
			{
				MessageBox.Show("File 1 doesn't exist, please select another file");
				return;
			}

			//Check if file 2 is safe and valid.
			if ( textBox2.Text == null || textBox2.Text == string.Empty )
			{
				MessageBox.Show("File 2 not selected, please select");
				return;
			}

			if ( !File.Exists ( textBox2.Text ) )
			{
				MessageBox.Show("File 2 doesn't exist, please select another file");
				return;
			}

			//If you need code to compare two directories, this would 
			//be the place to write that code.

			//Call DoCompare which will compare two files.
			DoCompare ( textBox1.Text, textBox2.Text );

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
				MessageBox.Show( "An exception occurred while comparing\n" + xe.StackTrace );
			}
			finally
			{
				tw.Close();
			}

			if (  isEqual )
			{
				//This means the files were identical for given options.
				MessageBox.Show ( "Files Identical for the given options");
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


            sw1.Write("<html><style> table { table-layout: fixed; } </style>");
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

		private void menuItem2_Click(object sender, System.EventArgs e)
		{
			//MessageBox.Show("Microsoft Corp.\n" + "Copyright 2004");
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
	}
}
