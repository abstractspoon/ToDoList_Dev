#ifndef CHRONICLE_COMMON_H
#define CHRONICLE_COMMON_H

#if defined(_WIN32)
#  define CHRONICLE_PLATFORM_WINDOWS
#endif

namespace Chronicle
{

/*!
    The possible error codes returned by some Chronicle functions.
    @headerfile Chronicle/Common.h <Chronicle/Common.h>
 */
enum Error
{
    NoError,            //!< Success, no error.
    NotInitialized,     //!< The facility has not been initialized.
    NotCapturing,       //!< No capture is currently in progress.
    NotPaused,          //!< The current capture is not paused.
    NotAvailable,       //!< The facility is not available.
    FailedToStart,      //!< The capture could not be started.
    ReadError,          //!< An unexpected read error has occurred.
    WriteError,         //!< An unexpected write error has occurred.
    InvalidParameter,   //!< At least one of the parameters specified is not valid.
    FileNotFound,       //!< The specified file cannot be found.
    WrongState,         //!< The object is in the wrong state.
    OutOfMemory,        //!< Insufficient memory to complete the operation.
    UnknownError        //!< Failure, reason unknown.
};

typedef unsigned long Duration;     //!< Duration, specified in milliseconds (1000ms = 1s).
typedef unsigned int Index;         //!< Position in an ordered collection (first element is at index 0).
typedef unsigned long long Time;    //!< Timestamp, specified in milliseconds (1000ms = 1s).

}

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

#endif // CHRONICLE_COMMON_H
