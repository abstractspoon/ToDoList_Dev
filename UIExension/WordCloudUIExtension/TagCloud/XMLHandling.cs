using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Reflection;
using System.IO;

namespace TagCloud
{
  /// <summary>
  /// XML file handling: read and write XML files, which hold the tags shown in the cloud
  /// </summary>
  public class XMLHandling
  {
    // Keywords in XML tags file
    const string XML_ROOT = "Tags";
    const string XML_PARENT_TAG = "Tag";
    const string XML_ATTRIBUTE_TEXT = "Text";
    const string XML_ATTRIBUTE_OCC = "Occurrence";
    const string XML_ATTRIBUTE_LAST = "Last";

    /// <summary>
    /// Gets the (default) file name of "Tags.xml".
    /// This file holds the texts, their occurrence and their last access time.
    /// </summary>
    /// <returns></returns>
    private static string GetDefaultTagFileName()
    {
      string tagfile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
      return Path.Combine(tagfile, "Tags.xml"); 
    }
  
    /// <summary>
    /// Reads the attributes of the given file "Tags.xml" (default) and stores them
    /// in the sorted list sc. The filled list sc is returned by reference.
    /// In case of success, true will be returned, else false.
    /// </summary>
    /// <param name="sc"></param>
    /// <returns></returns>
    public static bool ReadTagFile(out SortedList<string, TagCloudControl.StringItem> sc)
    {
      // get the default filename with the tags
      string filename = GetDefaultTagFileName();
      return ReadTagFile(out sc, filename);
    }

    /// <summary>
    /// Overloaded
    /// Reads the attributes of the given file filename and stores them
    /// in the sorted list sc. The filled list sc is returned by reference.
    /// In case of success, true will be returned, else false.
    /// </summary>
    /// <param name="sc"></param>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool ReadTagFile(out SortedList<string, TagCloudControl.StringItem> sc, string filename)
    {
      sc = new SortedList<string, TagCloudControl.StringItem>();
      try
      {
        if (!File.Exists(filename)) return false;
        XmlDocument xmlDoc = new XmlDocument();
        xmlDoc.Load(filename);
        XmlElement root = xmlDoc.DocumentElement;
        string text;
        double occ;
        long last;
        foreach (XmlNode daten in root.ChildNodes)
        {
          // read the three attributes
          text = daten.Attributes[XML_ATTRIBUTE_TEXT].InnerText;
          if (!double.TryParse(daten.Attributes[XML_ATTRIBUTE_OCC].InnerText, out occ)) occ = 1;
          if (!long.TryParse(daten.Attributes[XML_ATTRIBUTE_LAST].InnerText, out last)) last = DateTime.Now.Ticks;
          TagCloudControl.StringItem si = new TagCloudControl.StringItem(text, occ, true, last);
          if (!sc.ContainsKey(text)) sc.Add(text, si);
        }
        return true;
      }
      catch
      {
        return false;
      }
    }

    /// <summary>
    /// Writes the attributes of the sorted list sc into a default file "Tags.xml".
    /// If "Tags.xml" exists already, it will first be backuped to "Tags.bak".
    /// In case of success, true will be returned, else false.
    /// </summary>
    /// <param name="sc"></param>
    /// <returns></returns>
    public static bool WriteTagFile(SortedList<string, TagCloudControl.StringItem> sc)
    {
      // get the default filename with the tags
      string filename = GetDefaultTagFileName();
      return WriteTagFile(sc, filename);
    }

    /// <summary>
    /// Overloaded
    /// Writes the attributes of the sorted list sc into a file filename.
    /// If filename exists already, it will first be backuped to "filename.bak".
    /// Example: filename = abc.xml  -> backup it to abc.bak
    /// In case of success, true will be returned, else false.
    /// </summary>
    /// <param name="sc"></param>
    /// <returns></returns>
    public static bool WriteTagFile(SortedList<string, TagCloudControl.StringItem> sc, string filename)
    {
      try
      {
        // make a backup copy
        if (File.Exists(filename))
        {
          string bakfilename = Path.ChangeExtension(filename, "bak");
          File.Copy(filename, bakfilename, true);
        }
        XmlDocument xmlDoc = new XmlDocument();

        // write down the XML declaration
        XmlDeclaration xmlDeclaration = xmlDoc.CreateXmlDeclaration("1.0", "ISO-8859-1", null);

        // create the root element
        XmlElement rootNode = xmlDoc.CreateElement(XML_ROOT);
        xmlDoc.InsertBefore(xmlDeclaration, xmlDoc.DocumentElement);
        xmlDoc.AppendChild(rootNode);

        // create a new <Tag> element and add it to the root node
        XmlElement parentNode;// = xmlDoc.CreateElement(XML_PARENT);
        string text = "";
        foreach (TagCloudControl.StringItem si in sc.Values)
        {
          // create a new <Tag> element and add it to the root node
          parentNode = xmlDoc.CreateElement(XML_PARENT_TAG);
          // set the attributes text, occurrence and last
          text = sc.Keys[sc.Values.IndexOf(si)];  
          parentNode.SetAttribute(XML_ATTRIBUTE_TEXT, text);
          parentNode.SetAttribute(XML_ATTRIBUTE_OCC, si.Occurrence.ToString());
          parentNode.SetAttribute(XML_ATTRIBUTE_LAST, si.LastAccessTime.ToString());

          xmlDoc.DocumentElement.PrependChild(parentNode);
        }
        // save to the XML file
        xmlDoc.Save(filename);
        return true;
      }
      catch
      {
        return false;
      }
    }

  }
}
