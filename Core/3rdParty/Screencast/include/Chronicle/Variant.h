#ifndef CHRONICLE_VARIANT_H
#define CHRONICLE_VARIANT_H

#include <Chronicle/Common.h>
#include <Chronicle/Image.h>
#include <Chronicle/String.h>
#include <Chronicle/Types.h>

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
    A container class for a value, supporting multiple types.
    For object types (e.g. TypeString), variants are <em>implicitly shared</em>,
    meaning that copied variants point to the same object (e.g. String).
    @headerfile Chronicle/Variant.h <Chronicle/Variant.h>
 */
class CHRONICLESHARED_EXPORT Variant
{
public:
    //! The possible value types for a variant.
    enum Type
    {
        TypeInvalid,            //!< The variant is invalid.
        TypeInt,                //!< The variant has a signed integer value.
        TypeUnsignedInt,        //!< The variant has an unsigned integer value.
        TypeBool,               //!< The variant has a boolean value.
        TypeLongLong,           //!< The variant has a signed long long integer value.
        TypeUnsignedLongLong,   //!< The variant has an unsigned long long integer value.
        TypeFloat,              //!< The variant has a single-precision floating point value.
        TypeDouble,             //!< The variant has a double-precision floating point value.
        TypeString,             //!< The variant has a String value.
        TypeColor,              //!< The variant has a Color value.
        TypePoint,              //!< The variant has a Point value.
        TypePointF,             //!< The variant has a PointF value.
        TypeRectangle,          //!< The variant has a Rectangle value.
        TypeRectangleF,         //!< The variant has a RectangleF value.
        TypeVector2D,           //!< The variant has a Vector2D value.
        TypeVector3D,           //!< The variant has a Vector3D value.
        TypeBox3D,              //!< The variant has a Box3D value.
        TypeImage,              //!< The variant has an Image value.
        NumberOfTypes           //!< @private
    };

    /*!
        Constructs an invalid variant.
        @see isValid(), clear()
     */
    Variant();
    /*!
        Constructs a copy of a variant.
        @param other A variant.
     */
    Variant(const Variant &other);

    /*!
        Constructs a variant with a signed integer value.
        @param val The signed integer value.
        @see setValue(int), toInt()
     */
    Variant(int val);
    /*!
        Constructs a variant with an unsigned integer value.
        @param val The unsigned integer value.
        @see setValue(unsigned int), toUnsignedInt()
     */
    Variant(unsigned int val);
    /*!
        Constructs a variant with a signed long long integer value.
        @param val The signed integer value.
        @see setValue(long long), toLongLong()
     */
    Variant(long long val);
    /*!
        Constructs a variant with an unsigned integer value.
        @param val The unsigned integer value.
        @see setValue(unsigned long long), toUnsignedLongLong()
     */
    Variant(unsigned long long val);
    /*!
        Constructs a variant with a boolean value.
        @param val The boolean value.
        @see setValue(bool), toBool()
     */
    Variant(bool val);
    /*!
        Constructs a variant with a single-precision floating point value.
        @param val The single-precision floating point value.
        @see setValue(float), toFloat()
     */
    Variant(float val);
    /*!
        Constructs a variant with a double-precision floating point value.
        @param val The double-precision floating point value.
        @see setValue(double), toDouble()
     */
    Variant(double val);
    /*!
        Constructs a variant with a string value.
        @param str The string value.
        @see setValue(const String &), toString()
     */
    Variant(const String &str);
    /*!
        Constructs a variant with a color value.
        @param clr The color value.
        @see setValue(const Color &), toColor()
     */
    Variant(const Color &clr);
    /*!
        Constructs a variant with a point value.
        @param pt The point value.
        @see setValue(const Point &), toPoint()
     */
    Variant(const Point &pt);
    /*!
        Constructs a variant with a point value.
        @param pt The point value.
        @see setValue(const PointF &), toPointF()
     */
    Variant(const PointF &pt);
    /*!
        Constructs a variant with a rectangle value.
        @param rect The rectangle value.
        @see setValue(const Rectangle &), toRectangle()
     */
    Variant(const Rectangle &rect);
    /*!
        Constructs a variant with a rectangle value.
        @param rect The rectangle value.
        @see setValue(const RectangleF &), toRectangleF()
     */
    Variant(const RectangleF &rect);
    /*!
        Constructs a variant with a 2D vector value.
        @param vec The 2D vector value.
        @see setValue(const Vector2D &), toVector2D()
     */
    Variant(const Vector2D &vec);
    /*!
        Constructs a variant with a 3D vector value.
        @param vec The 3D vector value.
        @see setValue(const Vector3D &), toVector3D()
     */
    Variant(const Vector3D &vec);
    /*!
        Constructs a variant with a 3D box value.
        @param box The 3D box value.
        @see setValue(const Box3D &), toBox3D()
     */
    Variant(const Box3D &box);
    /*!
        Constructs a variant with an image value.
        @param img The image value.
        @see setValue(const Image &), toImage()
     */
    Variant(const Image &img);

