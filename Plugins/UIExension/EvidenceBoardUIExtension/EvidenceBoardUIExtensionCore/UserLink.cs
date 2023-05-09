using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Diagnostics;

using Abstractspoon.Tdl.PluginHelpers;

namespace EvidenceBoardUIExtension
{
	public class UserLinkAttributes
	{
		// ------------------------------------------------------------

		[Flags]
		public enum Mask
		{
			None		= 0x00,

			Color		= 0x01,
			Thickness	= 0x02,
			Label		= 0x04,
			Type		= 0x08,
			Arrows		= 0x10,

			All			= 0xff
		}

		// ------------------------------------------------------------

		public enum EndArrows
		{
			None,
			Start,
			Finish,
			Both
		}

		// ---------------------------------------------------

		public static UserLinkAttributes Defaults { get; private set; } = new UserLinkAttributes();

		// ---------------------------------------------------

		private Color m_Color = Color.Empty;
		private int m_Thickness = 1;

		// ---------------------------------------------------

		private Color DefaultColor = Color.Red;
		private int DefaultThickness = 1;
		private EndArrows DefaultArrows = EndArrows.Finish;
		private string DefaultLabel = string.Empty;
		private string DefaultType = string.Empty;

		// ---------------------------------------------------

		public UserLinkAttributes()
		{
			Color = DefaultColor;
			Thickness = DefaultThickness;
			Arrows = DefaultArrows;
			Label = DefaultLabel;
			Type = DefaultType;
		}

		public Color Color
		{
			get { return (m_Color == Color.Empty) ? Defaults.Color : m_Color; }
			set { m_Color = value; }
		}

		public int Thickness
		{
			get { return m_Thickness; }
			set { m_Thickness = Math.Min(Math.Max(1, value), 5); }
		}

		public EndArrows Arrows = EndArrows.None;
		public string Label = string.Empty;
		public string Type = string.Empty;

		public bool Copy(UserLinkAttributes other, Mask mask = Mask.All)
		{
			if ((mask == Mask.None) || Matches(other, mask))
				return false;

			if (mask.HasFlag(Mask.Thickness))
				Thickness = other.Thickness;

			if (mask.HasFlag(Mask.Arrows))
				Arrows = other.Arrows;

			if (mask.HasFlag(Mask.Color))
				Color = other.Color;

			if (mask.HasFlag(Mask.Label))
				Label = other.Label;

			if (mask.HasFlag(Mask.Type))
				Type = other.Type;

			return true;
		}

		public bool Matches(UserLinkAttributes other, Mask mask = Mask.All)
		{
			if (mask == Mask.None)
				return false;

			if (mask.HasFlag(Mask.Thickness) && (other.Thickness != Thickness))
				return false;

			if (mask.HasFlag(Mask.Arrows) && (other.Arrows != Arrows))
				return false;

			if (mask.HasFlag(Mask.Color) && (other.Color != Color))
				return false;

			if (mask.HasFlag(Mask.Label) && (other.Label != Label))
				return false;

			if (mask.HasFlag(Mask.Type) && (other.Type != Type))
				return false;

			return true;
		}
	}

	///////////////////////////////////////////////////////////////////////

	public class UserLink
	{
		public UserLink(uint fromId, uint toId, UserLinkAttributes attrib)
		{
			Debug.Assert((fromId != 0) && (toId != 0) && (fromId != toId));

			FromId = fromId;
			ToId = toId;

			if (attrib != null)
				Attributes.Copy(attrib);
		}

		public uint FromId { get; private set; } = 0;
		public uint ToId { get; private set; } = 0;

		public UserLinkAttributes Attributes { get; private set; } = new UserLinkAttributes();

		public bool ChangeToId(uint toId)
		{
			if ((toId == 0) || (toId == FromId))
				return false;

			ToId = toId;
			return true;
		}

		public bool IdsMatch(uint fromId, uint toId)
		{
			return ((FromId == fromId) && (ToId == toId));
		}

		public bool IdsMatch(UserLink other)
		{
			if (other == null)
				return false;

			return ((FromId == other.FromId) && (ToId == other.ToId));
		}

		public override string ToString()
		{
			return Encode();
		}

		public static bool TryParse(string input, uint taskId, out UserLink link)
		{
			const int IdIndex = 0;
			const int ColorIndex = 1;
			const int ThicknessIndex = 2;
			const int ArrowsIndex = 3;
			const int LabelIndex = 4;
			const int TypeIndex = 5;

			link = null;
			string[] parts = input.Split(':');

			if (parts.Count() <= ArrowsIndex)
				return false;

			uint toId;
			int argb, thickness, arrows;

			if (!uint.TryParse(parts[IdIndex], out toId) ||
				!int.TryParse(parts[ColorIndex], out argb) ||
				!int.TryParse(parts[ThicknessIndex], out thickness) ||
				!int.TryParse(parts[ArrowsIndex], out arrows))
			{
				return false;
			}

			// some validation
			if ((toId <= 0) || (thickness < 0) || (arrows < 0) || (arrows >= 4))
				return false;

			if (toId == taskId)
			{
				Debug.Assert(false);
				return false;
			}

			var attrib = new UserLinkAttributes()
			{
				Color = ((argb == 0) ? Color.Empty : Color.FromArgb(argb)),
				Thickness = ((thickness == 0) ? 1 : thickness),
				Arrows = (UserLinkAttributes.EndArrows)arrows,
				Label = ((parts.Count() > LabelIndex) ? parts[LabelIndex].DecodeBase64() : string.Empty),
				Type = ((parts.Count() > TypeIndex) ? parts[TypeIndex].DecodeBase64() : string.Empty),
			};

			link = new UserLink(taskId, toId, attrib);
			return true;
		}

		public string Encode()
		{
			return string.Format("{0}:{1}:{2}:{3}:{4}:{5}", 
								ToId.ToString(), 
								Attributes.Color.ToArgb(), 
								Attributes.Thickness.ToString(), 
								((int)Attributes.Arrows).ToString(),
								Attributes.Label.EncodeBase64(),
								Attributes.Type.EncodeBase64());
		}
	}

}

