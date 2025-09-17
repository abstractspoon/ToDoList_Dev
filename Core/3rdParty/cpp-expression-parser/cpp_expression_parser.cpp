 // cpp_expression_parser.cpp : Defines the entry point for the application.


#include "stdafx.h"
#include "shunting-yard.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>

void Assert(double actual, double expected, const wchar_t* expr = 0) 
{
	double diff = actual - expected;
	if (diff < 0) diff *= -1;
	if (diff < 1e-15) 
	{
		if(expr) 
		{
			std::cout << "  '" << expr << "' indeed evaluated to " << expected << "." << std::endl;
		} 
		else 
		{
			std::cout << "  actual value '" << actual << "' indeed matches the expected value '" << expected << "'" << std::endl;
		}
	} 
	else 
	{
		if(expr) 
		{
			std::cout << "  FAILURE '" << expr << "' evaluated to " <<
				actual << " and NOT " << expected << "!" << std::endl;
		} 
		else 
		{
			std::cout << "  FAILURE, actual value '" << actual <<
				"' does not match the expected value '" << expected <<
				"'" << std::endl;
		}
	}
 }
 
 void Assert(const wchar_t* expr, double expected,
	 std::map<std::wstring, double>* vars = 0) 
 {
	 double actual = calculator::calculate(expr, vars);
	 Assert(actual, expected, expr);
 }
 
 int main(int argc, wchar_t** argv)
 {
	 std::map<std::wstring, double> vars;
	 vars[L"pi"] = 3.14;
	 vars[L"b1"] = 0;
	 
	 std::cout << "\nTests with static calculate::calculate()\n" << std::endl;
	 
	 Assert(L"-pi + 1", -2.14, &vars);
	 Assert(L"-pi + 1 * b1", -3.14, &vars);
	 
	 Assert(L"(20+10)*3/2-3", 42.0);
	 Assert(L"1 << 4", 16.0);
	 Assert(L"1+(-2*3)", -5);
	 
	 std::cout << "\nTests with calculate::compile() & calculate::eval()\n" << std::endl;
	 
	 calculator c1;
	 c1.compile(L"-pi+1", &vars);
	 Assert(c1.eval(), -2.14);
	 
	 calculator c2(L"pi+4", &vars);
	 Assert(c2.eval(), 7.14);
	 Assert(c2.eval(), 7.14);
	 
	 calculator c3(L"pi+b1+b2", &vars);
	 
	 vars[L"b2"] = 1;
	 Assert(c3.eval(&vars), 4.14);
	 
	 vars[L"b2"] = .86;
	 Assert(c3.eval(&vars), 4);
	 
	 std::cout << "\nTesting boolean expressions\n" << std::endl;
	 
	 Assert(L"3 < 3", false);
	 Assert(L"3 <= 3", true);
	 Assert(L"3 > 3", false);
	 Assert(L"3 >= 3", true);
	 Assert(L"3 == 3", true);
	 Assert(L"3 != 3", false);

	 Assert(L"(3 && true) == true", true);
	 Assert(L"(3 && 0) == true", false);
	 Assert(L"(3 || 0) == true", true);
	 Assert(L"(false || 0) == true", false);
	 Assert(L"((1 || 0) || (0 || 1)) && (((1 || 0) || (0 || 1)) || ((1 || 0) || (0 || 1))) == true", true);
	 Assert(L"((1 || 0) || (0 || 1)) && (((1 || 0) || (0 || 1)) && ((1 || 0) || (0 || 1))) == true", true);
	 Assert(L"((1 || 0) || (0 || 1)) && (((0 || 0) || (0 || 1)) && ((1 || 0) || (0 || 1))) == true", true);
	 
	 std::cout << "\nTesting exception management\n" << std::endl;
	 
	 try 
	 {
		 c3.eval();
	 } 
	 catch(std::domain_error err) 
	 {
		 std::cout << "  THROWS as expected" << std::endl;
	 }
	 
	 try 
	 {
		 vars.erase(L"b2");
		 c3.eval(&vars);
	 } 
	 catch(std::domain_error err) 
	 {
		 std::cout << "  THROWS as expected" << std::endl;
	 }
	 
	 try 
	 {
		 vars.erase(L"b1");
		 vars[L"b2"] = 0;
		 c3.eval(&vars);
		 std::cout << "  Do not THROW as expected" << std::endl;
	 } 
	 catch(std::domain_error err) 
	 {
		 std::cout << "  If it THROWS it's a problem!" << std::endl;
	 }
	 
	 std::cout << "\nEnd testing" << std::endl;
	 
	 return 0;
 }
 
 
