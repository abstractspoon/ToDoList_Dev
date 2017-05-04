
/////////////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

using Abstractspoon.Tdl.PluginHelpers;

/////////////////////////////////////////////////////////////////////////////

namespace GoogleCalendarImpExp
{
    public static class Helper
    {
        private static int s_hasFiles = -1;

        private static readonly string[] REQUIRED = 
	        {
// 		        "System.IO.dll",
// 		        "System.Net.Http.dll",
// 		        "System.Net.Http.Extensions.dll",
// 		        "System.Net.Http.Primitives.dll",
// 		        "System.Net.Http.WebRequest.dll",
// 		        "System.Runtime.dll",
// 		        "System.Threading.Tasks.dll",
		        "Zlib.Portable.dll",
		        "Google.Apis.Auth.dll",
		        "Google.Apis.Auth.PlatformServices.dll",
		        "Google.Apis.Calendar.v3.dll",
		        "Google.Apis.Core.dll",
		        "Google.Apis.dll",
		        "Google.Apis.PlatformServices.dll",
		        "log4net.dll",
		        "Microsoft.Threading.Tasks.dll",
		        "Microsoft.Threading.Tasks.Extensions.Desktop.dll",
		        "Microsoft.Threading.Tasks.Extensions.dll",
		        "Newtonsoft.Json.dll"
	        };

        public static bool HasRequiredComponents()
        {
            if (s_hasFiles == -1)
            {
                string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

                foreach (string req in REQUIRED)
                {
                    string reqPath = Path.Combine(appPath, req);

                    if (!File.Exists(reqPath))
                    {
                        s_hasFiles = 0;
                        break;
                    }
                }

                if (s_hasFiles == 0)
                {
                    // Download with user's consent
                    // TODO
                }
            }

            return (s_hasFiles != 0);
        }
    }

    public class GoogleCalendarImportCore
    {
        public bool Import(string srcFilePath, TaskList destTasks, bool bSilent, Preferences prefs, string sKey)
        {
            if (!Helper.HasRequiredComponents())
                return false;

//             int nVal = prefs.GetProfileInt("bob", "dave", 20);
//             int nVal2 = prefs.GetProfileInt("bob", "phil", 20);
// 
//             // add some dummy values to prefs
//             prefs.WriteProfileInt("bob", "dave", 10);
// 
//             Task task = srcTasks.GetFirstTask();
// 
//             String sTitle = task.GetTitle();
// 

            return true;
        }
    }

    public class GoogleCalendarExportCore
    {
        public bool Export(TaskList srcTasks, string sDestFilePath, bool bSilent, Preferences prefs, string sKey)
        {
            if (!Helper.HasRequiredComponents())
                return false;

            int nVal = prefs.GetProfileInt("bob", "dave", 20);
            int nVal2 = prefs.GetProfileInt("bob", "phil", 20);

            // add some dummy values to prefs
            prefs.WriteProfileInt("bob", "dave", 10);

            Task task = srcTasks.GetFirstTask();

            String sTitle = task.GetTitle();


            return true;
        }
    }
}
