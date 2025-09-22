// Source: http://www.daniweb.com/software-development/cpp/code/427500/calculator-using-shunting-yard-algorithm#
// Author: Jesse Brown
// Modifications: Brandon Amos, redpois0n
#include "StdAfx.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include <iosfwd>

#include "shunting-yard.h"

std::map<std::wstring, int> calculator::buildOpPrecedence()
{
	std::map<std::wstring, int> opp;

	// Create the operator precedence map based on C++ default
	// precedence order as described on cppreference website:
	// http://en.cppreference.com/w/c/language/operator_precedence
	opp[L"^"] = 2;

	opp[L"*"] = 3; 
	opp[L"/"] = 3; 
	opp[L"%"] = 3;
	opp[L"+"] = 4; 

	opp[L"-"] = 4;

	opp[L"<<"] = 5; 
	opp[L">>"] = 5;
	opp[L"<"] = 6; 

	opp[L"<="] = 6; 
	opp[L">="] = 6; 
	opp[L">"] = 6;

	opp[L"=="] = 7; 
	opp[L"!="] = 7;

	opp[L"&&"] = 11;
	opp[L"||"] = 12;
	opp[L"("] = 16;

	return opp;
}

// Builds the opPrecedence map only once:
std::map<std::wstring, int> calculator::opPrecedence = calculator::buildOpPrecedence();

#define isvariablechar(c) (isalpha(c) || c == '_')

TokenQueue_t calculator::toRPN(const wchar_t* expr,
							   std::map<std::wstring, double>* vars,
							   std::map<std::wstring, int> opPrecedence)
{
	TokenQueue_t rpnQueue; std::stack<std::wstring> operatorStack;
	bool lastTokenWasOp = true;

	// In one pass, ignore whitespace and parse the expression into RPN
	// using Dijkstra's Shunting-yard algorithm.
	while (*expr && isspace(*expr)) 
		++expr;

	while (*expr)
	{
		if (isdigit(*expr))
		{
			// If the token is a number, add it to the output queue.
			wchar_t* nextChar = 0;
			double digit = wcstod(expr, &nextChar);
#     ifdef DEBUG
			std::cout << digit << std::endl;
#     endif
			rpnQueue.push(new Token<double>(digit, NUM));
			expr = nextChar;
			lastTokenWasOp = false;
		}
		else if (isvariablechar(*expr))
		{
			// If the function is a variable, resolve it and
			// add the parsed number to the output queue.
			std::wstringstream ss;
			ss << *expr;
			++expr;
			while (isvariablechar(*expr) || isdigit(*expr))
			{
				ss << *expr;
				++expr;
			}

			bool found = false;
			double val;

			std::wstring key = ss.str();

			if (key == L"true")
			{
				found = true; val = 1;
			}
			else if (key == L"false")
			{
				found = true; val = 0;
			}
			else if (vars)
			{
				std::map<std::wstring, double>::iterator it = vars->find(key);
				if (it != vars->end()) { found = true; val = it->second; }
			}

			if (found)
			{
				// Save the number
#     ifdef DEBUG
				std::cout << val << std::endl;
#     endif
				rpnQueue.push(new Token<double>(val, NUM));;
			}
			else
			{
				// Save the variable name:
#     ifdef DEBUG
				std::cout << key << std::endl;
#     endif
				rpnQueue.push(new Token<std::wstring>(key, VAR));
			}

			lastTokenWasOp = false;
		}
		else
		{
			// Otherwise, the variable is an operator or paranthesis.
			switch (*expr)
			{
			case '(':
				operatorStack.push(L"(");
				++expr;
				break;

			case ')':
				while (operatorStack.top().compare(L"("))
				{
					rpnQueue.push(new Token<std::wstring>(operatorStack.top(), OP));
					operatorStack.pop();
				}
				operatorStack.pop();
				++expr;
				break;

			default:
				{
					// The token is an operator.
					//
					// Let p(o) denote the precedence of an operator o.
					//
					// If the token is an operator, o1, then
					//   While there is an operator token, o2, at the top
					//       and p(o1) <= p(o2), then
					//     pop o2 off the stack onto the output queue.
					//   Push o1 on the stack.
					std::wstringstream ss;
					ss << *expr;
					++expr;
					while (*expr && !isspace(*expr) && !isdigit(*expr)
						   && !isvariablechar(*expr) && *expr != '(' && *expr != ')')
					{
						ss << *expr;
						++expr;
					}
					ss.clear();
					std::wstring str;
					ss >> str;
#           ifdef DEBUG
					std::cout << str << std::endl;
#           endif
					if (lastTokenWasOp)
					{
						// Convert unary operators to binary in the RPN.
						if (!str.compare(L"-") || !str.compare(L"+"))
						{
							rpnQueue.push(new Token<double>(0, NUM));
						}
						else
						{
							throw std::domain_error(
								"Unrecognized unary operator: '" /*+ str +*/ "'.");
						}
					}

					while (!operatorStack.empty() &&
						   opPrecedence[str] >= opPrecedence[operatorStack.top()])
					{
						rpnQueue.push(new Token<std::wstring>(operatorStack.top(), OP));
						operatorStack.pop();
					}
					operatorStack.push(str);
					lastTokenWasOp = true;
				}
			}
		}
		while (*expr && isspace(*expr)) ++expr;
	}

	while (!operatorStack.empty())
	{
		rpnQueue.push(new Token<std::wstring>(operatorStack.top(), OP));
		operatorStack.pop();
	}

	return rpnQueue;
}

