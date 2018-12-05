namespace ZetaHtmlEditControl
{
	using System.Drawing;
	using System.Reflection;
	using System.Windows.Forms;

	public class MyToolStripRender :
		ToolStripProfessionalRenderer
	{
		protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
		{
			var mi = e.Item as ToolStripMenuItem;
			if (mi != null)
			{
				e.TextColor = e.Text == getShortcutText(mi)
								? SystemColors.GrayText
								: SystemColors.MenuText;
			}

			base.OnRenderItemText(e);
		}

		private string getShortcutText(ToolStripMenuItem mi)
		{
			var t= (string)mi.GetType().InvokeMember(
				@"GetShortcutText",
				BindingFlags.InvokeMethod | BindingFlags.NonPublic | BindingFlags.Instance,
				null,
				mi,
				new object[] { });

			return t;
		}
	}
}