#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

vector<char>v;

void inputPattern(string p) {
	for (int i = 0; i < p.size(); i++) {
		v.push_back(p[i]);
	}
}

void find(string str) {
	bool check = true;
	for (int i = 0; i <= v.size()-str.size(); i++) {
		check = true;
		for (int j = 0; j < str.size(); j++) {
			if (str[j] != v[i + j]) {
				check = false;
				break;
			}
		}
		if (check) {
			cout << i << endl;
			break;
		}
	}
	if (!check) {
		cout << "-1" << endl;
	}
}

void del(int start, int length) {
	vector<char>::iterator it;
	v.erase(v.begin() + start, v.begin() + start + length);
	for (int i = 0; i < v.size(); i++) {
		cout << v[i];
	}
	cout << endl;
}

void freq() {
	int f[26] = { 0 };
	for (int i = 0; i < v.size(); i++) {
		f[v[i] - 'a']++;
	}
	for (int i = 0; i < 26; i++) {
		if (f[i]) {
			cout << (char)(i + 'a') << " ";
			cout<< f[i] << endl;
		}
	}

}

void chdel(char c) {
	vector<char>::iterator it;
	for (it = v.begin(); it != v.end();) {
		if (*it == c) {
			it = v.erase(it);
		}
		else {
			it++;
		}
	}
	if (!v.empty()) {
		for (it = v.begin(); it != v.end(); it++) {
			cout << *it;
		}
		cout << endl;
	}
}

int main() {
	string pattern;
	string command;

	cin >> pattern;
	inputPattern(pattern);
	while (cin >> command) {
		if (command == "find") {
			string str;
			cin >> str;
			find(str);
		}
		else if (command == "del") {
			int s, l;
			cin >> s >> l;
			del(s, l);
		}
		else if (command == "freq") {
			char c;
			freq();
		}
		else if (command == "chdel") {
			char c;
			cin >> c;
			chdel(c);
			
		}

	}


}