#ifndef CHRONICLE_CHRONICLE_DECL_H
#define CHRONICLE_CHRONICLE_DECL_H

#ifndef CHRONICLE_DECL_EXPORT
#  if defined(_WIN32)
#    define CHRONICLE_DECL_EXPORT __declspec(dllexport)
#  endif
#  ifndef CHRONICLE_DECL_EXPORT
#    define CHRONICLE_DECL_EXPORT
#  endif
#endif

#ifndef CHRONICLE_DECL_IMPORT
#  if defined(_WIN32)
#    define CHRONICLE_DECL_IMPORT __declspec(dllimport)
#  endif
#  ifndef CHRONICLE_DECL_IMPORT
#    define CHRONICLE_DECL_IMPORT
#  endif
#endif

#if defined(CHRONICLE_LIBRARY)
#  define CHRONICLESHARED_EXPORT CHRONICLE_DECL_EXPORT
#else
#  define CHRONICLESHARED_EXPORT CHRONICLE_DECL_IMPORT
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

#endif // CHRONICLE_CHRONICLE_DECL_H
