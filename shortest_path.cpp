#include<iostream>
#include<limits.h>
using namespace std;

class Graph {
private:
	int lengthmatrix[100][100];
	int dist[100];
	int parent[100];
	bool visitornot[100];
	int NumofVertexesPrivate = 0;
public:
	Graph();
	void CreatelengthMatrix(int num);
	void FileTheLengthMatrix(int vertexP, int vertexQ, int length);
	void FileZero();
	void ShortestPath(const int, const int);
	int choose(const int);
	void printSolution(int num,int src);
	void printSolutionSEC(int parent[],int num);
};
Graph graphclass;
Graph::Graph() {};

void Graph::CreatelengthMatrix(int num) {			//建立動態二維舉證
	NumofVertexesPrivate = num;
	/*dist = new int[num+1];					//一維動態陣列
	parent = new int[num+1];
	visitornot = new bool[num+1];	//一維動態陣列
	lengthmatrix = new int* [num+1];
	for (int i = 0; i < num+1; i++) {
		lengthmatrix[i] = new int[num+1];
	}*/
}
void Graph::FileTheLengthMatrix(int P, int Q, int length) {
	lengthmatrix[P][Q] = length;
	lengthmatrix[Q][P] = length;
}
void Graph::FileZero() {
	for (int i = 1; i <= NumofVertexesPrivate; i++) {
		for (int k = 1; k <= NumofVertexesPrivate; k++) {
			lengthmatrix[i][k] = 99999;
		}
	}
}
void Graph::ShortestPath(int n, int src) {
	for (int i = 1; i <= n; i++) {
		parent[0] = -1;
		visitornot[i] = false;
		dist[i] = lengthmatrix[src][i];
	}
	visitornot[src] = true;
	dist[src] = 0;
	for (int i = 1; i <= n-1 ; i++) {
		int u = choose(n);
		visitornot[u] = true;
		for (int w = 1; w <= n; w++) {
			if (!visitornot[w] && lengthmatrix[u][w]>0) {
				if (dist[u] + lengthmatrix[u][w] < dist[w]) {
					dist[w] = dist[u] + lengthmatrix[u][w];
					parent[w] = u;
				}
			}
		}
	}
	printSolution(n,src);
}
void Graph::printSolution(int n,int src) {
	for (int i =1 ; i <= n; i++) {
		if (i  != src) {
			cout << "Distance of " << src << " to " << i  << ": " << dist[i] << endl;
			cout << "Path: " << src;
			printSolutionSEC(parent, i);
			cout << endl;
		}
	}
}
void Graph::printSolutionSEC(int*parents,int j) {
	if (parents[j] == -1) {
		return;
	}
	printSolutionSEC(parents,parents[j]);
	cout <<" "<< j;
}
int Graph::choose(int num) {
	int min = INT_MAX, min_index;
	for (int i = 1; i <= NumofVertexesPrivate; i++) {
		if (visitornot[i] == false && dist[i] <= min) {
			min = dist[i], min_index = i;
		}
	}
	return min_index;
}


int main() {
	int NumofVertexes = 0;
	int Numofedges = 0;
	int StartingVertex = 0;
	cin >> NumofVertexes >> Numofedges >> StartingVertex;
	graphclass.CreatelengthMatrix(NumofVertexes);

	graphclass.FileZero();
	int vertexP = 0;
	int vertexQ = 0;
	int length = 0;
	for (int i = 0; i < Numofedges; i++) {
		cin >> vertexP >> vertexQ >> length;
		graphclass.FileTheLengthMatrix(vertexP, vertexQ, length);
	}
	graphclass.ShortestPath(NumofVertexes, StartingVertex);

	return 0;
}