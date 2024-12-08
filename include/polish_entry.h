

#include <iostream>
#include "stack_queue.h"
#include <map>
#include <string>

using namespace std;

std::map<char, int> priorities
{
	{'+', 1},
	{'-', 1},
	{'*', 2},
	{'/', 2}
};


enum types { number, operation, open_bracket, close_bracket };

class Terms
{
protected:
	types type;
public:
	types getType() const noexcept { return type; }
	virtual double getValue() const noexcept = 0;
	virtual char getOperation() const noexcept = 0;
	virtual int getPriority() const noexcept = 0;
};

class Number : public Terms
{
	double value;
	char getOperation() const noexcept { return 0; }
	int getPriority() const noexcept { return 0; }

public:
	double getValue() const noexcept { return value; }
	Number(double value)
	{
		this->value = value;
		this->type = number;
	}
};

class Operation : public Terms
{
protected:
	char op;
	int priority;
	double getValue() const noexcept { return 0; }
public:
	char getOperation() const noexcept { return op; }
	int getPriority() const noexcept { return priority;  }
	Operation(char op)
	{
		this->op = op;
		this->priority = priorities[op];
		this->type = operation;
	}
};

class Close_Bracket : public Terms
{
	char bracket;
	double getValue() const noexcept { return 0; }
	int getPriority() const noexcept { return 0; }
	char getOperation() const noexcept { return 0; }
public:
	Close_Bracket()
	{
		bracket = ')';
		type = close_bracket;
	}
};

class Open_Bracket : public Terms
{
	char bracket;
	double getValue() const noexcept { return 0; }
	int getPriority() const noexcept { return 0; }
	char getOperation() const noexcept { return 0; }
public:
	Open_Bracket()
	{
		bracket = '(';
		type = open_bracket;
	}
};

