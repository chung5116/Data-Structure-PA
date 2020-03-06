#include<iostream>
#include<string>
using namespace std;	

template<class T>
class Polynomial {

public:
	Polynomial();
	Polynomial(const int& degree);
	~Polynomial();
	T Eval(const T& x);
	template<class F>
	friend istream& operator>>(istream& is, Polynomial<F>& pol);
private:
	T* coef;
	int degree;				
};

template<class T>
Polynomial<T>::Polynomial(const int& degree) {
	if (degree < 0) {
		this->coef = nullptr;
		this->degree = -1;
	}
	else {
		this->coef = new T[degree + 1];
		this->degree = degree;
	}

}

template<class T>
Polynomial<T>::~Polynomial() {
	if (this->coef != nullptr) {
		delete[]this->coef;
		this->coef = nullptr;
	}
	this->degree = -1;
}

template<class T>
istream& operator>>(istream& is, Polynomial<T>& pol) {
	for (int i = 0; i < pol.degree + 1; ++i) {
		is >> pol.coef[i];
	}
	return is;
}

template<class T>
T Polynomial<T>::Eval(const T& x) {

	T val = T();
	T pow = static_cast<T>(1);
	for (int i = 0; i < this->degree + 1; ++i) {
		val += pow * coef[i];
		pow *= x;
	}
	return val;
}






int main() {
	string type;
	int degree;
	while (cin >> type) {
		cin >> degree;
		if (type == "int") {
			Polynomial<int>pol(degree);
			int x;
			cin >> pol >> x;
			cout << pol.Eval(x) << endl;
		}
		else if (type == "float") {
			Polynomial<float>pol(degree);
			float x;
			cout.precision(6);
			cin >> pol >> x;
			cout << pol.Eval(x) << endl;
		}
		else if (type == "double") {
			Polynomial<double>pol(degree);
			double x;
			cout.precision(10);
			cin >> pol >> x;
			cout << pol.Eval(x) << endl;
		}




	}


	return 0;
}