    /*!
        \fn Variant(const wchar_t *str)
        Constructs a variant with the Unicode string \a str.
        @param str A pointer to null-terminated wide character data.
        @see setValue(const wchar_t *), toString()
    */
#if defined(_MSC_VER)
    Variant(const __wchar_t *str);
    Variant(const unsigned __int16 *str);
#else
    Variant(const wchar_t *str);
#endif

    //! Destroys the variant.
    ~Variant();

    /*!
        Sets the variant to be a copy of \a other.
        @param other The variant to copy.
        @return Returns a reference to this variant.
    */
    Variant & operator=(const Variant &other);

    /*!
        Erases the variant value, making it invalid.
        @see isValid(), Variant()
     */
    void clear();

    /*!
        Sets the variant to be a copy of \a other.
        @param other The variant to copy.
    */
    void setValue(const Variant &other);

    /*!
        Replaces the existing value with a signed integer value.
        @param val The signed integer value.
        @see Variant(int), toInt()
     */
    void setValue(int val);
    /*!
        Replaces the existing value with an unsigned integer value.
        @param val The unsigned integer value.
        @see Variant(unsigned int), toUnsignedInt()
     */
    void setValue(unsigned int val);
    /*!
        Replaces the existing value with a signed long integer value.
        @param val The signed long long integer value.
        @see Variant(long long), toLongLong()
     */
    void setValue(long long val);
    /*!
        Replaces the existing value with an unsigned long long integer value.
        @param val The unsigned long long integer value.
        @see Variant(unsigned long long), toUnsignedLongLong()
     */
    void setValue(unsigned long long val);
    /*!
        Replaces the existing value with a boolean value.
        @param val The boolean value.
        @see Variant(bool), toBool()
     */
    void setValue(bool val);
    /*!
        Replaces the existing value with a single-precision floating point value.
        @param val The single-precision floating point value.
        @see Variant(float), toFloat()
     */
    void setValue(float val);
    /*!
        Replaces the existing value with a double-precision floating point value.
        @param val The double-precision floating point value.
        @see Variant(double), toDouble()
     */
    void setValue(double val);
    /*!
        Replaces the existing value with a string value.
        @param str The string value.
        @see Variant(const String &), toString()
     */
    void setValue(const String &str);
    /*!
        \fn setValue(const wchar_t *str)
        Replaces the existing value with a Unicode string value.
        @param str A pointer to null-terminated wide character data.
        @see Variant(const wchar_t *), toString()
    */
#if defined(_MSC_VER)
    void setValue(const __wchar_t *str);
    void setValue(const unsigned __int16 *str);
#else
    void setValue(const wchar_t *str);
#endif
    /*!
        Replaces the existing value with a color value.
        @param clr The color value.
        @see Variant(const Color &), toColor()
     */
    void setValue(const Color &clr);
    /*!
        Replaces the existing value with a point value.
        @param pt The point value.
        @see Variant(const Point &), toPoint()
     */
    void setValue(const Point &pt);
    /*!
        Replaces the existing value with a point value.
        @param pt The point value.
        @see Variant(const PointF &), toPointF()
     */
    void setValue(const PointF &pt);
    /*!
        Replaces the existing value with a rectangle value.
        @param rect The rectangle value.
        @see Variant(const Rectangle &), toRectangle()
     */
    void setValue(const Rectangle &rect);
    /*!
        Replaces the existing value with a rectangle value.
        @param rect The rectangle value.
        @see Variant(const RectangleF &), toRectangleF()
     */
    void setValue(const RectangleF &rect);
    /*!
        Replaces the existing value with a 2D vector value.
        @param vec The 2D vector value.
        @see Variant(const Vector2D &), toVector2D()
     */
    void setValue(const Vector2D &vec);
    /*!
        Replaces the existing value with a 3D vector value.
        @param vec The 3D vector value.
        @see Variant(const Vector3D &), toVector3D()
     */
    void setValue(const Vector3D &vec);
    /*!
        Replaces the existing value with a 3D box value.
        @param box The 3D box value.
        @see Variant(const Box3D &), toBox3D()
     */
    void setValue(const Box3D &box);
    /*!
        Replaces the existing value with an image value.
        @param img The image value.
        @see Variant(const Image &), toImage()
     */
    void setValue(const Image &img);

