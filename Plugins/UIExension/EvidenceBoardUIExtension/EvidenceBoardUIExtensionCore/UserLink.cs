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

	public class UserLinkTarget
	{
		private uint m_Id = 0;
		private PointF m_ImageCoords = new PointF(-1, -1);
		private bool m_UsesId = false;

		// -------------------------------------------------

		public UserLinkTarget(uint toId)
		{
			Set(toId);
		}

		public UserLinkTarget(PointF relativeImageCoords)
		{
			Set(relativeImageCoords);
		}

		public uint Id { get { return m_Id; } }
		public bool UsesId { get { return m_UsesId; } }
		public PointF RelativeImageCoords {	get { return m_ImageCoords; } }

		public bool IsValid(uint fromId)
		{
			if (!IsValid())
				return false;

			// else
			return (!m_UsesId || (m_Id != fromId));
		}

		public static bool IsValid(PointF ptImage)
		{
			return ((ptImage.X >= 0) ||
					(ptImage.X <= 1) ||
					(ptImage.Y >= 0) ||
					(ptImage.Y <= 1));
		}

		public bool IsValid()
		{
			if (m_UsesId)
				return (m_Id != 0);

			// else
			return IsValid(m_ImageCoords);
		}

		public bool Set(UserLinkTarget target)
		{
			if (target.m_UsesId)
				return Set(target.Id);

			// else
			return Set(target.RelativeImageCoords);
		}

		public bool Set(uint toId)
		{
			if (toId == 0)
			{
				Debug.Assert(false);
				return false;
			}

			m_Id = toId;
			m_UsesId = true;
			m_ImageCoords = PointF.Empty;

			return true;
		}

		public bool Set(PointF relativeImageCoords)
		{
			if (!IsValid(relativeImageCoords))
			{
				Debug.Assert(false);
				return false;
			}

			m_ImageCoords = relativeImageCoords;
			m_Id = 0;
			m_UsesId = false;

			return true;
		}

		public static bool TryParse(string input, uint taskId, out UserLinkTarget target)
		{
			uint toId;

			if (uint.TryParse(input, out toId))
			{
				if (toId == taskId)
				{
					Debug.Assert(false);

					target = null;
					return false;
				}

				// else
				target = new UserLinkTarget(toId);
				return target.IsValid();
			}

			// else
			float relX = 0, relY = 0;
			string[] coords = input.Split('-');

			if ((coords.Count() != 2) || 
				!float.TryParse(coords[0], out relX) ||
				!float.TryParse(coords[1], out relY))
			{
				target = null;
				return false;
			}

			// else
			target = new UserLinkTarget(new PointF(relX, relY));
			return target.IsValid();
		}

		public string Encode()
		{
			if (Id != 0)
				return Id.ToString();

			// else
			return string.Format("{0}-{1}", m_ImageCoords.X, m_ImageCoords.Y);
		}
	}

	///////////////////////////////////////////////////////////////////////

	public class UserLink
	{
		public UserLink(uint fromId, UserLinkTarget target, UserLinkAttributes attrib)
		{
			Debug.Assert((fromId != 0) && target.IsValid() && (fromId != target.Id));

			FromId = fromId;
			Target = target;

			if (attrib != null)
				Attributes.Copy(attrib);
		}

		public UserLink(uint fromId, uint toId, UserLinkAttributes attrib) 
			: 
			this(fromId, new UserLinkTarget(toId), attrib)
		{
		}

		public UserLink(uint fromId, PointF relativeImageCoords, UserLinkAttributes attrib)
			: 
			this(fromId, new UserLinkTarget(relativeImageCoords), attrib)
		{
		}

		public uint FromId { get; private set; } = 0;
		public UserLinkTarget Target { get; private set; } = new UserLinkTarget(PointF.Empty);

		public UserLinkAttributes Attributes { get; private set; } = new UserLinkAttributes();

		public bool ChangeToId(uint toId)
		{
			if ((toId == 0) || (toId == FromId))
			{
				Debug.Assert(false);
				return false;
			}

			Target.Set(toId);

			return true;
		}

		public bool IdsMatch(uint fromId, uint toId)
		{
			return ((FromId == fromId) && 
					(toId != 0) && 
					(Target.Id == toId));
		}

		public bool IdsMatch(UserLink other)
		{
			if (other == null)
				return false;

			return ((FromId == other.FromId) && 
					(Target.Id == other.Target.Id) && 
					(Target.RelativeImageCoords == other.Target.RelativeImageCoords));
		}

		public bool SetTarget(UserLinkTarget target)
		{
			return Target.Set(target);

		}

		public bool SetRelativeImageCoords(PointF relativeImageCoords)
		{
			return Target.Set(relativeImageCoords);
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

			UserLinkTarget target;
			int argb = 0, thickness = 0, arrows = 0;

			if (!UserLinkTarget.TryParse(parts[IdIndex], taskId, out target))
				return false;

			if (!int.TryParse(parts[ColorIndex], out argb) ||
				!int.TryParse(parts[ThicknessIndex], out thickness) ||
				!int.TryParse(parts[ArrowsIndex], out arrows))
			{
				return false;
			}

			// some validation
			if ((thickness < 0) || (arrows < 0) || (arrows >= 4))
				return false;

			var attrib = new UserLinkAttributes()
			{
				Color = ((argb == 0) ? Color.Empty : Color.FromArgb(argb)),
				Thickness = ((thickness == 0) ? 1 : thickness),
				Arrows = (UserLinkAttributes.EndArrows)arrows,
				Label = ((parts.Count() > LabelIndex) ? parts[LabelIndex].DecodeBase64() : string.Empty),
				Type = ((parts.Count() > TypeIndex) ? parts[TypeIndex].DecodeBase64() : string.Empty),
			};

			link = new UserLink(taskId, target, attrib);
			return true;
		}

		public string Encode()
		{
			return string.Format("{0}:{1}:{2}:{3}:{4}:{5}", 
								Target.Encode(), 
								Attributes.Color.ToArgb(), 
								Attributes.Thickness, 
								(int)Attributes.Arrows,
								Attributes.Label.EncodeBase64(),
								Attributes.Type.EncodeBase64());
		}
	}

}

