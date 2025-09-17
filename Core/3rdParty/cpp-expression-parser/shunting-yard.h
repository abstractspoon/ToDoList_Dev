// Source: http://www.daniweb.com/software-development/cpp/code/427500/calculator-using-shunting-yard-algorithm#
// Author: Jesse Brown
// Modifications: Brandon Amos

#ifndef _SHUNTING_YARD_H
#define _SHUNTING_YARD_H

#include <map>
#include <stack>
#include <string>
#include <queue>

enum tokType { NONE, OP, VAR, NUM };

struct TokenBase 
{
  tokType type;
  virtual ~TokenBase() {}
};

template<class T> class Token : public TokenBase 
{
public:
  Token (T t, tokType type) : val(t) { this->type=type; }
  T val;
};

typedef std::queue<TokenBase*> TokenQueue_t;

class calculator 
{
public:
	calculator() {}
	calculator(const wchar_t* expr,
			   std::map<std::wstring, double>* vars = 0,
			   std::map<std::wstring, int> opPrecedence = opPrecedence);
	~calculator();

	void compile(const wchar_t* expr,
				 std::map<std::wstring, double>* vars = 0,
				 std::map<std::wstring, int> opPrecedence = opPrecedence);
	double eval(std::map<std::wstring, double>* vars = 0);
	std::wstring str();

	static double calculate(const wchar_t* expr,
							std::map<std::wstring, double>* vars = 0);
private:
	TokenQueue_t RPN;

	static std::map<std::wstring, int> opPrecedence;
	static std::map<std::wstring, int> buildOpPrecedence();

private:
	static double calculate(TokenQueue_t RPN,
							std::map<std::wstring, double>* vars = 0);
	static void cleanRPN(TokenQueue_t& rpn);
	static TokenQueue_t toRPN(const wchar_t* expr,
							  std::map<std::wstring, double>* vars,
							  std::map<std::wstring, int> opPrecedence = opPrecedence);

};

#endif // _SHUNTING_YARD_H
