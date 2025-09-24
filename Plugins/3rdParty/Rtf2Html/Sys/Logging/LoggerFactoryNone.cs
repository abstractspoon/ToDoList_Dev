// -- FILE ------------------------------------------------------------------
// name       : LoggerFactoryNone.cs
// project    : System Framelet
// created    : Leon Poyyayil - 2005.05.04
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------

namespace Itenso.Sys.Logging
{

	// ------------------------------------------------------------------------
	internal sealed class LoggerFactoryNone : LoggerFactory
	{

		// ----------------------------------------------------------------------
		public override ILogger GetLogger( string name )
		{
			if ( logger == null )
			{
				lock ( mutex )
				{
					if ( logger == null )
					{
						logger = new LoggerNone();
					}
				}
			}
			return logger;
		} // GetLogger

		// ----------------------------------------------------------------------
		// members
		private static readonly object mutex = new object();
		private static LoggerNone logger;

	} // class LoggerFactoryNone

} // namespace Itenso.Sys.Logging
// -- EOF -------------------------------------------------------------------
