#ifndef CHRONICLE_WAYPOINT_H
#define CHRONICLE_WAYPOINT_H

#include <Chronicle/Attribute.h>
#include <Chronicle/Common.h>
#include <Chronicle/String.h>
#include <Chronicle/Variant.h>

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
    A waypoint represents an event in the metadata stream.

    It encapsulates a category, an attribute, a timestamp and a duration.

    To create an instance of a %Waypoint, use the Facade::createWaypoint() function.

    @headerfile Chronicle/Waypoint.h <Chronicle/Waypoint.h>
    @see Attribute class
 */
class CHRONICLESHARED_EXPORT Waypoint
{
public:
    /*!
        Destroys the waypoint.
        @warning Do \a not destroy a waypoint that has been passed
        to the Facade::waypointReached() member function.
     */
    void destroy();

    /*!
        Sets the waypoint category.
        The category is initially set when the waypoint is constructed,
        but the category can be changed afterwards.
        @param category The waypoint category.
        @see category()
     */
    void setCategory(const String &category);
    /*!
        Returns the waypoint category.
        @see Facade::createWaypoint(), setCategory()
     */
    const String &category() const { return _category; }

    /*!
        Sets the waypoint time.
        The time is automatically set by the waypoint is constructed,
        but the time can be changed afterwards.
        @param time The waypoint time.
        @see time()
     */
    void setTime(Time time);
    /*!
        Returns the waypoint time.
        The time is automatically set when the waypoint is constructed.
        @see Facade::createWaypoint(), setTime()
     */
    Time time() const { return _time; }

    /*!
        Sets the waypoint duration.
        The default duration is zero (0).
        @param duration The waypoint duration.
        @see duration()
     */
    void setDuration(Duration duration);
    /*!
        Returns the waypoint duration.
        @see setDuration()
     */
    Duration duration() const { return _duration; }

    /*!
        Adds an attribute to the waypoint.

        @warning The waypoint has ownership of the attribute; do \a not destroy
        the attribute after this method returns, and do \a not attempt to
        access the attribute once ownership of the waypoint has been passed
        to the Facade or after the waypoint itself has been destroyed.

        @param name The name of the attribute.
        @param value The value of the attribute.
        @return A pointer to the newly-created attribute.
     */
    Attribute *addAttribute(const String &name, const Variant &value = Variant());

    /*!
        Adds an attribute to the waypoint.

        @warning The waypoint takes ownership of the attribute; do \a not destroy
        the attribute after this method returns, and do \a not attempt to
        access the attribute once ownership of the waypoint has been passed
        to the Facade or after the waypoint itself has been destroyed.

        Adding an attribute that is already in the waypoint has no effect.

        @param attribute A pointer to an attribute.
     */
    void addAttribute(Attribute *attribute);

    /*!
        Adds a subattribute to a parent attribute in the waypoint.

        @note If there is no attribute with the name \a parentName then it will be
        automatically added to the waypoint.

        @warning The waypoint has ownership of the subattribute; do \a not destroy
        the subattribute after this method returns, and do \a not attempt to
        access the subattribute once ownership of the waypoint has been passed
        to the Facade or after the waypoint itself has been destroyed.

        @param parentName The name of the parent attribute.
        @param name The name of the subattribute.
        @param value The value of the subattribute.
        @return A pointer to the newly-created subattribute.
     */
    Attribute *addSubattribute(const String &parentName,
                               const String &name, const Variant &value = Variant());

    /*!
        Returns \a true if the waypoint has attributes, otherwise \a false.
        @see addAttribute(const String &name, const String &value),
             addAttribute(Attribute *attribute), attributes()
     */
    bool hasAttributes() const;

    /*!
        Returns the collection of attributes for this waypoint.
        @warning The return value is \a NULL if there are no attributes.
        @warning Do \a not attempt to use the collection after once
        the waypoint has been passed to the Facade, or after the waypoint
        itself has been destroyed.
        @return A pointer to the attribute list, or \a NULL if no attributes
        have been added to the waypoint.
        @see addAttribute(const String &name, const String &value),
             addAttribute(Attribute *attribute), hasAttributes()
     */
     AttributeList *attributes() const { return _attributes; }

     /*!
        Returns a copy of the waypoint. All attributes are also copied.
        @see AttributeList::clone()
      */
     Waypoint *clone() const;

private:
    friend class Facade;

    Waypoint();
    Waypoint(const String &category);
    Waypoint(const Waypoint &other);
    ~Waypoint();

private:
    String _category;
    Time _time;
    Duration _duration;
    AttributeList *_attributes;
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

#endif // CHRONICLE_WAYPOINT_H
