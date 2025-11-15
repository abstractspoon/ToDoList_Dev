
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;
using System.Web.UI;
using Microsoft.Web.WebView2.Core;

using JSONExporterPlugin;
using JSViewUIExtension.Properties;

using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////

namespace JSViewUIExtension
{
	public partial class JSViewUIExtensionCore : System.Windows.Forms.UserControl, IUIExtension
	{
        const string FontName = "Tahoma";

		const string DataPlaceholder	= "{{{JSVIEW_USERDATA}}}";
		const string CodePlaceholder	= "{{{JSVIEW_USERCODE}}}";
		const string StylesPlaceholder	= "{{{JSVIEW_STYLESHEET}}}";

		// -------------------------------------------------------------

		IntPtr m_HwndParent;
		Translator m_Trans;
		String m_TypeId, m_UiName;
		Font m_ControlsFont;

		JsTaskItems m_Items;
		uint m_SelectedTaskId;
		string m_TasklistPath;

		DateTime m_HtmlCreationDate = DateUtil.NullDate;
		DateTime m_CodeCreationDate = DateUtil.NullDate;
		DateTime m_StylesCreationDate = DateUtil.NullDate;

		// -------------------------------------------------------------

		// Data files -> Tasklist folder
		string JsDataFilePath	{ get { return FilePathFromName("JSViewData.js", true); } }
		string HtmlFilePath		{ get { return FilePathFromName("JSView.html", true); } }

		// Code files -> Exe folder
		string JsCodeFilePath	{ get { return FilePathFromName("JSViewCode.js", false); } }
		string StylesFilePath	{ get { return FilePathFromName("JSView.css", false); } }

		string JsDataFileUri	{ get { return UriFromFilePath(JsDataFilePath); } }
		string JsCodeFileUri	{ get { return UriFromFilePath(JsCodeFilePath); } }
		string HtmlFileUri		{ get { return UriFromFilePath(HtmlFilePath); } }
		string StylesFileUri	{ get { return UriFromFilePath(StylesFilePath); } }

		// -------------------------------------------------------------

		public JSViewUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
			m_Trans = trans;

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);
			m_Items = new JsTaskItems();

			BorderStyle = BorderStyle.FixedSingle;

