// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXTOLEDISPATCHDRIVER_H)
#define __EXTOLEDISPATCHDRIVER_H

class CExtOleDispatchDriver : public COleDispatchDriver
{
public:
	CExtOleDispatchDriver()
	{
	};
	CExtOleDispatchDriver(
		LPDISPATCH lpDispatch,
		BOOL bAutoRelease = TRUE
		)
		: COleDispatchDriver(
			lpDispatch,
			bAutoRelease
			)
	{
	};
	CExtOleDispatchDriver(
		const COleDispatchDriver& dispatchSrc
		)
		: COleDispatchDriver( dispatchSrc )
	{
	};

	void Release()
	{
		ReleaseDispatch();
	};
	
	IDispatch & operator*()
	{
		ASSERT( ((LPDISPATCH)(*this)) != NULL );
		return * ((LPDISPATCH)(*this));
	};
	
	IDispatch * * operator&()
	{
		ASSERT( ((LPDISPATCH)(*this)) != NULL );
		return &m_lpDispatch;
	};
	
	IDispatch * operator->()
	{
		ASSERT( ((LPDISPATCH)(*this)) != NULL );
		return ((LPDISPATCH)(*this));
	};

	IDispatch * operator=(IDispatch * lp)
	{
		if( lp != NULL )
			lp->AddRef();
		if( m_lpDispatch )
			m_lpDispatch->Release();
		m_lpDispatch = lp;
		return lp;
	};

	IDispatch * operator=(IUnknown * lp)
	{
		IUnknown * pTemp = m_lpDispatch;
		m_lpDispatch = NULL;
		if( lp != NULL )
			lp->QueryInterface(
				IID_IDispatch,
				(void**)&m_lpDispatch
				);
		if( pTemp )
			pTemp->Release();
		return m_lpDispatch;
	};

	BOOL operator!() const
	{
		return
			( ((LPDISPATCH)(*((COleDispatchDriver*)this))) == NULL)
				? TRUE : FALSE;
	};

	HRESULT PutPropertyByName(LPCOLESTR lpsz, VARIANT * pVar)
	{
		ASSERT( ((LPDISPATCH)(*this)) != NULL );
		ASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if( SUCCEEDED(hr) )
			hr = PutProperty(((LPDISPATCH)(*this)), dwDispID, pVar);
		return hr;
	};
	HRESULT GetPropertyByName(LPCOLESTR lpsz, VARIANT * pVar)
	{
		ASSERT( ((LPDISPATCH)(*this)) != NULL );
		ASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if( SUCCEEDED(hr) )
			hr = GetProperty(((LPDISPATCH)(*this)), dwDispID, pVar);
		return hr;
	};
	HRESULT PutPropertyByName(LPCSTR lpsz, VARIANT * pVar)
	{
		USES_CONVERSION;
		return
			PutPropertyByName(
				(LPCOLESTR)(A2COLE(lpsz)),
				pVar
				);
	};
	HRESULT GetPropertyByName(LPCSTR lpsz, VARIANT * pVar)
	{
		USES_CONVERSION;
		return
			GetPropertyByName(
				(LPCOLESTR)(A2COLE(lpsz)),
				pVar
				);
	};

