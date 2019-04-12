using System;
using System.Drawing;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public interface IGraphicEngine : IDisposable
    {
        SizeF Measure(IWord word);
        void Draw(LayoutItem layoutItem);
        void DrawEmphasized(LayoutItem layoutItem);
    }
}
