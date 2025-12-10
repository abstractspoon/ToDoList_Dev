using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

// -----------------------------------------------------------

namespace JSViewUIExtension
{
	class WebView2Ex : Microsoft.Web.WebView2.WinForms.WebView2
	{
		// -----------------------------------------------------------

		const uint GW_CHILD = 5;

		[DllImport("user32.dll")]
		static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

		[DllImport("user32.dll")]
		static extern IntPtr SetFocus(IntPtr hWnd);

		// -----------------------------------------------------------

		public JObject CallJsFunction(string fnNameAndArgs)
		{
			return CallJsFunction<JObject>(fnNameAndArgs);
		}

		public T CallJsFunction<T>(string fnNameAndArgs)
		{
			if (CoreWebView2 != null)
			{
				var task = ExecuteScriptAsync(fnNameAndArgs);
				int nTries = 100;

				while (!task.IsCompleted && !task.IsCanceled && (nTries-- > 0))
				{
					Application.DoEvents();
					Thread.Sleep(10);
				}

				if (nTries > 0)
				{
					try
					{
						// Because ExecuteScriptAsync calls JSON.stringify on whatever
						// string is returned, if the result itself was created with 
						// JSON.stringify, then we end up with doubly-encoded quotes.
						// So we take these precautionary measures.
						var json = task.Result
									   .Replace("\\\"", "\"")
									   .Trim('\"');

						return JsonConvert.DeserializeObject<T>(json);
					}
					catch (System.Exception /*ex*/)
					{
					}
				}
			}

			return default(T);
		}

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
