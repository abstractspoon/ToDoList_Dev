/****************************************************************************\
Datei  : Color.h
Projekt: Farbverwaltung
Inhalt : CColor Deklaration
Datum  : 10.01.1999
Autor  : Christian Rodemeyer
Hinweis: © 1999 by Christian Rodemeyer
\****************************************************************************/
#pragma once

#ifndef __COLOR_H                                               
#define __COLOR_H

/****************************************************************************\
 CColor: Repräsentiert einen Farbwert
\****************************************************************************/
class CColor
{
public:

  // COLORREF Konvertierung
  // ----------------------
  CColor(COLORREF cr = 0);
  operator COLORREF() const;

  // RGB - Routinen
  // --------------
  void SetRed(int red);     // 0..255  
  void SetGreen(int green); // 0..255
  void SetBlue(int blue);   // 0..255
  void SetRGB(int red, int blue, int green);

  int GetRed() const;   // 0..255
  int GetGreen() const; // 0..255
  int GetBlue() const;  // 0..255

  // HLS - Routinen
  // --------------
  void SetHue(float hue);               // 0.0 .. 360.0
  void SetLuminance(float luminance);   // 0.0 .. 1.0
  void SetSaturation(float saturation); // 0.0 .. 1.0
  void SetHLS(float hue, float luminance, float saturation);

  float GetHue() const;        // 0.0 .. 360.0
  float GetLuminance() const;  // 0.0 .. 1.0
  float GetSaturation() const; // 0.0 .. 1.0

private:

  // Konvertierung
  // -------------
  void ToRGB(); // logisch konstant, nicht physikalisch
  void ToHLS(); // logisch konstant, nicht physikalisch
  static unsigned char ToRGB1(float rm1, float rm2, float rh);

  // Daten
  // -----
  union // Byteweiser Zugriff auf die COLORREF Struktur
  {
    COLORREF      m_colorref;
    unsigned char m_color[4];
  };
  enum {c_red = 0, c_green = 1, c_blue = 2, c_null = 3}; // enum Hack für colorbyte-Index

  float m_hue;         // 0.0 .. 360.0  // Winkel
  float m_saturation;  // 0.0 .. 1.0    // Prozent
  float m_luminance;   // 0.0 .. 1.0    // Prozent

  // Flag für Lazy Evaluation
  bool m_bIsRGB; 
  bool m_bIsHLS; 
};

#endif

