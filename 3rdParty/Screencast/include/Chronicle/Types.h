#ifndef CHRONICLE_TYPES_H
#define CHRONICLE_TYPES_H

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
    A simple container class for an RGBA (32-bit) color value.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Color
{
    //! Constructs a null (black, no alpha) color.
    Color()
        : r(0), g(0), b(0), a(0) {}
    //! Constructs a color from \a red, \a green, \a blue and \a alpha values.
    Color(int r_, int g_, int b_, int a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}

    int r;  //!< The red component of the color (0-255)
    int g;  //!< The green component of the color (0-255)
    int b;  //!< The blue component of the color (0-255)
    int a;  //!< The alpha component of the color (0-255)
};

/*!
    A simple container class for an integer (2D) point value.
    Useful for specifying a screen position, etc.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Point
{
    //! Constructs a (0,0) point.
    Point()
        : x(0), y(0) {}
    //! Constructs an (\a x,\a y) point.
    Point(int x_, int y_)
        : x(x_), y(y_) {}

    int x;  //!< The x coordinate.
    int y;  //!< The y coordinate.
};

/*!
    A simple container class for a floating-point (2D) point value.
    Useful for specifying a canvas position, etc.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct PointF
{
    //! Constructs a (0.0,0.0) point.
    PointF()
        : x(0), y(0) {}
    //! Constructs an (\a x,\a y) point.
    PointF(double x_, double y_)
        : x(x_), y(y_) {}

    double x;   //!< The x coordinate.
    double y;   //!< The y coordinate.
};

/*!
    A simple container class for an integer rectangle value.
    Useful for specifying a screen region, etc.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Rectangle
{
    //! Constructs an empty rectangle.
    Rectangle()
        : x(0), y(0), width(0), height(0) {}
    //! Constructs a rectangle at origin (\a x_,\a y_) with size (\a width,\a height).
    Rectangle(int x_, int y_, int width_, int height_)
        : x(x_), y(y_), width(width_), height(height_) {}

    //! Returns \a true if the rectangle is empty, otherwise \a false.
    bool isEmpty() const { return (!width && !height); }

    int x;      //!< The x coordinate of the rectangle origin.
    int y;      //!< The y coordinate of the rectangle origin.
    int width;  //!< The width of the rectangle.
    int height; //!< The height of the rectangle.
};

/*!
    A simple container class for a floating-point rectangle value.
    Useful for specifying a canvas region, etc.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct RectangleF
{
    //! Constructs an empty rectangle.
    RectangleF()
        : x(0), y(0), width(0), height(0) {}
    //! Constructs a rectangle at origin (\a x_,\a y_) with size (\a width,\a height).
    RectangleF(double x_, double y_, double width_, double height_)
        : x(x_), y(y_), width(width_), height(height_) {}

    double x;       //!< The x coordinate of the rectangle origin.
    double y;       //!< The y coordinate of the rectangle origin.
    double width;   //!< The width of the rectangle.
    double height;  //!< The height of the rectangle.
};

/*!
    A simple container class for a 2D vector.
    Useful for specifying a position or direction in 2D.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Vector2D
{
    //! Constructs a null (0.0,0.0) vector.
    Vector2D()
        : x(0), y(0) {}
    //! Constructs an (\a x_,\a y_) vector.
    Vector2D(double x_, double y_)
        : x(x_), y(y_) {}

    double x;   //!< The x coordinate.
    double y;   //!< The y coordinate.
};


/*!
    A simple container class for a 3D vector.
    Useful for specifying a position or direction in 3D.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Vector3D
{
    //! Constructs a null (0.0,0.0,0.0) vector.
    Vector3D()
        : x(0), y(0), z(0) {}
    //! Constructs an (\a x_,\a y_,\a z) vector.
    Vector3D(double x_, double y_, double z_)
        : x(x_), y(y_), z(z_) {}

    double x;   //!< The x coordinate.
    double y;   //!< The y coordinate.
    double z;   //!< The z coordinate.
};

/*!
    A simple container class for a 3D box.
    Useful for specifying a region in 3D.
    @headerfile Chronicle/Types.h <Chronicle/Types.h>
 */
struct Box3D
{
    //! Constructs an empty box.
    Box3D() {}
    //! Constructs a box with the specified \a center and \a extent.
    Box3D(const Vector3D &center_, const Vector3D &extent_)
        : center(center_), extent(extent_) {}

    Vector3D center;    //!< The center point.
    Vector3D extent;    //!< The extent.
};

//! Returns \a true if \a clr1 is equal to \a clr2, otherwise \a false.
inline bool operator==(const Color &clr1, const Color &clr2)
{
    return (clr1.r == clr2.r && clr1.g == clr2.g &&
            clr1.b == clr2.b && clr1.a == clr2.a);
}

//! Returns \a true if \a pt1 is equal to \a pt2, otherwise \a false.
inline bool operator==(const Point &pt1, const Point &pt2)
{
    return (pt1.x == pt2.x && pt1.y == pt2.y);
}

//! Returns \a true if \a pt1 is equal to \a pt2, otherwise \a false.
inline bool operator==(const PointF &pt1, const PointF &pt2)
{
    return (pt1.x == pt2.x && pt1.y == pt2.y);
}

//! Returns \a true if \a rect1 is equal to \a rect2, otherwise \a false.
inline bool operator==(const Rectangle &rect1, const Rectangle &rect2)
{
    return (rect1.x == rect2.x && rect1.y == rect2.y &&
        rect1.width == rect2.width && rect2.height == rect2.height);
}

//! Returns \a true if \a rect1 is equal to \a rect2, otherwise \a false.
inline bool operator==(const RectangleF &rect1, const RectangleF &rect2)
{
    return (rect1.x == rect2.x && rect1.y == rect2.y &&
        rect1.width == rect2.width && rect2.height == rect2.height);
}

//! Returns \a true if \a vec1 is equal to \a vec2, otherwise \a false.
inline bool operator==(const Vector2D &vec1, const Vector2D &vec2)
{
    return (vec1.x == vec2.x && vec1.y == vec2.y);
}

//! Returns \a true if \a vec1 is equal to \a vec2, otherwise \a false.
inline bool operator==(const Vector3D &vec1, const Vector3D &vec2)
{
    return (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z);
}

//! Returns \a true if \a box1 is equal to \a box2, otherwise \a false.
inline bool operator==(const Box3D &box1, const Box3D &box2)
{
    return (box1.center == box2.center && box1.extent == box2.extent);
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

#endif // CHRONICLE_TYPES_H
