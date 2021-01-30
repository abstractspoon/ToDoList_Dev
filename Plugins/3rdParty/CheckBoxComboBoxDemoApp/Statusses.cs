using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PresentationControls;

namespace DemoApp
{
    /// <summary>
    /// Class used for demo purposes. This could be anything listed in a CheckBoxComboBox.
    /// </summary>
    public class Status
    {
        public Status(int id, string name) { _Id = id; _Name = name; }

        private int _Id;
        private string _Name;

        public int Id { get { return _Id; } set { _Id = value; } }
        public string Name { get { return _Name; } set { _Name = value; } }

        /// <summary>
        /// Now used to return the Name.
        /// </summary>
        /// <returns></returns>
        public override string ToString() { return Name; }
    }
    /// <summary>
    /// Class used for demo purposes. A list of "Status". 
    /// This represents the custom "IList" datasource of anything listed in a CheckBoxComboBox.
    /// </summary>
    public class StatusList : List<Status>
    {
    }

}
