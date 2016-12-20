using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Globalization;

namespace TagCloud
{
  public partial class TagCloudControl : UserControl
  {
    #region Class CloudItem

    private class CloudItem
    {
      public Rectangle Position;
      public OutputDesign Design;
      public double Occurrence;
      public bool Touched;

      /// <summary>
      /// Constructor
      /// </summary>
      /// <param name="occurrence"></param>
      /// <param name="show"></param>
      public CloudItem(double occurrence)
      {
        Occurrence = occurrence;
        Touched = false;
      }
    }    // class CloudItem
    
    #endregion

    #region Class OutputDesign

    public class OutputDesign
    {
      public SolidBrush drawBrush = new System.Drawing.SolidBrush(System.Drawing.Color.MidnightBlue);
      public Font drawFont = new System.Drawing.Font("Calibri", 10.0f, FontStyle.Bold);

      /// <summary>
      /// Constructor
      /// </summary>
      /// <param name="font"></param>
      /// <param name="size"></param>
      /// <param name="color"></param>
      public OutputDesign(string font, float size, Color color)
      {
        this.drawBrush = new System.Drawing.SolidBrush(color);
        this.drawFont = new System.Drawing.Font(font, size, FontStyle.Bold);
      }
    } // class OutputDesign

    #endregion

    #region Class StringItem

    public class StringItem
    {
      public double Occurrence;
      public bool Show;
      public long LastAccessTime;
      public string Text;

      /// <summary>
      /// Constructor
      /// </summary>
      /// <param name="occurrence"></param>
      /// <param name="show"></param>
      public StringItem(string text, double occurrence, bool show)
      {
        Text = text;
        Occurrence = occurrence;
        Show = show;
        // take the actual timestamp for last access time
        LastAccessTime = DateTime.Now.Ticks;
      }

      /// <summary>
      /// Constructor
      /// </summary>
      /// <param name="occurrence"></param>
      /// <param name="show"></param>
      /// <param name="last"></param>
      public StringItem(string text, double occurrence, bool show, long last)
      {
        Text = text;
        Occurrence = occurrence;
        Show = show;
        LastAccessTime = last;
      }
    
    } // class StringItem

    #endregion

    #region Variables

    // lists
    // CloudCollection comprises all tags shown in the cloud
    private static SortedList<string, CloudItem> CloudCollection = new SortedList<string, CloudItem>();
    // The output designs: how the tags are presented, depending on their weight (occurrence)
    private static List<OutputDesign> OutputDesigns = new List<OutputDesign>();
    // StringCollection manages all items
    public static SortedList<string, StringItem> StringCollection = new SortedList<string, StringItem>();
    // StringCollection manages all items shown in the cloud (<= StringCollection)
    private static SortedList<string, StringItem> StringCollection_Shown = new SortedList<string, StringItem>();

    // private variables
    private int m_ControlWidth;                            // control's width
    private int m_ControlHeight;                           // control's height
    private Color m_ControlBackColor = Color.MintCream;    // control's back color
    private bool m_Frame = false;                          // true: text item (tag) is surrounded by a rectangle
    private bool m_Underline = false;                      // true: text item will be underlined on mouse hover
    private static int m_LastTouched = -100;               // last touched text item (tag)

    public enum TextLanguage
    {
      English,
      German,
      HTML
    }

    #endregion

    #region Events

    [Description("Delegates a click onto the user control to the wrapper")]
    // delegate declaration
    public delegate void OnUserControlClick(string Text, double Occurrence);
    // event declaration
    public event OnUserControlClick clickHandler;
    
    #endregion
    
    #region Properties

    /// <summary>
    /// Sets or gets the control's back color
    /// </summary>
    public Color ControlBackColor
    {
      get { return m_ControlBackColor; }
      set
      {
        m_ControlBackColor = value;
        this.Refresh();
      }
    }
    
    /// <summary>
    /// Sets or gets the condition for drawing a rectangle
    /// around the text strings.
    /// </summary>
    public bool ControlTextFrame
    {
      get { return m_Frame; }
      set
      {
        m_Frame = value;
        this.Refresh();
      }
    }

    /// <summary>
    /// 
    /// </summary>
    public bool ControlTextUnderline
    {
      get { return m_Underline; }
      set
      {
        m_Underline = value;
        this.Refresh();
      }
    }

    /// <summary>
    /// Sets or gets the control's height.
    /// Due to a height change, the cloud is set and painted new.
    /// </summary>
    public int ControlHeight
    {
      get { return m_ControlHeight; }
      set
      {
        m_ControlHeight = this.Height = value;
        SetCloud();
        this.Refresh();
      }
    }

