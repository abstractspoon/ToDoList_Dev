using System.Drawing;

namespace WinFormsSyntaxHighlighter
{
    public class SyntaxStyle
    {
        public bool Bold { get; set; }
        public bool Italic { get; set; }
        public Color TextColor { get; set; }

        public SyntaxStyle(Color textColor, bool bold, bool italic)
        {
			TextColor = textColor;
            Bold = bold;
            Italic = italic;
        }

        public SyntaxStyle(Color textColor)
            : this(textColor, false, false)
        {
        }

		public SyntaxStyle(SyntaxStyle other)
			: this(other.TextColor, other.Bold, other.Italic)
		{
		}
	}
}
