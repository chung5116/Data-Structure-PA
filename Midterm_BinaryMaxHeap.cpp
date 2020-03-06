#include<iostream>
#include<string>
#include<vector>
#include<sstream>
using namespace std;

class Tree {
private:
	vector<int>keys;
	int searchkey(int key, int i);
	void swap(int& a, int& b);
	void maxheapify(int i);
public:
	Tree();
	void initial();
	void insert(vector<int>);
	void del(vector<int>);
	void print();

};
Tree::Tree() {
}

void Tree::initial() {
	keys.push_back(-1);
}
void Tree::swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}

void Tree::print() {
	for (int i = 1; i < keys.size(); i++) {
		cout << keys[i];
		if (i == keys.size() - 1) {
			cout << endl;
		}
		else cout << " ";
	}
}


void Tree::insert(vector<int>t) {
	int lp, rp;
	for (int i = 0; i < t.size(); i++) {
		lp = keys.size();
		keys.push_back(t[i]);
		while (lp > 1) {
			rp = lp / 2;
			if (keys[rp] > keys[lp]) break;
			swap(keys[rp], keys[lp]);
			lp = rp;
		}
	}
}

void Tree::maxheapify(int root) {
	int left = root * 2;
	int right = root * 2 + 1;
	int largest;

	if (right <= keys.size() - 1 && keys[right] > keys[root]) {					/*這邊注意 要先看右邊再看左邊不然會error*/
		largest = right;
	}
	else {	
		largest = root;
	}
	if (left <= keys.size() - 1 && keys[left] > keys[largest]) {
		largest = left;
	}
	if (largest != root) {
		swap(keys[largest], keys[root]);
		maxheapify(largest);
	}
}

void Tree::del(vector<int>vec) {
	int lp, rp;
	for (int i = 0; i < vec.size(); i++) {
		if (keys.size() == 1) {
			cout << "The heap is empty." << endl;
			return;
		}
		lp = searchkey(vec[i], 1);
		if (lp == -1) {
			cout << "Value " << vec[i] << " is not in the heap." << endl;
		}
		else {
			while (lp != -1) {
				rp = keys.size() - 1;
				swap(keys[lp], keys[rp]);
				keys.pop_back();
				maxheapify(lp);
				lp = searchkey(vec[i], 1);
			}
		}
	}
}


int Tree::searchkey(int record, int lp) {
	int rp = keys.size() - 1;
	if (keys[rp] == record)return rp;
	while (lp < rp) {
		int rhs = searchkey(record, lp * 2 + 1);
		int lhs = searchkey(record, lp * 2);
		if (lhs == -1 && rhs == -1) {
			return(keys[lp] == record ? lp : -1);
		}
		else
			return(rhs > lhs ? rhs : lhs);
	}
	return -1;
}

int main() {
	string input;
	vector<int>data;
	Tree tree;
	tree.initial();
	while (getline(cin, input)) {
		istringstream iss(input);
		string cmd;
		iss >> cmd;
		data.clear();
		int record;
		while (iss >> record) {
			data.push_back(record);
		}
		if (cmd == "Exit") {
			return 0;
		}
		else if (cmd == "Print") {
			tree.print();
		}
		else if (cmd == "Insert") {
			tree.insert(data);
		}
		else if (cmd == "Delete") {
			tree.del(data);
		}
	}

}