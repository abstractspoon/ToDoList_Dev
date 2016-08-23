#ifndef CHRONICLE_METADATA_H
#define CHRONICLE_METADATA_H

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
    The collection of Chronicle metadata definitions.
    @headerfile Chronicle/Metadata.h <Chronicle/Metadata.h>
 */
class CHRONICLESHARED_EXPORT Metadata
{
public:
    /*!
        The list of canonical Waypoint categories.
        @see toString(WaypointCategory), Facade::createWaypoint(const String &), Waypoint class
     */
    enum WaypointCategory
    {
        DialogsCategory,            //!< Dialog box waypoints.
        FilesCategory,              //!< Files and documents waypoints.
        ProductsCategory,           //!< Products waypoints.
        SettingsCategory,           //!< Settings waypoints.
        ToolsCategory,              //!< Tools and commands waypoints.
        ViewCategory,               //!< View and camera waypoints.
        WaypointCategoryCount       //!< @private
    };

    /*!
        The list of canonical Attribute names.
        @see toString(AttributeName), Attribute class, Waypoint class
     */
    enum AttributeName
    {
        Camera,                     //!< Parent of \a Camera* subattributes
        CameraAspectRatio,          //!< Variant::TypeDouble; subattribute of \a Camera
        CameraCenter,               //!< Variant::TypeVector3D; subattribute of \a Camera
        CameraClipBottomTop,        //!< Variant::TypeVector2D; subattribute of \a Camera
        CameraClipLeftRight,        //!< Variant::TypeVector2D; subattribute of \a Camera
        CameraClipNearFar,          //!< Variant::TypeVector2D; subattribute of \a Camera
        CameraEye,                  //!< Variant::TypeVector3D; subattribute of \a Camera
        CameraFOVY,                 //!< Variant::TypeDouble (in degrees); subattribute of \a Camera
        CameraName,                 //!< Variant::TypeString; subattribute of \a Camera
        CameraIsOrthographic,       //!< Variant::TypeBool (default \a true for 2D, \a false for 3D); subattribute of \a Camera
        CameraUp,                   //!< Variant::TypeVector3D; subattribute of \a Camera
        CommandCategory,            //!< Variant::TypeString
        CommandInvokedByScript,     //!< Variant::TypeBool
        CommandName,                //!< Variant::TypeString
        CommandParameters,          //!< Parent of command parameter subattributes
        DialogAction,               //!< Variant::TypeString @see toString(DialogActionType)
        DialogImage,                //!< Variant::TypeImage or Variant::TypeString (path or URL of image file)
        DialogItem,                 //!< Parent of \a DialogItem* subattributes
        DialogItemName,             //!< Variant::TypeString
        DialogItemPreviousValue,    //!< Subattribute of \a DialogItem
        DialogItemRectangle,        //!< Variant::TypeRectangle (relative to \a DialogRectangle origin)
        DialogItemValue,            //!< Subattribute of \a DialogItem
        DialogRectangle,            //!< Variant::TypeRectangle
        DialogTitle,                //!< Variant::TypeString
        DocumentAction,             //!< Variant::TypeString @see toString(DocumentActionType)
        DocumentBounds,             //!< Variant::TypeRectangle, Variant::TypeRectangleF or Variant::TypeBox3D
        DocumentChangeRegion,       //!< Variant::TypeRectangle, Variant::TypeRectangleF or Variant::TypeBox3D
        DocumentHasUnsavedChanges,  //!< Variant::TypeBool
        DocumentName,               //!< Variant::TypeString
        DocumentPath,               //!< Variant::TypeString
        DocumentSettings,           //!< Parent of document setting subattributes
        DocumentThumbnail,          //!< Variant::TypeImage or Variant::TypeString (path or URL of image file)
        DocumentURL,                //!< Variant::TypeString
        Plugin,                     //!< Parent of \a Plugin* subattributes
        PluginBuild,                //!< Variant::TypeString; subattribute of \a Plugin
        PluginEnabled,              //!< Variant::TypeBool; subattribute of \a Plugin
        PluginName,                 //!< Variant::TypeString; subattribute of \a Plugin
        PluginPath,                 //!< Variant::TypeString; subattribute of \a Plugin
        PluginSettings,             //!< Parent of plug-in setting subattributes; subattribute of \a Plugin
        PluginURL,                  //!< Variant::TypeString; subattribute of \a Plugin
        PluginVersion,              //!< Variant::TypeString; subattribute of \a Plugin
        ProductBuild,               //!< Variant::TypeString
        ProductLanguage,            //!< Variant::TypeString
        ProductLicense,             //!< Variant::TypeString
        ProductLine,                //!< Variant::TypeString
        ProductMaster,              //!< Varaint::TypeString
        ProductName,                //!< Variant::TypeString
        ProductRelease,             //!< Variant::TypeString
        ProductSettings,            //!< Parent of product setting subattributes
        ProductURL,                 //!< Variant::TypeString
        ProductVersion,             //!< Variant::TypeString
        ScriptName,                 //!< Variant::TypeString
        ScriptParameters,           //!< Parent of script parameter subattributes
        ScriptPath,                 //!< Variant::TypeString
        ScriptSettings,             //!< Parent of script setting subattributes
        SettingPreviousValue,       //!< Subattribute of setting subattributes
        SystemGraphics,             //!< Variant::TypeString
        SystemLanguage,             //!< Variant::TypeString
        SystemMemory,               //!< Variant::TypeUnsignedLongLong
        SystemOS,                   //!< Variant::TypeString
        SystemVariables,            //!< Parent of system variable subattributes
        ToolCategory,               //!< Variant::TypeString
        ToolIcon,                   //!< Variant::TypeImage or Variant::TypeString (path or URL of image file)
        ToolID,                     //!< Variant::TypeInt, Variant::TypeUnsignedInt or Variant::TypeString
        ToolInvokedByScript,        //!< Variant::TypeBool
        ToolName,                   //!< Variant::TypeString
        ToolParameters,             //!< Parent of tool parameter subattributes
        ToolSettings,               //!< Parent of tool setting subattributes
        TooltipID,                  //!< Variant::TypeInt, Variant::TypeUnsignedInt or Variant::TypeString
        TooltipImage,               //!< Variant::TypeImage or Variant::TypeString (path or URL of image file)
        TooltipName,                //!< Variant::TypeString
        TooltipText,                //!< Variant::TypeString
        UPIConfigFilePath,          //!< Variant::TypeString
        User,                       //!< Parent of \a User* subattributes
        UserEmail,                  //!< Variant::TypeString; subattribute of \a User
        UserID,                     //!< Variant::TypeString; subattribute of \a User
        UserName,                   //!< Variant::TypeString; subattribute of \a User
        UserURL,                    //!< Variant::TypeString; subattribute of \a User
        Viewport,                   //!< Parent of \a Viewport* subattributes
        ViewportBounds,             //!< Variant::TypeRectangle or Variant::TypeRectangleF; subattribute of \a Viewport
        ViewportName,               //!< Variant::TypeString; subattribute of \a Viewport
        ViewportRectangle,          //!< Variant::TypeRectangle or Variant::TypeRectangleF; subattribute of \a Viewport
        AttributeNameCount          //!< @private
    };

