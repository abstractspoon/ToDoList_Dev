
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.Threading;
using System.Diagnostics;
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
        const string FontName   = "Tahoma";
        const string AboutBlank = "about:blank";

		const string DataPlaceholder	= "{{{JSVIEW_USERDATA}}}";
		const string CodePlaceholder	= "{{{JSVIEW_USERCODE}}}";
		const string StylesPlaceholder	= "{{{JSVIEW_STYLESHEET}}}";
		const string ColorsPlaceholder  = "{{{JSVIEW_COLORUTIL}}}";

		// -------------------------------------------------------------

		IntPtr m_HwndParent;
		Translator m_Trans;
		String m_TypeId, m_UiName;
		Font m_ControlsFont;

		JsTaskItems m_Items;
		JObject m_AppPrefs;
		uint m_SelectedTaskId;

		// Data resource files -> Tasklist folder
		ResourceFile m_JsDataFile	= new ResourceFile("JSViewData.js", true);
		ResourceFile m_HtmlFile		= new ResourceFile("JSView.html", true);

		// Code resource files -> Exe folder
		ResourceFile m_JsCodeFile	= new ResourceFile("JSViewCode.js", false);
		ResourceFile m_StylesFile	= new ResourceFile("JSView.css", false);
		ResourceFile m_ColorsFile	= new ResourceFile("JSColorUtil.js", false);

		// -------------------------------------------------------------

		public JSViewUIExtensionCore(String typeId, String uiName, IntPtr hwndParent, Translator trans)
		{
			m_TypeId = typeId;
			m_UiName = uiName;
			m_HwndParent = hwndParent;
			m_Trans = trans;

			m_ControlsFont = new Font(FontName, 8, FontStyle.Regular);
			m_Items = new JsTaskItems();
			m_AppPrefs = new JObject();

			InitializeComponent();
			InitializeAsync();
		}

		async void InitializeAsync()
        {
			m_WebView.WebMessageReceived += new EventHandler<CoreWebView2WebMessageReceivedEventArgs>(OnWebMessageReceived);
			m_WebView.NavigationCompleted += new EventHandler<CoreWebView2NavigationCompletedEventArgs>(OnNavigationCompleted);
			m_WebView.NavigationStarting += new EventHandler<CoreWebView2NavigationStartingEventArgs>(OnNavigationStarting);

			await m_WebView.EnsureCoreWebView2Async(null);

			UpdateBrowserAppPreferences();
			m_WebView.CoreWebView2.Navigate(m_HtmlFile.Uri); // will have already been built in UpdateTasks
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
			// 'Data' files are stored with the tasklist
			m_HtmlFile.TasklistPath = m_JsDataFile.TasklistPath = tasks.GetFilePath();

			// Tracked whether HTML file was updated
			string defaultHtml = Resources.JSViewDefaultPage
										  .Replace(DataPlaceholder, m_JsDataFile.Uri)
										  .Replace(StylesPlaceholder, m_StylesFile.Uri)
										  .Replace(ColorsPlaceholder, m_ColorsFile.Uri)
										  .Replace(CodePlaceholder, m_JsCodeFile.Uri);

			bool htmlUpdated = m_HtmlFile.CheckUpdate(defaultHtml);

			// Update tasks
			var attribs = JSONUtil.GetAttributeList(tasks); // No translation

			switch (type)
			{
			case UIExtension.UpdateType.Delete:
			case UIExtension.UpdateType.All:
				{
					// Rebuild all items
					m_Items.Populate(tasks, attribs);

					// Refresh page
					ExportItemsToJsonAsJavascript();

					if (m_WebView.CoreWebView2 != null)
					{
						if (htmlUpdated || (m_WebView.CoreWebView2.Source == AboutBlank))
							m_WebView.CoreWebView2.Navigate(m_HtmlFile.Uri);
						else
							m_WebView.CoreWebView2.PostWebMessageAsString("Refresh");
					}
				}
				break;

			case UIExtension.UpdateType.New:
			case UIExtension.UpdateType.Edit:
				{
					Debug.Assert(m_WebView.CoreWebView2 != null);

					// In-place update
					if (m_Items.MergeAttributes(tasks, attribs))
					{
						// Selectively refresh page
						var items = new JsTaskItems();
						items.Populate(tasks, attribs);

						var message = string.Format("Refresh={0}", items.ToJson());
						m_WebView.CoreWebView2.PostWebMessageAsString(message);

						// And update data file so that subsequent
						// 'in-page' refreshes work as expected
						var thread = new Thread(() => ExportItemsToJsonAsJavascript());
						thread.Start();
					}
				}
				break;
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
            this.BackColor = theme.GetAppDrawingColor(UITheme.AppColor.AppBackLight);

			m_AppPrefs["BackColor"] = ColorTranslator.ToHtml(this.BackColor);
			UpdateBrowserAppPreferences();
		}

		public void SetTaskFont(String faceName, int pointSize)
		{
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(Preferences prefs, String key)
		{
			// Note: m_JsDataFile is always updated
			m_HtmlFile.Save(prefs, key);
			m_JsCodeFile.Save(prefs, key);
			m_StylesFile.Save(prefs, key);
			m_ColorsFile.Save(prefs, key);
		}

		public void LoadPreferences(Preferences prefs, String key, bool appOnly)
		{
            if (!appOnly)
            {
				// private settings
				m_HtmlFile.Load(prefs, key);
				m_JsCodeFile.Load(prefs, key);
				m_StylesFile.Load(prefs, key);
				m_ColorsFile.Load(prefs, key);

				// One time update for code files
				m_JsCodeFile.CheckUpdate(Resources.JSViewDefaultCode);
				m_StylesFile.CheckUpdate(Resources.JSViewDefaultStyles);
				m_ColorsFile.CheckUpdate(Resources.JSViewDefaultColorUtil);
			}

			// App settings
			m_AppPrefs["ColorTaskBackground"] = prefs.GetProfileBool("Preferences", "ColorTaskBackground", false);
			m_AppPrefs["StrikethroughDone"] = prefs.GetProfileBool("Preferences", "StrikethroughDone", true);

			UpdateBrowserAppPreferences();
		}

		// PRIVATE ------------------------------------------------------------------------------

		void UpdateBrowserAppPreferences()
		{
			var message = string.Format("Preferences={0}", m_AppPrefs.ToString());
			m_WebView?.CoreWebView2?.PostWebMessageAsString(message);
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

				// 3. Add translated attribute attributes
				// TODO
			};

			File.WriteAllLines(m_JsDataFile.Path, jsContent);
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
			UpdateBrowserAppPreferences();
			SelectTask(m_SelectedTaskId);
		}

		void OnNavigationStarting(object sender, CoreWebView2NavigationStartingEventArgs e)
		{
			e.Cancel = ((e.Uri != AboutBlank) && (e.Uri != m_HtmlFile.Uri));
		}
	}

	////////////////////////////////////////////////////////////////////////////

	class ResourceFile
	{
		string		m_TasklistPath	= string.Empty;
		string		m_FileName		= string.Empty;
		DateTime	m_CreationDate	= DateUtil.NullDate;
		bool		m_DataFile		= true;

		// --------------------------------------

		public ResourceFile(string fileName, bool dataFile)
		{
			m_FileName = fileName;
			m_DataFile = dataFile;
		}

		public string TasklistPath 
		{
			get { return m_TasklistPath; }

			set
			{
				// Sanity checks (for now)
				Debug.Assert(m_DataFile);
				Debug.Assert(string.IsNullOrEmpty(m_TasklistPath) ||
							 (value == m_TasklistPath));

				m_TasklistPath = value;
			}
		}

		public DateTime CreationDate
		{
			get { return m_CreationDate; }

			set
			{
				Debug.Assert(m_CreationDate == DateUtil.NullDate);
				m_CreationDate = value;
			}
		}

		public string Path
		{
			get
			{
				if (m_DataFile)
					return System.IO.Path.ChangeExtension(m_TasklistPath, m_FileName);

				// else
				return System.IO.Path.Combine(Application.StartupPath, m_FileName);
			}
		}

		public string Uri
		{
			get
			{
				return new Uri(Path).AbsoluteUri;
			}
		}

		public bool CheckUpdate(string defaultContents)
		{
			if (HasChanged())
				return false;

			File.WriteAllText(Path, defaultContents);
			m_CreationDate = File.GetCreationTime(Path);

			return true;
		}

		bool HasChanged()
		{
			if (!File.Exists(Path))
				return false;

			if (CreationDate != DateUtil.NullDate)
			{
				var lastMod = File.GetLastWriteTime(Path);
				return (lastMod != CreationDate);
			}

			// If we weren't provided with a creation date then we 
			// have no way of knowing if the file has been changed 
			// or not so we err on the side of caution
			return true;
		}

		public void Save(Preferences prefs, String key)
		{
			var entry = string.Format("{0}.CreationDate", m_FileName);
			prefs.WriteProfileString(key, entry, CreationDate.ToString("s"));
		}

		public bool Load(Preferences prefs, String key)
		{
			DateTime date;
			var entry = string.Format("{0}.CreationDate", m_FileName);

			if (!DateTime.TryParse(prefs.GetProfileString(key, entry, ""), out date))
				return false;

			CreationDate = date;
			return true;
		}
	}
}
