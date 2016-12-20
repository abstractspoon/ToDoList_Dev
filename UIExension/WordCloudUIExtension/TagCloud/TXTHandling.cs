using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;

namespace TagCloud
{

  /// <summary>
  /// Text file handling: reads a text file and builds a list of words contained in the text
  /// </summary>
  public class TXTHandling
  {
    private static ArrayList ExcludeList = new ArrayList();
    private static ArrayList IncludeList = new ArrayList();

    /// <summary>
    /// Sets the strings which should be excluded into the exclude list.
    /// Sets the strings which should be included into the include list.
    /// The strings are read from a file, depending on the parameter language.
    /// If clear == true, the lists will be cleared before being filled, else the
    /// new strings will be appended to the lists.
    /// </summary>
    /// <param name="language"></param>
    /// <param name="clear"></param>
    /// <returns></returns>
    private static int FillExcludeIncludeList(TagCloud.TagCloudControl.TextLanguage language, bool clear)
    {
      if (clear)
      {
        ExcludeList.Clear();
        IncludeList.Clear();
      }
      try
      {
        string txtfile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        switch (language) 
        {
          case TagCloudControl.TextLanguage.German:
            txtfile = Path.Combine(txtfile, "ExcludeList-de.txt");
            break;
          case TagCloudControl.TextLanguage.HTML:
            txtfile = Path.Combine(txtfile, "ExcludeList-html.txt");
            break;
          default:         // default is english
            txtfile = Path.Combine(txtfile, "ExcludeList-en.txt");
            break;
        }
        if (!File.Exists(txtfile)) return 0;  
        FileStream objFStreamRead = new FileStream(txtfile, FileMode.Open, FileAccess.Read, FileShare.Read);
        StreamReader objSReader = new StreamReader(objFStreamRead, System.Text.Encoding.Default);

        bool exclude = false;
        bool include = false;
        string line;
        while ((line = objSReader.ReadLine()) != null)
        {
          line = line.Trim();
          if (line.StartsWith("Exclude:"))
          {
            exclude = true;
            include = false;
          }
          if (line.StartsWith("Include:"))
          {
            exclude = false;
            include = true;
          }
          string[] items = line.Split(',');
          if (exclude)
          {
            foreach (string item in items)
            {
              if ((!ExcludeList.Contains(item)) && (item != "Exclude:")) ExcludeList.Add(item);
            }
          }
          if (include)
          {
            foreach (string item in items)
            {
              if ((!IncludeList.Contains(item)) && (item != "Include:")) IncludeList.Add(item);
            }
          }
        }
        objSReader.Close();
        return ExcludeList.Count;
      }
      catch
      {
        return 0;
      }
    }

    /// <summary>
    /// Examines a word newword, if it is "wanted".
    /// Wanted are the words, which are 
    /// - explicitely in the include list
    /// - not also not "unwanted"
    /// Unwanted are the words, which
    /// - have less than 2 letters or more than 35 letters
    /// - are pure numbers
    /// - are numbers with %, such as 75%
    /// - are ordinal numbers, such as 1st, 2nd, 3rd, 4th...
    /// - are roman numbers, such as MCM
    /// - are numbers of type x-y, such as 4000-6000
    /// - are part of the exclude list.
    /// If the word is wanted, the function returns true.
    /// </summary>
    /// <param name="newword"></param>
    /// <returns></returns>
    private static bool WantedWord(string newword)
    {
      // exclude too short and too long words
      if ((newword.Length <= 1) || (newword.Length > 35)) return false;
      // include all explicitely wanted words
      if (IncludeList.Contains(newword)) return true;
      // exclude pure numbers
      long num;
      if (long.TryParse(newword, out num)) return false;
      // exclude english ordinal numbers
      if ((newword.EndsWith("st") || newword.EndsWith("nd") || newword.EndsWith("rd") || newword.EndsWith("th")) && (newword.Length > 2))
      {
        if (long.TryParse(newword.Substring(0, newword.Length - 2), out num)) 
          return false;
      }
      // exclude percent numbers
      if ((newword.EndsWith("%")) && (newword.Length > 1))
      {
        if (long.TryParse(newword.Substring(0, newword.Length - 1), out num)) return false;
      }
      // exclude roman numbers
      if (IsRomanNumber(newword.ToLower())) return false;
      // exclude timespans
      if (IsTimeSpan(newword)) return false;
      // exclude trivials
      if (ExcludeList.Contains(newword.ToLower())) return false;
      return true;
    }

