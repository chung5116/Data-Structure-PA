//Project : Sparse Matrix
//Create : Wen-Kai Wang
//Date : 20190930
//E-mail : M10715100@mail.ntust.edu.tw

#include <iostream>
#include <exception>

#define ERROR_DIMENSIONS "Error dimensions!"

struct Triple { int row, col, value; };

class Matrix;

class MatrixNode
{
public:
	friend class Matrix;
	friend std::ostream& operator <<(std::ostream& out, MatrixNode* node);
	friend std::istream& operator >>(std::istream& in, Matrix& matrix);
	friend std::ostream& operator <<(std::ostream& out, const Matrix& matrix);
private:
	MatrixNode* down, * right;
	bool head;
	union
	{
		MatrixNode* next;
		Triple triple;
	};
	MatrixNode(bool b, Triple* t, bool n = false);
	MatrixNode(MatrixNode* node, bool T = false);
};

MatrixNode::MatrixNode(bool b, Triple* t, bool n)
{
	head = b;
	right = down = this;

	if (b) next = this;
	else
	{
		triple = *t;
		if (n) triple.value = 0 - triple.value;
	}
}

MatrixNode::MatrixNode(MatrixNode* node, bool T)           //(bool, ¤£¬O&)
{
	head = node->head;
	right = down = this;
	if (node->head) next = this;
	else
	{
		if (T)
		{
			triple.col = node->triple.row;
			triple.row = node->triple.col;
			triple.value = node->triple.value;
		}
		else
		{
			triple = node->triple;
		}
	}
}

std::ostream& operator <<(std::ostream& out, MatrixNode* node)
{
	if (node->head) return out;
	else out << node->triple.row << " " << node->triple.col << " " << node->triple.value << std::endl;
	return out;
}

class Matrix
{
public:
	Matrix() {}
	Matrix(const Matrix& matrix);
	~Matrix();

	const Matrix& operator =(const Matrix& rhs);
	Matrix operator +(const Matrix& rhs) const;
	Matrix operator -(const Matrix& rhs) const;
	Matrix operator *(const Matrix& rhs) const;
	Matrix Transpose() const;

	bool IsNull() const { return headnode->triple.value == 0; }

	friend std::istream& operator >>(std::istream& in, Matrix& matrix);
	friend std::ostream& operator <<(std::ostream& out, const Matrix& matrix);
private:
	MatrixNode* headnode;
};

Matrix::Matrix(const Matrix& matrix)
{
	headnode = new MatrixNode(matrix.headnode);
	int p = headnode->triple.row > headnode->triple.col ? headnode->triple.row : headnode->triple.col;
	if (p == 0)
	{
		headnode->right = headnode;
		headnode->down = headnode;
		return;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
	}

	int currentRow = 0;
	MatrixNode* last = head[0];

	for (MatrixNode* curRow = matrix.headnode->right; curRow != matrix.headnode; curRow = curRow->next)
	{
		for (MatrixNode* curCol = curRow->right; curCol != curRow; curCol = curCol->right)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(curCol);
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
		}
	}
	last->right = head[currentRow];

	for (int i = 0; i < matrix.headnode->triple.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = headnode;
	headnode->right = head[0];
	headnode->down = head[0];

	delete[] head;
	return;
}

Matrix::~Matrix()
{
	if (!headnode) return;
	for (MatrixNode* curRow = headnode->right; curRow != headnode;)
	{
		for (MatrixNode* curCol = curRow->right; curCol != curRow;)
		{
			MatrixNode* cur = curCol;
			curCol = curCol->right;
			delete cur;
		}

		MatrixNode* cur = curRow;
		curRow = curRow->next;
		delete cur;
	}
	delete headnode;
}

const Matrix& Matrix::operator =(const Matrix& rhs)
{
	headnode = new MatrixNode(rhs.headnode);
	int p = headnode->triple.row > headnode->triple.col ? headnode->triple.row : headnode->triple.col;
	if (p == 0)
	{
		headnode->right = headnode;
		headnode->down = headnode;
		return *this;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
	}

	int currentRow = 0;
	MatrixNode* last = head[0];

	for (MatrixNode* curRow = rhs.headnode->right; curRow != rhs.headnode; curRow = curRow->next)
	{
		for (MatrixNode* curCol = curRow->right; curCol != curRow; curCol = curCol->right)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(curCol);
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
		}
	}
	last->right = head[currentRow];

	for (int i = 0; i < rhs.headnode->triple.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = headnode;
	headnode->right = head[0];
	headnode->down = head[0];

	delete[] head;
	return *this;
}

