#ifndef CHRONICLE_FACADE_H
#define CHRONICLE_FACADE_H

#include <Chronicle/Attribute.h>
#include <Chronicle/Common.h>
#include <Chronicle/String.h>
#include <Chronicle/Types.h>
#include <Chronicle/Variant.h>
#include <Chronicle/Waypoint.h>

#ifndef CHRONICLE_CHRONICLE_DECL_H
#  include "Chronicle_decl.h"
#endif

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable:4251)
#endif


namespace Chronicle
{

class FacadeObserver;

/*!
    The %Facade class defines the interface to the Chronicle facility.

    It is not possible (or necessary) to construct a %Facade instance;
    all of the member functions are static.

    @note You must call the initialize() member function (once) before
    invoking the other member functions.

    @warning When you are finished with the %Facade, be sure to invoke
    the cleanup() member function; otherwise the facility will not be
    shutdown properly and resources will not be freed.

    @headerfile Chronicle/Facade.h <Chronicle/Facade.h>
 */
class CHRONICLESHARED_EXPORT Facade
{
public:
    /*!
        Initializes the %Chronicle facility.
        @note You must call this method before invoking other methods.
        If the %Facade is already initialized, calling this method has no effect.
        @warning You must call cleanup() before the process exits.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isInitialized()
     */
    static Error initialize();
    /*!
        Frees in-process resources used by the %Chronicle facility.
        If the %Facade is not initialized, calling this method has no effect.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isInitialized()
     */
    static Error cleanup();

    /*!
        Queries the intialized state.
        @returns \a true if the %Facade has been initialized, otherwise \a false.
        @see initialize(), cleanup()
     */
    static bool isInitialized();

    /*!
        Starts the capture.
        If a capture is already in progress, calling this method has no effect.
        To stop the capture, call the stopCapture() method.
        To pause the capture, call the pauseCapture() method.
        To resume a paused capture, call the resumeCapture() method.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isCapturing(), canStartCapture()
     */
    static Error startCapture();
    /*!
        Pauses the capture in progress.
        If there is no capture in progress, or the capture is already paused,
        calling this method has no effect.
        To resume the capture, call the resumeCapture() method.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isCapturing(), isCapturePaused(), canPauseCapture()
     */
    static Error pauseCapture();
    /*!
        Resumes the paused capture in progress.
        If there is no capture in progress, or the capture is not paused,
        calling this method has no effect.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isCapturing(), isCapturePaused(), canResumeCapture()
     */
    static Error resumeCapture();
    /*!
        Stops the capture in progress.
        If there is no capture in progress, calling this method has no effect.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see isCapturing(), canStopCapture()
     */
    static Error stopCapture();

    /*!
        Queries the capture state.
        @returns \a true if a capture is in progress, otherwise \a false.
        @see startCapture(), isCapturePaused()
     */
    static bool isCapturing();
    /*!
        Queries the capture paused state.
        @returns \a true if there is a capture in progress \a and if it is paused, otherwise \a false.
        @see pauseCapture(), isCapturing()
     */
    static bool isCapturePaused();

    /*!
        Queries whether a capture can be started. Useful for setting the state of UI widgets.
        @returns \a true if a capture can be started (\a and there is no capture in progress), otherwise \a false.
        @see startCapture(), isInitialized(), isCapturing()
     */
    static bool canStartCapture();
    /*!
        Queries whether a capture can be paused. Useful for setting the state of UI widgets.
        @returns \a true if a capture is in progress (\a and if it is not already paused), otherwise \a false.
        @see pauseCapture(), isCapturing(), isCapturePaused()
     */
    static bool canPauseCapture();
    /*!
        Queries whether a capture can be resumed. Useful for setting the state of UI widgets.
        @returns \a true if a capture is in progress (\a and if it is paused), otherwise \a false.
        @see resumeCapture(), isCapturing(), isCapturePaused()
     */
    static bool canResumeCapture();
    /*!
        Queries whether a capture can be stopped. Useful for setting the state of UI widgets.
        @returns \a true if a capture is in progress, otherwise \a false.
        @see stopCapture(), isCapturing()
     */
    static bool canStopCapture();