			InitializeComponent();
			InitializeAsync();
		}

		async void InitializeAsync()
        {
			await m_WebView.EnsureCoreWebView2Async(null);

 			m_WebView.WebMessageReceived += new EventHandler<CoreWebView2WebMessageReceivedEventArgs>(OnWebMessageReceived);
			m_WebView.NavigationCompleted += new EventHandler<CoreWebView2NavigationCompletedEventArgs>(OnNavigationCompleted);

//			m_WebView.CoreWebView2.Settings.IsBuiltInErrorPageEnabled = false;

//			await m_WebView.CoreWebView2.AddScriptToExecuteOnDocumentCreatedAsync("window.chrome.webview.postMessage(window.document.URL);");
//			await m_WebView.CoreWebView2.AddScriptToExecuteOnDocumentCreatedAsync("window.chrome.webview.addEventListener(\'message\', event => alert(event.data));");

			// Disable context menu
//			await m_WebView.CoreWebView2.ExecuteScriptAsync("window.addEventListener('contextmenu', window => {window.preventDefault();});");



			Navigate(HtmlFileUri); // will have already been built in UpdateTasks
		}

		// IUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 taskId)
		{
			m_SelectedTaskId = taskId;

			m_WebView?.CoreWebView2?.PostWebMessageAsString(string.Format("SelectTask={0}", taskId));

			// TODO

			return true/*false*/;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return true/*false*/;
		}

		public bool ScrollToSelectedTask()
		{
			return false;
		}

		public bool CanScrollToSelectedTask()
		{
			return false;
		}

		public void UpdateTasks(TaskList tasks, UIExtension.UpdateType type)
		{
			var attribs = JSONUtil.GetAttributeList(tasks); // No translation

			switch (type)
			{
			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				{
					// Rebuild
					m_Items.Populate(tasks, attribs);
				}
				break;

			case UIExtension.UpdateType.New:
			case UIExtension.UpdateType.Edit:
				{
					// In-place update
					if (!m_Items.MergeAttributes(tasks, attribs))
						return;
				}
				break;
			}

			m_TasklistPath = tasks.GetFilePath();

			// Initialise/update default Javascript and HTML files
			if (!HasFileChanged(HtmlFilePath, m_HtmlCreationDate))
			{
				string html = Resources.JSViewDefaultPage
									   .Replace(DataPlaceholder, JsDataFileUri)
									   .Replace(StylesPlaceholder, StylesFileUri)
									   .Replace(CodePlaceholder, JsCodeFileUri);

				File.WriteAllText(HtmlFilePath, html);
				m_HtmlCreationDate = File.GetLastWriteTime(HtmlFilePath);
			}

			if (!HasFileChanged(JsCodeFilePath, m_CodeCreationDate))
			{
				File.WriteAllText(JsCodeFilePath, Resources.JSViewDefaultCode);
				m_CodeCreationDate = File.GetLastWriteTime(JsCodeFilePath);
			}

			if (!HasFileChanged(StylesFilePath, m_StylesCreationDate))
			{
				File.WriteAllText(StylesFilePath, Resources.JSViewDefaultStyles);
				m_StylesCreationDate = File.GetLastWriteTime(StylesFilePath);
			}

			// Refresh page
			ExportItemsToJsonAsJavascript();

			if (m_WebView.CoreWebView2 != null)
			{
				if (m_WebView.CoreWebView2.Source == "about:blank")
					Navigate(HtmlFileUri);
				else
					m_WebView.CoreWebView2.PostWebMessageAsString("Refresh");
			}
		}

		public bool GetTask(UIExtension.GetTask getTask, ref UInt32 taskId)
		{
			taskId = 0;

			// TODO

			return (taskId != 0);
		}

		public bool SelectTask(String text, UIExtension.SelectTask selectTask, bool caseSensitive, bool wholeWord, bool findReplace)
		{
            if (text == String.Empty)
                return false;

			// TODO
	
			// all else
			return false;
		}


        public Bitmap SaveToImage()
        {
            return null;
        }

		public bool WantTaskUpdate(Task.Attribute attrib)
		{
			return true;
		}
	   
		public bool WantSortUpdate(Task.Attribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref Task task)
		{
			return false;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}

		public bool DoIdleProcessing()
		{
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
			return false;
		}

		public UIExtension.HitTestResult HitTest(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			return UIExtension.HitTestResult.Nowhere;
		}

		public UInt32 HitTestTask(Int32 xPos, Int32 yPos, UIExtension.HitTestReason reason)
		{
			return 0;
		}

		public void SetUITheme(UITheme theme)
		{
            this.BackColor = /*m_Toolbar.BackColor =*/ theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			// Set the toolbar colors to be the same as the back color
// 			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarDark, BackColor);
// 			theme.SetAppDrawingColor(UITheme.AppColor.ToolbarLight, BackColor);
// 
// 			m_TBRenderer.SetUITheme(theme);
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			prefs.WriteProfileString(key, "HtmlCreationDate", m_HtmlCreationDate.ToString("s"));
			prefs.WriteProfileString(key, "CodeCreationDate", m_CodeCreationDate.ToString("s"));
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            {
				// private settings
				DateTime.TryParse(prefs.GetProfileString(key, "HtmlCreationDate", ""), out m_HtmlCreationDate);
				DateTime.TryParse(prefs.GetProfileString(key, "CodeCreationDate", ""), out m_CodeCreationDate);
			}

			// App settings
		}

		// PRIVATE ------------------------------------------------------------------------------

		bool HasFileChanged(string filePath, DateTime creationDate)
		{
			if (!File.Exists(filePath))
				return false;

			if (creationDate != DateUtil.NullDate)
			{
				var lastMod = File.GetLastWriteTime(filePath);
				return (lastMod != creationDate);
			}

			// If we weren't provided with a creation date then we 
			// have no way of knowing if the file has been changed 
			// or not so we err on the side of caution
			return true;
		}

		string FilePathFromName(string fileName, bool dataFile)
		{
			if (dataFile)
				return Path.ChangeExtension(m_TasklistPath, fileName);

			// else
			return Path.Combine(Application.StartupPath, fileName);
		}

		static string UriFromFilePath(string filePath)
		{
			return new Uri(filePath).AbsoluteUri;
		}

		void ExportItemsToJsonAsJavascript()
		{
			var jOutput = new JObject();
			jOutput.Add(new JProperty("Tasks", m_Items.ToJson())); // No translation

			string json = jOutput.ToString();
// #if DEBUG
// 			File.WriteAllText(FilePathFromName("JSViewData.json"), json);
// #endif

			var jsContent = new string[]
			{
				// 1. Add the tasks
				"var json = `",
				json.Replace('\\', '/'),
				"`;",
				"var tasks = JSON.parse(json).Tasks;",
				"\n\n",

				// 2. Add translated attribute attributes
				// TODO
			};

			File.WriteAllLines(JsDataFilePath, jsContent);
		}

		void Navigate(string uri)
		{
			if (m_WebView != null && m_WebView.CoreWebView2 != null)
			{
				// 				if (!uri.StartsWith("https://") || !uri.StartsWith("http://"))
				// 					uri = "https://" + uri;

				m_WebView.CoreWebView2.Navigate(uri);
			}
		}

		protected override void OnHandleDestroyed(EventArgs e)
		{
			m_WebView.Dispose();

			base.OnHandleDestroyed(e);
		}

		void NotifyParentSelChange(UInt32 taskId)
		{
			var parent = new UIExtension.ParentNotify(m_HwndParent);

			parent.NotifySelChange(taskId);
		}

		void OnWebMessageReceived(object sender, CoreWebView2WebMessageReceivedEventArgs e)
		{
			string message = e.TryGetWebMessageAsString();

			if (!string.IsNullOrWhiteSpace(message))
			{
				if (message.StartsWith("SelectTask="))
				{
					string strId = message.Substring(11);
					uint taskId;

					if (uint.TryParse(strId, out taskId))
					{
						if (new UIExtension.ParentNotify(m_HwndParent).NotifySelChange(taskId))
							m_SelectedTaskId = taskId;
						else
							SelectTask(m_SelectedTaskId);
					}
				}
			}
		}

		void OnNavigationCompleted(object sender, CoreWebView2NavigationCompletedEventArgs e)
		{
			SelectTask(m_SelectedTaskId);
		}
	}
}
