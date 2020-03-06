#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Maze {
private:
	vector<vector<int>>pathDirection;		//0.1.2.3 ->上下左右
public:
	void output();
	void findPath(vector<vector<int>>board, int x, int y, vector<int>path);
	void reset() {
		pathDirection.clear();
	}
};

void Maze::findPath(vector<vector<int>>board, int x, int y, vector<int>path) {
	if (x == board.size() - 1 && y == board[0].size() - 1) {
		pathDirection.push_back(path);
		return;
	}
	board[x][y] = 1;
	if (x != 0 && board[x - 1][y] == 0) {				//上
		x = x - 1;
		path.push_back(0);
		findPath(board, x, y, path);
		path.pop_back();
		x = x + 1;
	}
	if (y != 0 && board[x][y - 1] == 0) {				//左
		y = y - 1;
		path.push_back(2);
		findPath(board, x, y, path);
		path.pop_back();
		y = y + 1;
	}
	if (y != board[x].size() - 1 && board[x][y + 1] == 0) {				//右
		y = y + 1;
		path.push_back(3);
		findPath(board, x, y, path);
		path.pop_back();
		y = y - 1;
	}
	if (x != board.size()-1 && board[x + 1][y] == 0) {				//下
		x = x + 1;
		path.push_back(1);
		findPath(board, x, y, path);
		path.pop_back();
		x = x - 1;
	}
	
	
}

void Maze::output() {
	if (pathDirection.size() == 0) {
		cout << "No path" << endl;
	}
	else {
		for (int i = 0; i < pathDirection.size(); i++) {
			int x = 0;
			int y = 0;
			cout << "(0,0)";
			for (int j = 0; j < pathDirection[i].size(); j++) {
				if (pathDirection[i][j] == 0) {			//上
					x = x - 1;
				}
				else if (pathDirection[i][j] == 1) {	   //下
					x = x + 1;
				}
				else if (pathDirection[i][j] == 2) {				//左	
					y = y - 1;
				}
				else if (pathDirection[i][j] == 3) {			//右
					y = y + 1;
				}
				cout << ",(" << x << "," << y << ")";
			}
			cout << endl;
		}
	}
}



int main() {
	Maze manager = Maze();
	int m, n;
	while (cin >> m >> n) {
		vector<vector<int>>board;
		for (int i = 0; i < m; i++) {
			board.push_back(vector<int>());
			string temp;
			cin >> temp;
			for (int j = 0; j < n; j++) {
				board[i].push_back(temp[j] - '0');
			}
		}
		manager.reset();
		vector<int>path;
		manager.findPath(board, 0, 0, path);
		manager.output();
	}
	return 0;

}