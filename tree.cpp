#include<iostream>
#include<istream>
#include<string>
#include<math.h>
using namespace std;
int howmanykeyvalue;    //���X��keyvalue
int depth = 1;
int LeftSize = 1;

class Tree {
private:
	struct TreeNode {
		TreeNode* leftchild;   // �H�U��ܨC�@��node���|��pointer���Vchild
		TreeNode* rightchild;
		TreeNode* parent;
		TreeNode* current;
		int key;             // node����a��information
	};
	int testerror = 0;
	TreeNode* root;        // �Hroot�@���s����ʾ𪺰_�I
	void addleafPrivate(int key, TreeNode* Ptr);
	void deleteNodeprivate(int keyvalue, TreeNode* current);
	void swapdeleteleftNode(int keyvalue, TreeNode* current);
	void swapdeleteRightNode(int keyvalue, TreeNode* current);
	void swapdeleteTwoNode(int keyvalue, TreeNode* current,string leftright);
	int findsmallestNode(TreeNode* current);
	void removeroot();
	void Inorderprivate(TreeNode* current);
	void Preorderprivate(TreeNode* current);
	void Postorderprivate(TreeNode* current);
	void removeSubtree(TreeNode* ptr);
	int searchkeynodePrivate(int keyvalue, TreeNode* ptr);
	void searchLeftSize(TreeNode* ptr);
public:
	Tree();
	~Tree();
	TreeNode* createNode(int keyvalue);                  //TreeNode* �O�^�ǭ�
	void addleaf(int keyvalue);
	void deleteNode(int keyvalue);
	void searchkeynode(int keyvalue);
	void Inorder();
	void Preorder();
	void Postorder();
};
Tree tree;
Tree::Tree() {
	root = NULL;
}

Tree::TreeNode* Tree::createNode(int keyvalue) {
	TreeNode* node = new TreeNode;
	node->key = keyvalue;
	node->leftchild = NULL;
	node->rightchild = NULL;
	node->parent = NULL;
	return node;
}
void Tree::addleaf(int keyvalue) {
	addleafPrivate(keyvalue, root);
}

void Tree::addleafPrivate(int keyvalue, TreeNode* ptr) { 
	if (root == NULL) {
		root = createNode(keyvalue);
	}
	else if (keyvalue < ptr->key){
		if (ptr->leftchild != NULL) {
			addleafPrivate(keyvalue, ptr->leftchild);
		}
		else {
			ptr->leftchild = createNode(keyvalue);
			ptr->leftchild->parent = ptr;
		}
	}
	else if (keyvalue > ptr->key) {
		if (ptr->rightchild != NULL) {
			addleafPrivate(keyvalue, ptr->rightchild);
		}
		else {
			ptr->rightchild = createNode(keyvalue);
			ptr->rightchild->parent = ptr;
		}
	}
}
void Tree::deleteNode(int keyvalue) {
	deleteNodeprivate(keyvalue, root);
}
void Tree::deleteNodeprivate(int keyvalue, TreeNode* current) {
	if (root->key == keyvalue) {
		removeroot();
	}
	else if (keyvalue < current->key) {             //����
		if (current->leftchild->key == keyvalue) {
			if (current->leftchild->leftchild != NULL && current->leftchild->rightchild != NULL) {
				swapdeleteTwoNode(keyvalue, current->leftchild,"left");  
			}
			else {
				swapdeleteleftNode(keyvalue, current->leftchild);   //one children
			}
		}
		else {
			deleteNodeprivate(keyvalue, current->leftchild);
		}
	}
	else if (keyvalue > current->key) {				//�k��
		if (current->rightchild->key == keyvalue) {
			if (current->rightchild->leftchild != NULL && current->rightchild->rightchild != NULL) {
				swapdeleteTwoNode(keyvalue, current->rightchild,"right");
			}
			else {
				swapdeleteRightNode(keyvalue, current->rightchild);    //one children
			}
		}
		else {
			deleteNodeprivate(keyvalue, current->rightchild);
		}
	}
}

