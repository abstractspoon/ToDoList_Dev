// -- FILE ------------------------------------------------------------------
// name       : LoggerBase.cs
// project    : System Framelet
// created    : Leon Poyyayil - 2006.05.11
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Sys.Logging
{

	// ------------------------------------------------------------------------
	public abstract class LoggerBase
	{

		// ----------------------------------------------------------------------
		public virtual string Context
		{
			get { return string.Empty; }
		} // Context

		// ----------------------------------------------------------------------
		public virtual bool IsSupportedException( Exception exception )
		{
			bool supported = true;

			if ( exception is System.Threading.ThreadAbortException )
			{
				supported = false;
			}

			return supported;
		} // IsSupportedException

		// ----------------------------------------------------------------------
		public virtual IDisposable PushContext( string context )
		{
			if ( context == null )
			{
				throw new ArgumentNullException( "context" );
			}
			return new LoggerContextDisposable( Logger );
		} // PushContext

		// ----------------------------------------------------------------------
		public virtual int ContextDepth
		{
			get { return 0; }
		} // ContextDepth

		// ----------------------------------------------------------------------
		public virtual void PopContext()
		{
		} // PopContext

		// ----------------------------------------------------------------------
		protected abstract ILogger Logger { get; }

	} // class LoggerBase

} // namespace Itenso.Sys.Logging
// -- EOF -------------------------------------------------------------------