    /*!
        Shows the %Chronicle utility's main window.
        The utility is made the frontmost application.
        @returns Chronicle::NoError if successful, otherwise the error code.
     */
    static Error showUtility();
    /*!
        Opens the %Chronicle website in the system-default web browser.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see openFAQ()
     */
    static Error openWebsite();
    /*!
        Opens the %Chronicle FAQ page in the system-default web browser.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see openWebsite()
     */
    static Error openFAQ();

    /*!
        Adds an observer to receive notifications from the %Facade.

        @warning The collection of observers is unordered so do \a not make any
        assumption about the order in which observers will be notified.

        @note Does \a not take ownership of the observer instance.
        Has no effect if the observer is already in the collection.

        @param observer A pointer to an instance of a FacadeObserver subclass.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see removeObserver()
     */
    static Error addObserver(FacadeObserver *observer);
    /*!
        Removes the specified observer.
        Has no effect if the observer is not in the collection.
        @param observer A pointer to the observer instance which to remove.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see addObserver()
     */
    static Error removeObserver(FacadeObserver *observer);

    /*!
        Creates a %Waypoint instance.

        Optionally add one or more attributes using Waypoint::addAttribute().
        Optionally set the duration of the waypoint using Waypoint::setDuration().
        The time of the waypoint is set automatically; this can be
        overridden using Waypoint::setTime().

        For commonly-used waypoints it is usually more convenient to call the
        appropriate static member function(s) in the Facade::Waypoints class.

        @warning The caller assumes ownership of the waypoint until the waypoint
        is recording by invoking waypointReached().

        @param category The waypoint category.
        @return A %Waypoint instance, or \a NULL if not successful.
     */
    static Waypoint *createWaypoint(const String &category);

    /*!
        Reports that the specified waypoint has been reached.

        The waypoint will be recorded in the %Chronicle metadata stream.

        Takes ownership of the specified waypoint, regardless of success.
        Do \a not attempt to access the waypoint upon return (it may already have been destroyed).

        @param waypoint A pointer to the waypoint to record.
        @returns Chronicle::NoError if successful, otherwise the error code.
        @see createWaypoint()
      */
    static Error waypointReached(Waypoint *waypoint);

    /*!
        Returns the current time.
        @see Waypoint::setTime(), Waypoint::time();
     */
    static Time currentTime();

    /*!
        Returns the elapsed amount of time since \a time.
        @see currentTime(), Waypoint::time(), Waypoint::setDuration()
     */
    static Duration elapsedTimeSince(Time time);

    /*!
        Provides convenient functions for constructing commonly- and frequently-used waypoints.
        After a Waypoint is fully constructed it still needs to be reported using Facade::waypointReached().
        @see Facade::createWaypoint()
        @headerfile Chronicle/Facade.h <Chronicle/Facade.h>
     */
    class CHRONICLESHARED_EXPORT Waypoints
    {
    public:
        /*!
            Constructs a Metadata::ToolsCategory waypoint for when a command is entered.
            @param command The command.
            @returns The constructed Waypoint instance.
            @see commandFromPlugin(), commandParameterEntered(), commandCompleted(), Facade::waypointReached()
         */
        static Waypoint *commandEntered(const String &command);
        /*!
            Adds a command parameter to a Metadata::ToolsCategory waypoint for a command.
            @param waypoint The Waypoint returned by commandEntered().
            @param parameter The parameter.
            @param value The parameter value (if there is one, default is none).
            @returns The Attribute instance added to \a waypoint.
            @see commandCompleted(), Facade::waypointReached()
         */
        static Attribute *commandParameterEntered(Waypoint *waypoint, const String &parameter, const Variant &value = Variant());
        /*!
            Completes a Metadata::ToolsCategory waypoint for a command by setting its duration.
            @param waypoint The Waypoint returned by commandEntered().
            @returns The same \a waypoint.
            @see Facade::elapsedTimeSince(), Facade::waypointReached()
         */
        static Waypoint *commandCompleted(Waypoint *waypoint);
        /*!
            Adds a plugin attribute to a Metadata::ToolsCategory waypoint for a command.
            @param waypoint The Waypoint returned by commandEntered().
            @param pluginName The plugin name.
            @returns The Attribute instance added to \a waypoint.
            @see commandParameterEntered(), commandCompleted(), Facade::waypointReached()
         */
        static Attribute *commandFromPlugin(Waypoint *waypoint, const String &pluginName);