void Tree::removeroot() {
	if (root->leftchild == NULL && root->rightchild != NULL) {
		root = root->rightchild;
		delete root->parent;
		root->parent = NULL;
	}
	else if (root->leftchild != NULL && root->rightchild == NULL) {
		root = root->leftchild;
		delete root->parent;
		root->parent = NULL;
	}
	else {
		int  smallestroot = findsmallestNode(root->rightchild);
		root->key = smallestroot;

	}
}
void Tree::swapdeleteTwoNode(int keyvalue, TreeNode* current,string leftright) {     //current = 23
    //TreeNode* smallestnode = findsmallestNode(current->rightchild);
	int smallestnodeVaule = findsmallestNode(current->rightchild);
	current->key = smallestnodeVaule;

}

int Tree::findsmallestNode(TreeNode* current) {     //�Ĥ@��current = keyvalue

	if (current->leftchild != NULL) {
		return findsmallestNode(current->leftchild);
	}

	if (current->parent->leftchild == current) {
		if (current->rightchild != NULL) {
			current->parent->leftchild = current->rightchild;
			current->rightchild->parent = current->parent;
		}
		else if (current->rightchild == NULL && current->leftchild == NULL) {
			current->parent->leftchild = NULL;
		}
	}
	else if (current->parent->rightchild == current) {
		if (current->rightchild != NULL) {
			current->parent->rightchild = current->rightchild;
			current->rightchild->parent = current->parent;
		}
		else if (current->rightchild == NULL && current->leftchild == NULL) {
			current->parent->rightchild = NULL;
		}
	}
	int smallestkey = current->key;
	delete current;
	return smallestkey;
}


void Tree::swapdeleteleftNode(int keyvalue, TreeNode* current) {			//12
	if (current->leftchild == NULL&& current->rightchild != NULL) {
		current->parent->leftchild = current->rightchild;
		current->rightchild->parent = current->parent;
		delete current;
	}
	else if(current->rightchild == NULL && current->leftchild != NULL) {
		current->parent->leftchild = current->leftchild;
		current->leftchild->parent = current->parent;
		delete current;
	}
	else if (current->rightchild == NULL && current->leftchild == NULL) {
		current->parent->leftchild = NULL;
		delete current;
	}
}

void Tree::swapdeleteRightNode(int keyvalue, TreeNode* current) {			//current�Q�R�x
	if (current->leftchild == NULL && current->rightchild != NULL) {
		current->parent->rightchild = current->rightchild;
		current->rightchild->parent = current->parent;
		delete current;
	}
	else if (current->rightchild == NULL && current->leftchild != NULL) {
		current->parent->rightchild = current->leftchild;
		current->leftchild->parent = current->parent;
		delete current;
	}
	else if (current->rightchild == NULL && current->leftchild == NULL) {
		current->parent->rightchild = NULL;
		delete current;
	}
}

void Tree::searchkeynode(int keyvalue) {
	int tmpdepth = searchkeynodePrivate(keyvalue, root);
	cout << "LeftSize = " << LeftSize << ", "; 
	cout << "Depth = " << depth<< endl;
	//cout << "the search key" << keyvalue << endl;
}
int Tree::searchkeynodePrivate(int keyvalue, TreeNode* ptr) {
	if (root->key == keyvalue) {
		depth = 1;
		searchLeftSize(root->leftchild);
		++LeftSize;
	}
	else if (keyvalue < ptr->key) {
		++depth;
		searchkeynodePrivate(keyvalue, ptr->leftchild);
	}
	else if (keyvalue > ptr->key) {
		++depth;
		searchkeynodePrivate(keyvalue, ptr->rightchild);
	}
	else if (keyvalue == ptr->key) {
		if (ptr->leftchild != NULL) {
			searchLeftSize(ptr->leftchild);
			++LeftSize;
		}
	}
	return depth;
}
void Tree::searchLeftSize(TreeNode* ptr) {
	if (ptr->leftchild != NULL) {
		++LeftSize;
		searchLeftSize(ptr->leftchild);
	}

	if (ptr->rightchild != NULL) {
		++LeftSize;
		searchLeftSize(ptr->rightchild);
	}
 }




