// -- FILE ------------------------------------------------------------------
// name       : RtfDocumentProperty.cs
// project    : RTF Framelet
// created    : Leon Poyyayil - 2008.05.23
// language   : c#
// environment: .NET 2.0
// copyright  : (c) 2004-2009 by Itenso GmbH, Switzerland
// --------------------------------------------------------------------------
using System;
using System.Text;
using Itenso.Sys;

namespace Itenso.Rtf.Model
{

	// ------------------------------------------------------------------------
	public sealed class RtfDocumentProperty : IRtfDocumentProperty
	{

		// ----------------------------------------------------------------------
		public RtfDocumentProperty( int propertyKindCode, string name, string staticValue ) :
			this( propertyKindCode, name, staticValue, null )
		{
		} // RtfDocumentProperty

		// ----------------------------------------------------------------------
		public RtfDocumentProperty( int propertyKindCode, string name, string staticValue, string linkValue )
		{
			if ( name == null )
			{
				throw new ArgumentNullException( "name" );
			}
			if ( staticValue == null )
			{
				throw new ArgumentNullException( "staticValue" );
			}
			this.propertyKindCode = propertyKindCode;
			switch ( propertyKindCode )
			{
				case RtfSpec.PropertyTypeInteger:
					this.propertyKind = RtfPropertyKind.IntegerNumber;
					break;
				case RtfSpec.PropertyTypeRealNumber:
					this.propertyKind = RtfPropertyKind.RealNumber;
					break;
				case RtfSpec.PropertyTypeDate:
					this.propertyKind = RtfPropertyKind.Date;
					break;
				case RtfSpec.PropertyTypeBoolean:
					this.propertyKind = RtfPropertyKind.Boolean;
					break;
				case RtfSpec.PropertyTypeText:
					this.propertyKind = RtfPropertyKind.Text;
					break;
				default:
					this.propertyKind = RtfPropertyKind.Unknown;
					break;
			}
			this.name = name;
			this.staticValue = staticValue;
			this.linkValue = linkValue;
		} // RtfDocumentProperty

		// ----------------------------------------------------------------------
		public int PropertyKindCode
		{
			get { return this.propertyKindCode; }
		} // PropertyKindCode

		// ----------------------------------------------------------------------
		public RtfPropertyKind PropertyKind
		{
			get { return this.propertyKind; }
		} // PropertyKind

		// ----------------------------------------------------------------------
		public string Name
		{
			get { return this.name; }
		} // Name

		// ----------------------------------------------------------------------
		public string StaticValue
		{
			get { return this.staticValue; }
		} // StaticValue

		// ----------------------------------------------------------------------
		public string LinkValue
		{
			get { return this.linkValue; }
		} // LinkValue

		// ----------------------------------------------------------------------
		public override bool Equals( object obj )
		{
			if ( obj == this )
			{
				return true;
			}
			
			if ( obj == null || GetType() != obj.GetType() )
			{
				return false;
			}

			return IsEqual( obj );
		} // Equals

		// ----------------------------------------------------------------------
		private bool IsEqual( object obj )
		{
			RtfDocumentProperty compare = obj as RtfDocumentProperty; // guaranteed to be non-null
			return
				compare != null &&
				this.propertyKindCode == compare.propertyKindCode &&
				this.propertyKind == compare.propertyKind &&
				this.name.Equals( compare.name ) &&
				CompareTool.AreEqual( this.staticValue, compare.staticValue ) &&
				CompareTool.AreEqual( this.linkValue, compare.linkValue );
		} // IsEqual

		// ----------------------------------------------------------------------
		public override int GetHashCode()
		{
			return HashTool.AddHashCode( GetType().GetHashCode(), ComputeHashCode() );
		} // GetHashCode

		// ----------------------------------------------------------------------
		private int ComputeHashCode()
		{
			int hash = this.propertyKindCode;
			hash = HashTool.AddHashCode( hash, this.propertyKind );
			hash = HashTool.AddHashCode( hash, this.name );
			hash = HashTool.AddHashCode( hash, this.staticValue );
			hash = HashTool.AddHashCode( hash, this.linkValue );
			return hash;
		} // ComputeHashCode

		// ----------------------------------------------------------------------
		public override string ToString()
		{
			StringBuilder buf = new StringBuilder( this.name );
			if ( staticValue != null )
			{
				buf.Append( "=" );
				buf.Append( this.staticValue );
			}
			if ( linkValue != null )
			{
				buf.Append( "@" );
				buf.Append( this.linkValue );
			}
			return buf.ToString();
		} // ToString

		// ----------------------------------------------------------------------
		// members
		private readonly int propertyKindCode;
		private readonly RtfPropertyKind propertyKind;
		private readonly string name;
		private readonly string staticValue;
		private readonly string linkValue;

	} // class RtfDocumentProperty

} // namespace Itenso.Rtf.Model
// -- EOF -------------------------------------------------------------------
