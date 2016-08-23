/****************************************************************************\
Datei  : Color.cpp
Projekt: Farbverwaltung
Inhalt : CColor Implementierung
Datum  : 10.01.1999
Autor  : Christian Rodemeyer
Hinweis: © 1999 by Christian Rodemeyer
         Info über HLS Konvertierungsfunktion
         - Foley and Van Dam: "Fundamentals of Interactive Computer Graphics"  
         - MSDN: 'HLS Color Spaces'
         - MSDN: 'Converting Colors Between RGB and HLS' 
\****************************************************************************/

#include "StdAfx.h"
#include "Color.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CColor::CColor(COLORREF cr)
: m_bIsRGB(true), m_bIsHLS(false), m_colorref(cr)
{}

CColor::operator COLORREF() const
{
  const_cast<CColor*>(this)->ToRGB();
  return m_colorref;
}

// RGB

void CColor::SetRed(int red)
{
  ASSERT(0 <= red && red <= 255);
  ToRGB();
  m_color[c_red] = static_cast<unsigned char>(red);
  m_bIsHLS = false;
}

void CColor::SetGreen(int green)
{
  ASSERT(0 <= green && green <= 255);
  ToRGB();
  m_color[c_green] = static_cast<unsigned char>(green);
  m_bIsHLS = false;
}

void CColor::SetBlue(int blue)
{
  ASSERT(0 <= blue && blue <= 255);
  ToRGB();
  m_color[c_blue] = static_cast<unsigned char>(blue);
  m_bIsHLS = false;
}

void CColor::SetRGB(int red, int blue, int green)
{
  ASSERT(0 <= red && red <= 255);
  ASSERT(0 <= green && green <= 255);
  ASSERT(0 <= blue && blue <= 255);

  m_color[c_red]   = static_cast<unsigned char>(red);
  m_color[c_green] = static_cast<unsigned char>(green);
  m_color[c_blue]  = static_cast<unsigned char>(blue);
  m_bIsHLS = false;
  m_bIsRGB = true;
}

int CColor::GetRed() const
{
  const_cast<CColor*>(this)->ToRGB();
  return m_color[c_red];
}

int CColor::GetGreen() const
{
  const_cast<CColor*>(this)->ToRGB();
  return m_color[c_green];
}

int CColor::GetBlue() const
{
  const_cast<CColor*>(this)->ToRGB();
  return m_color[c_blue];
}

// HSL

void CColor::SetHue(float hue)
{
  ASSERT(hue >= 0.0 && hue <= 360.0);

  ToHLS();
  m_hue = hue;
  m_bIsRGB = false;
}

void CColor::SetSaturation(float saturation)
{
  ASSERT(saturation >= 0.0 && saturation <= 1.0); // 0.0 ist undefiniert

  ToHLS();
  m_saturation = saturation;
  m_bIsRGB = false;
}

void CColor::SetLuminance(float luminance)
{
  ASSERT(luminance >= 0.0 && luminance <= 1.0);

  ToHLS();
  m_luminance = luminance;
  m_bIsRGB = false;
}

void CColor::SetHLS(float hue, float luminance, float saturation)
{
  ASSERT(hue >= 0.0 && hue <= 360.0);
  ASSERT(luminance >= 0.0 && luminance <= 1.0);
  ASSERT(saturation >= 0.0 && saturation <= 1.0); // 0.0 ist undefiniert

  m_hue = hue;
  m_luminance = luminance;
  m_saturation = saturation;
  m_bIsRGB = false;
  m_bIsHLS = true;
}

float CColor::GetHue() const
{
  const_cast<CColor*>(this)->ToHLS();
  return m_hue;
}

float CColor::GetSaturation() const
{
  const_cast<CColor*>(this)->ToHLS();
  return m_saturation;
}

float CColor::GetLuminance() const
{
  const_cast<CColor*>(this)->ToHLS();
  return m_luminance;
}

// Konvertierung

void CColor::ToHLS() 
{
  if (!m_bIsHLS)
  {
    // Konvertierung
    unsigned char minval = min(m_color[c_red], min(m_color[c_green], m_color[c_blue]));
    unsigned char maxval = max(m_color[c_red], max(m_color[c_green], m_color[c_blue]));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
   
    m_luminance = msum / 510.0f;

    if (maxval == minval) 
    {
      m_saturation = 0.0f;
      m_hue = 0.0f; 
    }   
    else 
    { 
      float rnorm = (maxval - m_color[c_red]  ) / mdiff;      
      float gnorm = (maxval - m_color[c_green]) / mdiff;
      float bnorm = (maxval - m_color[c_blue] ) / mdiff;   

      m_saturation = (m_luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

      if (m_color[c_red]   == maxval) m_hue = 60.0f * (6.0f + bnorm - gnorm);
      if (m_color[c_green] == maxval) m_hue = 60.0f * (2.0f + rnorm - bnorm);
      if (m_color[c_blue]  == maxval) m_hue = 60.0f * (4.0f + gnorm - rnorm);
      if (m_hue > 360.0f) m_hue = m_hue - 360.0f;
    }
    m_bIsHLS = true;
  }
}

void CColor::ToRGB() 
{
  if (!m_bIsRGB)
  {
    if (m_saturation == 0.0) // Grauton, einfacher Fall
    {
      m_color[c_red] = m_color[c_green] = m_color[c_blue] = unsigned char(m_luminance * 255.0);
    }
    else
    {
      float rm1, rm2;
         
      if (m_luminance <= 0.5f) rm2 = m_luminance + m_luminance * m_saturation;  
      else                     rm2 = m_luminance + m_saturation - m_luminance * m_saturation;
      rm1 = 2.0f * m_luminance - rm2;   
      m_color[c_red]   = ToRGB1(rm1, rm2, m_hue + 120.0f);   
      m_color[c_green] = ToRGB1(rm1, rm2, m_hue);
      m_color[c_blue]  = ToRGB1(rm1, rm2, m_hue - 120.0f);
    }
    m_bIsRGB = true;
  }
}

unsigned char CColor::ToRGB1(float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;
 
  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
                   
  return static_cast<unsigned char>(rm1 * 255);
}