    /// <summary>
    /// Sets or gets the control's width.
    /// Due to a width change, the cloud is set and painted new.
    /// </summary>
    public int ControlWidth
    {
      get { return m_ControlWidth; }
      set
      {
        m_ControlWidth = this.Width = value;
        SetCloud();
        this.Refresh();
      }
    }

    /// <summary>
    /// Gets the most clicked text item from
    /// the list with text items (StringCollection).
    /// If there is more than one text item with the 
    /// same most occurence, the first one in alphabetical
    /// order will be returned.
    /// </summary>
    public string MostClickedItem
    {
      get
      {
        double most = 0;
        string moststring = "";
        int i = 0;
        foreach (StringItem si in StringCollection.Values)
        {
          if (si.Occurrence > most)
          {
            most = si.Occurrence;
            moststring = StringCollection.Keys[i];  
          }
          i++;
        }
        return moststring; 
      }
    }

    /// <summary>
    /// Gets the least clicked text item from
    /// the list with text items (StringCollection).
    /// If there is more than one text item with the 
    /// same least occurence, the first one in alphabetical
    /// order will be returned.
    /// </summary>
    public string LeastClickedItem
    {
      get
      {
        double least = double.MaxValue;
        string leaststring = "";
        int i = 0;
        foreach (StringItem si in StringCollection.Values)
        {
          if (si.Occurrence < least)
          {
            least = si.Occurrence;
            leaststring = StringCollection.Keys[i];
          }
          i++;
        }
        return leaststring;
      }
    }

    /// <summary>
    /// Gets the oldest clicked text item from
    /// the list with text items (StringCollection).
    /// If there is more than one text item with the 
    /// same oldest access time, the first one in alphabetical
    /// order will be returned.
    /// </summary>
    public string OldestClickedItem
    {
      get
      {
        long oldest = long.MaxValue;
        string oldeststring = "";
        int i = 0;
        foreach (StringItem si in StringCollection.Values)
        {
          if (si.LastAccessTime < oldest)
          {
            oldest = si.LastAccessTime;
            oldeststring = StringCollection.Keys[i];
          }
          i++;
        }
        return oldeststring;
      }
    }

    /// <summary>
    /// Gets the youngest clicked text item from
    /// the list with text items (StringCollection).
    /// If there is more than one text item with the 
    /// same youngest access time, the first one in alphabetical
    /// order will be returned.
    /// </summary>
    public string YoungestClickedItem
    {
      get
      {
        long youngest = 0;
        string youngeststring = "";
        int i = 0;
        foreach (StringItem si in StringCollection.Values)
        {
          if (si.LastAccessTime > youngest)
          {
            youngest = si.LastAccessTime;
            youngeststring = StringCollection.Keys[i];
          }
          i++;
        }
        return youngeststring;
      }
    }

    /// <summary>
    /// Gets the occurrence of the least clicked item.
    /// </summary>
    public double LeastOccurence
    {
      get
      {
        double least = double.MaxValue;
        bool found = false;
        foreach (StringItem si in StringCollection.Values)
        {
          if (si.Occurrence < least)
          {
            least = si.Occurrence;
            found = true;
          }
        }
        return (found)? least : 0;
      }
    }

    /// <summary>
    /// Gets the number of the items in StringCollection
    /// </summary>
    /// <returns></returns>
    public int ItemsCount
    {
      get
      {
        return StringCollection.Count;
      }
    }

    /// <summary>
    /// Gets the number of the items in StringCollection_Red
    /// </summary>
    /// <returns></returns>
    public int ShownItemsCount
    {
      get
      {
        return StringCollection_Shown.Count;
      }
    }

    #endregion

    #region Methods for OutputDesign Management

    /// <summary>
    /// Sets the 5 predefined output designs into the OutputDesigns collection.
    /// An output design consists of the text style, the text size and the text color.
    /// If update = true, the cloud will be painted new.
    /// </summary>
    /// <param name="update">true: update the cloud</param>
    public void SetAllDesigns(bool update)
    {
      OutputDesigns.Clear();
      OutputDesigns.Add(new OutputDesign("Calibri", 10.0f, Color.FromArgb(131, 188, 216)));
      OutputDesigns.Add(new OutputDesign("Calibri", 10.0f, Color.FromArgb(65, 136, 207)));
      OutputDesigns.Add(new OutputDesign("Calibri", 12.0f, Color.FromArgb(0, 158, 255)));
      OutputDesigns.Add(new OutputDesign("Calibri", 14.0f, Color.FromArgb(0, 158, 250)));
      OutputDesigns.Add(new OutputDesign("Calibri", 16.0f, Color.FromArgb(255, 153, 0)));
      if (update)
      {
        SetCloud();
        this.Refresh();
      }
    }

