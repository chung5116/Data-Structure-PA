// Project : Two Three Tree
// Create : Zheng-Yi, Shen
// Date : 20191101
// E-mail : m10715098@gapps.ntust.edu.tw  

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int priority(char op) {
	switch (op) {
	case '*': case '/':
		return 2;
	case '+': case '-':
		return 1;
	default:
		return 0;
	}
}

string inToPostfix(string infix) {
	vector<char> stack;
	string postfix = "";
	for (int i = 0; i < infix.length(); i++) {
		switch (infix[i]) {
		case ' ':
			break;
		case '+': case '-': case '*': case '/':
			while (stack.size() > 0 && priority(stack.back()) >= priority(infix[i])) {
				postfix += stack.back();
				postfix += " ";
				stack.pop_back();
			}
		case '(':
			stack.push_back(infix[i]);
			break;
		case ')':
			while (stack.back() != '(') {
				postfix += stack.back();
				postfix += " ";
				stack.pop_back();
			}
			stack.pop_back(); // pop (
			break;
		default:
			postfix += infix[i];
			while (isdigit(infix[i + 1])) {
				i++;
				postfix += infix[i];
			}
			postfix += " ";
			break;
		}
	}
	while (stack.size() > 0) {
		postfix += stack.back();
		stack.pop_back();
		if (stack.size() > 0) {
			postfix += " ";
		}
	}
	return postfix;
}

double postToDouble(string postfix) {
	vector<double> stack;
	string numberStr = "";
	double n1 = 0, n2 = 0;

	for (int i = 0; i < postfix.length(); i++) {
		switch (postfix[i]) {
		case ' ':
			if (numberStr != "") {
				stack.push_back(stod(numberStr));
				numberStr = "";
			}
			break;
		case '+': case '-':	case '*':case '/':
			n1 = stack.back();
			stack.pop_back();
			n2 = stack.back();
			stack.pop_back();
			if (postfix[i] == '+') stack.push_back(n2 + n1);
			else if (postfix[i] == '-') stack.push_back(n2 - n1);
			else if (postfix[i] == '*') stack.push_back(n2 * n1);
			else if (postfix[i] == '/') stack.push_back(n2 / n1);
			break;
		default:
			numberStr += postfix[i];
			break;
		}
	}
	return stack.back();
}

int main(void) {
	string cmd;
	while (getline(cin, cmd)) {
		string temp = inToPostfix(cmd);
		double ans = postToDouble(temp);
		cout << ans << endl;
	}
	return 0;
}