        /*!
            Constructs a Metadata::FilesCategory waypoint for when a new document is opened.
            @param name The document name (for example, "Untitled").
            @param path The document name (if there is one; default is none).
            @param thumbnail The document thumbnail (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *newDocument(const String &name, const String &path = String(), const Variant &thumbnail = Variant());
        /*!
            Constructs a Metadata::FilesCategory waypoint for when the current document changes.
            @param name The document name (for example, "Building Plans").
            @param path The document path (if there is one; default is none).
            @param thumbnail The document thumbnail (image, path or URL; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *currentDocument(const String &name, const String &path = String(), const Variant &thumbnail = Variant());
        /*!
            Constructs a Metadata::FilesCategory waypoint for when an existing document is opened.
            @param name The document name (for example, "Small Motor Assembly").
            @param path The document path.
            @param thumbnail The document thumbnail (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *documentOpened(const String &name, const String &path, const Variant &thumbnail = Variant());
        /*!
            Constructs a Metadata::FilesCategory waypoint for when a document is saved.
            @param name The document name (for example, "My Rough Sketches").
            @param path The document path.
            @param thumbnail The document thumbnail (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *documentSaved(const String &name, const String &path, const Variant &thumbnail = Variant());
        /*!
            Constructs a Metadata::FilesCategory waypoint for when a document is reverted (to a saved version).
            @param name The document name (for example, "Financial Report").
            @param path The document path.
            @param thumbnail The document thumbnail (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *documentReverted(const String &name, const String &path, const Variant &thumbnail = Variant());
        /*!
            Constructs a Metadata::FilesCategory waypoint for when a document is closed.
            @param name The document name (for example, "Supreme Court Ruling").
            @param path The document path.
            @param thumbnail The document thumbnail (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *documentClosed(const String &name, const String &path, const Variant &thumbnail = Variant());

        /*!
            Constructs a Metadata::DialogsCategory waypoint for a modal dialog session.
            @param title The dialog title (for example, "Preferences").
            @param screenRect The rectangle of the dialog box, in screen coordinates.
            @param image A screenshot of the dialog (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see modalDialogItemChanged(), modalDialogClosed()
         */
        static Waypoint *modalDialogOpened(const String &title, const Rectangle &screenRect = Rectangle(),
                                           const Variant &image = Variant());
        /*!
            Adds an attribute to a Metadata::DialogsCategory waypoint to indicate that a dialog item
            changed value. This method should be called for each changed dialog item just before
            invoking modalDialogClosed().
            @param waypoint The Waypoint returned by modalDialogOpened().
            @param name The dialog item name (for example, "Show Status Bar").
            @param value The new value of the dialog item.
            @param previousValue The old value of the dialog item.
            @param itemRect The rectangle of the dialog item, in dialog coordinates.
            @returns The same \a waypoint.
         */
        static Waypoint *modalDialogItemChanged(Waypoint *waypoint, const String &name,
                                                const Variant &value, const Variant &previousValue = Variant(),
                                                const Rectangle &itemRect = Rectangle());
        /*!
            Completes a Metadata::DialogsCategory waypoint for a modal dialog session by setting its duration.
            @param waypoint The Waypoint returned by modalDialogOpened().
            @param screenRect The rectangle of the dialog box, in screen coordinates.
            @param image A screenshot of the dialog (image or image file path; default is none).
            @returns The same \a waypoint.
            @see modalDialogItemChanged(), Facade::elapsedTimeSince(), Facade::waypointReached()
         */
        static Waypoint *modalDialogClosed(Waypoint *waypoint, const Rectangle &screenRect = Rectangle(),
                                           const Variant &image = Variant());