void Tree::Inorder() {
	Inorderprivate(root);
		//cout << "howmanykeyvalue="<< howmanykeyvalue << endl;
}
void Tree::Preorder() {
	Preorderprivate(root);
}
void Tree::Postorder() {
	Postorderprivate(root);
}


void Tree::Inorderprivate(TreeNode* current) {                     //current �� root
	if (current) {
		if (root != NULL) {
			if (current->leftchild != NULL) {
				Inorderprivate(current->leftchild);
			}
			cout << current->key << " ";
			++testerror;
			if (current->rightchild != NULL) {
				Inorderprivate(current->rightchild);
			}
		}
	}
}

void Tree::Preorderprivate(TreeNode* current) {
	if (current) {
		if (root != NULL) {
			cout << current->key << " ";
			if (current->leftchild != NULL) {
				Preorderprivate(current->leftchild);
			}
			if (current->rightchild != NULL) {
				Preorderprivate(current->rightchild);
			}
		}
	}
}

void Tree::Postorderprivate(TreeNode* current) {
	if (current) {
		if (root != NULL) {
			if (current->leftchild != NULL) {
				Postorderprivate(current->leftchild);
			}
			if (current->rightchild != NULL) {
				Postorderprivate(current->rightchild);
			}
			cout << current->key << " ";
		}
	}
}

Tree::~Tree() {
	removeSubtree(root);
}
void Tree::removeSubtree(TreeNode* ptr) {
	if (ptr != NULL) {
		if (ptr->leftchild != NULL) {
			removeSubtree(ptr->leftchild);
		}
		if (ptr->rightchild != NULL) {
			removeSubtree(ptr->rightchild);
		}
		delete ptr;
	}
}