Matrix Matrix::operator +(const Matrix& rhs) const
{
	if (headnode->triple.row != rhs.headnode->triple.row || headnode->triple.col != rhs.headnode->triple.col) throw std::runtime_error(ERROR_DIMENSIONS);

	Triple size = headnode->triple;
	size.value = 0;

	Matrix ans;

	int p = size.row >= size.col ? size.row : size.col;
	ans.headnode = new MatrixNode(false, &size);
	if (p == 0)
	{
		ans.headnode->right = ans.headnode;
		ans.headnode->down = ans.headnode;
		return ans;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
		head[i]->next = head[i];
	}

	int currentRow = 0;
	MatrixNode* last = head[0];
	MatrixNode* curRow = rhs.headnode->right, * _curRow = (*this).headnode->right;
	for (; curRow != rhs.headnode && _curRow != (*this).headnode; curRow = curRow->next, _curRow = _curRow->next)
	{
		MatrixNode* _curCol = _curRow->right, * curCol = curRow->right;
		for (; curCol != curRow && _curCol != _curRow; curCol = curCol->right, _curCol = _curCol->right)
		{
			while (_curCol->triple.col < curCol->triple.col && _curCol != _curRow)
			{
				if (_curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = _curCol->triple.row;
					last = head[currentRow];
				}

				last = last->right = new MatrixNode(false, &(_curCol->triple));
				head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
				_curCol = _curCol->right;
			}
			if (_curCol == _curRow) break;

			while (curCol->triple.col < _curCol->triple.col && curCol != curRow)
			{
				if (curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = curCol->triple.row;
					last = head[currentRow];
				}

				last = last->right = new MatrixNode(false, &(curCol->triple));
				head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
				curCol = curCol->right;
			}
			if (curCol == curRow) break;

			Triple sum = _curCol->triple;
			sum.value = _curCol->triple.value + curCol->triple.value;
			if (sum.value != 0)
			{
				if (_curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = _curCol->triple.row;
					last = head[currentRow];
				}
				last = last->right = new MatrixNode(false, &sum);
				head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
			}
		}

		while (_curCol != _curRow)
		{
			if (_curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = _curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(_curCol->triple));
			head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
			_curCol = _curCol->right;
		}

		while (curCol != curRow)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(curCol->triple));
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
			curCol = curCol->right;
		}
	}

	while (_curRow != headnode)
	{
		MatrixNode* _curCol = _curRow->right;
		for (; _curCol != _curRow; _curCol = _curCol->right)
		{
			if (_curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = _curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(_curCol->triple));
			head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
		}
		_curRow = _curRow->next;
	}

	while (curRow != rhs.headnode)
	{
		MatrixNode* curCol = curRow->right;
		for (; curCol != curRow; curCol = curCol->right)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(curCol->triple));
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
		}
		curRow = curRow->next;
	}
	last->right = head[currentRow];

	for (int i = 0; i < size.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = ans.headnode;
	ans.headnode->right = head[0];
	ans.headnode->down = head[0];

	delete[] head;
	return ans;
}

