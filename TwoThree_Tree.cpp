#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
#define maxkeys 2
using namespace std;

class BTree {
private:
	struct Node {
		int key[2];
		int howmanykeys;							//current number of data
		bool isLeaf;						// it's a leaf or not
		Node* Child[3];    // 0=  left   1=middle		  2= right
		Node* parent;
		Node* tmpnode;
		int wherenewchildposition;          //下面的child分裂 新的要放哪個位置
		int depth;
		/*Node* LeftChild;
		Node* MiddleChild;
		Node* RightChild;*/
	};
	Node* root;
	Node* createNewNode(bool isleaf, int depth);
	void insertPrivate(int key, Node* t);
	void InsertNotfull(int key, Node* t);
	void split(int i, Node* t);
	void splitrecursive(int i, Node* t, Node* parent);
	void adjustDepth(Node* t, int depth);
public:
	BTree();
	void insert(int key);
	void print();

};

BTree::BTree() {
	root = NULL;
}

BTree::Node* BTree::createNewNode(bool isleaf, int depth) {
	Node* y;
	y = new Node;
	y->isLeaf = isleaf;
	y->tmpnode = NULL;
	y->wherenewchildposition = -1;
	y->depth = depth;
	for (int i = 0; i < 3; i++) {
		y->Child[i] = NULL;
	}
	y->parent = NULL;
	y->howmanykeys = 0;
	return y;
}

void BTree::adjustDepth(Node* t, int newdepth) {
	t->depth = newdepth;
	++newdepth;
	if (!t->isLeaf) {
		for (int i = 0; i <= t->howmanykeys; i++) {
			adjustDepth(t->Child[i], newdepth);
		}
	}
}

void BTree::insert(int key) {
	if (!root) {						// If tree is empty
		root = new Node;
		root->isLeaf = true;
		root->key[0] = key;
		root->depth = 1;
		root->howmanykeys = 1;
		for (int i = 0; i < 3; i++) {
			root->Child[i] = NULL;
		}
	}
	else {					// If root is not full call insertnotfull function to insert data
		insertPrivate(key, root);
	}
}

void BTree::insertPrivate(int key, Node* t) {
	if (!t->isLeaf) {								// t is not leaf
		int i = t->howmanykeys - 1;
		while (i >= 0 && t->key[i] > key) {
			i--;
		}
		insertPrivate(key, t->Child[i + 1]);
	}
	else if (t->isLeaf) {							// t is a leaf
		if (t->howmanykeys != maxkeys) {			// if t is not full
			int i = t->howmanykeys - 1;
			while (i >= 0 && t->key[i] > key) {
				t->key[i + 1] = t->key[i];
				i--;
			}
			t->key[i + 1] = key;
			t->howmanykeys += 1;
		}
		else {						//split the node;
			split(key, t);
		}
	}
}



void BTree::split(int key, Node* t) {
	vector<int>temp;
	Node* newnode;
	Node* newroot;
	Node* parent;
	Node* newparent;
	if (t == root) {
		for (int i = 0; i < t->howmanykeys; i++) {		//把值放進vector裡排列大小
			temp.push_back(t->key[i]);
		}
		temp.push_back(key);
		sort(temp.begin(), temp.end());
		int index = (temp.size() / 2);
		for (int i = 0; i < maxkeys; i++) {			//clear the key of node
			t->key[i] = 0;
		}
		newroot = createNewNode(false, 1);
		newnode = createNewNode(t->isLeaf, t->depth);				//create a new node for right side
		newnode->parent = newroot;
		t->parent = newroot;


		newroot->Child[0] = t;
		newroot->howmanykeys++;
		newroot->Child[1] = newnode;
		newroot->key[0] = temp[index];     // give the middle key to the new root


		newnode->Child[0] = t->Child[2];			//give left side node's last child to right side node
		t->Child[2] = NULL;

		t->key[0] = temp[0];			//sing the new key to node;
		t->howmanykeys--;
		newnode->key[0] = temp[index + 1];
		newnode->howmanykeys++;
		root = newroot;
		//depth++;
		adjustDepth(root, 1);
	}


	else if (t->isLeaf) {
		parent = t->parent;
		if (parent->howmanykeys == maxkeys) {		//如果split會造成parent split的話
			temp.push_back(t->key[0]);
			temp.push_back(t->key[1]);
			temp.push_back(key);
			sort(temp.begin(), temp.end());

			//create
			newnode = createNewNode(t->isLeaf, t->depth);
			parent->tmpnode = newnode;
			//key
			t->key[0] = 0;
			t->key[1] = 0;
			newnode->key[0] = temp[2];
			newnode->howmanykeys++;
			t->key[0] = temp[0];
			t->howmanykeys--;


			if (t == parent->Child[0]) {
				parent->wherenewchildposition = 0;
			}
			else if (t == parent->Child[1]) {
				parent->wherenewchildposition = 1;
			}
			else if (t == parent->Child[2]) {
				parent->wherenewchildposition = 2;
			}


			splitrecursive(temp[1], parent, parent->parent);		//
		}
		else {
			newnode = createNewNode(t->isLeaf, t->depth);
			for (int i = 0; i < t->howmanykeys; i++) {		//把值放進vector裡排列大小
				temp.push_back(t->key[i]);
			}
			temp.push_back(key);
			sort(temp.begin(), temp.end());
			for (int i = 0; i < maxkeys; i++) {			//clear the key of node
				t->key[i] = 0;
			}
			t->key[0] = temp[0];
			t->howmanykeys--;
			newnode->key[0] = temp[2];
			newnode->howmanykeys++;
			if (t == parent->Child[0]) {
				//key
				parent->key[1] = parent->key[0];
				parent->key[0] = temp[1];
				//child
				parent->Child[2] = parent->Child[1];
				parent->Child[1] = newnode;
				parent->Child[0] = t;
				parent->howmanykeys++;
				//parent
				t->parent = parent;
				newnode->parent = parent;
			}
			else if (t == parent->Child[1]) {
				//key
				parent->key[1] = temp[1];
				//child
				parent->Child[2] = newnode;
				parent->howmanykeys++;
				//parent
				t->parent = parent;
				newnode->parent = parent;
			}
		}

	}

}


