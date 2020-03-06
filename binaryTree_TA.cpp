//Project : Binary Search Tree
//Create : Wei-Hsuan Liu
//Date : 20191002
//E-mail : M10815205@mail.ntust.edu.tw 

#include <iostream>
using namespace std;


class Node
{
private:
	Node* left_child = NULL;
	Node* right_child = NULL;
	int data = 0;
	int LeftSize = 0;
	int total = 0;

public:
	Node();
	Node* insert_node(Node*, int);
	Node* delete_node(Node*, int);
	void search_by_value(Node*, int);
	void search_by_rank(Node*, int);
	void print_inorder(Node*);
	void print_preorder(Node*);
	void print_postorder(Node*);
};

Node::Node() {};


Node* Node::insert_node(Node* t, int insert_data)
{
	Node* new_node;    
	Node* current_node;  
	Node* parent_node = NULL;   //


	new_node = new Node();
	new_node->left_child = NULL;
	new_node->data = insert_data;
	new_node->right_child = NULL;
	new_node->LeftSize = 1;
	bool success = false;


	if (t == NULL)
	{
		cout << "[INSERT SUCCESS]" << endl;
		new_node->total++;
		return new_node;
	}
	else
	{
		current_node = t;
		while (current_node != NULL)
		{
			parent_node = current_node;
			if (insert_data < parent_node->data)  current_node = current_node->left_child;       //
			else if (insert_data > parent_node->data) current_node = current_node->right_child;   //
			else if (insert_data == parent_node->data)
			{
				cout << "Duplicate data." << endl;
				break;
			}
		}

		// 
		if (insert_data < parent_node->data)
		{
			parent_node->left_child = new_node;
			cout << "[INSERT SUCCESS]" << endl;
			success = true;
			t->total++;
		}
		else if (insert_data > parent_node->data)
		{
			parent_node->right_child = new_node;
			cout << "[INSERT SUCCESS]" << endl;
			success = true;
			t->total++;
		}

		if (success == true)
		{
			current_node = t;
			while (current_node != NULL && current_node != new_node)
			{
				parent_node = current_node;
				if (insert_data < parent_node->data)
				{
					current_node->LeftSize = current_node->LeftSize + 1;
					current_node = current_node->left_child;       //
				}
				else if (insert_data > parent_node->data) current_node = current_node->right_child;   //
			}
		}
	}

	return t;
}

//
Node* Node::delete_node(Node* t, int delete_data)
{
	Node* search_t;         //
	Node* parent_node;      //
	Node* delete_t = NULL;  //
	Node* temp;             //
	int position = 0;       //
	bool get_data = false;  //

	search_t = t;
	parent_node = search_t;

	
	while (search_t != NULL)
	{
		if (delete_data == search_t->data)
		{
			get_data = true;
			break;
		}
		else
		{
			parent_node = search_t;

			if (delete_data < search_t->data)      //
			{
				position = -1;					//左邊
				search_t = search_t->left_child;
			}
			else if (delete_data > search_t->data) //
			{
				position = 1;			//右邊
				search_t = search_t->right_child;
			}
		}
	}

	if (get_data == false)  cout << "No such value to delete." << endl;
	else if (get_data == true)
	{
		search_t = t;
		parent_node = search_t;

		//
		while (search_t != NULL)
		{
			if (delete_data == search_t->data)  break;
			else
			{
				parent_node = search_t;

				if (delete_data < search_t->data)
				{
					search_t->LeftSize = search_t->LeftSize - 1;
					search_t = search_t->left_child;
				}
				else if (delete_data > search_t->data) search_t = search_t->right_child;
			}
		}

		if (position == -1)  delete_t = parent_node->left_child;
		else if (position == 1) delete_t = parent_node->right_child;
		else if (position == 0) delete_t = parent_node;

		if (delete_t->left_child == NULL && delete_t->right_child == NULL)
		{
			if (position == 0) t = NULL;
			else if (position == 1) parent_node->right_child = NULL;
			else if (position == -1) parent_node->left_child = NULL;
		}
		else if (delete_t->left_child == NULL && delete_t->right_child != NULL)  //
		{
			if (position == 0) t = t->right_child;
			else if (position == 1) parent_node->right_child = delete_t->right_child;
			else if (position == -1) parent_node->left_child = delete_t->right_child;
		}
		else if (delete_t->right_child == NULL && delete_t->left_child != NULL)  //
		{
			if (position == 0) t = t->left_child;
			else if (position == 1) parent_node->right_child = delete_t->left_child;
			else if (position == -1) parent_node->left_child = delete_t->left_child;
		}
		else if (delete_t->left_child != NULL && delete_t->right_child != NULL)//
		{
			temp = delete_t->right_child;              //

			if (temp->left_child != NULL)       //  左邊有child
			{
				parent_node = delete_t;                //
				while (temp->left_child != NULL)       
				{
					parent_node = temp;
					temp->LeftSize = temp->LeftSize - 1;
					temp = temp->left_child;
				}
				delete_t->data = temp->data;           
				parent_node->left_child = temp->right_child;
			}
			else if (temp->left_child == NULL)			//左邊空
			{
				if (position == 0)
				{
					parent_node = delete_t;
					delete_t->data = temp->data;       //
					temp->left_child = delete_t->left_child;
					temp->LeftSize = delete_t->LeftSize;
					t = temp;
				}
				else if (position == -1)
				{
					delete_t->data = temp->data;       //
					temp->left_child = delete_t->left_child;
					temp->LeftSize = delete_t->LeftSize;
					parent_node->left_child = temp;    //
				}
				else if (position == 1)
				{
					delete_t->data = temp->data;       
					temp->left_child = delete_t->left_child;
					temp->LeftSize = delete_t->LeftSize;
					parent_node->right_child = temp;  
				}
			}
		}
		cout << "[DELETE SUCCESS]" << endl;
		t->total--;
	}
	return t;
}


