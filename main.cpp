#include <iostream>
#include <stack>
#include <string>
#include <math.h>

using namespace std;

bool isOperand(const char& c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool isDigit(const char& c)
{
	return (c >= '0' && c <= '9');
}

bool isOperator(const char& c)
{
	return (c == '(' || c == ')' || c == '^' || c == '*' || c == '/' || c == '+' || c == '-');
}

int getPrecedence(const char& c)
{
	switch (c)
	{
	case '(':
	case ')':
		return 4;
	case '^':
		return 3;
	case '*':
	case '/':
		return 2;
	case '+':
	case '-':
		return 1;

	default:
		return -1;
	}
}

int doExponent(const string& lhs, const string& rhs)
{
	cout << lhs << " exponent " << rhs << endl;
	return pow(stoi(lhs), stoi(rhs));
}

int doMultiply(const string& lhs, const string& rhs)
{
	cout << lhs << " multiply " << rhs << endl;
	return stoi(lhs) * stoi(rhs);
}

int doDivision(const string& lhs, const string& rhs)
{
	cout << lhs << " divide " << rhs << endl;
	return stoi(lhs) / stoi(rhs);
}

int doAddition(const string& lhs, const string& rhs)
{
	cout << lhs << " add " << rhs << endl;
	return stoi(lhs) + stoi(rhs);
}

int doSubtract(const string& lhs, const string& rhs)
{
	cout << lhs << " subtract " << rhs << endl;
	return stoi(lhs) - stoi(rhs);
}

string infixToPostfix(const string& infix)
{
	string postfix = "";
	stack<string> st;

	int operandValue = 0;
	bool operandIsDigit = false;

	cout << "infix to postfix.... " << endl;


	for (auto c = infix.begin(); c != infix.end(); ++c)
	{
		//cout << *c /*<< "isOperand=" << isOperand(*c) << " isOperator=" << isOperator(*c)*/ << endl;
		//postfix += *c;
		if (isOperand(*c))
		{
			if (isDigit(*c))
			{
				//int i = *c;
				operandValue = (operandValue * 10) + (int)(*c - '0');
				//cout << "digit -- " << operandValue << endl;
				operandIsDigit = true;
			}
			else
			{
				postfix += *c + " ";
			}
			//cout << "operand" << *c << endl;
		}
		else
		{
			if (operandIsDigit)
			{
				//cout << " end of digit " << operandValue << endl;
				postfix += to_string(operandValue) + " ";
				//postfix += to_string(operandValue);
				operandValue = 0;
				operandIsDigit = false;
			}

			//cout << "operator" << string(1, *c) << endl;
			if (st.empty() || getPrecedence(*c) > getPrecedence(st.top()[0]))
			{
				// if stack empty, or precedence of scanned > top, then push onto stack
				st.push(string(1, *c));
				//cout << st.top();
			}
			else if (getPrecedence(*c) < getPrecedence(st.top()[0]))
			{
				// if scanned operator's precedence is less than stack's top
				// then pop, and output the top
				do
				{
					postfix += st.top() + " ";
					st.pop();
				} while (getPrecedence(*c) < getPrecedence(st.top()[0]));

				if (getPrecedence(*c) == getPrecedence(st.top()[0]))
				{
					while (!st.empty() && getPrecedence(*c) == getPrecedence(st.top()[0]))
					{
						postfix += st.top() + " ";
						st.pop();
					}
				}

				st.push(string(1, *c));
			}
			else if (getPrecedence(*c) == getPrecedence(st.top()[0]))
			{
				while (!st.empty() && getPrecedence(*c) == getPrecedence(st.top()[0]))
				{
					postfix += st.top() + " ";
					st.pop();
				}

				if (st.empty())
				{
					st.push(string(1, *c));
				}
			}
		}

		cout << "... postfix=" << postfix << "." << "stacksize " << st.size() << endl;
	}

	if (operandIsDigit)
	{
		cout << " end of digit " << operandValue << endl;
		postfix += to_string(operandValue) + " ";
		//postfix += to_string(operandValue);
		operandValue = 0;
		operandIsDigit = false;
	}

	while (!st.empty())
	{
		postfix += st.top() + " ";
		st.pop();
	}

	return postfix;
}

string evaluatePostfix(const string& postfix)
{
	string result = "";

	stack<string> st;

	int operandValue = 0;
	bool operandIsDigit = false;

	cout << "Evaluate..." << endl;

	for (auto c = postfix.begin(); c < postfix.end(); ++c)
	{
		//cout << "eval " << *c << endl;
		if (isOperand(*c))
		{
			if (isDigit(*c))
			{
				operandValue = (operandValue * 10) + (int)(*c - '0');
				operandIsDigit = true;
			}
			else if (*c == ' ')
			{
				string s = to_string(operandValue);
				//cout << "push " << s << "." << endl;
				st.push(s);

				operandValue = 0;
			}
			else
			{
				string s = string(1, *c);
				//cout << "push " << s << "." << endl;
				st.push(s);
			}
		}
		else if (operandIsDigit && *c == ' ')
		{
			string s = to_string(operandValue);
			//cout << "push " << s << "." << endl;
			st.push(s);

			operandValue = 0;
			operandIsDigit = false;
		}
		else if (isOperator(*c))
		{
			// scanned operator, operate on top 2 operands in stack
			// and push the result onto stack
			string rhs = st.top();
			rhs.erase(0, rhs.find_first_not_of(" "));
			rhs.erase(rhs.find_last_not_of(" ") + 1);
			st.pop();

			string lhs = st.top();
			lhs.erase(0, lhs.find_first_not_of(" "));
			lhs.erase(lhs.find_last_not_of(" ") + 1);
			st.pop();

			if (*c == '^')
			{
				st.push(to_string(doExponent(lhs, rhs)));
			}
			else if (*c == '*')
			{
				st.push(to_string(doMultiply(lhs, rhs)));
			}
			else if (*c == '/')
			{
				st.push(to_string(doDivision(lhs, rhs)));
			}
			else if (*c == '+')
			{
				st.push(to_string(doAddition(lhs, rhs)));
			}
			else if (*c == '-')
			{
				st.push(to_string(doSubtract(lhs, rhs)));
			}
		}
	}

	if (st.size() == 1)
	{
		result = st.top();
	}
	else
	{
		result = "error";
	}

	return result;
}

int main()
{
	string exp;

	cout << "enter expression" << endl;

	cin >> exp;

	string postfix = infixToPostfix(exp);

	cout << "postfix=" << postfix << endl;

	string result = evaluatePostfix(postfix);

	cout << "result=" << result << endl;

	return 0;
}