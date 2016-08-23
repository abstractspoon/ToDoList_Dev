#ifndef CHRONICLE_IMAGE_H
#define CHRONICLE_IMAGE_H

#ifdef _WIN32
#  ifndef _WINDOWS_
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#  endif
#endif

#include <Chronicle/Common.h>
#include <Chronicle/String.h>

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
    A simple container class for images.
    Images are <em>implicitly shared</em>, meaning that copied
    images point to the same pixel data.
    @headerfile Chronicle/Image.h <Chronicle/Image.h>
 */
class CHRONICLESHARED_EXPORT Image
{
public:
    /*!
        Constructs a null image.
        @see isNull()
     */
    Image();
    /*!
        Constructs a copy of \a other.
        @param other An image.
     */
    Image(const Image &other);
#if defined(CHRONICLE_PLATFORM_WINDOWS)
    /*!
        Constructs an image from an HBITMAP handle.
        @note This function takes ownership of the HBITMAP handle.
        @warning This function is only available on Windows.
     */
    Image(HBITMAP bitmap);
#endif

    //! Destroys the image, releasing any resources.
    ~Image();

    /*!
        Sets the image to be a copy of \a other.
        @param other The image to copy.
        @return Returns a reference to this image.
    */
    Image & operator=(const Image &other) { return assign(other); }

    //! Returns \a true if this is a null image, otherwise \a false.
    bool isNull() const;

    //! Returns the width of the image. For null images, returns 0.
    int width() const { return _width; }
    //! Returns the height of the image. For null images, returns 0.
    int height() const { return _height; }

#if defined(CHRONICLE_PLATFORM_WINDOWS)
    /*!
        Returns the HBITMAP handle for the image.
        @warning This function is only available on Windows.
     */
    HBITMAP asHBITMAP() const { return _bitmap; }
#endif

    /*!
        Replaces the existing pixel data with the image in \a filename.
        @param filename The image file to load.
        @returns Chronicle::NoError if successful, otherwise the error code.
     */
    Error load(const String &filename);
    /*!
        Writes the current pixel data to the image \a filename.
        @param filename The image file to save.
        @returns Chronicle::NoError if successful, otherwise the error code.
     */
    Error save(const String &filename) const;

    //! Returns a base64-encoded string representation of the image.
    String toBase64() const;

private:
    Image & assign(const Image &other);
#if defined(CHRONICLE_PLATFORM_WINDOWS)
    Image & assign(HBITMAP bitmap);
#endif
    void clear();

private:
    int _width;
    int _height;

#if defined(CHRONICLE_PLATFORM_WINDOWS)
    HBITMAP _bitmap;
#endif
};

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

#endif // CHRONICLE_IMAGE_H