	HRESULT GetIDOfName(LPCOLESTR lpsz, DISPID* pdispid)
	{
		return ((LPDISPATCH)(*this))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, LOCALE_USER_DEFAULT, pdispid);
	};
	
	HRESULT Invoke0(DISPID dispid, VARIANT * pvarRet = NULL)
	{
		DISPPARAMS dispparams = { NULL, NULL, 0, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke0(LPCOLESTR lpszName, VARIANT * pvarRet = NULL)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr = Invoke0(dispid, pvarRet);
		return hr;
	};
	HRESULT Invoke0(LPCSTR lpszName, VARIANT * pvarRet = NULL)
	{
		USES_CONVERSION;
		return
			Invoke0(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarRet
				);
	};
	
	HRESULT Invoke1(
		DISPID dispid,
		VARIANT * pvarParam1,
		VARIANT * pvarRet = NULL
		)
	{
		DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke1(
		LPCOLESTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr = Invoke1(dispid, pvarParam1, pvarRet);
		return hr;
	};
	HRESULT Invoke1(
		LPCSTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			Invoke1(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParam1,
				pvarRet
				);
	};
	
	HRESULT Invoke2(
		DISPID dispid,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarRet = NULL
		)
	{
		COleVariant varArgs[2] = { *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 2, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke2(
		LPCOLESTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
		return hr;
	};
	HRESULT Invoke2(
		LPCSTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			Invoke2(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParam1,
				pvarParam2,
				pvarRet
				);
	};

	HRESULT Invoke3(
		DISPID dispid,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarRet = NULL)
	{
		COleVariant varArgs[3] = {
			*pvarParam3, *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 3, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke3(
		LPCOLESTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr =
			Invoke3(
				dispid,
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarRet
				);
		return hr;
	};
	HRESULT Invoke3(
		LPCSTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			Invoke3(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarRet
				);
	};

	HRESULT Invoke4(
		DISPID dispid,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarRet = NULL)
	{
		COleVariant varArgs[4] = {
			*pvarParam4, *pvarParam3, *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 4, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke4(
		LPCOLESTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr =
			Invoke4(
				dispid,
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarParam4,
				pvarRet
				);
		return hr;
	};
	HRESULT Invoke4(
		LPCSTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			Invoke4(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarParam4,
				pvarRet
				);
	};

	HRESULT Invoke5(
		DISPID dispid,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarParam5,
		VARIANT * pvarRet = NULL)
	{
		COleVariant varArgs[5] = {
			*pvarParam5, *pvarParam4, *pvarParam3, *pvarParam2, *pvarParam1 };
		DISPPARAMS dispparams = { &varArgs[0], NULL, 5, 0};
		return ((LPDISPATCH)(*this))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	};
	HRESULT Invoke5(
		LPCOLESTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarParam5,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr =
			Invoke5(
				dispid,
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarParam4,
				pvarParam5,
				pvarRet
				);
		return hr;
	};
	HRESULT Invoke5(
		LPCSTR lpszName,
		VARIANT * pvarParam1,
		VARIANT * pvarParam2,
		VARIANT * pvarParam3,
		VARIANT * pvarParam4,
		VARIANT * pvarParam5,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			Invoke5(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParam1,
				pvarParam2,
				pvarParam3,
				pvarParam4,
				pvarParam5,
				pvarRet
				);
	};

	HRESULT InvokeN(
		DISPID dispid,
		VARIANT * pvarParams,
		int nParams = 0,
		VARIANT * pvarRet = NULL
		)
	{
		DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
		return
			((LPDISPATCH)(*this))->Invoke(
				dispid,
				IID_NULL,
				LOCALE_USER_DEFAULT,
				DISPATCH_METHOD,
				&dispparams,
				pvarRet,
				NULL,
				NULL
				);
	};

	HRESULT InvokeN(
		LPCOLESTR lpszName,
		VARIANT * pvarParams,
		int nParams = 0,
		VARIANT * pvarRet = NULL
		)
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if( SUCCEEDED(hr) )
			hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
		return hr;
	};
	HRESULT InvokeN(
		LPCSTR lpszName,
		VARIANT * pvarParams,
		int nParams = 0,
		VARIANT * pvarRet = NULL
		)
	{
		USES_CONVERSION;
		return
			InvokeN(
				(LPCOLESTR)(A2COLE(lpszName)),
				pvarParams,
				nParams,
				pvarRet
				);
	};
	
	static HRESULT GetProperty(
		IDispatch * pDisp,
		DISPID dwDispID,
		VARIANT * pVar
		)
	{
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		return
			pDisp->Invoke(
				dwDispID,
				IID_NULL,
				LOCALE_USER_DEFAULT,
				DISPATCH_PROPERTYGET,
				&dispparamsNoArgs,
				pVar,
				NULL,
				NULL
				);
	};

	static HRESULT PutProperty(
		IDispatch * pDisp,
		DISPID dwDispID,
		VARIANT * pVar
		)
	{
		DISPPARAMS dispparams = {NULL, NULL, 1, 1};
		dispparams.rgvarg = pVar;
		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		if(		pVar->vt == VT_UNKNOWN
			||	pVar->vt == VT_DISPATCH
			||	(pVar->vt & VT_ARRAY)
			||	(pVar->vt & VT_BYREF)
			)
		{
			HRESULT hr =
				pDisp->Invoke(
					dwDispID,
					IID_NULL,
					LOCALE_USER_DEFAULT,
					DISPATCH_PROPERTYPUTREF,
					&dispparams,
					NULL,
					NULL,
					NULL
					);
			if( SUCCEEDED(hr) )
				return hr;
		}

		return
			pDisp->Invoke(
				dwDispID,
				IID_NULL,
				LOCALE_USER_DEFAULT,
				DISPATCH_PROPERTYPUT,
				&dispparams,
				NULL,
				NULL,
				NULL
				);
	};

}; // class CExtOleDispatchDriver


#endif // __EXTOLEDISPATCHDRIVER_H


