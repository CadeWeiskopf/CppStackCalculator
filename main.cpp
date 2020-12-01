#include <iostream>
#include <stack>

using namespace std;

bool isOperand(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool isOperator(char c)
{
	return (c == '(' || c == ')' || c == '^' || c == '*' || c == '/' || c == '+' || c == '-');
}

int getPrecedence(char c)
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

string infixToPostfix(const string& infix)
{
	string postfix = "";
	stack<string> st;

	for (auto c = infix.begin(); c != infix.end(); ++c)
	{
		//cout << *c /*<< "isOperand=" << isOperand(*c) << " isOperator=" << isOperator(*c)*/ << endl;
		//postfix += *c;
		if (isOperand(*c))
		{
			postfix += *c;
			//cout << "operand" << *c << endl;
		}
		else
		{
			//cout << "operator" << string(1, *c) << endl;
			if (st.empty() || getPrecedence(*c) > getPrecedence(st.top()[0]))
			{
				// if stack empty, or precedence of scanned > top, then push onto stack
				st.push(string(1, *c));
				cout << st.top();
			}
			else if (getPrecedence(*c) < getPrecedence(st.top()[0]))
			{
				// if scanned operator's precedence is less than stack's top
				// then pop, and output the top
				do
				{
					postfix += st.top();
					st.pop();
				} while (getPrecedence(*c) < getPrecedence(st.top()[0]));

				if (getPrecedence(*c) == getPrecedence(st.top()[0]))
				{
					while (!st.empty() && getPrecedence(*c) == getPrecedence(st.top()[0]))
					{
						postfix += st.top();
						st.pop();
					}
				}

				st.push(string(1, *c));
			}
			else if (getPrecedence(*c) == getPrecedence(st.top()[0]))
			{
				while (getPrecedence(*c) == getPrecedence(st.top()[0]))
				{
					postfix += st.top();
					st.pop();
				}
			}
		}
	}

	while (!st.empty())
	{
		postfix += st.top();
		st.pop();
	}

	return postfix;
}

int main()
{
	string exp;

	cout << "enter expression" << endl;

	cin >> exp;

	string postfix = infixToPostfix(exp);

	cout << "postfix=" << postfix << endl;

	return 0;
}