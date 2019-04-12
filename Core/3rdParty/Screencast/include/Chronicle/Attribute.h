#ifndef CHRONICLE_ATTRIBUTE_H
#define CHRONICLE_ATTRIBUTE_H

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

class AttributeList;

/*!
    A named value, with optional subattributes.

    Use the Waypoint::addAttribute(const String &name, const String &value)
    to create and add an attribute to a waypoint.

    Or use Attribute::create(const String &name, const Variant &value) to
    create an attribute and Waypoint::addAttribute(Attribute *attribute)
    to add the attribute to a waypoint.

    Optionally use addSubattribute() to add one or more subattributes to an
    attribute. It is also possible to add subattributes to a subattribute.

    @headerfile Chronicle/Attribute.h <Chronicle/Attribute.h>
    @see Waypoint class
 */
class CHRONICLESHARED_EXPORT Attribute
{
public:
    /*!
        Creates an attribute with a name and value.

        Optionally add subattributes using addSubattribute().

        @warning The caller assumes ownership of the attribute until the
        attribute is added to a waypoint by calling
        Waypoint::addAttribute(Attribute *attribute).

        @param name The attribute name.
        @param value The attribute value.
        @returns An %Attribute instance.
     */
    static Attribute *create(const String &name, const Variant &value = Variant());

    /*!
        Destroys the attribute.
        @warning Do \a not destroy an attribute that has been added
        to a waypoint.
     */
    void destroy();

    /*!
        Sets the attribute name.
        The name is initially set when the attribute is constructed,
        but the name can be changed afterwards.
        @param name The attribute name.
        @see name()
     */
    void setName(const String &name);
    /*!
        Returns the attribute name.
        @see setName()
     */
    const String &name() const { return _name; }

    /*!
        Sets the attribute value.
        The value is initially set when the attribute is constructed,
        but the value can be changed afterwards.
        @param value The attribute value.
        @see value()
     */
    void setValue(const Variant &value);
    /*!
        Returns the attribute value.
        @see setValue()
     */
    const Variant &value() const { return _value; }

    /*!
        Adds a subattribute to this attribute.
        @param name The attribute name.
        @param value The attribute value.
        @returns A pointer to the newly-created attribute.
     */
    Attribute *addSubattribute(const String &name, const Variant &value = Variant());
    /*!
        Returns \a true if this attribute has subattributes,
        otherwise \a false.
        @see addSubattribute(), subattributes()
     */
    bool hasSubattributes() const;
    /*!
        Returns the collection of subattributes for this attribute.
        @warning The return value is \a NULL if there are no subattributes.
        @warning Do \a not attempt to use the collection after the owner
        waypoint has been passed to the Facade, or after the attribute
        itself has been destroyed.
        @returns A pointer to the attribute list.
        @see hasSubattributes(), addSubattribute()
     */
    AttributeList *subattributes() const { return _subattributes; }

    /*!
        Returns a copy of the attribute. All subattributes are also copied.
        @see AttributeList::clone()
     */
    Attribute *clone() const;

private:
    friend class Waypoint;

    Attribute();
    Attribute(const String &name, const Variant &value);
    Attribute(const Attribute &other);
    ~Attribute();

private:
    String _name;
    Variant _value;
    AttributeList *_subattributes;
};

/*!
    An ordered collection of attributes.
    @see Attribute, Waypoint classes
    @headerfile Chronicle/Attribute.h <Chronicle/Attribute.h>
 */
class CHRONICLESHARED_EXPORT AttributeList
{
private:
    friend class Attribute;
    friend class Waypoint;

    AttributeList();
    AttributeList(const AttributeList &other);
    ~AttributeList();

    static AttributeList _empty;

public:
    /*!
        Adds \a attribute to the end of the list.
        @note The list assumes ownership of \a attribute.
        @param attribute The attribute to add.
        @returns The position of the attribute in the list.
        @see at(), count()
     */
    Index add(Attribute *attribute);
    /*!
        Returns the attribute at the specified position in the list.
        @warning Do not destroy the returned attribute.
        @param index A value in the range of [0 .. count() - 1].
        @return The pointer to the attribute, or \a NULL if the index is out of range.
        @see count(), isEmpty()
     */
    Attribute *at(Index index) const;
    /*!
        Removes and deletes all attributes in the list.
     */
    void clear();
    /*!
        Returns a copy of the attribute list.
        @see Attribute::clone()
     */
    AttributeList *clone() const;
    /*!
        Returns the number of items in the list.
        @see isEmpty()
     */
    Index count() const;
    /*!
        Returns \a true if the list has no items, otherwise \a false.
        @see count()
     */
    bool isEmpty() const;
    /*!
        Searches the list for an attribute for a specific \a name.
        @warning Do not destroy the returned attribute.
        @returns The first attribute in the list with the name \a name,
        or \a NULL if no attribute in the list has that name.
     */
    Attribute *find(const String &name) const;
    /*!
        Removes \a attribute from the list.
        @param attribute The attribute to remove.
        @returns If successful, the pointer to the attribute, otherwise \a NULL.
        @note The caller assumes ownership of the attribute.
     */
    Attribute *remove(Attribute *attribute);
    /*!
        Removes the attribute at the specified position in the list.
        @param index A value in the range of [0 .. count() - 1].
        @returns If successful, the pointer to the attribute, otherwise \a NULL.
        @note The caller assumes ownership of the attribute.
        @see count(), at()
     */
    Attribute *removeAt(Index index);

private:
    struct AttributeListImpl *_impl;
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

#endif // CHRONICLE_ATTRIBUTE_H