Matrix Matrix::operator -(const Matrix& rhs) const
{
	if (headnode->triple.row != rhs.headnode->triple.row || headnode->triple.col != rhs.headnode->triple.col) throw std::runtime_error(ERROR_DIMENSIONS);

	Triple size = headnode->triple;
	size.value = 0;

	Matrix ans;

	int p = size.row >= size.col ? size.row : size.col;
	ans.headnode = new MatrixNode(false, &size);
	if (p == 0)
	{
		ans.headnode->right = ans.headnode;
		ans.headnode->down = ans.headnode;
		return ans;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
		head[i]->next = head[i];
	}

	int currentRow = 0;
	MatrixNode* last = head[0];
	MatrixNode* curRow = rhs.headnode->right, * _curRow = (*this).headnode->right;
	for (; curRow != rhs.headnode && _curRow != (*this).headnode; curRow = curRow->next, _curRow = _curRow->next)
	{
		MatrixNode* _curCol = _curRow->right, * curCol = curRow->right;
		for (; curCol != curRow && _curCol != _curRow; curCol = curCol->right, _curCol = _curCol->right)
		{
			while (_curCol->triple.col < curCol->triple.col && _curCol != _curRow)
			{
				if (_curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = _curCol->triple.row;
					last = head[currentRow];
				}

				last = last->right = new MatrixNode(false, &(_curCol->triple));
				head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
				_curCol = _curCol->right;
			}
			if (_curCol == _curRow) break;

			while (curCol->triple.col < _curCol->triple.col && curCol != curRow)
			{
				if (curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = curCol->triple.row;
					last = head[currentRow];
				}

				last = last->right = new MatrixNode(false, &(curCol->triple), true);
				head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
				curCol = curCol->right;
			}
			if (curCol == curRow) break;

			Triple sub = _curCol->triple;
			sub.value = _curCol->triple.value - curCol->triple.value;
			if (sub.value != 0)
			{
				if (_curCol->triple.row > currentRow)
				{
					last->right = head[currentRow];
					currentRow = _curCol->triple.row;
					last = head[currentRow];
				}
				last = last->right = new MatrixNode(false, &sub);
				head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
			}
		}

		while (_curCol != _curRow)
		{
			if (_curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = _curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(_curCol->triple));
			head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
			_curCol = _curCol->right;
		}

		while (curCol != curRow)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(curCol->triple), true);
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
			curCol = curCol->right;
		}
	}

	while (_curRow != headnode)
	{
		MatrixNode* _curCol = _curRow->right;
		for (; _curCol != _curRow; _curCol = _curCol->right)
		{
			if (_curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = _curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(_curCol->triple));
			head[_curCol->triple.col]->next = head[_curCol->triple.col]->next->down = last;
		}
		_curRow = _curRow->next;
	}

	while (curRow != rhs.headnode)
	{
		MatrixNode* curCol = curRow->right;
		for (; curCol != curRow; curCol = curCol->right)
		{
			if (curCol->triple.row > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curCol->triple.row;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(false, &(curCol->triple), true);
			head[curCol->triple.col]->next = head[curCol->triple.col]->next->down = last;
		}
		curRow = curRow->next;
	}
	last->right = head[currentRow];

	for (int i = 0; i < size.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = ans.headnode;
	ans.headnode->right = head[0];
	ans.headnode->down = head[0];

	delete[] head;
	return ans;
}

Matrix Matrix::operator *(const Matrix& rhs) const
{
	if (headnode->triple.col != rhs.headnode->triple.row) throw std::runtime_error(ERROR_DIMENSIONS);

	Triple size = headnode->triple;
	size.col = rhs.headnode->triple.row;
	size.value = 0;

	Matrix ans;

	int p = size.row >= size.col ? size.row : size.col;
	ans.headnode = new MatrixNode(false, &size);
	if (p == 0)
	{
		ans.headnode->right = ans.headnode;
		ans.headnode->down = ans.headnode;
		return ans;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
		head[i]->next = head[i];
	}

	int currentRow = 0;
	MatrixNode* last = head[0];
	MatrixNode* curRow = rhs.headnode->down;
	for (; curRow != rhs.headnode; curRow = curRow->next)
	{
		MatrixNode* _curRow = (*this).headnode->right;
		for (; _curRow != (*this).headnode; _curRow = _curRow->next)
		{
			MatrixNode* _curCol = _curRow->right, * curCol = curRow->down;
			Triple mul = _curCol->triple;
			mul.col = curCol->triple.col;
			mul.value = 0;
			for (; curCol != curRow && _curCol != _curRow; curCol = curCol->down, _curCol = _curCol->right)
			{
				while (_curCol->triple.col < curCol->triple.row && _curCol != _curRow)
				{
					_curCol = _curCol->right;
				}
				if (_curCol == _curRow) break;

				while (curCol->triple.row < _curCol->triple.col && curCol != curRow)
				{
					curCol = curCol->down;
				}
				if (curCol == curRow) break;

				mul.value += _curCol->triple.value * curCol->triple.value;
			}
			if (mul.value != 0)
			{
				if (mul.col > currentRow)
				{
					last->down = head[currentRow];
					currentRow = mul.col;
					last = head[currentRow];
				}
				last = last->down = new MatrixNode(false, &mul);
				head[mul.row]->next = head[mul.row]->next->right = last;
			}
		}
	}
	last->down = head[currentRow];

	for (int i = 0; i < size.row; ++i)
	{
		head[i]->next->right = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = ans.headnode;
	ans.headnode->right = head[0];
	ans.headnode->down = head[0];

	delete[] head;
	return ans;
}

Matrix Matrix::Transpose() const
{
	Matrix ans;
	ans.headnode = new MatrixNode(headnode, true);
	int p = headnode->triple.row > headnode->triple.col ? headnode->triple.row : headnode->triple.col;
	if (p == 0)
	{
		ans.headnode->right = ans.headnode;
		ans.headnode->down = ans.headnode;
		return ans;
	}


	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
	}

	int currentRow = 0;
	MatrixNode* last = head[0];

	for (MatrixNode* curCol = headnode->down; curCol != headnode; curCol = curCol->next)
	{
		for (MatrixNode* curRow = curCol->down; curRow != curCol; curRow = curRow->down)
		{
			if (curRow->triple.col > currentRow)
			{
				last->right = head[currentRow];
				currentRow = curRow->triple.col;
				last = head[currentRow];
			}

			last = last->right = new MatrixNode(curRow, true);
			head[curRow->triple.row]->next = head[curRow->triple.row]->next->down = last;
		}
	}
	last->right = head[currentRow];

	for (int i = 0; i < ans.headnode->triple.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = ans.headnode;
	ans.headnode->right = head[0];
	ans.headnode->down = head[0];

	delete[] head;
	return ans;
}

std::istream& operator >>(std::istream& in, Matrix& matrix)
{
	Triple s;
	in >> s.row >> s.col >> s.value;

	int p = s.row >= s.col ? s.row : s.col;
	matrix.headnode = new MatrixNode(false, &s);
	if (p == 0)
	{
		matrix.headnode->right = matrix.headnode;
		matrix.headnode->down = matrix.headnode;
		return in;
	}

	MatrixNode** head = new MatrixNode * [p];
	for (int i = 0; i < p; ++i)
	{
		head[i] = new MatrixNode(true, 0);
		head[i]->next = head[i];
	}

	int currentRow = 0;
	MatrixNode* last = head[0];


	for (int i = 0; i < s.value; ++i)
	{
		Triple t;
		in >> t.row >> t.col >> t.value;

		if (t.row > currentRow)
		{
			last->right = head[currentRow];
			currentRow = t.row;
			last = head[currentRow];
		}

		last = last->right = new MatrixNode(false, &t);
		head[t.col]->next = head[t.col]->next->down = last;
	}
	last->right = head[currentRow];

	for (int i = 0; i < s.col; ++i)
	{
		head[i]->next->down = head[i];
	}

	for (int i = 0; i < p - 1; ++i)
	{
		head[i]->next = head[i + 1];
	}
	head[p - 1]->next = matrix.headnode;
	matrix.headnode->right = head[0];
	matrix.headnode->down = head[0];

	delete[] head;
	return in;
}

std::ostream& operator <<(std::ostream& out, const Matrix& matrix)
{
	for (MatrixNode* curRow = matrix.headnode->right; curRow != matrix.headnode; curRow = curRow->next)
	{
		for (MatrixNode* curCol = curRow->right; curCol != curRow; curCol = curCol->right)
		{
			out << curCol;
		}
	}
	return out;
}

int main(void)
{
	Matrix a, b;
	char op;

	while (std::cin >> op)
	{
		try
		{
			std::cin >> a;
			Matrix c;
			switch (op)
			{
			case '+':
				std::cin >> b;
				c = a + b;
				std::cout << c;
				break;
			case '-':
				std::cin >> b;
				c = a - b;
				std::cout << c;
				break;
			case 'T':
				c = a.Transpose();
				std::cout << c;
				break;
			case '*':
				std::cin >> b;
				c = a * b;
				std::cout << c;
				break;
			default:
				break;
			}
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}