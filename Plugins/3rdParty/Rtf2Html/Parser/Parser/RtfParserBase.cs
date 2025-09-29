// -- FILE ------------------------------------------------------------------
// name       : RtfParserBase.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.20
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections;

namespace Itenso.Rtf.Parser
{

	// ------------------------------------------------------------------------
	public abstract class RtfParserBase : IRtfParser
	{

		// ----------------------------------------------------------------------
		protected RtfParserBase()
		{
		} // RtfParserBase

		// ----------------------------------------------------------------------
		protected RtfParserBase( params IRtfParserListener[] listeners )
		{
			if ( listeners != null )
			{
				foreach ( IRtfParserListener listener in listeners )
				{
					AddParserListener( listener );
				}
			}
		} // RtfParserBase

		// ----------------------------------------------------------------------
		public bool IgnoreContentAfterRootGroup
		{
			get { return ignoreContentAfterRootGroup; }
			set { ignoreContentAfterRootGroup = value; }
		} // IgnoreContentAfterRootGroup

		// ----------------------------------------------------------------------
		public void AddParserListener( IRtfParserListener listener )
		{
			if ( listener == null )
			{
				throw new ArgumentNullException( "listener" );
			}
			if ( this.listeners == null )
			{
				this.listeners = new ArrayList();
			}
			if ( !this.listeners.Contains( listener ) )
			{
				this.listeners.Add( listener );
			}
		} // AddParserListener

		// ----------------------------------------------------------------------
		public void RemoveParserListener( IRtfParserListener listener )
		{
			if ( listener == null )
			{
				throw new ArgumentNullException( "listener" );
			}
			if ( this.listeners != null )
			{
				if ( this.listeners.Contains( listener ) )
				{
					this.listeners.Remove( listener );
				}
				if ( this.listeners.Count == 0 )
				{
					this.listeners = null;
				}
			}
		} // RemoveParserListener

		// ----------------------------------------------------------------------
		public void Parse( IRtfSource rtfTextSource )
		{
			if ( rtfTextSource == null )
			{
				throw new ArgumentNullException( "rtfTextSource" );
			}
			DoParse( rtfTextSource );
		} // Parse

		// ----------------------------------------------------------------------
		protected abstract void DoParse( IRtfSource rtfTextSource );

		// ----------------------------------------------------------------------
		protected void NotifyParseBegin()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.ParseBegin();
				}
			}
		} // NotifyParseBegin

		// ----------------------------------------------------------------------
		protected void NotifyGroupBegin()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.GroupBegin();
				}
			}
		} // NotifyGroupBegin

		// ----------------------------------------------------------------------
		protected void NotifyTagFound( IRtfTag tag )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.TagFound( tag );
				}
			}
		} // NotifyTagFound

		// ----------------------------------------------------------------------
		protected void NotifyTextFound( IRtfText text )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.TextFound( text );
				}
			}
		} // NotifyTextFound

		// ----------------------------------------------------------------------
		protected void NotifyGroupEnd()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.GroupEnd();
				}
			}
		} // NotifyGroupEnd

		// ----------------------------------------------------------------------
		protected void NotifyParseSuccess()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.ParseSuccess();
				}
			}
		} // NotifyParseSuccess

		// ----------------------------------------------------------------------
		protected void NotifyParseFail( RtfException reason )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.ParseFail( reason );
				}
			}
		} // NotifyParseFail

		// ----------------------------------------------------------------------
		protected void NotifyParseEnd()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfParserListener listener in this.listeners )
				{
					listener.ParseEnd();
				}
			}
		} // NotifyParseEnd

		// ----------------------------------------------------------------------
		// members
		private bool ignoreContentAfterRootGroup;
		private ArrayList listeners;

	} // class RtfParserBase

} // namespace Itenso.Rtf.Parser
// -- EOF -------------------------------------------------------------------