    /*!
        Returns \a true if the variant is valid, otherwise \a false.
        @see type()
     */
    bool isValid() const { return (_value.type != TypeInvalid); }

    /*!
        Returns the type of the value.
        @see isValid()
     */
    Type type() const { return _value.type; }

    /*!
        Returns the signed integer value.
        @see Variant(int), setValue(int)
     */
    int toInt() const;
    /*!
        Returns the unsigned integer value.
        @see Variant(unsigned int), setValue(unsigned int)
     */
    unsigned int toUnsignedInt() const;
    /*!
        Returns the signed long long integer value.
        @see Variant(long long), setValue(long long)
     */
    long long toLongLong() const;
    /*!
        Returns the unsigned long long integer value.
        @see Variant(unsigned long long), setValue(unsigned long long)
     */
    unsigned long long toUnsignedLongLong() const;
    /*!
        Returns the boolean value.
        @see Variant(bool), setValue(bool)
     */
    bool toBool() const;
    /*!
        Returns the single-precision floating point value.
        @see Variant(float), setValue(float)
     */
    float toFloat() const;
    /*!
        Returns the double-precision floating point value.
        @see Variant(double), setValue(double)
     */
    double toDouble() const;
    /*!
        Returns the variant as a String if the variant has type()
        TypeString, TypeInt, TypeUnsignedInt, TypeLongLong,
        TypeUnsignedLongLong, TypeBool, TypeFloat, or TypeDouble;
        otherwise returns an empty String.
        @see Variant(const String &), Variant(const wchar_t *),
             setValue(const String &), setValue(const wchar_t *)
     */
    String toString() const;
    /*!
        Returns the variant as a Color if the variant has type()
        TypeColor; otherwise returns a null Color.
        @see Variant(const Color &), setValue(const Color &)
     */
    Color toColor() const;
    /*!
        Returns the variant as a Point if the variant has type()
        TypePoint or TypePointF; otherwise returns a null Point.
        @see Variant(const Point &), setValue(const Point &)
     */
    Point toPoint() const;
    /*!
        Returns the variant as a PointF if the variant has type()
        TypePointF, TypeVector2D or TypePoint;
        otherwise returns a null PointF.
        @see Variant(const PointF &), setValue(const PointF &)
     */
    PointF toPointF() const;
    /*!
        Returns the variant as a Rectangle if the variant has type()
        TypeRectangle or TypeRectangleF; otherwise returns a null Rectangle.
        @see Variant(const Rectangle &), setValue(const Rectangle &)
     */
    Rectangle toRectangle() const;
    /*!
        Returns the variant as a RectangleF if the variant has type()
        TypeRectangleF or TypeRectangle; otherwise returns a null RectangleF.
        @see Variant(const RectangleF &), setValue(const RectangleF &)
     */
    RectangleF toRectangleF() const;
    /*!
        Returns the variant as a Vector2D if the variant has type()
        TypeVector3D or TypePoint; otherwise returns a null Vector2D.
        @see Variant(const Vector2D &), setValue(const Vector2D &)
     */
    Vector2D toVector2D() const;
    /*!
        Returns the variant as a Vector3D if the variant has type()
        TypeVector3D; otherwise returns a null Vector3D.
        @see Variant(const Vector3D &), setValue(const Vector3D &)
     */
    Vector3D toVector3D() const;
    /*!
        Returns the variant as a Box3D if the variant has type()
        TypeBox3D; otherwise returns an empty Box3D.
        @see Variant(const Box3D &), setValue(const Box3D &)
     */
    Box3D toBox3D() const;
    /*!
        Returns the variant as an Image if the variant has type()
        TypeImage or TypeString; otherwise returns a null Image.
        @see Variant(const Image &), setValue(const Image &)
     */
    Image toImage() const;

private:
    // char is not supported, use wchar_t instead!
    Variant(const char *); // not implemented
    void setValue(const char *); // not implemented

private:
    //! \private
    struct SharedPtr
    {
        SharedPtr(void *v) : ptr(v), ref(1) {}
        void *ptr;
        long ref;
    };

    //! \private
    struct Value
    {
        Value() : type(TypeInvalid), is_shared(false) {}
        //! \private
        union Data
        {
            int i;
            unsigned int u;
            bool b;
            float f;
            double d;
            long long ll;
            unsigned long long ull;
            SharedPtr *shared;
        } data;
        Type type;
        bool is_shared;
    };
    Value _value;
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

#endif // CHRONICLE_VARIANT_H
