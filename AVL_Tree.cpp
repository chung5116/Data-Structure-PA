#include<iostream>
#include<string>
using namespace std;

class AVL_Tree {
private:
	struct Node {
		Node* left_child = NULL;
		Node* right_child = NULL;
		int hight;
		int value = 0;
	};
	Node* root;
	Node* InsertPrivate(int value, Node*n);
	void PreorderPrivate(Node*);
	int getHeight(Node*);
	Node* LL(Node*);
	Node* LR(Node*);
	Node* RR(Node*);
	Node* RL(Node*);
	Node* RemovePrivate(int value, Node* a);
	Node* findMax(Node* a);
	int max(int a, int b);

public:
	AVL_Tree();
	void Insert(int value);
	void Remove(int value);
	void Preorder();

};

AVL_Tree avltree;

AVL_Tree::AVL_Tree() {
	root = NULL;
};

void AVL_Tree::Insert(int value) {
	root = InsertPrivate(value, root);
}

AVL_Tree::Node* AVL_Tree::InsertPrivate(int value, Node* n) {
	if (!n) {
		n = new Node;
		n->value = value;
		n->hight = 0;
		n->left_child = n->right_child = NULL;
	}
	else if (value < n->value) {
		n->left_child = InsertPrivate(value, n->left_child);
		if (getHeight(n->left_child) - getHeight(n->right_child) == 2) {
			if (value < n->left_child->value) {		//LL
				n = LL(n);
			}
			else {                         //LR
				n = LR(n);
			}
		}
	}
	else if (value > n->value) {
		n->right_child = InsertPrivate(value, n->right_child);
		if (getHeight(n->right_child) - getHeight(n->left_child) == 2) {
			if (value > n->right_child->value) {			//RR
				n = RR(n);
			}
			else {									//RL
				n = RL(n);
			}
		}
	}

	n->hight = max(getHeight(n->right_child), getHeight(n->left_child)) + 1;
	return n;
}

int AVL_Tree::getHeight(Node* t) {
	if (!t) {
		return -1;
	}
	else
		return t->hight;
}

AVL_Tree::Node* AVL_Tree::LL(Node* a) {
	Node* b = a->left_child;
	a->left_child = b->right_child;
	b->right_child = a;
	a->hight = max(getHeight(a->left_child), getHeight(a->right_child)) + 1;
	b->hight = max(getHeight(b->left_child), getHeight(b->right_child)) + 1;
	return b;
}

AVL_Tree::Node* AVL_Tree::LR(Node* a) {
	a->left_child = RR(a->left_child);
	return LL(a);
}

AVL_Tree::Node* AVL_Tree::RR(Node* a) {
	Node* b = a->right_child;
	a->right_child = b->left_child;
	b->left_child = a;
	a->hight = max(getHeight(a->left_child), getHeight(a->right_child)) + 1;
	b->hight = max(getHeight(b->left_child), getHeight(b->right_child)) + 1;
	return b;
}

AVL_Tree::Node* AVL_Tree::RL(Node* a) {
	a->right_child = LL(a->right_child);
	return RR(a);
}

void AVL_Tree::Remove(int value) {
	root = RemovePrivate(value, root);
}

AVL_Tree::Node* AVL_Tree::RemovePrivate(int value, Node* t) {
	Node* temp;
	if (!t) {
		return NULL;
	}
	//search the node
	else if (value < t->value) {
		t->left_child = RemovePrivate(value, t->left_child);
	}
	else if (value > t->value) {
		t->right_child = RemovePrivate(value, t->right_child);
	}

	//with 2 children
	else if (t->left_child && t->right_child) {
 		temp = findMax(t->left_child);
		t->value = temp->value;
		t->left_child = RemovePrivate(t->value, t->left_child);
	}

	//with one or zero child
	else {
		temp = t;				//temp  =  delete node
		if (!t->left_child) {
			t = t->right_child;
		}
		else if (!t->right_child) {
			t = t->left_child;
		}
		delete temp;
	}
	if (!t) {
		return t;
	}
	t->hight = max(getHeight(t->left_child), getHeight(t->right_child)) + 1;

	// If is nubalanced

	if (getHeight(t->left_child) - getHeight(t->right_child)==2) {			//left side
		//LL case
		if (getHeight(t->left_child->left_child) - getHeight(t->left_child->right_child) == 1) {
			return LL(t);
		}
		else {
			return LR(t);
		}
	}
	else if (getHeight(t->right_child) - getHeight(t->left_child) == 2) {			//right side
		//RR case
		if (getHeight(t->right_child->right_child) - getHeight(t->right_child->left_child) == 1) {
			return RR(t);
		}
		else {
			return RL(t);
		}
	}
	return t;
}


AVL_Tree::Node* AVL_Tree::findMax(Node* t) {
	if (!t) {
		return NULL;
	}
	else if (!t->right_child) {
		return t;
	}
	else {
		return (t->right_child);
	}
}

void AVL_Tree::Preorder() {
	PreorderPrivate(root);
	cout << endl;
}


void AVL_Tree::PreorderPrivate(Node* current) {
	if (current == root) {
		cout << current->value;
		if (current->left_child != NULL)
			PreorderPrivate(current->left_child);
		if (current->right_child != NULL)
			PreorderPrivate(current->right_child);
	}
	else {
		cout << "," << current->value;
		if (current->left_child != NULL)
			PreorderPrivate(current->left_child);
		if (current->right_child != NULL)
			PreorderPrivate(current->right_child);
	}
}

int AVL_Tree::max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}


int main() {
	string select;
	int value;
	while (cin >> select) {
		if (select[0] == 'I') {
			cin >> value;
			avltree.Insert(value);
		}
		else if (select[0] == 'R') {
			cin >> value;
			avltree.Remove(value);
		}
		else if (select[0] == 'P') {
			avltree.Preorder();
		}
	}
	return 0;
}