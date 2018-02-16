using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public interface ILayout
    {
        // Returns the number of words that fitted
        int Arrange(IEnumerable<IWord> words, IGraphicEngine graphicEngine);

        IEnumerable<LayoutItem> GetWordsInArea(RectangleF area);
        SizeF GetUsedSize();
		SizeF GetSize();
    }
}