    /// <summary>
    /// Examines a word newword, if it is "wanted".
    /// Unwanted are the words, which
    /// - contain at least one digit
    /// - contain %
    /// - have less than 2 letters or more than 25 letters
    /// - are part of the exclude list hor HTML.
    /// If the word is wanted, the function returns true.
    /// </summary>
    /// <param name="newword"></param>
    /// <returns></returns>
    private static bool WantedHTMLWord(string newword)
    {
      // exclude words with numbers
      if (ContainsDigit(newword)) return false;

      // exclude percent numbers
      if (newword.Contains("%")) return false;

      // exclude too short and too long words
      if ((newword.Length <= 1) || (newword.Length > 25)) return false;

      // exclude HTML keywords
      if (ExcludeList.Contains(newword.ToLower())) return false;
      return true;
    }

    /// <summary>
    /// Checks, if a string number is a roman number.
    /// If yes, the fuction returns true.
    /// </summary>
    /// <param name="number"></param>
    /// <returns></returns>
    private static bool IsRomanNumber(string number)
    {
      string rome = "^m*(d?c{0,3}|c[dm])(l?x{0,3}|x[lc])(v?i{0,3}|i[vx])$";
      return (Regex.Match(number, rome).Length > 0)? true : false;
    }

    /// <summary>
    /// Checks, if a string word contains at least one digit.
    /// If yes the function returns true.
    /// </summary>
    /// <param name="word"></param>
    /// <returns></returns>
    private static bool ContainsDigit(string word)
    {
      foreach (char c in word)
        if (char.IsDigit(c)) return true;
      return false;
    }

    /// <summary>
    /// Checks, if a string timespan contains a timespan or any combination of 
    /// two numbers like "4000-6000".
    /// If yes, the fuction returns true.
    /// </summary>
    /// <param name="timespan"></param>
    /// <returns></returns>
    private static bool IsTimeSpan(string timespan)
    {
      string delimStr = "–-";
      char[] delimiter = delimStr.ToCharArray();
      bool isnumber = false;
      if ((timespan.Contains("–")) || (timespan.Contains("-")))
      {
        string[] words = timespan.Split(delimiter);
        isnumber = true;
        int num;
        foreach (string word in words)
        {
          if (!int.TryParse(word, out num))
            isnumber = false;
        }
      }
      return isnumber;
    }

    /// <summary>
    /// Reads all the words of a text file filename and sets the 
    /// important (wanted) words into the sorted list sc.
    /// The occurrence of every word in the text is regarded.
    /// </summary>
    /// <param name="sc"></param>
    /// <param name="filename"></param>
    /// <param name="language"></param>
    /// <returns></returns>
    public static bool ReadTextFile(out SortedList<string, TagCloudControl.StringItem> sc, string filename, TagCloud.TagCloudControl.TextLanguage language)
    {
      // give all entries the same timestamp
      long last = DateTime.Now.Ticks; 
      // read the exclude and include strings from a file into two lists
      FillExcludeIncludeList(language, true);
      sc = new SortedList<string, TagCloudControl.StringItem>();
      try
      {
        if (!File.Exists(filename)) return false;
        FileStream objFStreamRead = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
        StreamReader objSReader = new StreamReader(objFStreamRead, System.Text.Encoding.Default);
        string line;

        string delimStr = "\t'<>/:“„;,. !?()[]\"»«=";
        char[] delimiter = delimStr.ToCharArray();

        string trimStr = "–-";
        char[] trimmer = trimStr.ToCharArray();
        while ((line = objSReader.ReadLine()) != null)
        {
          line = line.Trim(trimmer);
          string[] words = line.Split(delimiter);
          foreach (string word in words)
          {
            string newword = word.Trim(trimmer);
            if (WantedWord(newword))
            {
              if (!sc.ContainsKey(newword))
              {
                sc.Add(newword, new TagCloudControl.StringItem(newword, 1, true, last));
              }
              else
              {
                // get the previous occurrence
                int ind = sc.IndexOfKey(newword);
                double occ = sc.Values[ind].Occurrence;
                sc.RemoveAt(ind);
                sc.Add(newword, new TagCloudControl.StringItem(newword, occ + 1, true, last));
              }
            }
          } // foreach
        } // while
        objSReader.Close();
        return true;
      }
      catch
      {
        return false;
      }
    }

