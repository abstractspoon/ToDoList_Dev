// Calculator.h: interface for the CCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATOR_H__EB1697DA_C962_4EAF_9E6E_00EBC6D74D47__INCLUDED_)
#define AFX_CALCULATOR_H__EB1697DA_C962_4EAF_9E6E_00EBC6D74D47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalculator  
{
public:
	CCalculator();
	virtual ~CCalculator();

	static double Evaluate(LPCWSTR szExpression);

};

#endif // !defined(AFX_CALCULATOR_H__EB1697DA_C962_4EAF_9E6E_00EBC6D74D47__INCLUDED_)