    //! The list of canonical dialog actions. @see DialogAction
    enum DialogActionType
    {
        ModalDialog,                //!< A modal dialog was opened and closed.
        ModelessDialogOpen,         //!< A modeless dialog was opened.
        ModelessDialogItemChange,   //!< A modeless dialog item changed value.
        ModelessDialogClose,        //!< A modeless dialog was closed.
        DialogActionCount           //!< @private
    };

    //! The list of canonical document actions. @see DocumentAction
    enum DocumentActionType
    {
        DocumentNew,                //!< A new document was created.
        DocumentActivate,           //!< The document was activated (made the current document).
        DocumentOpen,               //!< The document was opened.
        DocumentSave,               //!< The document was saved.
        DocumentRevert,             //!< The document was being reverted to its last save.
        DocumentClose,              //!< The document was closed.
        DocumentActionCount         //!< @private
    };

    //! Returns the text string for \a name.
    static String toString(AttributeName name);

    //! Returns the text string for \a action.
    static String toString(DialogActionType action);

    //! Returns the text string for \a action.
    static String toString(DocumentActionType action);

    //! Returns the text string for \a category.
    static String toString(WaypointCategory category);

    //! Returns \a true if the canonical value for an Attribute named \a name is an image, otherwise \a false.
    static bool hasImageValue(const String &name);

private:
    static const wchar_t *_attributeNames[];
    static const wchar_t *_dialogActions[];
    static const wchar_t *_documentActions[];
    static const wchar_t *_waypointCategories[];
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

#endif // CHRONICLE_METADATA_H