void BTree::splitrecursive(int key, Node* t, Node* parent) {
	Node* newnode;
	Node* newchildnode;
	Node* newparent;
	vector<int>temp;
	
	newchildnode = t->tmpnode;
	t->tmpnode = NULL;
	//create
	newnode = createNewNode(t->isLeaf, t->depth);

	temp.push_back(t->key[0]);
	temp.push_back(t->key[1]);
	temp.push_back(key);
	sort(temp.begin(), temp.end());
	//key
	t->key[0] = 0;
	t->key[1] = 0;
	newnode->key[0] = temp[2];
	newnode->howmanykeys++;
	t->key[0] = temp[0];
	t->howmanykeys--;
	//child
	if (t->wherenewchildposition == 0) {
		newnode->Child[0] = t->Child[1];
		newnode->Child[1] = t->Child[2];
		t->Child[1]->parent = newnode;
		t->Child[2]->parent = newnode;
		t->Child[1] = newchildnode;
		t->Child[2] = NULL;
		newchildnode->parent = t;

	}
	else if (t->wherenewchildposition == 1) {
		newnode->Child[0] = newchildnode;
		newchildnode->parent = newnode;
		newnode->Child[1] = t->Child[2];
		t->Child[2]->parent = newnode;
		t->Child[2] = NULL;
	}
	else if (t->wherenewchildposition == 2) {
		newnode->Child[0] = t->Child[2];
		t->Child[2]->parent = newnode;
		t->Child[2] = NULL;
		newnode->Child[1] = newchildnode;
		newchildnode->parent = newnode;
	}
	if (t == root) {																	//if t is root 
		newparent = createNewNode(false, 1);
		newparent->key[0] = temp[1];
		newparent->howmanykeys++;
		newparent->Child[0] = t;
		newparent->Child[1] = newnode;
		root = newparent;
		t->parent = newparent;
		newnode->parent = newparent;
		//depth++;
		adjustDepth(root, 1);
	}
	else {
		if (parent->howmanykeys == maxkeys) {					//造成parent split
			if (t == parent->Child[0]) {
				parent->wherenewchildposition = 0;
			}
			else if (t == parent->Child[1]) {
				parent->wherenewchildposition = 1;
			}
			else if (t == parent->Child[2]) {
				parent->wherenewchildposition = 2;
			}

			parent->tmpnode = newnode;
			splitrecursive(temp[1], parent, parent->parent);
		}
		else {																			//可插入parent node
			if (temp[1] > parent->key[0]) {
				parent->key[1] = temp[1];
				parent->howmanykeys++;
				parent->Child[2] = newnode;
				newnode->parent = parent;
			}
			else {
				parent->Child[2] = parent->Child[1];
				parent->Child[1] = newnode;
				newnode->parent = parent;
				parent->key[1] = parent->key[0];
				parent->key[0] = temp[1];
				parent->howmanykeys++;
			}
		}
	}
}



void BTree::InsertNotfull(int key, Node* t) {
	int i = t->howmanykeys - 1;				//the index of keys array
	if (t->isLeaf) {					// IF node is leaf
		while (i >= 0 && t->key[i] > key) {
			t->key[i + 1] = t->key[i];
			i--;
		}
		t->key[i + 1] = key;
		t->howmanykeys += 1;
	}
	else {			// If node is not leaf
		while (i >= 0 && t->key[i] > key) {				//計算要去child 的index
			i--;
		}
		if (t->Child[i + 1]->howmanykeys == maxkeys) {				//如果child 

		}
	}

}



void BTree::print() {
	Node* current = root;
	int currentdepth = root->depth;
	queue<Node*>q;
	while (current) {
		for (int i = 0; i <= current->howmanykeys; i++) {
			if (current->Child[i]) {
				q.push(current->Child[i]);
			}
		}
		if (current == root) {
			cout << current->key[0];
			for (int i = 1; i < current->howmanykeys; i++) {
				cout << " " << current->key[i];
			}
		}
		else {
			if (current->depth == currentdepth + 1) {					//跑到下一層去
				cout << endl;
				cout << current->key[0];
				for (int i = 1; i < current->howmanykeys; i++) {
					cout << " " << current->key[i];
				}
			}
			else if (current->depth == currentdepth) {
				cout << " /";
				for (int i = 0; i < current->howmanykeys; i++) {
					cout << " " << current->key[i];
				}
			}
		}
		

		currentdepth = current->depth;
		if (q.size() != 0) {
			current = q.front();
			q.pop();
		}
		else {
			current = NULL;
			cout << endl;
		}
	}

}

int main() {
	string select;
	int key;
	BTree btree;
	while (cin >> select) {
		if (select == "insert") {
			cin >> key;
			btree.insert(key);
		}
		else if (select == "print") {
			btree.print();
		}
	}

	return 0;
}