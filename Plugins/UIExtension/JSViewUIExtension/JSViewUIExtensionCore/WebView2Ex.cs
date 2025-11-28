using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

// -----------------------------------------------------------

namespace JSViewUIExtension
{
	class WebView2Ex : Microsoft.Web.WebView2.WinForms.WebView2
	{
		// -----------------------------------------------------------

		public const uint GW_CHILD = 5;

		[DllImport("user32.dll")]
		public static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

		[DllImport("user32.dll")]
		public static extern IntPtr SetFocus(IntPtr hWnd);

		// -----------------------------------------------------------

		// This fixes a bug in pre-release versions of WebView2
		protected override void OnGotFocus(EventArgs e)
		{
			base.OnGotFocus(e);

			var child = GetWindow(Handle, GW_CHILD);
			SetFocus(child);
		}
	}
}