void Node::search_by_value(Node* t, int search_data)
{
	int count = 0;
	bool get_data = false;

	while (t != NULL)
	{
		count++;
		if (t->data == search_data)
		{
			cout << "LeftSize = " << t->LeftSize << ", Depth = " << count << endl;
			get_data = true;
			break;
		}
		else if (search_data < t->data) t = t->left_child;
		else if (search_data > t->data) t = t->right_child;
	}
	if (get_data == false) cout << "No such value." << endl;
}


void Node::search_by_rank(Node* t, int rank)
{
	int select = 0;

	if (rank > t->total || rank < 1) cout << "No such value." << endl;
	else
	{
		cout << "The " << rank << "th smallest is ";
		while (t != NULL)
		{
			if (rank == t->LeftSize)
			{
				cout << t->data << endl;
				break;
			}
			else if (rank < t->LeftSize) t = t->left_child;
			else if (rank > t->LeftSize)
			{
				rank = rank - t->LeftSize;
				t = t->right_child;
			}
		}
	}
}


void Node::print_inorder(Node* t)
{
	if (t != NULL)
	{
		print_inorder(t->left_child);    //L
		cout << t->data << " ";			 //V
		print_inorder(t->right_child);   //R
	}
}


void Node::print_preorder(Node* t)
{
	if (t != NULL)
	{
		cout << t->data << " ";			 //V
		print_preorder(t->left_child);   //L
		print_preorder(t->right_child);  //R
	}
}


void Node::print_postorder(Node* t)
{
	if (t != NULL)
	{
		print_postorder(t->left_child);   //L
		print_postorder(t->right_child);  //R
		cout << t->data << " ";			  //V
	}
}

int main()
{
	Node* tree = NULL;
	int data;
	int select = 0;

	while (cin >> select)
	{
		switch (select)
		{
		case 1:
			cin >> data;
			tree = tree->insert_node(tree, data);
			break;
		case 2:
			cin >> data;
			tree = tree->delete_node(tree, data);
			break;
		case 3:
			cin >> data;
			tree->search_by_value(tree, data);
			break;
		case 4:
			cin >> data;
			tree->search_by_rank(tree, data);
			break;
		case 5:
			tree->print_inorder(tree);
			cout << endl;
			break;
		case 6:
			tree->print_preorder(tree);
			cout << endl;
			break;
		case 7:
			tree->print_postorder(tree);
			cout << endl;
			break;
		default:
			cout << "Input Error." << endl;
		}
	}
	return 0;
}