    /// <summary>
    /// Sets an output design into the list OutputDesigns.
    /// The list comprises 5 different designs, so the standard list 
    /// can only be overwritten by changing one of the elements 0..4.
    /// Returns true, if number >= 0 and <= 4 and successfull entry,
    /// else false
    /// </summary>
    /// <param name="number">number of element 0...4</param>
    /// <param name="font">font name, like "Arial"</param>
    /// <param name="size">font size</param>
    /// <param name="color">font color</param>
    /// <returns></returns>
    public bool SetDesign(int number, string font, float size, Color color)
    {
      if ((number > 4) || (number < 0)) return false;
      try
      {
        OutputDesign od = new OutputDesign(font, size, color);
        return SetDesign(number, od);
      }
      catch
      {
        return false;
      }
    }

    /// <summary>
    /// Overloaded
    /// Sets an output design into the list OutputDesigns.
    /// The list comprises 5 different designs, so the standard list 
    /// can only be overwritten by changing one of the elements 0..4.
    /// Returns true, if number >= 0 and <= 4 and successfull entry,
    /// </summary>
    /// <param name="number">number of element 0...4</param>
    /// <param name="od">Output design</param>
    /// <returns></returns>
    public bool SetDesign(int number, OutputDesign od)
    {
      if ((number > 4) || (number < 0)) return false;
      try
      {
        OutputDesigns[number] = od;
        SetCloud();
        this.Refresh();
        return true;
      }
      catch
      {
        return false;
      }
    }

    #endregion

    #region Methods for String Management

