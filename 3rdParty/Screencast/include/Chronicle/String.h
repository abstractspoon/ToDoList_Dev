#ifndef CHRONICLE_STRING_H
#define CHRONICLE_STRING_H

#include <stdlib.h>

#ifndef CHRONICLE_CHRONICLE_DECL_H
#  include "Chronicle_decl.h"
#endif

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif

namespace Chronicle
{

/*!
    A simple string class used by many Chronicle classes.
    Strings are <em>implicitly shared</em>, meaning that copied
    strings point to the same wide character data buffer.
    @headerfile Chronicle/String.h <Chronicle/String.h>
 */
class CHRONICLESHARED_EXPORT String
{
public:
    /*!
        Constructs an empty string.
        @see assign()
     */
    String();
    /*!
        Constructs a copy of \a other.
        @param other A string.
     */
    String(const String &other);
    /*!
        \fn String(const wchar_t *str)
        Constructs a string initialized with the Unicode string \a str.
        @param str A pointer to null-terminated wide character data.
     */
#if defined(_MSC_VER)
    String(const __wchar_t *str);
    String(const unsigned __int16 *str);
#else
    String(const wchar_t *str);
#endif
    //! Destroys the string.
    ~String();

    /*!
        Sets the string to be a copy of \a other.
        @param other The string to copy.
        @return Returns a reference to this string.
    */
    String & assign(const String &other);

    /*!
        Sets the string to be a copy of \a other.
        @param other The string to copy.
        @return Returns a reference to this string.
    */
    String & operator=(const String &other);

    /*!
        \fn String & assign(const wchar_t *str);
        Assigns a new value to the string.
        @param str A pointer to null-terminated wide character data.
        @return Returns a reference to this string.
    */
#if defined(_MSC_VER)
    String & assign(const __wchar_t *str);
    String & assign(const unsigned __int16 *str);
#else
    String & assign(const wchar_t *str);
#endif

    /*!
        Sets the string to be empty.
        @see assign(), isEmpty()
    */
    void clear();

#if defined(_MSC_VER)
    operator const __wchar_t *() const;
    operator const unsigned __int16 *() const;
#else
    //! Returns the string as a null-terminated Unicode string.
    operator const wchar_t *() const;
#endif

    //! \private
    bool cmp(const String &other) const;

    //! Returns \a true if this string is equal to \a other, otherwise \a false.
    inline bool operator==(const String &other) const { return cmp(other); }
    //! Returns \a true this string is not equal to \a other, otherwise \a false.
    inline bool operator!=(const String &other) const { return !cmp(other); }

#if defined(_MSC_VER)
    inline bool operator==(const __wchar_t *str) const { return cmp(String(str)); }
    inline bool operator!=(const __wchar_t *str) const { return !cmp(String(str)); }
    inline bool operator==(const unsigned __int16 *str) const { return cmp(String(str)); }
    inline bool operator!=(const unsigned __int16 *str) const { return !cmp(String(str)); }
#else
    //! Returns \a true if this string is equal to \a str, otherwise \a false.
    inline bool operator==(const wchar_t *str) const { return cmp(String(str)); }
    //! Returns \a true this string is not equal to \a str, otherwise \a false.
    inline bool operator!=(const wchar_t *str) const { return !cmp(String(str)); }
#endif

    /*!
        Returns whether the string is empty.
        @see clear(), length()
    */
    bool isEmpty() const;

    /*!
        Returns the number of characters in the string.
        @see isEmpty()
    */
    size_t length() const;

private:
    // char is not supported, use wchar_t instead!
    String(const char *); // not implemented
    String & assign(const char *); // not implemented

private:
    wchar_t *_s;
};

class StringComparisonHelper
{
public:
    inline StringComparisonHelper(const String &str) : s(&str) {}
private:
    friend inline bool operator==(const String &, const StringComparisonHelper &);
    const String *s;
};

//! \private
inline bool operator==(const String &s1, const StringComparisonHelper &s2)
{
    return s1.cmp(*s2.s);
}

//! \private
inline bool operator!=(const String &s1, const StringComparisonHelper &s2)
{
    return !operator==(s1, s2);
}

}

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

//-
// ==================================================================
// (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved. By using
// this code,  you  are  agreeing  to the terms and conditions of the
// License  Agreement  included  in  the documentation for this code.
// AUTODESK  MAKES  NO  WARRANTIES,  EXPRESS  OR  IMPLIED,  AS TO THE
// CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
// IT.  AUTODESK PROVIDES THE CODE ON AN 'AS-IS' BASIS AND EXPLICITLY
// DISCLAIMS  ANY  LIABILITY,  INCLUDING CONSEQUENTIAL AND INCIDENTAL
// DAMAGES  FOR ERRORS, OMISSIONS, AND  OTHER  PROBLEMS IN THE  CODE.
//
// Use, duplication,  or disclosure by the U.S. Government is subject
// to  restrictions  set forth  in FAR 52.227-19 (Commercial Computer
// Software Restricted Rights) as well as DFAR 252.227-7013(c)(1)(ii)
// (Rights  in Technical Data and Computer Software),  as applicable.
// ==================================================================
//+

#endif // CHRONICLE_STRING_H
