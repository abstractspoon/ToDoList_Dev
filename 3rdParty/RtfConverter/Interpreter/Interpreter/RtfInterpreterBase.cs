// -- FILE ------------------------------------------------------------------
// name       : RtfInterpreterBase.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.21
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Collections;

namespace Itenso.Rtf.Interpreter
{

	// ------------------------------------------------------------------------
	public abstract class RtfInterpreterBase : IRtfInterpreter
	{

		// ----------------------------------------------------------------------
		protected RtfInterpreterBase(
			params IRtfInterpreterListener[] listeners
		)
		{
			if ( listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in listeners )
				{
					AddInterpreterListener( listener );
				}
			}
		} // RtfInterpreterBase

		// ----------------------------------------------------------------------
		public void AddInterpreterListener( IRtfInterpreterListener listener )
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
		} // AddInterpreterListener

		// ----------------------------------------------------------------------
		public void RemoveInterpreterListener( IRtfInterpreterListener listener )
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
		} // RemoveInterpreterListener

		// ----------------------------------------------------------------------
		public void Interpret( IRtfGroup rtfDocument )
		{
			if ( rtfDocument == null )
			{
				throw new ArgumentNullException( "rtfDocument" );
			}
			DoInterpret( rtfDocument );
		} // Interpret

		// ----------------------------------------------------------------------
		protected abstract void DoInterpret( IRtfGroup rtfDocument );

		// ----------------------------------------------------------------------
		protected void NotifyBeginDocument()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.BeginDocument( this.context );
				}
			}
		} // NotifyBeginDocument

		// ----------------------------------------------------------------------
		protected void NotifyInsertText( string text )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.InsertText( this.context, text );
				}
			}
		} // NotifyInsertText

		// ----------------------------------------------------------------------
		protected void NotifyInsertSpecialChar( RtfVisualSpecialCharKind kind )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.InsertSpecialChar( this.context, kind );
				}
			}
		} // NotifyInsertSpecialChar

		// ----------------------------------------------------------------------
		protected void NotifyInsertBreak( RtfVisualBreakKind kind )
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.InsertBreak( this.context, kind );
				}
			}
		} // NotifyInsertBreak

		// ----------------------------------------------------------------------
		protected void NotifyInsertImage( RtfVisualImageFormat format,
			int width, int height, int desiredWidth, int desiredHeight,
			int scaleWidthPercent, int scaleHeightPercent, string imageDataHex
		)
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.InsertImage(
						this.context,
						format,
						width,
						height,
						desiredWidth,
						desiredHeight,
						scaleWidthPercent,
						scaleHeightPercent,
						imageDataHex );
				}
			}
		} // NotifyInsertImage

		// ----------------------------------------------------------------------
		protected void NotifyEndDocument()
		{
			if ( this.listeners != null )
			{
				foreach ( IRtfInterpreterListener listener in this.listeners )
				{
					listener.EndDocument( this.context );
				}
			}
		} // NotifyEndDocument

		// ----------------------------------------------------------------------
		protected RtfInterpreterContext Context
		{
			get { return context; }
		} // Context

		// ----------------------------------------------------------------------
		// members
		private readonly RtfInterpreterContext context = new RtfInterpreterContext();
		private ArrayList listeners;

	} // class RtfInterpreterBase

} // namespace Itenso.Rtf.Interpreter
// -- EOF -------------------------------------------------------------------
