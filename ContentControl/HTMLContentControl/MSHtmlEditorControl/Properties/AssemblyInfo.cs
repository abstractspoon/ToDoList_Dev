#region Using directives

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Security.Permissions;

#endregion

// define the securty setting for the class
// ask for full trust or unmanaged code access (both should not be needed)
// [assembly:PermissionSetAttribute(SecurityAction.RequestMinimum, Name="FullTrust")]
// [assembly: SecurityPermissionAttribute(SecurityAction.RequestMinimum, Flags = SecurityPermissionFlag.UnmanagedCode)]

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("Html.Editor.Control")]
[assembly: AssemblyDescription("Html Content Editor Control")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Microsoft UK")]
[assembly: AssemblyProduct("Html.Editor.Control")]
[assembly: AssemblyCopyright("Copyright @ 2004")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Revision and Build Numbers by using the '*' as shown below:
[assembly: AssemblyVersion("2.0.*")]
