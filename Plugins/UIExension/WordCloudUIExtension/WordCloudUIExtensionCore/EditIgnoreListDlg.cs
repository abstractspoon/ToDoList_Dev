using System;
using System.Windows.Forms;
using System.IO;

namespace WordCloudUIExtension
{
	public partial class EditIgnoreListDlg : Form
	{
		protected EditIgnoreListDlg(string filePath)
		{
			InitializeComponent();

			IgnoredWords.Text = File.ReadAllText(filePath);
			IgnoredWords.SelectionStart = IgnoredWords.TextLength;
		}

		public static bool DoEdit(string filePath)
		{
			var dlg = new EditIgnoreListDlg(filePath);

			if (dlg.ShowDialog() != DialogResult.OK)
				return false;

			File.WriteAllText(filePath, dlg.IgnoredWords.Text);
			return true;
		}
	}
}
