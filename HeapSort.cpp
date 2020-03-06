#include<iostream>
using namespace std;
void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void MaxHeapify(int* arr, int root, int length) {
	int left = root * 2,
		right = 2 * root + 1, largest;
	if (left <= length && arr[left] > arr[root]) {
		largest = left;
	}
	else {
		largest = root;
	}
	if (right <= length && arr[right] > arr[largest]) {
		largest = right;
	}
	if (largest != root) {
		swap(&arr[largest], &arr[root]);
		MaxHeapify(arr, largest, length);
	}
}

void HeapSort(int* arr,int NumOfelements) {
	for (int i = NumOfelements / 2; i >= 1; i--) {
		MaxHeapify(arr, i, NumOfelements);
	}
}

void heap(int* arr, int& numofelements) {
	swap(&arr[1], &arr[numofelements]);
	numofelements--;
	HeapSort(arr, numofelements);
}



int main() {
	int NumOfelements = 0;
	int CurrentElement = 1;
	cin >> NumOfelements;
	int* arrayele;
	arrayele = new int[NumOfelements + 1];
	arrayele[0] = 0;  //讓0為0
	for (int i = 1; i <= NumOfelements; i++) {
		int tmp;
		cin >> tmp;
		arrayele[i] = tmp;
	}
	HeapSort(arrayele,NumOfelements);


	char command[5]; //給command的
	while (cin >> command) {
		if (command[0] == 's') {                //show
			cout << arrayele[CurrentElement] << endl;
		}
		else if (command[0] == 'l') {			//left
			CurrentElement = CurrentElement * 2;
			cout << arrayele[CurrentElement] << endl;
		}
		else if (command[0] == 'r') {			//right
			CurrentElement = CurrentElement * 2+1;
			cout << arrayele[CurrentElement] << endl;
		}
		else if (command[0] == 'h') {			//heap
			CurrentElement = 1;
			heap(arrayele, NumOfelements);
		}
	}
	return 0;
}