        /*!
            Constructs a Metadata::DialogsCategory waypoint for when a modeless dialog box is opened.
            @param title The dialog title (for example, "Layers").
            @param screenRect The rectangle of the dialog box, in screen coordinates.
            @param image A screenshot of the dialog (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see modelessDialogItemChanged(), modelessDialogClosed(), Facade::waypointReached()
         */
        static Waypoint *modelessDialogOpened(const String &title, const Rectangle &screenRect = Rectangle(),
                                              const Variant &image = Variant());
        /*!
            Constructs a Metadata::DialogsCategory waypoint for when a modeless dialog item changes value.
            @param dialogTitle The dialog title (for example, "Layers").
            @param itemName The dialog item name (for example, "Transparency").
            @param value The new value of the dialog item.
            @param previousValue The old value of the dialog item.
            @param dialogRect The rectangle of the dialog box, in screen coordinates.
            @param itemRect The rectangle of the dialog item, in dialog coordinates.
            @param dialogImage A screenshot of the dialog (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see modelessDialogOpened(), modelessDialogClosed(), Facade::waypointReached()
         */
        static Waypoint *modelessDialogItemChanged(const String &dialogTitle,
                                                   const String &itemName,
                                                   const Variant &value,
                                                   const Variant &previousValue = Variant(),
                                                   const Rectangle &dialogRect = Rectangle(),
                                                   const Rectangle &itemRect = Rectangle(),
                                                   const Variant &dialogImage = Variant());
        /*!
            Constructs a Metadata::DialogsCategory waypoint for when a modeless dialog box is closed.
            @param title The dialog title (for example, "Layers").
            @param screenRect The rectangle of the dialog box, in screen coordinates.
            @param image A screenshot of the dialog (image or image file path; default is none).
            @returns The constructed Waypoint instance.
            @see modelessDialogOpened(), Facade::waypointReached()
         */
        static Waypoint *modelessDialogClosed(const String &title, const Rectangle &screenRect = Rectangle(),
                                              const Variant &image = Variant());

