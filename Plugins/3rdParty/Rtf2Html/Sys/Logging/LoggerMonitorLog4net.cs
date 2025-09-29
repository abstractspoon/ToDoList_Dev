// -- FILE ------------------------------------------------------------------
// name       : LoggerMonitorLog4net.cs
// project    : System Framelet
// created    : Leon Poyyayil - 2006.05.12
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections;

namespace Itenso.Sys.Logging
{

	// ------------------------------------------------------------------------
	public sealed class LoggerMonitorLog4net : ILoggerMonitor
	{

		// ----------------------------------------------------------------------
		public LoggerMonitorLog4net()
		{
		} // LoggerMonitorLog4net

		// ----------------------------------------------------------------------
		public void Register( ILoggerListener loggerListener, string context )
		{
			if ( loggerListener == null )
			{
				throw new ArgumentNullException( "loggerListener" );
			}
			string checkedContext = ArgumentCheck.NonemptyTrimmedString( context, "context" );
			lock ( listenerListsByContext )
			{
				ArrayList contextListeners = listenerListsByContext[ checkedContext ] as ArrayList;
				if ( contextListeners == null )
				{
					contextListeners = new ArrayList( 5 );
					listenerListsByContext.Add( checkedContext, contextListeners );
				}
				lock ( contextListeners )
				{
					contextListeners.Add( loggerListener );
				}
			}
		} // Register

		// ----------------------------------------------------------------------
		public void Unregister( ILoggerListener loggerListener, string context )
		{
			if ( loggerListener == null )
			{
				throw new ArgumentNullException( "loggerListener" );
			}
			string checkedContext = ArgumentCheck.NonemptyTrimmedString( context, "context" );
			lock ( listenerListsByContext )
			{
				ArrayList contextListeners = listenerListsByContext[ checkedContext ] as ArrayList;
				if ( contextListeners != null )
				{
					lock ( contextListeners )
					{
						contextListeners.Remove( loggerListener );
					}
					if ( contextListeners.Count == 0 )
					{
						listenerListsByContext.Remove( checkedContext );
					}
				}
			}
		} // Unregister

		// ----------------------------------------------------------------------
		internal void Handle( ILoggerEvent loggerEvent )
		{
			if ( loggerEvent == null )
			{
				throw new ArgumentNullException( "loggerEvent" );
			}

			string eventContext = loggerEvent.Context;

			ArrayList contextListeners = null;
			if ( listenerListsByContext.Count > 0 )
			{
				lock ( listenerListsByContext )
				{
					contextListeners = listenerListsByContext[ eventContext ] as ArrayList;
					if ( contextListeners == null )
					{
						foreach ( string rootContext in listenerListsByContext.Keys )
						{
							if ( eventContext.StartsWith( rootContext ) )
							{
								contextListeners = listenerListsByContext[ rootContext ] as ArrayList;
								break;
							}
						}
					}
				}
			}

			if ( contextListeners != null && contextListeners.Count > 0 )
			{
				lock ( contextListeners )
				{
					foreach ( ILoggerListener loggerListener in contextListeners )
					{
						loggerListener.Handle( loggerEvent );
					}
				}
			}
		} // Handle

		// ----------------------------------------------------------------------
		// members
		private readonly Hashtable listenerListsByContext = new Hashtable();

	} // class LoggerMonitorLog4net

} // namespace Itenso.Sys.Logging
// -- EOF -------------------------------------------------------------------