int main() {
	char inputvalue[100];
	while (cin.getline(inputvalue, sizeof(inputvalue))) {
		int step;
		int checknum;
		int keyvalue;      //�ƭ�
		int localnum;
		int keyvaluearray[30];
		for (step = 0; step < int(sizeof(inputvalue));) {
			if (inputvalue[step] - '0' == 1) {
				checknum = 0;     //��l��
				bool samekeyvalue = false;      //�P�_���S�����ƪ�keyvalue
				step += 2;
				while (inputvalue[step] != ' ' && inputvalue[step] != '\0') {
					++step;
					++checknum;         //��X�X���
				}
				keyvalue = 0;       //��l��keyvalue
				for (localnum = 0; localnum < checknum; localnum++) {            //��Xkeyvalue
					keyvalue = keyvalue + int(pow(10, localnum)) * int(inputvalue[step - 1 - localnum] - '0');
				}
				for (localnum = 0; localnum < howmanykeyvalue; localnum++) {
					if (keyvaluearray[localnum] == keyvalue) {
						cout << "Duplicate data." << endl;
						samekeyvalue = true;
						break;
					}
				}
				if (samekeyvalue == false) {         //�p�G�S�����ƪ�
					tree.addleaf(keyvalue);      //�W�[node
					keyvaluearray[howmanykeyvalue] = keyvalue;             //��keyvalue��i�}�C��K�����j�p
					++howmanykeyvalue;                      //���X��keyvalue
					cout << "[INSERT SUCCESS]" << endl;
				}
				++step;
			}




			if (inputvalue[step] - '0' == 2) {
				bool booldeletekeyvalue = true;       //�ݦ��S���n�Qdelete�����o���s�b
				int tmpswap = 0;
				int localnum3, localnum4;         //�ƶ���
				for (localnum3 = 0; localnum3 < howmanykeyvalue - 1; localnum3++) {
					for (localnum4 = 0; localnum4 < howmanykeyvalue - 1 - localnum3; localnum4++) {
						if (keyvaluearray[localnum4] > keyvaluearray[localnum4 + 1]) {
							tmpswap = keyvaluearray[localnum4 + 1];
							keyvaluearray[localnum4 + 1] = keyvaluearray[localnum4];
							keyvaluearray[localnum4] = tmpswap;
							//swap(keyvaluearray[localnum4], keyvaluearray[localnum4 + 1]);
						}
					}
				};

				int deletekey = 0;
				checknum = 0;     //��l��
				step += 2;
				while (inputvalue[step] != '\0' && inputvalue[step] != ' ') {                                    //&& inputvalue[step] != ' ' OJ�| runtime error
					++step;
					++checknum;         //��X�X���
				}
				for (localnum = 0; localnum < checknum; localnum++) {            //��Xdeletekey
					deletekey = deletekey + int(pow(10, localnum)) * int(inputvalue[step - 1 - localnum] - '0');
				}
				int forlocalnum2 = 0;
				for (forlocalnum2 = 0; forlocalnum2 < howmanykeyvalue; forlocalnum2++) {
					if (keyvaluearray[forlocalnum2] == deletekey) {
						booldeletekeyvalue = false;
						keyvaluearray[forlocalnum2] = 0;
						tree.deleteNode(deletekey);			//�R��node
						for (forlocalnum2 = forlocalnum2; forlocalnum2 < howmanykeyvalue; forlocalnum2++) {					//��᭱�Ʀr���e��
							keyvaluearray[forlocalnum2] = keyvaluearray[forlocalnum2 + 1];
						}
						cout << "[DELETE SUCCESS]" << endl;
						howmanykeyvalue--;
					}
				}
				if (booldeletekeyvalue == true) {
					cout << "No such value to delete." << endl;
				}
				++step;
			}

			if (inputvalue[step] - '0' == 3) {
				depth = 1;
				LeftSize = 1;

				bool boolsearchkeyvalue = true;
				int searchkey = 0;
				checknum = 0;     //��l��
				step += 2;
				while (inputvalue[step] != '\0' && inputvalue[step] != ' ') {
					++step;
					++checknum;         //��X�X���
				}
				for (localnum = 0; localnum < checknum; localnum++) {            //��Xdeletekey
					searchkey = searchkey + int(pow(10, localnum)) * int(inputvalue[step - 1 - localnum] - '0');
				}
				int forlocalnum3 = 0;
				for (forlocalnum3 = 0; forlocalnum3 < howmanykeyvalue; forlocalnum3++) {
					if (keyvaluearray[forlocalnum3] == searchkey) {
						boolsearchkeyvalue = false;
						tree.searchkeynode(searchkey);
					}
				}
				if (boolsearchkeyvalue == true) {
					cout << "No such value." << endl;
				}
				step++;
			}

			if (inputvalue[step] - '0' == 4) {
				int smallestnum = 0;
				checknum = 0;     //��l��
				step += 2;
				while (inputvalue[step] != '\0' && inputvalue[step] != ' ') {
					++step;
					++checknum;         //��X�X���
				}
				for (localnum = 0; localnum < checknum; localnum++) {            //��Xdeletekey
					smallestnum = smallestnum + int(pow(10, localnum)) * int(inputvalue[step - 1 - localnum] - '0');
				}
				if (smallestnum <= howmanykeyvalue && smallestnum > 0) {
					cout << "The " << smallestnum << "th smallest is " << keyvaluearray[smallestnum - 1] << endl;
				}
				else
				{
					cout << "No such value." << endl;
				}
				step++;
			}
			if (inputvalue[step] - '0' == 5) {
				tree.Inorder();
				cout << endl;
				step += 2;
			}
			if (inputvalue[step] - '0' == 6) {
				tree.Preorder();
				cout << endl;
				step += 2;
			}
			if (inputvalue[step] - '0' == 7) {
				tree.Postorder();
				cout << endl;
				step += 2;
			}
			if (inputvalue[step] == '\0'|| inputvalue[step-1] == '\0') {
				break;
			}
		}
		step = 0;
		int localstep = 0;
		for (localstep = 0; localstep < int(sizeof(inputvalue)); localstep++) {
			inputvalue[localstep] = '\0';
		}
	}
	return 0;
}