        /*!
            Constructs a Metadata::SettingsCategory waypoint for when a product (application) setting changes.
            @param name The setting name.
            @param value The setting value.
            @param previousValue The previous value of the setting.
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *productSettingChanged(const String &name, const Variant &value,
                                               const Variant &previousValue = Variant());

        /*!
            Constructs a Metadata::ProductsCategory waypoint for identifying the product.
            @note For products that have integrated UPI, use registerProductFromUPIConfigFile() instead.
            @param name The product name.
            @param version The product version.
            @param build The product build.
            @param url The product URL.
            @returns The constructed Waypoint instance.
            @note After this function returns, report the product language with productLanguage().
            @see Facade::waypointReached()
         */
        static Waypoint *registerProduct(const String &name, const String &version,
                                         const String &build, const String &url = String());
        /*!
            Constructs a Metadata::ProductsCategory waypoint for identifying the product.
            @note If your product hasn't integrated UPI, use registerProduct() instead.
            @param path The path to the product UPI configuration file.
            @returns The constructed Waypoint instance.
            @note After this function returns, report the product language with productLanguage().
            @see Facade::waypointReached()
         */
        static Waypoint *registerProductFromUPIConfigFile(const String &path);
        /*!
            Adds language information to a Metadata::ProductsCategory waypoint.
            @param waypoint The Waypoint returned by registerProduct() or registerProductFromUPIConfigFile().
            @param language The product language (a valid IETF BCP 47 language tag).
            @returns The same \a waypoint.
            @see http://www.w3.org/International/articles/language-tags/
            @see http://www.rfc-editor.org/rfc/bcp/bcp47.txt
         */
        static Waypoint *productLanguage(Waypoint *waypoint, const String &language);
        /*!
            Constructs a Metadata::ProductsCategory waypoint for identifying a plug-in.
            @param name The plug-in name.
            @param version The plug-in version.
            @param build The plug-in build.
            @param url The plug-in URL.
            @param enabled \a true if the plug-in is enabled, otherwise \a false.
            @returns The constructed Waypoint instance.
            @see pluginSettingChanged(), Facade::waypointReached()
         */
        static Waypoint *registerPlugin(const String &name, const String &version,
                                        const String &build, const String &url = String(),
                                        bool enabled = true);
        /*!
            Constructs a Metadata::SettingsCategory waypoint for when a plug-in setting changes.
            @note The plug-in should be have been previously registered using registerPlugin().
            @param pluginName The plug-in name.
            @param settingName The setting name.
            @param value The setting value.
            @param previousValue The previous value of the setting.
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *pluginSettingChanged(const String &pluginName, const String &settingName,
                                              const Variant &value, const Variant &previousValue = Variant());

        /*!
            Constructs a Metadata::ToolsCategory waypoint for when the use of a (non-instantaneous) tool begins.
            @note Don't use this function for instantaneous tools, use toolUsed() instead.
            @param name The tool name.
            @param toolID The tool identifier.
            @returns The constructed Waypoint instance.
            @see toolFinished(), toolFromPlugin()
         */
        static Waypoint *toolStarted(const String &name, const Variant &toolID = Variant());
        /*!
            Completes a Metadata::ToolsCategory waypoint for a when the use of a (non-instantaneous) tool ends.
            @param waypoint The Waypoint returned by toolStarted().
            @returns The same \a waypoint.
            @see Facade::elapsedTimeSince(), Facade::waypointReached()
         */
        static Waypoint *toolFinished(Waypoint *waypoint);
        /*!
            Constructs a Metadata::ToolsCategory waypoint for when an instantaneous tool is used.
            @note Don't use this function for non-instantaneous tools, use toolStarted() instead.
            @param name The tool name.
            @param toolID The tool identifier.
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *toolUsed(const String &name, const Variant &toolID = Variant());
        /*!
            Adds a tool parameter to a Metadata::ToolsCategory waypoint for a tool.
            @param waypoint The Waypoint returned by toolStarted() or toolUsed().
            @param parameter The parameter.
            @param value The parameter value (if there is one, default is none).
            @returns The Attribute instance added to \a waypoint.
            @see toolFinished(), toolFromPlugin(), Facade::waypointReached()
         */
        static Attribute *toolParameter(Waypoint *waypoint, const String &parameter, const Variant &value = Variant());
        /*!
            Adds a plug-in attribute to a Metadata::ToolsCategory waypoint for a tool.
            @note The plug-in should be have been previously registered using registerPlugin().
            @param waypoint The Waypoint returned by toolStarted() or toolUsed().
            @param pluginName The plug-in name.
            @returns The Attribute instance added to \a waypoint.
            @see toolFinished(), Facade::waypointReached()
         */
        static Attribute *toolFromPlugin(Waypoint *waypoint, const String &pluginName);

        /*!
            Constructs a Metadata::ViewCategory waypoint for when a camera changes.
            @note After constructing the waypoint, call (in any order) to completely
            define the camera parameters:
            cameraViewport() to define the viewport;
            one (and only one) of cameraFrustum(), cameraOrtho() or cameraPerspective()
            to define the projection; and
            cameraLookAt() to define the view.
            After the camera parameters have been defined, call Facade::waypointReached().
            @param cameraName The camera name (optional).
            @returns The constructed Waypoint instance.
         */
        static Waypoint *cameraChanged(const String &cameraName = String());
        /*!
            Adds camera parameter attributes to a Metadata::ViewCategory waypoint.
            @param waypoint The Waypoint returned by cameraChanged().
            @param left,right The coordinates for the left and right vertical clipping planes.
            @param bottom,top The coordinates for the bottom and top horizontal clipping planes.
            @param nearVal,farVal The distances to the near and far depth clipping planes.
            @returns The same \a waypoint.
            @see cameraLookAt(), cameraOrtho(), cameraPerspective(), cameraViewport(), Facade::waypointReached()
         */
        static Waypoint *cameraFrustum(Waypoint *waypoint, double left, double right,
                                       double bottom, double top, double nearVal, double farVal);
        /*!
            Adds camera parameter attributes to a Metadata::ViewCategory waypoint.
            @param waypoint The Waypoint returned by cameraChanged().
            @param eye The position of the eye point.
            @param center The position of the reference point.
            @param up The direction of the up vector.
            @returns The same \a waypoint.
            @see cameraFrustum(), cameraOrtho(), cameraPerspective(), cameraViewport(), Facade::waypointReached()
         */
        static Waypoint *cameraLookAt(Waypoint *waypoint, const Vector3D &eye,
                                      const Vector3D &center, const Vector3D &up);
        /*!
            Adds camera parameter attributes to a Metadata::ViewCategory waypoint.
            @param waypoint The Waypoint returned by cameraChanged().
            @param left,right The coordinates for the left and right vertical clipping planes.
            @param bottom,top The coordinates for the bottom and top horizontal clipping planes.
            @param nearVal,farVal The distances to the nearer and farther depth clipping planes.
            @returns The same \a waypoint.
            @see cameraFrustum(), cameraOrtho(), cameraPerspective(), cameraViewport(), Facade::waypointReached()
         */
        static Waypoint *cameraOrtho(Waypoint *waypoint, double left, double right,
                                     double bottom, double top, double nearVal, double farVal);
        /*!
            Adds camera parameter attributes to a Metadata::ViewCategory waypoint.
            @param waypoint The Waypoint returned by cameraChanged().
            @param fovy The field of view angle, in degrees, in the y direction.
            @param aspect The aspect ratio that determines the field of view in the x direction.
            @param zNear The distance from the viewer to the near clipping plane.
            @param zFar The distance from the viewer to the far clipping plane.
            @returns The same \a waypoint.
            @see cameraFrustum(), cameraLookAt(), cameraOrtho(), cameraViewport(), Facade::waypointReached()
         */
        static Waypoint *cameraPerspective(Waypoint *waypoint, double fovy, double aspect,
                                           double zNear, double zFar);

