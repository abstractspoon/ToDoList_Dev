
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Reflection;
using System.Web.UI;

using JSONExporterPlugin;
using Newtonsoft.Json.Linq;

using Abstractspoon.Tdl.PluginHelpers;

///////////////////////////////////////////////////////////////////////////

namespace JSViewUIExtension
{
	public partial class JSViewUIExtensionCore : System.Windows.Forms.UserControl, IUIExtension
	{
        private const string FontName = "Tahoma";

		private static   string ResourcePath	= Application.StartupPath;
		private readonly string ResourcePathUri = UriFromFileName("");

		private readonly string JsDataFilePath = FilePathFromName("JSViewData.js");
		private readonly string JsCodeFilePath = FilePathFromName("JSViewCode.js");
		private readonly string HtmlFilePath   = FilePathFromName("JSViewData.html");

		private readonly string JsDataFileUri = UriFromFileName("JSViewData.js");
		private readonly string JsCodeFileUri = UriFromFileName("JSViewCode.js");
		private readonly string HtmlFileUri	  = UriFromFileName("JSViewData.html");

		private static string FilePathFromName(string fileName)
		{
			return Path.Combine(ResourcePath, fileName);
		}

		private static string UriFromFileName(string fileName)
		{
			string filePath = FilePathFromName(fileName);
			return new Uri(filePath).AbsoluteUri;
		}

		// -------------------------------------------------------------

		private IntPtr m_HwndParent;
		private Translator m_Trans;
		private String m_TypeId, m_UiName;
		private Font m_ControlsFont;

		private JsTaskItems m_Items;

// 		private IIControls.ToolStripEx m_Toolbar;
// 		private ImageList m_TBImageList;
// 		private UIThemeToolbarRenderer m_TBRenderer;

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
			return false;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
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
			var attribs = tasks.GetAvailableAttributes();

			switch (type)
			{
			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				// Rebuild
				m_Items.Populate(tasks, attribs);
				break;

			case UIExtension.UpdateType.New:
			case UIExtension.UpdateType.Edit:
				// In-place update
				if (!m_Items.MergeAttributes(tasks, attribs))
					return;
				break;
			}

			// Initialise default Javascript and HTML files
			if (!File.Exists(HtmlFilePath))
				File.WriteAllText(HtmlFilePath, JSViewUIExtension.Properties.Resources.JSViewDefaultPage);

			if (!File.Exists(JsCodeFilePath))
				File.WriteAllText(JsCodeFilePath, JSViewUIExtension.Properties.Resources.JSViewDefaultCode);

			// Refresh page
			ExportItemsToJsonAsJavascript();
			Navigate(HtmlFileUri);
		}

		private void ExportItemsToJsonAsJavascript()
		{
			var jOutput = new JObject();
			jOutput.Add(new JProperty("Tasks", m_Items.ToJson()));

			string json = JsonExporter.ToJson(jOutput);

			var jsContent = new string[]
			{
				"var json = `",
				json.Replace('\\', '/'),
				"`;",
				"var tasks = JSON.parse(json).Tasks;"

				// Add translated attribute attributes!
				// TODO
			};

			File.WriteAllLines(JsDataFilePath, jsContent);
		}

/*
		private void ExportTasklistToJsonAsJavascript(TaskList tasks)
		{
			string json = new JSONExporter().Export(tasks);

			var jsContent = new string[]
			{
						"var json = `",
						json.Replace('\\', '/'),
						"`;",
						"var tasks = JSON.parse(json).Tasks;"
			};

			File.WriteAllLines(JsDataFilePath, jsContent);
		}
*/

/*
		private bool ProcessTaskUpdate(Task task, 
									   UIExtension.UpdateType type,
									   HashSet<UInt32> taskIds)
		{
			if (!task.IsValid())
				return false;

			// Ignore reference tasks
			if (task.GetReferenceID() != 0)
				return true;

			// TODO

			if (taskIds != null)
				taskIds.Add(task.GetID());

			// Process children
			Task subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, taskIds))
				subtask = subtask.GetNextTask();

			return true;
		}
*/

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
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            { 
                // private settings
            }

            // App settings
        }

		// PRIVATE ------------------------------------------------------------------------------

		private void Navigate(string uri)
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

		private void NotifyParentSelChange(UInt32 taskId)
		{
			var parent = new UIExtension.ParentNotify(m_HwndParent);

			parent.NotifySelChange(taskId);
		}

	}
}
