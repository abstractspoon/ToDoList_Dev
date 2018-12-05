namespace ZetaHtmlEditControl
{
	using System;
	using System.Diagnostics;
	using System.IO;
	using System.Reflection;
	using System.Security;
	using System.Threading;
	using Microsoft.Win32;

	public static class WebBrowserHelper
	{
		public static void SafeSwitchToHighestInternetExplorerVersionAsync()
		{
			ThreadPool.QueueUserWorkItem(delegate { SafeSwitchToHighestInternetExplorerVersion(); });
		}

		private static void SafeSwitchToHighestInternetExplorerVersion()
		{
			try
			{
				var iv = getInstalledInternetExplorerMajorVersion();

				if (iv >= 10)
				{
					switchWebBrowserToInternetExplorer10();
				}
				if (iv >= 9)
				{
					switchWebBrowserToInternetExplorer9();
				}
				else if (iv >= 8)
				{
					switchWebBrowserToInternetExplorer8();
				}
				else if (iv >= 7)
				{
					switchWebBrowserToInternetExplorer7();
				}
			}
			catch (SecurityException x)
			{
				// Ignore and eat silently.
				Trace.TraceError(
					string.Format(
						@"Ignoring registry exception while setting Internet Explorer mode: {0}", 
						x.Message));
			}
		}

		private static int getInstalledInternetExplorerMajorVersion()
		{
			var key = Registry.LocalMachine.OpenSubKey(
				@"SOFTWARE\Microsoft\Internet Explorer\",
				false);

			if (key == null)
			{
				return 7;
			}
			else
			{
				var raw = key.GetValue(@"Version");
				if (raw == null)
				{
					return 7;
				}
				else
				{
					var raws = raw.ToString().Trim();
					var i = raws.IndexOf('.');
					return i > 0 ? Convert.ToInt32(raws.Substring(0, i)) : 7;
				}
			}
		}

		private static void switchWebBrowserToInternetExplorer7()
		{
			doSwitchWebBrowser(7000);
		}

		private static void switchWebBrowserToInternetExplorer8()
		{
			doSwitchWebBrowser(8000);
		}

		private static void switchWebBrowserToInternetExplorer9()
		{
			doSwitchWebBrowser(9000);
		}

		private static void switchWebBrowserToInternetExplorer10()
		{
			doSwitchWebBrowser(10000);
		}

		private static void doSwitchWebBrowser(uint mode)
		{
			// http://stackoverflow.com/questions/4612255/regarding-ie9-webbrowser-control

			var key = Registry.CurrentUser.OpenSubKey(
				@"SOFTWARE\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION",
				true);

			if (key != null)
			{
				try
				{
					var name = Path.GetFileName(Assembly.GetEntryAssembly().Location);

					// Check whether exists.
					var old = key.GetValue(name);
					if (old == null || Convert.ToUInt32(old) != mode)
					{
						key.SetValue(
							name,
							mode,
							RegistryValueKind.DWord);
					}
				}
				finally
				{
					key.Close();
				}
			}
		}
	}
}