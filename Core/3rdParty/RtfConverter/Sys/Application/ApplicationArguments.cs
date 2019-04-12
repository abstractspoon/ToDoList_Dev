// -- FILE ------------------------------------------------------------------
// name       : ApplicationArguments.cs
// project    : System Framelet
// created    : Jani Giannoudis - 2008.06.03
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Sys.Application
{

	// ------------------------------------------------------------------------
	public class ApplicationArguments
	{

		// ----------------------------------------------------------------------
		public ApplicationArguments()
		{
		} // ApplicationArguments

		// ----------------------------------------------------------------------
		public ArgumentCollection Arguments 
		{
			get { return this.arguments; }
		} // Arguments

		// ----------------------------------------------------------------------
		public bool IsValid
		{
			get { return this.arguments.IsValid; }
		} // IsValid

		// ----------------------------------------------------------------------
		public bool IsHelpMode
		{
			get 
			{
				foreach ( IArgument argument in this.arguments )
				{
					if ( argument is HelpModeArgument )
					{
						return (bool)argument.Value;
					}
				}
				return false;
			}
		} // IsHelpMode

		// ----------------------------------------------------------------------
		public void Load(string[] args)
		{
			for ( int i = 0; i < args.Length; i++ )
			{
				string commandLineArg = args[ i ];

				foreach ( IArgument argument in this.arguments )
				{
					if ( argument.IsLoaded )
					{
						continue;
					}
					argument.Load( commandLineArg );
					if ( argument.IsLoaded )
					{
						break;
					}
				}
			}
		} // Load

		// ----------------------------------------------------------------------
		// members
		private readonly ArgumentCollection arguments = new ArgumentCollection();

	} // class ApplicationArguments

} // namespace Itenso.Sys.Application
// -- EOF -------------------------------------------------------------------
