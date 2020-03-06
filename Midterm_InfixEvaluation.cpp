#include<iostream>
#include<string>
#include<stack>
#include<algorithm>
using namespace std;
stack<double>nums;
stack<char>ops;

int precedence(char op) {
	if (op == '(' || op == ' )') {			
		return 0;
	}
	else if (op == '*' || op == '/') {			
		return 2;
	}
	else               
		return 1;
}

double calculate(double a, double b, char op) {
	double ans;
	switch (op) {
	case'+':
		ans  = (a + b);
		break;
	case'-':
		ans = (a - b);
		break;
	case'*':
		ans = (a * b);
		break;
	case'/':
		ans = (a / b);
		break;
	}
	return ans;
}

void push(string input) {
	for (int i = 0; i < input.size(); i++) {
		if (isdigit(input[i])) {			//values
			int val = 0;
			while ( i < input.length() && isdigit(input[i])) {
				val = (val * 10) + (input[i] - '0');
				i++;
			}
			i--;
			nums.push(val);
		}
		else {									//operators
			if (input[i] == ' ') {
				continue;
			}
			else if (input[i] == '(') {
				ops.push(input[i]);
			}
			else if (input[i] == ')') {
				while (!ops.empty() && ops.top() != '(') {
					double val2 = nums.top();
					nums.pop();
					double val1 = nums.top();
					nums.pop();
					char op = ops.top();
					ops.pop();
					nums.push(calculate(val1, val2, op));
				}
				if (!ops.empty())
					ops.pop();
			}
			else {
				while (!ops.empty() && precedence(ops.top()) >= precedence(input[i])) {
					double val2 = nums.top();
					nums.pop();
					double val1 = nums.top();
					nums.pop();
					char op = ops.top();
					ops.pop();
					nums.push(calculate(val1, val2, op));
				}
				ops.push(input[i]);
			}
		}
	}
	while (!ops.empty()) {
		double val2 = nums.top();
		nums.pop();
		double val1 = nums.top();
		nums.pop();
		char op = ops.top();
		ops.pop();
		nums.push(calculate(val1, val2, op));
	}
	cout << nums.top() << endl;
}



int main() {
	string input;
	while (cin >> input) {
		
		push(input);
	}

	return 0;
}