double calculator::calculate(const wchar_t* expr,
							 std::map<std::wstring, double>* vars)
{

	// Convert to RPN with Dijkstra's Shunting-yard algorithm.
	TokenQueue_t rpn = toRPN(expr, vars);

	double ret = calculate(rpn);

	cleanRPN(rpn);

	return ret;
}

double calculator::calculate(TokenQueue_t rpn,
							 std::map<std::wstring, double>* vars)
{

	// Evaluate the expression in RPN form.
	std::stack<double> evaluation;

	while (!rpn.empty())
	{
		TokenBase* base = rpn.front();
		rpn.pop();

		// Operator:
		if (base->type == OP)
		{
			Token<std::wstring>* strTok = static_cast<Token<std::wstring>*>(base);
			std::wstring str = strTok->val;
			if (evaluation.size() < 2)
			{
				throw std::domain_error("Invalid equation.");
			}
			double right = evaluation.top(); evaluation.pop();
			double left = evaluation.top(); evaluation.pop();

			if (!str.compare(L"+"))
			{
				evaluation.push(left + right);
			}
			else if (!str.compare(L"*"))
			{
				evaluation.push(left * right);
			}
			else if (!str.compare(L"-"))
			{
				evaluation.push(left - right);
			}
			else if (!str.compare(L"/"))
			{
				evaluation.push(left / right);
			}
			else if (!str.compare(L"<<"))
			{
				evaluation.push((int)left << (int)right);
			}
			else if (!str.compare(L"^"))
			{
				evaluation.push(pow(left, right));
			}
			else if (!str.compare(L">>"))
			{
				evaluation.push((int)left >> (int)right);
			}
			else if (!str.compare(L"%"))
			{
				evaluation.push((int)left % (int)right);
			}
			else if (!str.compare(L"<"))
			{
				evaluation.push(left < right);
			}
			else if (!str.compare(L">"))
			{
				evaluation.push(left > right);
			}
			else if (!str.compare(L"<="))
			{
				evaluation.push(left <= right);
			}
			else if (!str.compare(L">="))
			{
				evaluation.push(left >= right);
			}
			else if (!str.compare(L"=="))
			{
				evaluation.push(left == right);
			}
			else if (!str.compare(L"!="))
			{
				evaluation.push(left != right);
			}
			else if (!str.compare(L"&&"))
			{
				evaluation.push((int)left && (int)right);
			}
			else if (!str.compare(L"||"))
			{
				evaluation.push((int)left || (int)right);
			}
			else
			{
				throw std::domain_error("Unknown operator: '" /*+ str +*/ "'.");
			}
		}
		else if (base->type == NUM)
		{ 
			// Number
			Token<double>* doubleTok = static_cast<Token<double>*>(base);
			evaluation.push(doubleTok->val);
		}
		else if (base->type == VAR)
		{ 
			// Variable
			if (!vars)
			{
				throw std::domain_error(
					"Detected variable, but the variable map is null.");
			}

			Token<std::wstring>* strTok = static_cast<Token<std::wstring>*>(base);

			std::wstring key = strTok->val;
			std::map<std::wstring, double>::iterator it = vars->find(key);

			if (it == vars->end())
			{
				throw std::domain_error(
					"Unable to find the variable '" /*+ key +*/ "'.");
			}
			evaluation.push(it->second);
		}
		else
		{
			throw std::domain_error("Invalid token.");
		}
	}
	return evaluation.top();
}

void calculator::cleanRPN(TokenQueue_t& rpn)
{
	while (rpn.size())
	{
		delete rpn.front();
		rpn.pop();
	}
}

/* * * * * Non Static Functions * * * * */

calculator::~calculator()
{
	cleanRPN(this->RPN);
}

calculator::calculator(const wchar_t* expr,
					   std::map<std::wstring, double>* vars,
					   std::map<std::wstring, int> opPrecedence)
{
	compile(expr, vars, opPrecedence);
}

void calculator::compile(const wchar_t* expr,
						 std::map<std::wstring, double>* vars,
						 std::map<std::wstring, int> opPrecedence)
{

	// Make sure it is empty:
	cleanRPN(this->RPN);

	this->RPN = calculator::toRPN(expr, vars, opPrecedence);
}

double calculator::eval(std::map<std::wstring, double>* vars)
{
	return calculate(this->RPN, vars);
}

/* * * * * For Debug Only * * * * */

std::wstring calculator::str()
{
	std::wstringstream ss;
	TokenQueue_t rpn = this->RPN;

	ss << L"calculator { RPN: [ ";

	while (rpn.size())
	{
		TokenBase* base = rpn.front();

		Token<double>* doubleTok = dynamic_cast<Token<double>*>(base);
		if (doubleTok)
			ss << doubleTok->val;

		Token<std::wstring>* strTok = dynamic_cast<Token<std::wstring>*>(base);
		if (strTok)
			ss << "'" << strTok->val << "'";

		rpn.pop();

		ss << (rpn.size() ? L", " : L"");
	}
	ss << L" ] }";
	return ss.str();
}
