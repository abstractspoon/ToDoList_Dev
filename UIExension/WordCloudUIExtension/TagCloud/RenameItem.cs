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
  public partial class RenameItem : Form
  {
    private string m_FromString;
    private string m_ToString;

    /// <summary>
    /// Returns the string entered into the text field To:
    /// </summary>
    public string NewString
    {
      get { return m_ToString; }
    }

    /// <summary>
    /// Sets the old string into the textbox
    /// </summary>
    public string OldString
    {
      set
      {
        m_FromString = value;
        textBoxFrom.Text = m_FromString; 
      }
    }

    /// <summary>
    /// Constructor
    /// </summary>
    public RenameItem()
    {
      InitializeComponent();
      // if return or enter is pressed: handle like button Rename
      this.AcceptButton = buttonRename;
    }

    /// <summary>
    /// Botton "Rename" handling: gets the new textbox contents and ends the dialog box
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void buttonRename_Click(object sender, EventArgs e)
    {
      m_ToString = textBoxTo.Text.Trim(); 
    }

  }
}
