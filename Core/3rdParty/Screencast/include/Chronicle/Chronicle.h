#ifndef CHRONICLE_CHRONICLE_H
#define CHRONICLE_CHRONICLE_H

/*!
    @namespace Chronicle
    Defines the complete public API for Chronicle facility.
    @headerfile Chronicle/Chronicle.h <Chronicle/Chronicle.h>
 */

#include <Chronicle/Attribute.h>
#include <Chronicle/Common.h>
#include <Chronicle/Facade.h>
#include <Chronicle/Metadata.h>
#include <Chronicle/String.h>
#include <Chronicle/Types.h>
#include <Chronicle/Variant.h>
#include <Chronicle/Waypoint.h>

/*!
    @mainpage Chronicle API Documentation

    @author Thomas White (thomas.white@autodesk.com)
    @copyright Copyright &copy; 2013 Autodesk, Inc. All rights reserved.

    This documentation is divided into the following sections:
    - @subpage intro_page
    - @subpage revisions_page
    - @subpage overview_page
    - @subpage init_page
    - @subpage waypoints_page
      - @ref register_sec
      - @ref tools_sec
      - @ref times_sec
      - @ref files_sec
      - @ref dialogs_sec
      - @ref settings_sec
    - @subpage capture_page
    - @subpage monitoring_page
    - @subpage utility_page

    @page intro_page Introduction

    Autodesk Project %Chronicle lets you capture your workflows, to easily
	create powerful and engaging learning materials.
	
    <b>Autodesk Project %Chronicle Website:</b>
    @par
    <a href="https://chronicle.autodesk.com/" target="_blank">
		https://chronicle.autodesk.com/
	</a>

    <b>%Chronicle Research Page:</b>
    @par
    <a href="http://research.autodesk.com/projects/chronicle" target="_blank">
		http://research.autodesk.com/projects/chronicle
	</a>

    <b>Research Transfer Project %Chronicle Site:</b>
    @par
    <a href="https://share.autodesk.com/sites/research/transfer/chronicle/SitePages/Home.aspx" target="_blank">
		https://share.autodesk.com/sites/research/transfer/chronicle/SitePages/Home.aspx
	</a>

    @page revisions_page Revision History

    @par \b 1.1
		- If the Utility is not installed, Chronicle::Facade::showUtility() now opens
		  the download page for the Utility in the system-default web browser.
		- Made Chronicle::Facade::openWebsite() and Chronicle::Facade::openFAQ() work
		  even if the Facade is not initialized.
    @par \b 0.9.4
        - Added Chronicle::Attribute::destroy() and Chronicle::Waypoint::destroy().
        - Made Chronicle::Attribute and Chronicle::Waypoint destructors private.
    @par \b 0.9.3
        - Bugfix release; no API changes.
    @par \b 0.9.2
        - Added support for reporting changes to modal and modeless dialog items:
          - Chronicle::Facade::Waypoints::modalDialogItemChanged()
          - Chronicle::Facade::Waypoints::modelessDialogItemChanged()
        - The previous value of a setting can now be specified in Chronicle::Facade::Waypoints::productSettingChanged().
        - Improved support for registering plug-ins, reporting plug-in settings,
          and specifying which commands and tools are provided by plug-ins:
          - Chronicle::Facade::Waypoints::commandFromPlugin()
          - Chronicle::Facade::Waypoints::registerPlugin()
          - Chronicle::Facade::Waypoints::pluginSettingChanged()
          - Chronicle::Facade::Waypoints::pluginSettingChanged()
          - Chronicle::Facade::Waypoints::toolFromPlugin()
        - Added option to report the URL in Chronicle::Facade::Waypoints::registerProduct().
        - Introduced support for reporting viewing-related waypoints:
          - Added Chronicle::Metadata::ViewCategory to the Chronicle::Metadata::WaypointCategory enumeration.
          - Added Chronicle::Metadata::CameraAspectRatio, Chronicle::Metadata::CameraClipBottomTop,
            Chronicle::Metadata::CameraClipLeftRight, Chronicle::Metadata::CameraClipNearFar,
            Chronicle::Metadata::CameraEye, Chronicle::Metadata::CameraUp,
            Chronicle::Metadata::Viewport, Chronicle::Metadata::ViewportBounds,
            Chronicle::Metadata::ViewportName, Chronicle::Metadata::ViewportRectangle.
          - Added Chronicle::Facade::Waypoints::cameraChanged(), Chronicle::Facade::Waypoints::cameraFrustum(),
            Chronicle::Facade::Waypoints::cameraLookAt(), Chronicle::Facade::Waypoints::cameraOrtho(),
            Chronicle::Facade::Waypoints::cameraPerspective(), Chronicle::Facade::Waypoints::cameraViewport(),
            Chronicle::Facade::Waypoints::viewport2DChanged().
    @par \b 0.9.1
        - Added support for UPI-based product registration and product language:
          - Chronicle::Facade::Waypoints::registerProductFromUPIConfigFile()
          - Chronicle::Facade::Waypoints::productLanguage()
        - Added modeless dialog waypoints:
          - Chronicle::Facade::Waypoints::modelessDialogOpened()
          - Chronicle::Facade::Waypoints::modelessDialogClosed()
        - Revisions to the Chronicle::Metadata::AttributeName enumeration:
          - Added Chronicle::Metadata::CommandCategory, Chronicle::Metadata::ProductLanguage,
            Chronicle::Metadata::ProductLine, Chronicle::Metadata::ProductMaster, Chronicle::Metadata::ProductRelease,
            Chronicle::Metadata::ToolCategory and Chronicle::Metadata::UPIConfigFilePath.
          - Renamed Chronicle::Metadata::SystemLocale to Chronicle::Metadata::SystemLanguage.
        - Added Chronicle::Attribute::clone(), Chronicle::AttributeList::clone() and Chronicle::Waypoint::clone().
        - Added Chronicle::Facade::openFAQ().
        - Renamed Facade::showRecorder() to Chronicle::Facade::showUtility().
        - Removed Facade::showUploadManager() and Facade::showPreferences().
    @par \b 0.9
        - Initial API release for review by product integration teams.

    @page overview_page Overview

    Most of the functionality is provided through the Chronicle::Facade class.

    @note The API expects Unicode (wchar_t) strings.

    @page init_page Initialization and Cleanup

    @code
    #include <Chronicle/Chronicle.h>

    // do this once at application startup
    Chronicle::Error err = Chronicle::Facade::initialize();

    // if you need to determine if the facility has been initialized
    bool initialized = Chronicle::Facade::isInitialized();

    // do this once at application exit
    Chronicle::Error err = Chronicle::Facade::cleanup();
    @endcode

    @page waypoints_page Recording Waypoints

    The Chronicle::Facade::Waypoints class contains many convenient methods
    to record the most common and frequently used waypoints.

    @section register_sec Registration

    Product registration is \em \b mandatory. This should be done immediately after initialization.

    For Autodesk products, the recommended way is to specify the path of the UPI configuration file.
    See the UPI homepage (http://upi.autodesk.com/) for details on how to integrate UPI in products.

    Example of registering a product using UPI:
    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *path = L"C:\\Program Files (x86)\\Autodesk\\Autodesk Design Review\\UPI\\upiconfig.xml";
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::registerProductFromUPIConfigFile(path);
    // report the product language by specifying a valid IETF BCP 47 language tag
    Chronicle::Facade::Waypoints::productLanguage(waypoint, L"en-US");
    // although not required, it is recommended to specify the human-readable product name
    waypoint->addAttribute(Chronicle::Metadata::toString(Chronicle::Metadata::ProductName), L"Autodesk Design Review");
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    For products that haven't integrated UPI, specify the product information.

    Example:
    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *productName = L"SampleApplication";
    const wchar_t *productVersion = L"2012";
    const wchar_t *productBuild = L"123456";
    const wchar_t *productURL = L"http://www.mycompany.com/sample/application";
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::registerProduct(productName,
        productVersion, productBuild, productURL);
    // report the product language by specifying a valid IETF BCP 47 language tag
    Chronicle::Facade::Waypoints::productLanguage(waypoint, L"ja-JP");
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    For products that have a plug-in (or add-in) architecture, report the installed plug-ins.

    Example:
    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *pluginName = L"SamplePlugin";
    const wchar_t *pluginVersion = L"1.0";
    const wchar_t *pluginBuild = L"42";
    const wchar_t *pluginURL = L"http://www.mycompany.com/sample/plugin";
    bool enabled = true;
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::registerPlugin(pluginName,
        pluginVersion, pluginBuild, pluginURL, enabled);
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    @section tools_sec Tools

    Examples of Chronicle::Metadata::ToolsCategory waypoints.

    The difference between a \a command and a \a tool is that a \a command is entered from a command line,
    while a \a tool is invoked through a GUI element such as a toolbar, menu item, etc.

    Reporting a command event:
    @code
    #include <Chronicle/Chronicle.h>

    // create the waypoint through the facade; it is automatically timestamped
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::commandEntered(L"LINE");

    // for command waypoints, optionally add parameters
    Chronicle::Facade::Waypoints::commandParameterEntered(waypoint, L"From point", L"#-2,1");
    Chronicle::Facade::Waypoints::commandParameterEntered(waypoint, L"To point", L"@5,0");
    Chronicle::Facade::Waypoints::commandParameterEntered(waypoint, L"To point", L"@0,3");
    Chronicle::Facade::Waypoints::commandParameterEntered(waypoint, L"To point", L"@-5,3");

    // to automatically set the duration, indicate when the command is completed
    Chronicle::Facade::Waypoints::commandCompleted(waypoint);

    // if the command is provided by a plug-in, specify the plug-in name
    Chronicle::Facade::Waypoints::commandFromPlugin(waypoint, L"SamplePluginName");

    // report the waypoint (this is always the last step)
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    // after reporting the waypoint, it is automatically deleted!
    waypoint = NULL;
    @endcode

    An \a instantaneous tool has an effect that happens immediately and the waypoint has no duration.
    A \a non-instataneous tool is typically interactive and the waypoint has a duration.

    Reporting a non-instantaneous tool event:
    @code
    #include <Chronicle/Chronicle.h>

    // create the waypoint through the facade; it is automatically timestamped
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::toolStarted(L"Circle", 1234);

    // for tool waypoints, optionally add parameters
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Origin", Chronicle::PointF(127.5, -64.0));
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Radius", 97.0);
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Line Width", 3.0);
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Filled", true);
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Fill Color", Chronicle::Color(255, 0, 128, 192));
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Line Color", Chronicle::Color(255, 255, 255));

    // to automatically set the duration, indicate when the tool is finished
    Chronicle::Facade::Waypoints::toolFinished(waypoint);

    // if the tool is provided by a plug-in, specify the plug-in name
    Chronicle::Facade::Waypoints::toolFromPlugin(waypoint, L"SamplePluginName");

    // report the waypoint (this is always the last step)
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    // after reporting the waypoint, it is automatically deleted!
    waypoint = NULL;
    @endcode

    Reporting an instantaneous tool event:
    @code
    #include <Chronicle/Chronicle.h>

    // create the waypoint through the facade; it is automatically timestamped
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::toolUsed(L"Text", 1234);

    // for tool waypoints, optionally add parameters
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Bounds", Chronicle::RectangleF(0, 0, 240, 64));
    Chronicle::Facade::Waypoints::toolParameter(waypoint, L"Text", "Hello, world!");

    // if the tool is provided by a plug-in, specify the plug-in name
    Chronicle::Facade::Waypoints::toolFromPlugin(waypoint, L"SamplePluginName");

    // report the waypoint (this is always the last step)
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    // after reporting the waypoint, it is automatically deleted!
    waypoint = NULL;
    @endcode

    @section times_sec Timestamps and Duration

    Explicitly setting the waypoint timestamp and/or duration.
    This will be necessary if the waypoint isn't created in real-time during capture.
    @code
    Chronicle::Time t = ...; // the system tick count when the event actually occurred
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::commandEntered(L"LINE");
    waypoint->setTime(t);
    waypoint->setDuration(5000); // in milliseconds, so in this example, 5 seconds
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    @section files_sec Files

    Examples of Chronicle::Metadata::FilesCategory waypoints.

    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *documentName = L"Drawing1";
    const wchar_t *documentPath = L"C:\\Documents\\Drawing1.dwg";

    Chronicle::Waypoint *waypoint;
    Chronicle::Error err;

    // whenever a new document is opened
    waypoint = Chronicle::Facade::Waypoints::newDocument(documentName);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever the current document changes (for appications supporting multiple documents)
    waypoint = Chronicle::Facade::Waypoints::currentDocument(documentName);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever an existing document is opened
    waypoint = Chronicle::Facade::Waypoints::documentOpened(documentName, documentPath);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever a document is saved
    waypoint = Chronicle::Facade::Waypoints::documentSaved(documentName, documentPath);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever a document is reverted
    waypoint = Chronicle::Facade::Waypoints::documentReverted(documentName, documentPath);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever a document is closed
    waypoint = Chronicle::Facade::Waypoints::documentClosed(documentName, documentPath);
    err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    @section dialogs_sec Dialogs

    Examples of Chronicle::Metadata::DialogsCategory waypoints.

    Reporting a modal dialog session:
    @code
    #include <Chronicle/Chronicle.h>

    // create the waypoint when the modal dialog is opened

    const wchar_t *dialogTitle = L"Insert";
    Chronicle::Rectangle dialogRect(64, 128, 320, 240); // in screen coordinates
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::modalDialogOpened(dialogTitle, dialogRect);

    // ... modal dialog session happens ...

    // when the modal dialog is about to close, record the changed dialog items/controls

    const wchar_t *itemName = L"Explode";
    bool itemNewValue = true;
    bool itemOldValue = false;
    Chronicle::Rectangle itemRect(16, 200, 80, 20); // in dialog coordinates
    Chronicle::Facade::Waypoints::modalDialogItemChanged(waypoint, itemName, itemNewValue, itemOldValue, itemRect);
    // ... record the other changed items/controls ...

    // when the modal dialog closes, record the closure and then finally report the waypoint

    Chronicle::Facade::Waypoints::modalDialogClosed(waypoint);
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    Reporting modeless dialog interactions:
    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *dialogTitle = L"Layers";
    Chronicle::Rectangle dialogRect(800, 100, 160, 480); // in screen coordinates
    Chronicle::Waypoint *waypoint;
    Chronicle::Error err;

    // whenever the modeless dialog is opened
    waypoint = Chronicle::Facade::Waypoints::modelessDialogOpened(dialogTitle, dialogRect);
    err = Chronicle::Facade::waypointReached(waypoint);

    // whenever an item/control in the modeless dialog changes value
    const wchar_t *itemName = L"Transparency";
    float itemNewValue = 1.0f;
    float itemOldValue = 0.25f;
    Chronicle::Rectangle itemRect(8, 460, 64, 20); // in dialog coordinates
    waypoint = Chronicle::Facade::Waypoints::modelessDialogItemChanged(dialogTitle, itemName, itemNewValue,
        itemOldValue, dialogRect, itemRect);

    // whenever the modeless dialog is closed
    waypoint = Chronicle::Facade::Waypoints::modelessDialogClosed(dialogTitle);
    err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    @section settings_sec Settings

    Examples of Chronicle::Metadata::SettingsCategory waypoint.

    Reporting a product setting change:
    @code
    #include <Chronicle/Chronicle.h>

    const wchar_t *settingName = L"AUTOSNAP";
    const int newValue = 63;
    const int oldValue = 31;
    Chronicle::Waypoint *waypoint = Chronicle::Facade::Waypoints::productSettingChanged(settingName, newValue, oldValue);
    Chronicle::Error err = Chronicle::Facade::waypointReached(waypoint);
    @endcode

    @page capture_page Controlling and Querying the Capture State

    @code
    #include <Chronicle/Chronicle.h>

    // controlling the capture

    Chronicle::Error err;

    err = Chronicle::Facade::startCapture();
    err = Chronicle::Facade::pauseCapture();
    err = Chronicle::Facade::resumeCapture();
    err = Chronicle::Facade::stopCapture();

    // querying the capture state

    bool flag;

    flag = Chronicle::Facade::isCapturing();
    flag = Chronicle::Facade::isCapturePaused();
    flag = Chronicle::Facade::canStartCapture();
    flag = Chronicle::Facade::canPauseCapture();
    flag = Chronicle::Facade::canResumeCapture();
    flag = Chronicle::Facade::canStopCapture();
    @endcode

    @page monitoring_page Monitoring the Capture State

    @code
    #include <Chronicle/Chronicle.h>
    #include <iostream>
    using namespace std;

    // Facade observer subclass

    class MyObserver : public Chronicle::Facade::Observer
    {
    private:
        MyObserver();

    public:
        static bool addToFacade();
        static void removeFromFacade();

        virtual void captureStarted();
        virtual void capturePaused();
        virtual void captureResumed();
        virtual void captureStopped();

    private:
        static MyObserver *s_this;
    };

    MyObserver *s_this = NULL;

    MyObserver::MyObserver()
    {
    }

    bool MyObserver::addToFacade()
    {
        if (s_this)
            return true;

        s_this = new MyObserver;
        if (Chronicle::Facade::addObserver(s_this) == Chronicle::NoError)
            return true;

        delete s_this;
        s_this = NULL;

        return false;
    }

    void MyObserver::removeFromFacade()
    {
        if (s_this)
        {
            Chronicle::Facade::removeObserver(s_this);
            delete s_this;
            s_this = NULL;
        }
    }

    void MyObserver::captureStarted()
    {
        // warning: the notification handler is called from a separate worker thread
        cout << "capture started";
    }

    void MyObserver::capturePaused()
    {
        // warning: the notification handler is called from a separate worker thread
        cout << "capture paused";
    }

    void MyObserver::captureResumed()
    {
        // warning: the notification handler is called from a separate worker thread
        cout << "capture resumed";
    }

    void MyObserver::captureStopped()
    {
        // warning: the notification handler is called from a separate worker thread
        cout << "capture stopped";
    }

    // add observer to facade after initialization

    void startup()
    {
        Chronicle::Error err = Chronicle::Facade::initialize();

        if (err == Chronicle::NoError)
        {
            MyObserver::addToFacade();
        }
    }

    // remove observer before facade cleanup

    void shutdown()
    {
        MyObserver::removeFromFacade();

        Chronicle::Facade::cleanup();
    }
    @endcode

    @page utility_page Controlling the Chronicle Desktop Utility

    @code
    #include <Chronicle/Chronicle.h>

    Chronicle::Error err;

    err = Chronicle::Facade::showUtility();
    err = Chronicle::Facade::openWebsite();
    err = Chronicle::Facade::openFAQ();
    @endcode
 */

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

#endif // CHRONICLE_CHRONICLE_H

