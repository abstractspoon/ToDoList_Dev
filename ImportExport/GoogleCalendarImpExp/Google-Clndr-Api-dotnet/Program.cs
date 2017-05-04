using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Google.Apis.Auth.OAuth2;
using System.Threading;
using Google.Apis.Util.Store;
using Google.Apis.Calendar.v3;
using Google.Apis.Services;
using Google.Apis.Calendar.v3.Data;


//Nuget package https://www.nuget.org/packages/Google.Apis.Calendar.v3/
//PM> Install-Package Google.Apis.Calendar.v3


namespace Google_Clndr_Api_dotnet
{
    class Program
    {
        static void Main(string[] args)
        {
            CalendarService service; 
            //// Service account Authentication 
            // IF you take the service account Email address you can give it access to any calendar you want just like you would a user.

            //String SERVICE_ACCOUNT_EMAIL = "1046123799103-6v9cj8jbub068jgmss54m9gkuk4q2qu8@developer.gserviceaccount.com";
            //string SERVICE_ACCOUNT_KEYFILE = @"C:\Users\HP_User\Downloads\Diamto Test Everything Project-ec242f46b936.p12";
            //service = Authentication.AuthenticateServiceAccount(SERVICE_ACCOUNT_EMAIL, SERVICE_ACCOUNT_KEYFILE);
           

            // Authenticate Oauth2
            String CLIENT_ID = "820478641063-fvbvdbdfi5t8ddd4ik6ib1jl1btsmnlf.apps.googleusercontent.com";
            String CLIENT_SECRET = "X1As16CiUCFxJLDGC2F6BuyS";
            service = Authentication.AuthenticateOauth(CLIENT_ID, CLIENT_SECRET, "AbstractSpoon ToDoList");

            CalendarList calList =  DaimtoCalendarListHelper.list(service, null);

            foreach (CalendarListEntry cal in calList.Items)
            {
               Console.WriteLine("Events for Calendar '{0}'", cal.Id);

               Events evtList = DaimtoEventHelper.list(service, cal.Id, null);

               foreach (Event evt in evtList.Items)
               {
                  Console.WriteLine("    {0} : {1}", evt.Start.Date, evt.Summary);
               }
            }

            int i = 1;


        }
    }
}
