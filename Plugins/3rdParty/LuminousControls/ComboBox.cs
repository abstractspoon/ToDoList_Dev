#region License
// Copyright © 2014 Łukasz Świątkowski
// http://www.lukesw.net/
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library.  If not, see <http://www.gnu.org/licenses/>.
#endregion

namespace Luminous.Windows.Forms
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Data;
    using System.Drawing;
    using System.Text;
    using System.Windows.Forms;
    using System.Reflection;

    /// <summary>
    /// Represents a Windows combo box control which can be used in a popup's content control.
    /// </summary>
    [ToolboxBitmap(typeof(System.Windows.Forms.ComboBox)), ToolboxItem(true), ToolboxItemFilter("System.Windows.Forms"), Description("Displays an editable text box with a drop-down list of permitted values.")]
    public partial class ComboBox : System.Windows.Forms.ComboBox
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PopupControl.ComboBox" /> class.
        /// </summary>
        public ComboBox()
        {
            InitializeComponent();
        }

        private static Type _modalMenuFilter;
        private static Type modalMenuFilter
        {
            get
            {
                if (_modalMenuFilter == null)
                {
                    _modalMenuFilter = Type.GetType("System.Windows.Forms.ToolStripManager+ModalMenuFilter");
                }
                if (_modalMenuFilter == null)
                {
                    _modalMenuFilter = new List<Type>(typeof(ToolStripManager).Assembly.GetTypes())
                        .Find(type => type.FullName == "System.Windows.Forms.ToolStripManager+ModalMenuFilter");
                }
                return _modalMenuFilter;
            }
        }

        private static MethodInfo _suspendMenuMode;
        private static MethodInfo suspendMenuMode
        {
            get
            {
                if (_suspendMenuMode == null)
                {
                    Type t = modalMenuFilter;
                    if (t != null)
                    {
                        _suspendMenuMode = t.GetMethod("SuspendMenuMode", BindingFlags.Static | BindingFlags.NonPublic | BindingFlags.Public);
                    }
                }
                return _suspendMenuMode;
            }
        }

        private static void SuspendMenuMode()
        {
            MethodInfo suspendMenuMode = ComboBox.suspendMenuMode;
            if (suspendMenuMode != null)
            {
                suspendMenuMode.Invoke(null, null);
            }
        }

        private static MethodInfo _resumeMenuMode;
        private static MethodInfo resumeMenuMode
        {
            get
            {
                if (_resumeMenuMode == null)
                {
                    Type t = modalMenuFilter;
                    if (t != null)
                    {
                        _resumeMenuMode = t.GetMethod("ResumeMenuMode", BindingFlags.Static | BindingFlags.NonPublic | BindingFlags.Public);
                    }
                }
                return _resumeMenuMode;
            }
        }

        private static void ResumeMenuMode()
        {
            MethodInfo resumeMenuMode = ComboBox.resumeMenuMode;
            if (resumeMenuMode != null)
            {
                resumeMenuMode.Invoke(null, null);
            }
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.Forms.ComboBox.DropDown" /> event.
        /// </summary>
        /// <param name="e">An <see cref="T:System.EventArgs" /> that contains the event data.</param>
        protected override void OnDropDown(EventArgs e)
        {
            base.OnDropDown(e);
            SuspendMenuMode();
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.Forms.ComboBox.DropDownClosed" /> event.
        /// </summary>
        /// <param name="e">An <see cref="T:System.EventArgs" /> that contains the event data.</param>
        protected override void OnDropDownClosed(EventArgs e)
        {
            ResumeMenuMode();
            base.OnDropDownClosed(e);
        }
    }
}