    /// <summary>
    /// Adds or replaces a text item to the list of items (StringCollection).
    /// If the item is new, its occurrence is set to 1,
    /// else its previous occurrence is incremented by 1.
    /// The attribute 'last' (last access) will be set to 'now'.
    /// </summary>
    /// <param name="text"></param>
    public void AddItem(string text)
    {
      if (!StringCollection.ContainsKey(text))
      {
        StringCollection.Add(text, new StringItem(text, 1, true));
      }
      else
      {
        int index = StringCollection.IndexOfKey(text);
        double occ = StringCollection.Values[index].Occurrence;
        StringCollection.Remove(text);
        StringCollection.Add(text, new StringItem(text, occ + 1, true));
      }
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Overloaded
    /// Adds a text item to the list of items (StringCollection).
    /// The attribute 'last' (last access) will be set to 'now'.
    /// </summary>
    /// <param name="text"></param>
    /// <param name="occ"></param>
    public void AddItem(string text, double occ)
    {
      if (StringCollection.ContainsKey(text))
      {
        StringCollection.Remove(text);
      }
      StringCollection.Add(text, new StringItem(text, occ, true));
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Overloaded
    /// Adds a text item to the list of items (StringCollection).
    /// The attribute 'last' (last access) will be set by last.
    /// </summary>
    /// <param name="text"></param>
    /// <param name="occ"></param>
    /// <param name="last"></param>
    public void AddItem(string text, double occ, long last)
    {
      if (StringCollection.ContainsKey(text))
      {
        StringCollection.Remove(text);
      }
      StringCollection.Add(text, new StringItem(text, occ, true, last));
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Overloaded
    /// Adds a complete list sc to the list of items (StringCollection).
    /// </summary>
    /// <param name="sc"></param>
    public void AddItem(SortedList<string, StringItem> sc)
    {
      for (int i = 0; i < sc.Count; i++)
      {
        double occ = sc.Values[i].Occurrence;
        long last = sc.Values[i].LastAccessTime;
        string text = sc.Keys[i];
        if (StringCollection.ContainsKey(text))
        {
          StringCollection.Remove(text);
        }
        StringCollection.Add(text, new StringItem(text, occ, true, last));
      }
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Overloaded
    /// Adds or replaces a text item to the list of items
    /// (StringCollection).
    /// If the item exists already in StringCollection, the variable regardmean will be interpreted:
    /// - if regardmean is true and the item is not yet member of CloudCollections, the occurrence will 
    ///   be set to the mean occurrence of the items in the StringCollection
    /// - otherwise its occurrence is incremented by 1.
    /// If the item does not exist, the variable regardmean will be interpreted:
    /// - false: the occurrence will be set to 1
    /// - true:  the occurrence will be set to the mean occurrence of the items
    ///          in the StringCollection
    /// The attribute 'last' (last access) will be set to 'now'.
    /// </summary>
    /// <param name="text"></param>
    /// <param name="regardleast"></param>
    public void AddItem(string text, bool regardmean)
    {
      if (StringCollection.ContainsKey(text))
      {
        if ((regardmean) && !CloudCollection.ContainsKey(text))
        {
          double mean = Statistics.Mean(StringWeightsShown);
          int meanint = (int)Math.Ceiling(mean);
          StringCollection.Remove(text);
          StringCollection.Add(text, new StringItem(text, (meanint <= 0) ? 1 : meanint, true));
        }
        else
        {
          int index = StringCollection.IndexOfKey(text);
          double occ = StringCollection.Values[index].Occurrence;
          StringCollection.Remove(text);
          StringCollection.Add(text, new StringItem(text, occ + 1, true));
        }
      }
      else
      {
        if (regardmean)
        {
          double mean = Statistics.Mean(StringWeightsShown);
          int meanint = (int)Math.Ceiling(mean);
          StringCollection.Add(text, new StringItem(text, (meanint <= 0) ? 1 : meanint, true));
        }
        else
        {
          StringCollection.Add(text, new StringItem(text, 1, true));
        }
      }
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Removes the item text from the item list (StringCollection).
    /// </summary>
    /// <param name="text"></param>
    public void RemoveItem(string text)
    {
      if (StringCollection.ContainsKey(text))
      {
        StringCollection.Remove(text);
        SetCloud();
        this.Refresh();
      }
    }

    /// <summary>
    /// Clears the list with the items (StringCollection) completely.
    /// </summary>
    public void ClearAllItems()
    {
      StringCollection.Clear();
      SetCloud();
      this.Refresh();
    }

    /// <summary>
    /// Returns all the items from the StringCollection.
    /// </summary>
    /// <returns></returns>
    public SortedList<string, StringItem> GetAllItems()
    {
      return StringCollection;
    }

    #endregion

    #region Methods for XML File Handling

    /// <summary>
    /// Reads the default XML file "Tags.xml" and sets its 
    /// contents into the cloud.
    /// Returns true, if successful.
    /// </summary>
    /// <returns></returns>
    public bool ReadTagFile()
    {
      SortedList<string, TagCloud.TagCloudControl.StringItem> sc = new SortedList<string, TagCloud.TagCloudControl.StringItem>();
      ClearAllItems();
      if (TagCloud.XMLHandling.ReadTagFile(out sc))
      {
        // insert the read items into the StringCollection
        AddItem(sc);
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Overloaded
    /// Reads the XML file filename and sets its 
    /// contents into the cloud.
    /// Returns true, if successful.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public bool ReadTagFile(string filename)
    {
      SortedList<string, TagCloud.TagCloudControl.StringItem> sc = new SortedList<string, TagCloud.TagCloudControl.StringItem>();
      ClearAllItems();
      if (TagCloud.XMLHandling.ReadTagFile(out sc, filename))
      {
        // insert the read items into the StringCollection
        AddItem(sc);
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Writes the contents of StringCollection into  
    /// the default XML file "Tags.xml".
    /// Returns true, if successful.
    /// </summary>
    /// <returns></returns>
    public bool WriteTagFile()
    {
      return TagCloud.XMLHandling.WriteTagFile(StringCollection);
    }

    /// <summary>
    /// Overloaded
    /// Writes the contents of StringCollection into  
    /// the file filename.
    /// Returns true, if successful.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public bool WriteTagFile(string filename)
    {
      return TagCloud.XMLHandling.WriteTagFile(StringCollection, filename);
    }

    #endregion

    #region Methods for Text and HTML File Reading

    /// <summary>
    /// Reads the text file filename and sets its filtered
    /// contents into the cloud.
    /// Returns true, if successful.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public bool ReadTxtFile(string filename, TextLanguage language)
    {
      SortedList<string, TagCloud.TagCloudControl.StringItem> sc = new SortedList<string, TagCloud.TagCloudControl.StringItem>();
      ClearAllItems();
      if (TagCloud.TXTHandling.ReadTextFile(out sc, filename, language))
      {
        // insert the read items into the StringCollection
        AddItem(sc);
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Reads the HTML file filename and sets its filtered
    /// contents into the cloud.
    /// Returns true, if successful.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public bool ReadHTMLFile(string filename, TextLanguage language)
    {
      SortedList<string, TagCloud.TagCloudControl.StringItem> sc = new SortedList<string, TagCloud.TagCloudControl.StringItem>();
      ClearAllItems();
      if (TagCloud.TXTHandling.ReadHTMLFile(out sc, filename, language))
      {
        // insert the read items into the StringCollection
        AddItem(sc);
        return true;
      }
      else
      {
        return false;
      }
    }
    
    #endregion

    #region Start and initialization

    /// <summary>
    /// Constructor
    /// </summary>
    public TagCloudControl()
    {
      InitializeComponent();
      // control's size
      m_ControlWidth = this.Width;
      m_ControlHeight = this.Height;
      // set the 5 predefined output designs into the OutputDesigns collection 
      SetAllDesigns(false);
    }
 
    #endregion

    #region Build the cloud

    /// <summary>
    /// Yields the string weights (occurrence) of StringCollection_Shown to a list.
    /// This list will be returned.
    /// </summary>
    private IEnumerable<double> StringWeightsShown
    {
      get
      {
        foreach (StringItem item in StringCollection_Shown.Values)
        {
          yield return item.Occurrence;
        }
      }
    }

    /// <summary>
    /// Sets the cloud. First, the original StringCollection will be copied to 
    /// a 'working' collection StringCollection_Shown. 
    /// StringCollection will not be modified.
    /// StringCollection_Shown will reduced to these items, which will be shown.
    /// StringCollection_Shown is the base for CloudCollection, which contains 
    /// all the items shown in the cloud, including their positions and dimensions.
    /// </summary>
    private void SetCloud()
    {
      // copy the original StringCollection to StringCollection_Shown
      StringCollection_Shown.Clear();
      for (int i = 0; i < StringCollection.Count; i++)
      {
        StringItem si = StringCollection.Values[i];
        string text = StringCollection.Keys[i];
        StringCollection_Shown.Add(text, si);
      }
      // reduce the original StringCollection, so that its copy StringCollection_Shown
      // will only contain these items, which can be shown in the cloud
      int j_merker = int.MaxValue;
      StringItem si_out = new StringItem("", 0, false);
      while ((j_merker = BuildCloud(ref si_out)) != -1)
      {
        StringCollection_Shown.RemoveAt(j_merker);
      }
      // copy the reduced list StringCollection_Shown to the CloudCollection
      CopyStringsToCloud();
    }

    /// <summary>
    /// Copies the items of StringCollection_Shown into CloudCollection.
    /// The position and dimension of each item will be calculated again and 
    /// inserted to CloudCollection (they are hold by the class CloudItem).
    /// </summary>
    private void CopyStringsToCloud()
    {
      CloudCollection.Clear(); 
      // calculate mean and standard deviation of the items in the StringCollection_Shown
      double mean;
      double stddev = Statistics.StandardDeviation(StringWeightsShown, out mean);
      int x_start = 0;     // x-start position of text
      int y_start = 0;     // y-start position of text
      int max_height = 0;  // max. height of one line of text strings
      for (int i = 0; i < StringCollection_Shown.Count; i++)
      {
        double weight = StringCollection_Shown.Values[i].Occurrence;
        string text = StringCollection_Shown.Keys[i];

        CloudItem ci = new CloudItem(weight);
        // set the design (= font), depending on the weight
        ci.Design = GetDesignFromWeight(weight, mean, stddev);
        // calculate the position within the cloud
        Graphics formGraphics = this.CreateGraphics();
        SizeF textsize = formGraphics.MeasureString(text, ci.Design.drawFont);
        formGraphics.Dispose();
        ci.Position.X = x_start;
        ci.Position.Y = y_start;
        ci.Position.Width = (int)textsize.Width;
        // check, if 'line end' of control has been reached
        if (x_start + ci.Position.Width > m_ControlWidth)
        {
          ci.Position.X = x_start = 0;
          y_start += max_height;
          ci.Position.Y = y_start;
          max_height = 0;
        }
        // to get a more dense package, it's sufficient to take the .Height, not .Height + 1
        max_height = Math.Max(max_height, (int)textsize.Height);
        ci.Position.Height = (int)textsize.Height;
        x_start += ci.Position.Width;

        // add the text to the cloud to be put out
        if (!CloudCollection.ContainsKey(text))
        {
          CloudCollection.Add(text, ci);
        }
      }
    }

    /// <summary>
    /// Builds the cloud contents. All items in StringCollection_Shown
    /// are examined to best fit into the user control. If items at the end won't fit
    /// (their rectangle would be beyond the bottom of the control), the weakest item will
    /// be found and its index in StringCollection_Shown returned. 
    /// It will then be deleted by the calling function.
    /// If all items fit, the function returns -1.
    /// </summary>
    /// <returns></returns>
    private int BuildCloud(ref StringItem si)
    {
      int j_merker = -1;
      // calculate mean and standard deviation of the items in the StringCollection_Shown
      double mean;
      double stddev = Statistics.StandardDeviation(StringWeightsShown, out mean);
      int x_start = 0;     // x-start position of text
      int y_start = 0;     // y-start position of text
      int max_height = 0;  // max. height of one line of text strings
      for (int i = 0; i < StringCollection_Shown.Count; i++)
      {
        double weight = StringCollection_Shown.Values[i].Occurrence;
        string text = StringCollection_Shown.Keys[i];
        CloudItem ci = new CloudItem(weight);
        // get the design (= font), depending on the weight
        ci.Design = GetDesignFromWeight(weight, mean, stddev);
        // calculate the position and dimension within the cloud
        Graphics formGraphics = this.CreateGraphics();
        SizeF textsize = formGraphics.MeasureString(text, ci.Design.drawFont);
        formGraphics.Dispose();
        // check, if the text item fits at the bottom of the control. 
        // If not, get the weakest item and return it
        if (y_start + (int)textsize.Height > m_ControlHeight)
        {
          j_merker = GetWeakestItem(ref si);
          return j_merker;
        }
        // check, if 'line end' (= right margin) of control has been reached
        if (x_start + (int)textsize.Width > m_ControlWidth)
        {
          x_start = 0;
          y_start += max_height;
          max_height = 0;
          // now check again, if the text item fits at the bottom of the control. 
          // If not, get the weakest item and return it
          if (y_start + (int)textsize.Height > m_ControlHeight)
          {
            j_merker = GetWeakestItem(ref si);
            return j_merker;
          }
        }
        // to get a more dense package, it's sufficient to take the .Height, not .Height + 1
        max_height = Math.Max(max_height, (int)textsize.Height);
        x_start += (int)textsize.Width;
      }
      return j_merker;
    }

    /// <summary>
    /// Searches the weakest item (least occurrence and oldest access)
    /// in StringCollection_Shown and returns its list index.
    /// </summary>
    /// <returns></returns>
    private static int GetWeakestItem(ref StringItem si)
    {
      double occ = double.MaxValue;
      int j_merker = 0;
      long last = long.MaxValue;
      // search items with least occurrence
      for (int j = 0; j < StringCollection_Shown.Count; j++)
      {
        double j_weight = StringCollection_Shown.Values[j].Occurrence;
        if (j_weight <= occ) occ = j_weight;
      }
      // search from the items with least occurrence this with oldest access
      for (int j = 0; j < StringCollection_Shown.Count; j++)
      {
        double j_weight = StringCollection_Shown.Values[j].Occurrence;
        long j_last = StringCollection_Shown.Values[j].LastAccessTime;
        if ((j_weight == occ) && (j_last <= last))
        {
          last = j_last;
          si = StringCollection_Shown.Values[j];
          j_merker = j;
        }
      }
      return j_merker;
    }

    /// <summary>
    /// Gets one of the 5 predefined text designs (0..4).
    /// The text design is calculated by the occurrence (weight) of the text
    /// in relation to the mean value and the standard deviation.
    /// </summary>
    /// <param name="occ">occurrence</param>
    /// <param name="mean">mean value</param>
    /// <param name="stddev">standard deviation</param>
    /// <returns>OutputDesigns(0..4)</returns>
    private OutputDesign GetDesignFromWeight(double occ, double mean, double stddev)
    {
      double factor = occ - mean;

      if (factor != 0 && stddev != 0) factor /= stddev;
      int od = (factor > 1) ? 4 : (factor > 0.5) ? 3 : (factor > 0) ? 2 : (factor > -0.5) ? 1 : 0;
      return OutputDesigns[od % 5];
    }

    #endregion

    #region Paint (Show the items in the cloud)

    /// <summary>
    /// Paints the text items, which are part of CloudCollection.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TagCloudControl_Paint(object sender, PaintEventArgs e)
    {
      Graphics formGraphics = e.Graphics;
      // set the user defined back color
      formGraphics.Clear(this.m_ControlBackColor);

      int i = 0;
      ICollection<string> text = CloudCollection.Keys;
      foreach (CloudItem ci in CloudCollection.Values)
      {

        // TODO: Show kann wieder raus, da sowieso immer true


 //       if (ci.Show)
 //       {
          if (ci.Touched)
          {
            // the touched text item will be put out underlined (if m_Underline = true)
            FontStyle fs = FontStyle.Bold;
            if (m_Underline) fs |= FontStyle.Underline;
            Font drawFont = new System.Drawing.Font(ci.Design.drawFont.FontFamily, ci.Design.drawFont.Size, fs);
            formGraphics.DrawString(text.ElementAt<string>(i++), drawFont, ci.Design.drawBrush, ci.Position.X, ci.Position.Y);
            drawFont.Dispose();
          }
          else
          {
            formGraphics.DrawString(text.ElementAt<string>(i++), ci.Design.drawFont, ci.Design.drawBrush, ci.Position.X, ci.Position.Y);
          }
          // draw a frame (rectangle), if desired
          if (m_Frame)
          {
            Pen p = new Pen(ci.Design.drawBrush, 1);
            formGraphics.DrawRectangle(p, ci.Position);
            p.Dispose();
          }
        }
   //   }
      formGraphics.Dispose();
    }

    #endregion

    #region TagCloudControl Click and MouseMove

    /// <summary>
    /// Checks, if a point p is within a rectangle r.
    /// The rectangle's border is not part of the rectangle. 
    /// If yes, returns true.
    /// </summary>
    /// <param name="p"></param>
    /// <param name="r"></param>
    /// <returns></returns>
    private bool PointIsInRectangle(Point p, Rectangle r)
    {
      return ((p.X > r.Left) && (p.X < r.Right) && (p.Y > r.Top) && (p.Y < r.Bottom)) ? true : false;
    }

    /// <summary>
    /// Checks, which text item is passed by mouse move.
    /// This item is marked as being touched and will be
    /// put out underlined. Underlining will be only done,
    /// if m_Underlined is true.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TagCloudControl_MouseMove(object sender, MouseEventArgs e)
    {
      if (m_Underline)
      {
        ICollection<string> text = CloudCollection.Keys;
        int i = 0;
        int touched = -1;
        Rectangle rc = new Rectangle(0, 0, 0, 0);
        foreach (CloudItem ci in CloudCollection.Values)
        {
          if (PointIsInRectangle(e.Location, ci.Position))
          {
            CloudCollection.Values[i].Touched = true;
            touched = i;
            rc = ci.Position;
          }
          else
          {
            CloudCollection.Values[i].Touched = false;
          }
          i++;
        }
        // refresh, but only, when hovered text item has changed
        if (touched != m_LastTouched)
        {
          m_LastTouched = touched;
          //       this.Refresh();
          this.Invalidate(rc, false);
        }
      }
    }

    /// <summary>
    /// Handles the event of mouse clicking onto the user control.
    /// Left button: The event clickHandler will be fired. It sends 
    /// the text item's text and its occurrence at click time.
    /// Right button: opens a context menu with Add... and Remove
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TagCloudControl_MouseClick(object sender, MouseEventArgs e)
    {
      // mouse left click: fire event
      if (e.Button == MouseButtons.Left)
      {
        MouseEventArgs mea = (MouseEventArgs)e;
        Point p = mea.Location;
        ICollection<string> text = CloudCollection.Keys;
        int i = 0;
        foreach (CloudItem ci in CloudCollection.Values)
        {
          if (PointIsInRectangle(p, ci.Position))
          {
            // MessageBox.Show(text.ElementAt<string>(i));
            if (clickHandler != null) clickHandler(text.ElementAt<string>(i), ci.Occurrence);
            // refresh the item counter for this text (increment 1) and the last access date
            AddItem(text.ElementAt<string>(i));
            return;
          }
          i++;
        }
      }
      // mouse right button click: show context menu with Add..., Rename... and Remove
      if (e.Button == MouseButtons.Right)
      {
        MouseEventArgs mea = (MouseEventArgs)e;
        Point p = mea.Location;
        ICollection<string> text = CloudCollection.Keys;
        int i = 0;
        string itemtext = "";
        foreach (CloudItem ci in CloudCollection.Values)
        {
          if (PointIsInRectangle(p, ci.Position))
          {
            itemtext = text.ElementAt<string>(i);
            break;
          }
          i++;
        }
        // enlarge the context menu text 'Remove' and 'Rename' dynamically with the item text
        if (itemtext.Length > 0)
        {
          toolStripMenuItemRemove.Text = "Remove '" + itemtext + "'";
          toolStripMenuItemRemove.Enabled = true;
          toolStripMenuItemRename.Text = "Rename '" + itemtext + "'...";
          toolStripMenuItemRename.Enabled = true;
        }
        else
        {
          toolStripMenuItemRemove.Enabled = false;
          toolStripMenuItemRename.Enabled = false;
        }
        contextMenuStrip.Show();
        contextMenuStrip.Left = Control.MousePosition.X;
        contextMenuStrip.Top = Control.MousePosition.Y;
      }
    }

    #endregion

    #region Context menu handling Add...,  Rename... and Remove

    /// <summary>
    /// Click onto context menu "Add...".
    /// Call the dialog box to enter a new item and then
    /// add the item to the StringCollection.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void toolStripMenuItemAdd_Click(object sender, EventArgs e)
    {
      // call the dialog box to enter the item to be added
      AddItem ai = new AddItem();
      if (ai.ShowDialog() == DialogResult.OK)
      {
        string add = ai.StringToBeAdded;
        if (add.Length > 0) AddItem(add, true);
      }
    }

    /// <summary>
    /// Click onto context menu "Remove".
    /// The item text to be removed is isolated and then the 
    /// item is removed from the StringCollection.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void toolStripMenuItemRemove_Click(object sender, EventArgs e)
    {
      // rebuild the item text
      // first remove the string "Remove '" == 8 chars at the beginning
      string item = toolStripMenuItemRemove.Text.Substring(8);
      // second remove the trailing "'"
      item = item.TrimEnd('\''); 
      RemoveItem(item);
    }

    /// <summary>
    /// Click onto context menu "Rename...".
    /// The item text to be renamed is isolated and then the 
    /// item is offered to be renamed. The old item will be deleted and
    /// the new item will be added with the same 'occurrence'.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void toolStripMenuItemRename_Click(object sender, EventArgs e)
    {
      // rebuild the item text
      // first remove the string "Rename '" == 8 chars at the beginning
      string item = toolStripMenuItemRename.Text.Substring(8);
      // second remove the trailing "'..."
      item = item.Substring(0, item.Length - 4);

      // call the dialog box to rename the item
      RenameItem ri = new RenameItem();
      ri.OldString = item;
      if (ri.ShowDialog() == DialogResult.OK)
      {
        string rename = ri.NewString;
        if (rename.Length > 0)
        {
          // get occurrence of old item
          ICollection<string> text = CloudCollection.Keys;
          int i = 0;
          double occ = 1.0;
          foreach (CloudItem ci in CloudCollection.Values)
          {
            if (text.ElementAt<string>(i) == item)
            {
              occ = ci.Occurrence; 
              break;
            }
            i++;
          }
          // remove the old item...
          RemoveItem(item);
          // ...and insert the new item with the same occurrence
          AddItem(rename, occ);
        }
      }
    }

    #endregion

    #region Drag & Drop

    /// <summary>
    /// DragEnter: sets the DragDropEffect
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TagCloudControl_DragEnter(object sender, DragEventArgs e)
    {
      if (e.Data.GetDataPresent(DataFormats.FileDrop))
        e.Effect = DragDropEffects.Link;
      else
        e.Effect = DragDropEffects.None;
    }

    /// <summary>
    /// DragDrop: gets the filename of the object drawn over the control.
    /// Checks the filename, if its contents can be shown in the cloud.
    /// Estimates the language of the text file and the probabilty that it is plain text or HTML.
    /// Reads the file appropriately and shows the words in the cloud.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void TagCloudControl_DragDrop(object sender, DragEventArgs e)
    {
      try
      {
        Array a = (Array)e.Data.GetData(DataFormats.FileDrop);
        if (a != null)
        {
          // get the drag & drop filename
          string filename = a.GetValue(0).ToString();

          // check, if it is a non-showable file format
          if (FileFormats.IsGraphicFile(filename) || FileFormats.IsAudioFile(filename) || FileFormats.IsVideoFile(filename) ||
              FileFormats.IsCompressedFile(filename) || FileFormats.IsProgramFile(filename) || FileFormats.IsDocumentFile(filename) ||
              FileFormats.IsSystemFile(filename) || FileFormats.IsDatabaseFile(filename))
          {
            MessageBox.Show("Sorry, but\n" + filename + "\nhas a contents, that can not be shown in the cloud.", "No display in cloud", MessageBoxButtons.OK, MessageBoxIcon.Error);
            return;
          }

          bool ishtml;
          TextLanguage language;
          // check text / HTML and German / English
          TXTHandling.GetFileTypeAndLanguage(filename, out ishtml, out language);
          if (ishtml)
            ReadHTMLFile(filename, language);
          else
            ReadTxtFile(filename, language);
        }
      }
      catch (Exception ex)
      {
        MessageBox.Show("Error in DragDrop function: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
      }
    }

    #endregion

  }
}
