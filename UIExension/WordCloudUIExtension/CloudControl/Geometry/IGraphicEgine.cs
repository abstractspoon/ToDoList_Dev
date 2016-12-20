using System;
using System.Drawing;

namespace Gma.CodeCloud.Controls.Geometry
{
    public interface IGraphicEngine : IDisposable
    {
        SizeF Measure(string text, int weight);
        void Draw(LayoutItem layoutItem);
        void DrawEmphasized(LayoutItem layoutItem);
    }
}
