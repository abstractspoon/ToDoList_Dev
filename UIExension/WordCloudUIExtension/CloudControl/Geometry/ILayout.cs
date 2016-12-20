using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public interface ILayout
    {
        void Arrange(IEnumerable<IWord> words, IGraphicEngine graphicEngine);
        IEnumerable<LayoutItem> GetWordsInArea(RectangleF area);
    }
}