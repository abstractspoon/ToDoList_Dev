// -- FILE ------------------------------------------------------------------
// name       : ILoggerEvent.cs
// project    : System Framelet
// created    : Leon Poyyayil - 2006.05.12
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;

namespace Itenso.Sys.Logging
{

	// ------------------------------------------------------------------------
	public interface ILoggerEvent
	{

		// ----------------------------------------------------------------------
		/// <value>the level of the logger event, always defined</value>
		LoggerLevel Level { get; }

		// ----------------------------------------------------------------------
		/// <value>the source which generated the logger event, never null or empty</value>
		string Source { get; }

		// ----------------------------------------------------------------------
		/// <value>the context in which the logger event occurred, never null but possibly empty</value>
		string Context { get; }

		// ----------------------------------------------------------------------
		/// <value>the message of the logger event, never null but possibly empty</value>
		string Message { get; }

		// ----------------------------------------------------------------------
		/// <value>the exception of the logger event, possibly null</value>
		Exception CaughtException { get; }

	} // interface ILoggerEvent

} // namespace Itenso.Sys.Logging
// -- EOF -------------------------------------------------------------------