class Translator : private Terms
{
	using Terms::type;
	TVector<Terms*> terms;
	TVector<Terms*> polish_notaion;
	std::string main_str;
	double getValue() const noexcept { return 0; }
	int getPriority() const noexcept { return 0; }
	char getOperation() const noexcept { return 0; }

public:
	Translator( std::string str ) : main_str(str) {}
	void fixed_analysis()
	{
		int number_status = 0;
		std::string N;
		for (int i = 0; i < main_str.size(); i++)
		{
			if (number_status == 0)
			{
				if (main_str[i] == '(') { terms.push_back(new Open_Bracket); }
				else if (main_str[i] == ')') { terms.push_back(new Close_Bracket); }
				else if (main_str[i] == '+' || main_str[i] == '-' || main_str[i] == '*' || main_str[i] == '/') {
					terms.push_back(new Operation(main_str[i]));
				}
				else if (isdigit(main_str[i]) || main_str[i] == '.')
				{
					number_status = 1;
					N += main_str[i];
				}
				else throw std::domain_error("invalid character_1");
			}
			else
			{
				if (isdigit(main_str[i]) || main_str[i] == '.')
				{
					N += main_str[i];
				}
				else if (main_str[i] == '(')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new Open_Bracket);
					N.clear();
				}
				else if (main_str[i] == '+' || main_str[i] == '-' || main_str[i] == '*' || main_str[i] == '/')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new Operation(main_str[i]));
					N.clear();
				}
				else if (main_str[i] == ')')
				{
					number_status = 0;
					terms.push_back(new Number(std::stod(N)));
					terms.push_back(new Close_Bracket);
					N.clear();
				}
				else throw std::domain_error("invalid character_2");
			}
		}
		if (!N.empty()) terms.push_back(new Number(std::stod(N)));
	}


	void syntax_analysis()
	{
		int state;
		Stack<char> bracketStack; 

		for (size_t i = 0; i < terms.size(); i++) {
			if (terms[i]->getType() == open_bracket) {
				bracketStack.push('(');
			}
			else if (terms[i]->getType() == close_bracket) {
				if (bracketStack.Empty()) {
					throw std::domain_error("Mismatched closing bracket");
				}
				bracketStack.pop();
			}
		}

		if (!bracketStack.Empty()) {
			throw std::domain_error("Mismatched opening bracket");
		}

		if (terms[0]->getType() == operation) throw std::domain_error("first one is operation");
		else if (terms[0]->getType() == number) state = 0;
		else if (terms[0]->getType() == open_bracket) state = 2;
		else if (terms[0]->getType() == close_bracket) throw std::domain_error("first one is close bracket");
		for ( int i = 1; i < terms.size(); i++)
		{
			switch (state)
			{
			case 0:
				if (terms[i]->getType() == open_bracket || terms[i]->getType() == number) throw std::domain_error("invalid character after operand");
				else if (terms[i]->getType() == operation) state = 1;
				else if (terms[i]->getType() == close_bracket) state = 3;
				break;

			case 1:
				if (terms[i]->getType() == operation || terms[i]->getType() == close_bracket) throw std::domain_error("invalid character after operation");
				else if (terms[i]->getType() == number) state = 0;
				else if (terms[i]->getType() == open_bracket) state = 2;
				break;

			case 2:
				if (terms[i]->getType() == operation || terms[i]->getType() == close_bracket) throw std::domain_error("invalid character open bracket");
				else if (terms[i]->getType() == number) state = 0;
				else if (terms[i]->getType() == open_bracket) state = 2;
				break;
				
			case 3:
				if (terms[i]->getType() == number || terms[i]->getType() == open_bracket) throw std::domain_error("invalid character after close bracket");
				else if (terms[i]->getType() == operation) state = 1;
				else if (terms[i]->getType() == close_bracket) state = 3;
				break;
			}

		}
		if (terms[terms.size() - 1]->getType() == operation || terms[terms.size() - 1]->getType() == open_bracket) throw std::domain_error("last character is operation");
	}

	void transformation()
	{
		Stack<Terms*> st; 
		for (int i = 0; i < terms.size(); i++) {
			if (terms[i]->getType() == number) {
				polish_notaion.push_back(terms[i]);
			}
			else if (terms[i]->getType() == operation) 
			{
				while (!st.Empty() && st.top()->getType() == operation && terms[i]->getPriority() <= st.top()->getPriority()) 
				{
					polish_notaion.push_back(st.top());
					st.pop();
				}
				st.push(terms[i]);
			}
			else if (terms[i]->getType() == open_bracket) 
			{
				st.push(terms[i]);
			}
			else if (terms[i]->getType() == close_bracket) 
			{
				while (!st.Empty() && st.top()->getType() != open_bracket) 
				{
					polish_notaion.push_back(st.top());
					st.pop();
				}
				if (!st.Empty()) 
				{
					st.pop(); 
				}
				else 
				{
					throw std::domain_error("Mismatched closing bracket");
				}
			}
		}

		while (!st.Empty()) {
			polish_notaion.push_back(st.top());
			st.pop();
		}
	}

	double calculation()
	{
		Stack<double> ans; 
		double right, left;
		types current;

		for (int i = 0; i < polish_notaion.size(); i++) {
			current = polish_notaion[i]->getType();
			if (current == number) {
				ans.push(polish_notaion[i]->getValue());
			}
			else {
				if (ans.Empty()) throw std::domain_error("Insufficient values in the stack for operation");
				right = ans.top();
				ans.pop();

				if (ans.Empty()) throw std::domain_error("Insufficient values in the stack for operation");
				left = ans.top();
				ans.pop();

				switch (polish_notaion[i]->getOperation()) {
				case '+': ans.push(left + right); break;
				case '-': ans.push(left - right); break;
				case '*': ans.push(left * right); break;
				case '/':
					if (right == 0.0) throw std::domain_error("division by zero");
					ans.push(left / right);
					break;
				}
			}
		}

		if (ans.Empty()) throw std::domain_error("No result available");
		return ans.top();
	}

	double solution()
	{
		this->fixed_analysis();
		this->syntax_analysis();
		this->transformation();
		return this->calculation();
	}
};