        /*!
            Adds a camera viewport attribute to a Metadata::ViewCategory waypoint.
            @param waypoint The Waypoint returned by cameraChanged().
            @param viewportRect The viewport rectangle, in screen coordinates.
            @returns The same \a waypoint.
            @see cameraFrustum(), cameraLookAt(), cameraOrtho(), cameraPerspective(), Facade::waypointReached()
         */
        static Waypoint *cameraViewport(Waypoint *waypoint, const Rectangle &viewportRect);

        /*!
            Constructs a Metadata::ViewCategory waypoint for when a 2D viewport changes.
            @param viewportRect The viewport rectangle, in screen coordinates.
            @param viewportBounds The viewport bounds, in document coordinates.
            @param documentBounds The document bounds; typically (0,0,width,height).
            @param viewportName The viewport name (optional).
            @returns The constructed Waypoint instance.
            @see Facade::waypointReached()
         */
        static Waypoint *viewport2DChanged(const Rectangle &viewportRect,
                                           const RectangleF &viewportBounds,
                                           const RectangleF &documentBounds,
                                           const String &viewportName = String());
    };
};

/*!
    The %FacadeObserver abstract base class defines the interface for receiving
    notifications from the Facade.

    To implement an observer, subclass %FacadeObserver and override the
    pure virtual member functions.

    To add an observer, invoke the Facade::addObserver() method.
    To remove an observer, invoke the Facade::removeObserver() method.

    @warning Do not call Facade::addObserver() or Facade::removeObserver()
    from within a notification handler.

    @headerfile Chronicle/Facade.h <Chronicle/Facade.h>
    @see Facade class
 */
class CHRONICLESHARED_EXPORT FacadeObserver
{
public:
    /*!
        Notification that a new capture has started.
        @warning This notification handler is usually invoked in a separate worker thread.
        You are responsible for implementing measures to ensure thread safety.
     */
    virtual void captureStarted() = 0;
    /*!
        Notification that the current capture is now paused.
        @warning This notification handler is usually invoked in a separate worker thread.
        You are responsible for implementing measures to ensure thread safety.
     */
    virtual void capturePaused() = 0;
    /*!
        Notification that the current capture (that was paused) has now resumed.
        @warning This notification handler is usually invoked in a separate worker thread.
        You are responsible for implementing measures to ensure thread safety.
     */
    virtual void captureResumed() = 0;
    /*!
        Notification that the capture in progess has now stopped.
        @warning This notification handler is usually invoked in a separate worker thread.
        You are responsible for implementing measures to ensure thread safety.
     */
    virtual void captureStopped() = 0;
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

#endif // CHRONICLE_FACADE_H
