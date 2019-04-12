

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Calendar
{
    public interface ITool
    {
        DayView DayView
        {
            get;
            set;
        }

        void Reset();

        void MouseMove( MouseEventArgs e );
        void MouseUp( MouseEventArgs e );
        void MouseDown( MouseEventArgs e );
    }
}