    /// <summary>
    /// Reads all the words of a text file filename and sets the 
    /// important (wanted) words into the sorted list sc.
    /// The occurrence of every word in the text is regarded.
    /// </summary>
    /// <param name="sc"></param>
    /// <param name="filename"></param>
    /// <param name="language"></param>
    /// <returns></returns>
    public static bool ReadHTMLFile(out SortedList<string, TagCloudControl.StringItem> sc, string filename, TagCloud.TagCloudControl.TextLanguage language)
    {
      // give all entries the same timestamp
      long last = DateTime.Now.Ticks;
      // read the exclude and include strings for the language into an include and an exclude list
      FillExcludeIncludeList(language, true);
      // read the exclude and include strings for HTML files and add them to the include and exclude list
      FillExcludeIncludeList(TagCloud.TagCloudControl.TextLanguage.HTML, false);
      sc = new SortedList<string, TagCloudControl.StringItem>();
      try
      {
        if (!File.Exists(filename)) return false;
        FileStream objFStreamRead = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
        StreamReader objSReader = new StreamReader(objFStreamRead, System.Text.Encoding.Default);
        string line;
        string delimStr = "\t#$&'=/<>+:“„;,. !?(){}[]\\–-_\"»«";
        char[] delimiter = delimStr.ToCharArray();

        string trimStr = "–-";
        char[] trimmer = trimStr.ToCharArray();
        while ((line = objSReader.ReadLine()) != null)
        {
          line = line.Trim(trimmer);
          string[] words = line.Split(delimiter);
          foreach (string word in words)
          {
            string newword = word.Trim(trimmer);
            // HTML filtered words
            if (WantedHTMLWord(newword))
            {
              // Language filtered words
              if (WantedWord(newword))
              {
                if (!sc.ContainsKey(newword))
                {
                  sc.Add(newword, new TagCloudControl.StringItem(newword, 1, true, last));
                }
                else
                {
                  // get the previous occurrence
                  int ind = sc.IndexOfKey(newword);
                  double occ = sc.Values[ind].Occurrence;
                  sc.RemoveAt(ind);
                  sc.Add(newword, new TagCloudControl.StringItem(newword, occ + 1, true, last));
                }
              }
            }
          } // foreach
        } // while
        objSReader.Close();
        return true;
      }
      catch
      {
        return false;
      }
    }

    /// <summary>
    /// Checks, if a file filename is of type HTML or not. In case of HTML,
    /// the function returns with out ishtml = true.
    /// Checks, if a file filename is of language German or English. In case of English,
    /// out language = TextLanguage.English will be returned, in case of German out language = TextLanguage.German.
    /// Attention:
    /// This is a very simple function and its 100% functionality is not ensured!
    /// </summary>
    /// <param name="filename"></param>
    /// <param name="ishtml"></param>
    /// <param name="language"></param>
    public static void GetFileTypeAndLanguage(string filename, out bool ishtml, out TagCloudControl.TextLanguage language)
    {
      int html_count = 0;     // counter for HTML probability
      int de_count = 0;       // counter for some known German words
      int en_count = 0;       // counter for some known English words

      string extension = Path.GetExtension(filename).ToUpper();
      if ((extension == "HTM") || (extension == "HTML")) html_count++;

      string delimStr = "'<>/:“„;,. !?()[]\"»«=";
      char[] delimiter = delimStr.ToCharArray();

      FileStream objFStreamRead = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
      StreamReader objSReader = new StreamReader(objFStreamRead, System.Text.Encoding.Default);

      string[] GermanWords = new string[] { "ICH", "DU", "EIN", "EINE", "DER", "DIE", "DAS", "UND", "ODER", "DIES", "DASS", "WIE", "NICHT", "ABER", "MAL", "FÜR" };
      string[] EnglishWords = new string[] { "ANY", "ARE", "SOME", "THE", "AND", "OR", "THAT", "HOW", "NOT", "BUT" };

      string line;
      while ((line = objSReader.ReadLine()) != null)
      {
        line = line.Trim().ToUpper();
        if (line.Contains("<!DOCTYPE HTML")) html_count++;
        if (line.Contains("<HEAD")) html_count++;
        if (line.Contains("<HTML")) html_count++;
        if (line.Contains("<BODY")) html_count++;

        string[] words = line.Split(delimiter);
        foreach (string word in words)
        {
          foreach (string germanword in GermanWords)
          {
            if (germanword == word) de_count++;
          }
          foreach (string englishword in EnglishWords)
          {
            if (englishword == word) en_count++;
          }
        }
      }
      objSReader.Close();
      // now prepare the return values
      // HTML or not HTML
      ishtml = (html_count >= 3) ? true : false;
      // German or English
      language = (de_count > en_count) ? TagCloudControl.TextLanguage.German : TagCloudControl.TextLanguage.English;
    }

    /// <summary>
    /// Checks, if a file filename is binary or contains plain text.
    /// If it contains plain text, true will be returned.
    /// Attention:
    /// This is a very simple function and its 100% functionality is not ensured!
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTextFile(string filename)
    {
      int ascii = 0;
      int nonascii = 0;
      // open file for reading
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
      byte[] buffer = new byte[fStream.Length];
      fStream.Read(buffer, 0, buffer.Length);
      // close file
      fStream.Close();
      if (buffer.Length == 0) return false; 
      foreach (byte b in buffer)
      {
        if ((b >= 32) && (b < 128)) ascii++;
        else nonascii++;
      }
      float a = (float)nonascii / (float)buffer.Length; 
      if (a > 0.1f)
        return false;
      else
        return true;
    }

  }


}
