#region Using directives

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

#endregion

namespace MSDN.Html.Editor
{

	/// <summary>
	/// Form used to enter an Html Image attribute
	/// Consists of Href, Text and Image Alignment
	/// </summary>
	public partial class EnterImageForm : Form
	{
		private static string ImageFilter = "Image Files (*.png, *.bmp, *.ico, *.jpg, *.jpeg, *.tiff, *.gif)|*.png;*.bmp;*.ico;*.jpg;*.jpeg;*.tiff;*.gif||";


		/// <summary>
		/// Public form constructor
		/// </summary>
		public EnterImageForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// define the text for the alignment
			this.listAlign.Items.AddRange(Enum.GetNames(typeof(ImageAlignOption)));

			// ensure default value set for target
			this.listAlign.SelectedIndex = (int)ImageAlignOption.Default;

			BrowseTitle = "Select Image File";
			BrowseFilter = ImageFilter;
		} 

		public ToolTip Tooltip
		{
			get { return this.toolTip1; }
		}

		public String BrowseTitle
		{
			get; set;
		}

		public String BrowseFilter
		{
			get; set;
		}

		/// <summary>
		/// Property for the text to display
		/// </summary>
		public string ImageText
		{
			get
			{
				return this.hrefText.Text;
			}
			set
			{
				this.hrefText.Text = value;
			}

		} //ImageText

		/// <summary>
		/// Property for the href for the image
		/// </summary>
		public string ImageLink
		{
			get
			{
				return this.hrefLink.Text.Trim();
			}
			set
			{
				this.hrefLink.Text = value.Trim();
			}

		} //ImageLink

		/// <summary>
		/// Property for the image align
		/// </summary>
		public ImageAlignOption ImageAlign
		{
			get
			{
				return (ImageAlignOption)this.listAlign.SelectedIndex;
			}
			set
			{
				this.listAlign.SelectedIndex = (int)value;
			}
		}

		public String LastBrowsedFolder { get; set; }

		public bool EnableHrefText
		{
			set { hrefText.Enabled = value; }
		}

		public bool EnableAlignment
		{
			set { listAlign.Enabled = value; }
		}

		public bool EnableImageWidth
		{
			set
			{
				enablePixelWidth.Enabled = value;
				pixelWidth.Enabled = (value && enablePixelWidth.Checked);
			}
		}

		public int ImageWidth
		{
			get
			{
				if (pixelWidth.Enabled && enablePixelWidth.Checked)
					return Math.Max(1, (int)pixelWidth.Value);

				//else
				return -1;
			}

			set
			{
				if (value <= 0)
				{
					enablePixelWidth.Checked = false;
					pixelWidth.Enabled = false;
				}
				else
				{
					enablePixelWidth.Checked = true;
					pixelWidth.Enabled = true;
					pixelWidth.Value = value;
				}
			}
		} 

		public static bool IsImagePath(string path)
		{
			try
			{
				string extension = System.IO.Path.GetExtension(path);

				if (String.IsNullOrEmpty(extension))
					return false;

				string filter = String.Format("*{0};", extension);

				return ImageFilter.Contains(filter);
			}
			catch (Exception /*e*/)
			{
				return false;
			}
		}

		private void fileBrowseBtn_Click(object sender, EventArgs e)
		{
			var dlg = new OpenFileDialog
			{
				InitialDirectory = LastBrowsedFolder,
				Title = BrowseTitle,

				AutoUpgradeEnabled = true,
				CheckFileExists = true,
				CheckPathExists = true,

				Filter = BrowseFilter,
				FilterIndex = 0,
				RestoreDirectory = true,

				ShowReadOnly = false
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				// Format filePath as file URI
				var uri = new System.Uri(dlg.FileName);
				ImageLink = uri.AbsoluteUri;

				LastBrowsedFolder = System.IO.Path.GetDirectoryName(dlg.FileName);
			}
		}

		protected override void OnLoad(EventArgs e)
		{
			base.OnLoad(e);

			if (!String.IsNullOrEmpty(ImageLink))
				hrefText.Select();
		}

		private void OnPixelWidthCheckChange(object sender, EventArgs e)
		{
			pixelWidth.Enabled = (enablePixelWidth.Checked && pixelWidth.Enabled);
		}
	}
}
