// -- FILE ------------------------------------------------------------------
// name       : LoggerFactory.cs
// project    : System Framelet
// created    : Leon Poyyayil - 2005.05.03
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Sys.Logging
{

	// ------------------------------------------------------------------------
	internal abstract class LoggerFactory
	{

		// ----------------------------------------------------------------------
		public static bool InitializeLoggerFactory( string factoryName )
		{
			if ( instance == null )
			{
				lock ( mutex )
				{
					if ( instance == null )
					{
						LoggerFactoryBuilder.SetDefaultLoggerFactory( factoryName );
					}
				}
			}
			return Instance.GetType().FullName.Equals( factoryName );
		} // InitializeLoggerFactory

		// ----------------------------------------------------------------------
		public static LoggerFactory Instance
		{
			get
			{
				if ( instance == null )
				{
					lock ( mutex )
					{
						if ( instance == null )
						{
							instance = LoggerFactoryBuilder.BuildFactoryInstance();
						}
					}
				}
				return instance;
			}
		} // Instance

		// ----------------------------------------------------------------------
		public abstract ILogger GetLogger( string name );

		// ----------------------------------------------------------------------
		public ILoggerMonitor Monitor
		{
			get
			{
				if ( monitor == null )
				{
					lock ( this )
					{
						if ( monitor == null )
						{
							monitor = CreateMonitor();
						}
					}
				}
				return monitor;
			}
		} // Monitor

		// ----------------------------------------------------------------------
		protected virtual ILoggerMonitor CreateMonitor()
		{
			return new LoggerMonitorNone();
		} // CreateMonitor

		// ----------------------------------------------------------------------
		public virtual void SetLogFile( string absoluteLogFileName, bool append, string messagePattern )
		{
			throw new InvalidOperationException( Strings.LoggerLogFileNotSupportedByType( GetType().FullName ) );
		} // SetLogFile

		// ----------------------------------------------------------------------
		// members
		private static readonly object mutex = new object();
		private static LoggerFactory instance;

		private ILoggerMonitor monitor;

	} // class LoggerFactory

} // namespace Itenso.Sys.Logging
// -- EOF -------------------------------------------------------------------
