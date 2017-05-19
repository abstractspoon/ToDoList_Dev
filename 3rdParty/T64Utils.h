// T64Utils.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_T64UTILS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
#define AFX_T64UTILS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace T64Utils
{
	typedef __int64 time64_t;

	static const int SECS_TO_FT_MULT	= 10000000;
	static const time64_t TIME_T_BASE	= 11644473600;
	static const time64_t T64_NULL		= 0xffffffffffffffff;
	
	static void T64ToFileTime(time64_t *pt, _FILETIME *pft)
	{
		if (pt && pft)
		{
			if (*pt == T64_NULL)
			{
				pft->dwLowDateTime = pft->dwHighDateTime = 0;
			}
			else
			{
				LARGE_INTEGER li = { 0 };  
			
				li.QuadPart = ((*pt) * SECS_TO_FT_MULT);
			
				pft->dwLowDateTime = li.LowPart;
				pft->dwHighDateTime = li.HighPart;
			}
		}
	}
	
	static void FileTimeToT64(_FILETIME *pft, time64_t *pt)
	{
		if (pt && pft)
		{
			if ((pft->dwLowDateTime == 0) && (pft->dwHighDateTime == 0))
			{
				*pt = T64_NULL;
			}
			else
			{
				LARGE_INTEGER li = { 0 }; 
			
				li.LowPart = pft->dwLowDateTime;
				li.HighPart = pft->dwHighDateTime;
			
				(*pt) = (li.QuadPart / SECS_TO_FT_MULT);
			}
		}
	}
	
	static void SystemTimeToT64(SYSTEMTIME *pst, time64_t *pt)
	{
		if (pt && pst)
		{
			const SYSTEMTIME ST_NULL = { 0 };

			if (memcmp(pst, &ST_NULL, sizeof(SYSTEMTIME)) == 0)
			{
				*pt = T64_NULL;
			}
			else
			{
				::FILETIME ft = { 0 }, ftLocal = { 0 };
			
				::SystemTimeToFileTime(pst, &ftLocal);
				::LocalFileTimeToFileTime(&ftLocal, &ft);
				FileTimeToT64(&ft, pt);
			
				*pt -= TIME_T_BASE;
			}
		}
	}
	
	static void T64ToSystemTime(time64_t *pt, SYSTEMTIME *pst)
	{
		if (pt && pst)
		{
			if (*pt == T64_NULL)
			{
				memset(pst, 0, sizeof(SYSTEMTIME));
			}
			else
			{
				::FILETIME ft = { 0 }, ftLocal = { 0 };
				time64_t t = ((*pt) + TIME_T_BASE);

				T64ToFileTime(&t, &ft);
				::FileTimeToLocalFileTime(&ft, &ftLocal);
				::FileTimeToSystemTime(&ftLocal, pst);
			}
		}
	}

}

#endif // !defined(AFX_T64UTILS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
