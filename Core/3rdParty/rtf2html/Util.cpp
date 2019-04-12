
#include "stdafx.h"
#include "util.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString LongToString(long newValue)
{

   long lvalue;
   CString str_rtc;
   lvalue = newValue;
   str_rtc.Format(_T("%d"),lvalue);
   return str_rtc;

} //LongToString()

long StringToLong(CString newValue)
{
   long lvalue;
   char *p_str;
   char *p_endstr;
   CString str;
   str = newValue;
   p_str=(LPSTR) LPCTSTR(str);
   lvalue = strtol(p_str, &p_endstr, 10);

   return lvalue;

} //StringToLong()

void StringToArray(const CString& strSrc, CStringArray& trgt) {
   CString strDelimiter=_T(";");
   CString strElement;
   long lPosition=0;

   while (lPosition<strSrc.GetLength()) {
      if ((strDelimiter.Find(strSrc.GetAt(lPosition))>=0)) {
         trgt.Add(strElement);
         strElement = "";
      } else {
         strElement += strSrc.GetAt(lPosition);
      } //if-else is_field_delimiter
      lPosition++;
   }
   trgt.Add(strElement); // add last
}