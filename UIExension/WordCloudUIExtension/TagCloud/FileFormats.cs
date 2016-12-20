using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TagCloud
{
  public class FileFormats
  {
    #region Graphic file formats

    /// <summary>
    /// Checks, if a file is of graphic file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsGraphicFile(string filename)
    {
      if (IsBMPFile(filename)) return true;
      if (IsDIBFile(filename)) return true;
      if (IsGIFFile(filename)) return true;
      if (IsJPGFile(filename)) return true;
      if (IsJPG_JFIFFile(filename)) return true;
      if (IsJPG_EXIFFile(filename)) return true;
      if (IsJPG_SPIFFFile(filename)) return true;
      if (IsPNGFile(filename)) return true;
      if (IsPCXFile(filename)) return true;
      if (IsTIFFile(filename)) return true;
      if (IsICOFile(filename)) return true;
      if (IsEPSFile(filename)) return true;
      if (IsWMFFile(filename)) return true;
      if (IsCURFile(filename)) return true;
      if (IsPSDFile(filename)) return true;
      if (IsJPEG2000File(filename)) return true;
      if (IsTHUMBSFile(filename)) return true;
      return false;
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsBMPFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2];
      fStream.Read(buffer, 0, 2);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "BM");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsDIBFile(string filename)
    {
      return IsBMPFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsGIFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[6];
      fStream.Read(buffer, 0, 6);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "GIF89a" || enc.GetString(buffer) == "GIF87a");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsJPGFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[3];
      fStream.Read(buffer, 0, 3);
      fStream.Close();
      return ((buffer[0] == 255) && (buffer[1] == 216) && (buffer[2] == 255));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsJPG_JFIFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[10];
      fStream.Read(buffer, 0, 10);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 255) && (buffer[1] == 216) && (buffer[2] == 255) && (enc.GetString(buffer, 6, 4) == "JFIF"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsJPG_EXIFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[10];
      fStream.Read(buffer, 0, 10);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 255) && (buffer[1] == 216) && (buffer[2] == 255) && (enc.GetString(buffer, 6, 4) == "Exif"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsJPG_SPIFFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 12) return false;
      byte[] buffer = new byte[11];
      fStream.Read(buffer, 0, 11);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 255) && (buffer[1] == 216) && (buffer[2] == 255) && (enc.GetString(buffer, 6, 5) == "SPIFF"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPNGFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[8];
      fStream.Read(buffer, 0, 8);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 0x89) && (enc.GetString(buffer, 1, 3) == "PNG") && (buffer[4] == 0x0D) && (buffer[5] == 0x0A) && (buffer[6] == 0x1A) && (buffer[7] == 0x0A));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPCXFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x0A) && (buffer[2] == 0x01) && (buffer[3] == 0x01) && 
              ((buffer[1] == 0x02) || (buffer[1] == 0x03) || (buffer[1] == 0x05)));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTIFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2]; fStream.Read(buffer, 0, 2); fStream.Seek(0, 0);
      byte[] buffer1 = new byte[3]; fStream.Read(buffer1, 0, 3); fStream.Seek(0, 0);
      byte[] buffer2 = new byte[4]; fStream.Read(buffer2, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (((enc.GetString(buffer) == "MM") && (buffer2[2] == 0x00) && (buffer2[3] == 0x2A)) ||
              ((enc.GetString(buffer) == "MM") && (buffer2[2] == 0x00) && (buffer2[3] == 0x2B)) ||
              ((enc.GetString(buffer) == "II") && (buffer2[2] == 0x2A) && (buffer2[3] == 0x00)) ||
              (enc.GetString(buffer1) == "I I"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsICOFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x00) && (buffer[1] == 0x00) && (buffer[2] == 0x01) && (buffer[3] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsEPSFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 25) return false;
      byte[] buffer = new byte[23]; 
      fStream.Read(buffer, 0, 23);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 11) == "%!PS-Adobe-") && (enc.GetString(buffer, 15, 5) == "EPSF-"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsWMFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0xD7) && (buffer[1] == 0xCD) && (buffer[2] == 0xC6) && (buffer[3] == 0x9A));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCURFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x00) && (buffer[1] == 0x00) && (buffer[2] == 0x02) && (buffer[3] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPSDFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "8BPS");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsJPEG2000File(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 40) return false;
      byte[] buffer = new byte[33];
      fStream.Read(buffer, 0, 33);
      fStream.Close();
      return ((buffer[0] == 0x00) && (buffer[1] == 0x00) && (buffer[2] == 0x00) && (buffer[3] == 0x0C) && 
              (buffer[4] == 0x6A) && (buffer[5] == 0x50) && (buffer[6] == 0x20) && (buffer[7] == 0x20) && 
              (buffer[8] == 0x0D) && (buffer[9] == 0x0A) && (buffer[10] == 0x87) && (buffer[11] == 0x0A) && 
              (buffer[12] == 0x00) && (buffer[13] == 0x00) && (buffer[14] == 0x00) && (buffer[15] == 0x14) && 
              (buffer[16] == 0x66) && (buffer[17] == 0x74) && (buffer[18] == 0x79) && (buffer[19] == 0x70) && 
              (buffer[20] == 0x6A) && (buffer[21] == 0x70) && (buffer[22] == 0x32));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTHUMBSFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 540) return false;
      byte[] buffer = new byte[16];
      fStream.Seek(512, 0);
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      return ((buffer[0] == 0xFD) && (buffer[1] == 0xFF) && (buffer[2] == 0xFF) && (buffer[3] == 0xFF) &&
              (buffer[12] == 0x04) && (buffer[13] == 0x00) && (buffer[14] == 0x00) && (buffer[15] == 0x00));
    }

    #endregion

    #region Audio file formats

    /// <summary>
    /// Checks, if a file is of audio file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsAudioFile(string filename)
    {
      if (IsWAVFile(filename)) return true;
      if (IsMIDIFile(filename)) return true;
      if (IsMP3File(filename)) return true;
      if (IsWMAFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsWAVFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 16) return false;
      byte[] buffer = new byte[15];
      fStream.Read(buffer, 0, 15);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 4) == "RIFF") && (enc.GetString(buffer, 8, 7) == "WAVEfmt"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMIDIFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer, 0, 4) == "MThd");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMP3File(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[3];
      fStream.Read(buffer, 0, 3);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 3) == "ID3") ||
             ((buffer[0] == 0xFF) && (buffer[1] >= 0xE0)));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsWMAFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      return ((buffer[0] == 0x30) && (buffer[1] == 0x26) && (buffer[2] == 0xB2) && (buffer[3] == 0x75) &&
        (buffer[4] == 0x8E) && (buffer[5] == 0x66) && (buffer[6] == 0xCF) && (buffer[7] == 0x11) &&
        (buffer[8] == 0xA6) && (buffer[9] == 0xD9) && (buffer[10] == 0x00) && (buffer[11] == 0xAA) &&
        (buffer[12] == 0x00) && (buffer[13] == 0x62) && (buffer[14] == 0xCE) && (buffer[15] == 0x6C));
    }

    #endregion

    #region Video file formats

    /// <summary>
    /// Checks, if a file is of video file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsVideoFile(string filename)
    {
      if (IsFLVFile(filename)) return true;
      if (IsAVIFile(filename)) return true;
      if (IsSWFFile(filename)) return true;
      if (IsMPGFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsFLVFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[3];
      fStream.Read(buffer, 0, 3);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "FLV");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsAVIFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 17) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 4) == "RIFF") && (enc.GetString(buffer, 8, 8) == "AVI LIST"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsWMVFile(string filename)
    {
      return IsWMAFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsASFFile(string filename)
    {
      return IsWMAFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsSWFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[3];
      fStream.Read(buffer, 0, 3);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 3) == "CWS") || (enc.GetString(buffer, 0, 3) == "FWS"));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMPGFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x00) && (buffer[1] == 0x00) && (buffer[2] == 0x01) && (buffer[3] >= 0xB0) && (buffer[3] <= 0xBF));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMPEGFile(string filename)
    {
      return IsMPGFile(filename);
    }

    #endregion

    #region Compressed file formats

    /// <summary>
    /// Checks, if a file is of compressed file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCompressedFile(string filename)
    {
      if (IsZIPFile(filename)) return true;
      if (IsLZHFile(filename)) return true;
      if (IsRARFile(filename)) return true;
      if (IsCABFile(filename)) return true;
      if (IsARJFile(filename)) return true;
      if (IsCATFile(filename)) return true;
      if (IsTARFile(filename)) return true;
      if (IsZOOFile(filename)) return true;
      if (IsISOFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsZIPFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x50) && (buffer[1] == 0x4B) && (buffer[2] == 0x03) && (buffer[3] == 0x04));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsLZHFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[5];
      fStream.Read(buffer, 0, 5);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer, 2, 3) == "-lh");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsRARFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4]; fStream.Read(buffer, 0, 4); fStream.Seek(0, 0);
      byte[] buffer1 = new byte[7]; fStream.Read(buffer1, 0, 7);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer) == "Rar!") && (buffer1[4] == 0x1A) && (buffer1[5] == 0x07) && (buffer1[6] == 0x00));
    }    

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCABFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4]; 
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "MSCF");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsARJFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2]; fStream.Read(buffer, 0, 2);
      fStream.Close();
      return ((buffer[0] == 0x60) && (buffer[1] == 0xEA));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCATFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[1];
      fStream.Read(buffer, 0, 1);
      fStream.Close();
      return (buffer[0] == 0x30);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTARFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[3];
      fStream.Read(buffer, 0, 3);
      fStream.Close();
      return ((buffer[0] == 0x1F) && (buffer[1] == 0x8B) && (buffer[2] == 0x08));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsZOOFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "ZOO ");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsISOFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10000) return false;
      byte[] buffer1 = new byte[5];
      byte[] buffer2 = new byte[5];
      byte[] buffer3 = new byte[5];
      fStream.Seek(0x8001, 0);
      fStream.Read(buffer1, 0, 5);
      fStream.Seek(0x8801, 0);
      fStream.Read(buffer2, 0, 5);
      fStream.Seek(0x9001, 0);
      fStream.Read(buffer3, 0, 5);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer1) == "CD001") || (enc.GetString(buffer2) == "CD001") || (enc.GetString(buffer3) == "CD001"));
    }

    #endregion

    #region Program and Development file formats

    /// <summary>
    /// Checks, if a file is of program file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsProgramFile(string filename)
    {
      if (IsEXEFile(filename)) return true;
      if (IsCOMFile(filename)) return true;
      if (IsOBJFile(filename)) return true;
      if (IsLIBFile(filename)) return true;
      if (IsDSPFile(filename)) return true;
      if (IsDSWFile(filename)) return true;
      if (IsTLBFile(filename)) return true;
      if (IsOCXFile(filename)) return true;
      if (IsVBXFile(filename)) return true;
      if (IsSUOFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsEXEFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2];
      fStream.Read(buffer, 0, 2);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "MZ");
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsDLLFile(string filename)
    {
      return IsEXEFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCOMFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[1];
      fStream.Read(buffer, 0, 1);
      fStream.Close();
      return (IsEXEFile(filename) || (buffer[0] == 0xE8) || (buffer[0] == 0xE9) || (buffer[0] == 0xEB));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsSYSFile(string filename)
    {
      return IsCOMFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsOBJFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2];
      fStream.Read(buffer, 0, 2);
      fStream.Close();
      return ((buffer[0] == 0x4C) && (buffer[1] == 0x01));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsLIBFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[8];
      fStream.Read(buffer, 0, 8);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 7) == "!<arch>") && (buffer[7] == 0x0A));
    }
    
    /// <summary>
    /// 
    /// </summary>
    public static bool IsDSPFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 30) return false;
      byte[] buffer = new byte[28];
      fStream.Read(buffer, 0, 28);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "# Microsoft Developer Studio");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsDSWFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 50) return false;
      byte[] buffer = new byte[41];
      fStream.Read(buffer, 0, 41);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "Microsoft Developer Studio Workspace File");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTLBFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (((enc.GetString(buffer, 0, 4) == "MSFT") &&
               (buffer[4] == 0x02) && (buffer[5] == 0x00) && (buffer[6] == 0x01) && (buffer[7] == 0x00)) ||
               (IsOCXFile(filename)));
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsOCXFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 0x4D) && (buffer[1] == 0x5A) && (buffer[2] == 0x90) && (buffer[3] == 0x00) &&
              (buffer[4] == 0x03) && (buffer[5] == 0x00) && (buffer[6] == 0x00) && (buffer[7] == 0x00) &&
              (buffer[8] == 0x04) && (buffer[9] == 0x00) && (buffer[10] == 0x00) && (buffer[11] == 0x00) &&
              (buffer[12] == 0xFF) && (buffer[13] == 0xFF) && (buffer[14] == 0x00) && (buffer[15] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsVBXFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 0x4D) && (buffer[1] == 0x5A) && 
              (buffer[6] == 0x00) && (buffer[7] == 0x00) &&
              (buffer[8] == 0x04) && (buffer[9] == 0x00) &&
              (buffer[12] == 0xFF) && (buffer[13] == 0xFF) && (buffer[14] == 0x00) && (buffer[15] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsSUOFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 40) return false;
      byte[] buffer = new byte[32];
      fStream.Read(buffer, 0, 32);
      fStream.Close();
      return ((buffer[0] == 0xD0) && (buffer[1] == 0xCF) && (buffer[2] == 0x11) && (buffer[3] == 0xE0) && (buffer[4] == 0xA1) && (buffer[5] == 0xB1) && (buffer[6] == 0x1A) && (buffer[7] == 0xE1) &&
              (buffer[8] == 0x00) && (buffer[9] == 0x00) && (buffer[10] == 0x00) && (buffer[11] == 0x00) && (buffer[12] == 0x00) && (buffer[13] == 0x00) && (buffer[14] == 0x00) && (buffer[15] == 0x00) &&
              (buffer[16] == 0x00) && (buffer[17] == 0x00) && (buffer[18] == 0x00) && (buffer[19] == 0x00) && (buffer[20] == 0x00) && (buffer[21] == 0x00) && (buffer[22] == 0x00) && (buffer[23] == 0x00) &&
              (buffer[24] == 0x3E) && (buffer[25] == 0x00) && (buffer[26] == 0x03) && (buffer[27] == 0x00) && (buffer[28] == 0xFE) && (buffer[29] == 0xFF) && (buffer[30] == 0x09) && (buffer[31] == 0x00));
    }


    #endregion

    #region Document file formats

    /// <summary>
    /// Checks, if a file is of document file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsDocumentFile(string filename)
    {
      if (IsOffice2007File(filename)) return true;
      if (IsOfficeFile(filename)) return true;
      if (IsPDFFile(filename)) return true;
      if (IsRTFFile(filename)) return true;
      if (IsCHMFile(filename)) return true;
      if (IsVCFFile(filename)) return true;
      if (IsWRIFile(filename)) return true;
      if (IsHLPFile(filename)) return true;
      if (IsGIDFile(filename)) return true;
      if (IsMSCFile(filename)) return true;
      if (IsPSTFile(filename)) return true;
      if (IsXMLFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsOffice2007File(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[8]; 
      fStream.Read(buffer, 0, 8);
      fStream.Close();
      return ((buffer[0] == 0x50) && (buffer[1] == 0x4B) && (buffer[2] == 0x03) && (buffer[3] == 0x04) &&
              (buffer[4] == 0x14) && (buffer[5] == 0x00) && (buffer[6] == 0x06) && (buffer[7] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsOfficeFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[8];
      fStream.Read(buffer, 0, 8);
      fStream.Close();
      return ((buffer[0] == 0xD0) && (buffer[1] == 0xCF) && (buffer[2] == 0x11) && (buffer[3] == 0xE0) &&
              (buffer[4] == 0xA1) && (buffer[5] == 0xB1) && (buffer[6] == 0x1A) && (buffer[7] == 0xE1));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPDFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "%PDF");
    }
 
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsRTFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[6];
      fStream.Read(buffer, 0, 6);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == @"{\rtf1");
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsCHMFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "ITSF");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsVCFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 15) return false;
      byte[] buffer = new byte[13]; 
      fStream.Read(buffer, 0, 13);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 0, 11) == "BEGIN:VCARD") && (buffer[11] == 0x0D) && (buffer[12] == 0x0A));
    }
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsWRIFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[2]; 
      fStream.Read(buffer, 0, 2);
      fStream.Close();
      return ((buffer[1] == 0xBE) && ((buffer[0] == 0x31) || (buffer[0] == 0x32)));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsHLPFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      return ((buffer[0] == 0x3F) && (buffer[1] == 0x5F) && (buffer[2] == 0x03) && (buffer[3] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsGIDFile(string filename)
    {
      return IsHLPFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMSCFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 60) return false;
      byte[] buffer = new byte[56];
      fStream.Read(buffer, 0, 56);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (IsOfficeFile(filename) ||
             ((enc.GetString(buffer, 0, 21) == "<?xml version=\"1.0\"?>") && 
              (buffer[21] == 0x0D) && (buffer[22] == 0x0A) &&
              (enc.GetString(buffer, 23, 33) == "<MMC_ConsoleFile ConsoleVersion=\"")));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPSTFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[4];
      fStream.Read(buffer, 0, 4);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer, 0, 4) == "!BDN");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsPABFile(string filename)
    {
      return IsPSTFile(filename);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsXMLFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 50) return false;
      byte[] buffer = new byte[45];
      fStream.Read(buffer, 0, 45);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      string xml = enc.GetString(buffer, 0, 45);
      xml = xml.Replace("\0", ""); 
      return ((enc.GetString(buffer, 0, 19) == "<?xml version=\"1.0\"") ||
              (enc.GetString(buffer, 0, 19) == "<?xml version='1.0'") ||
              (enc.GetString(buffer, 3, 19) == "<?xml version=\"1.0\"") ||
              (xml.Substring(2, 19) == "<?xml version=\"1.0\""));
    }

    #endregion
    
    #region System file formats

    /// <summary>
    /// Checks, if a file is of system file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsSystemFile(string filename)
    {
      if (IsLNKFile(filename)) return true;
      if (IsREGFile(filename)) return true;
      if (IsFONFile(filename)) return true;
      if (IsTTFFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsLNKFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[8];
      fStream.Read(buffer, 0, 8);
      fStream.Close();
      return ((buffer[0] == 0x4C) && (buffer[1] == 0x00) && (buffer[2] == 0x00) && (buffer[3] == 0x00) &&
              (buffer[4] == 0x01) && (buffer[5] == 0x14) && (buffer[6] == 0x02) && (buffer[7] == 0x00));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsREGFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 10) return false;
      byte[] buffer = new byte[7];
      fStream.Read(buffer, 0, 7);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return (enc.GetString(buffer) == "REGEDIT");
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsFONFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[16];
      fStream.Read(buffer, 0, 16);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((buffer[0] == 0x4D) && (buffer[1] == 0x5A) && (buffer[12] == 0xFF) && (buffer[13] == 0xFF));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsTTFFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[12];
      fStream.Read(buffer, 0, 12);
      fStream.Close();
      return ((buffer[0] == 0x00) && (buffer[1] == 0x01) && (buffer[2] == 0x00) && (buffer[3] == 0x00) &&
              (buffer[4] == 0x00));
    }

    #endregion

    #region Database file formats

    /// <summary>
    /// Checks, if a file is of database file format type.
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsDatabaseFile(string filename)
    {
      if (IsMDBFile(filename)) return true;
      return false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filename"></param>
    /// <returns></returns>
    public static bool IsMDBFile(string filename)
    {
      Stream fStream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
      if (fStream.Length < 20) return false;
      byte[] buffer = new byte[19]; fStream.Read(buffer, 0, 19);
      fStream.Close();
      ASCIIEncoding enc = new ASCIIEncoding();
      return ((enc.GetString(buffer, 4, 15) == "Standard Jet DB") && 
              (buffer[0] == 0x00) && (buffer[1] == 0x01) && (buffer[2] == 0x00) && (buffer[2] == 0x00));
    }    

    #endregion

    #region File formats TODO

    //public static bool IsSVNFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsDRWFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsFLIFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsMOVFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsMP4File(string filename)
    //{
    //  return false;
    //}
    //public static bool IsCDAFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsOLBFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsSCRFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsWPLFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsWPSFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsWKSFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsDBASEFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsARCFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsXLSFile(string filename)
    //{
    //  return false;
    //}
    //public static bool IsPPTFile(string filename)
    //{
    //  return false;
    //}

    #endregion

  }
}
