// Calculator.cpp: implementation of the CCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calculator.h"

#include "cpp-expression-parser\shunting-yard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalculator::CCalculator()
{

}

CCalculator::~CCalculator()
{

}

double CCalculator::Evaluate(LPCWSTR szExpression)
{
	try
	{
		return calculator::calculate(szExpression);
	}
	catch (...)
	{
		
	}

	return 0.0;
}
