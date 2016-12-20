using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TagCloud
{
  public partial class AddItem : Form
  {
    private string m_String;

    /// <summary>
    /// Returns the string entered into the text field
    /// </summary>
    public string StringToBeAdded
    {
      get { return m_String; }
    }

    /// <summary>
    /// Constructor
    /// </summary>
    public AddItem()
    {
      InitializeComponent();
      // if return or enter is pressed: handle like button Add
      this.AcceptButton = buttonAdd;
    }

    /// <summary>
    /// Botton "Add" handling: gets the textbox contents and ends the dialog box
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void buttonAdd_Click(object sender, EventArgs e)
    {
      m_String = textBoxAdd.Text.Trim(); 
    }
   
  }
}
