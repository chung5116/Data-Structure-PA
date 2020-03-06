#include <iostream>
#include<fstream>
#include<string>
using namespace std;

fstream file;

int variable = 0; //�ܼƼƶq

int boolvariable = 0; //�X��boolfunction;
char buffer[30];  //�x�spla�����e


void makebasic(int x);
int matrix[100][4];


class Excel {
private:

	int j = 0;
	int k = 0;  //���qpla��i�x�}�Ϊ�  j���� k���U�Ӧr��
	int index = 0;
	int howmanyset = 0;   //�ݦ��X��'-'
	char tmpbool = 0;
public:
	void makematrix(int variable);   //���X�̤j���a�|�x�}
	void makeboolfunctionMatrix(string buffer);   //�� -011 1 ���s��
	void firstgetresult(char inputvalue, int k);
	void expandothersroad(string buffer);
	void fillzero(int x);
	void simplematrix(int x);
	void gobacktosimple(int tmpindex, int x, int y);     //x=variable-1    y=result
	void createdot(string outputfile);
	string checklabel(int q);
	//~Excel();
};
Excel excel;

/*Excel::~Excel() {
	delete[] matrix;
	delete[] boolmatrix;
}*/
void Excel::makematrix(int variable) {
	int tmpfor = pow(2, variable) + 1;
	//matrix = new int[tmpfor * 4]();    //2^x+1 �C  4��;
	//boolmatrix = new int[boolvariable * (variable + 2)]();   //��boolfunction�x�}
	int s = 0;    //label 2^s
	int f = 1;    //label���ƶq
	int i1 = 0;
	int q = 1;
	int label = 1;      //a=1 b=2 c=3 d=4 �H������...
	for (i1 = 0; i1 <= pow(2, variable); i1++) {    //0~16
		matrix[i1][0] = i1;                       //�إ�index
		if ((i1 + 1) < pow(2, variable - 1)) {                //����̤U��������1~8
			matrix[i1 + 1][2] = (i1 + 1) * 2;				//�إ�else-edge  ��0������
			matrix[i1 + 1][3] = ((i1 + 1) * 2) + 1;  //�إ�then-edge ��1������
		}
	}
	for (s = 0; s < variable; s++) {											//�إ�label a.b.c.d....
		while (f <= pow(2, s)) {						  //�M�w�ƶq
			matrix[q][1] = label;   //�M�w��m
			q++;
			f++;
		}
		f = 1;
		label++;
	}
}

void Excel::makeboolfunctionMatrix(string buffer) {      //��buffer����iboolmatrix
	char value;
	howmanyset = 0;
	for (k = 0; k < variable + 2; k++) {
		value = buffer[k];
		//cout << value << endl;
		if (value == '-') {
			howmanyset++;
		}
	}

	if (howmanyset == 0) {
		for (k = 0; k < variable + 2; k++) {
			value = buffer[k];
			firstgetresult(value, k);
		}
	}
	else if (howmanyset != 0) {
		expandothersroad(buffer);
	}
}

void Excel::expandothersroad(string buffer) {
	string tmp1="0";
	string tmp2="0";
	int tmp;
	char value;
	while (true) {
		for (tmp = 0; tmp < (variable + 2); tmp++) {
			value = buffer[tmp];
			if (value == '-') {
				buffer[tmp] = '1';
				tmp1 = buffer;
				expandothersroad(buffer);
				buffer[tmp] = '0';
				tmp2 = buffer;
				expandothersroad(buffer);
			}

		}
		makeboolfunctionMatrix(buffer);
		break;
	}
}

void Excel::firstgetresult(char leftRight, int k) {      //�Ƕi�Ӫ�index
	if (k != variable - 1 && k != variable + 1) {
		if (k == 0) {
			if (leftRight == '1') {
				index = matrix[1][3];
			}
			if (leftRight == '0') {
				index = matrix[1][2];
			}
		}
		else if (leftRight == '1') {
			index = matrix[index][3];
		}
		else if (leftRight == '0') {
			index = matrix[index][2];
		}

	}
	else if (k == variable - 1) {
		tmpbool = leftRight;
	}
	else if (k == variable + 1) {                                //�̫�@��
		if (tmpbool == '1') {
			matrix[index][3] = leftRight - '0';
			//cout << *(matrix + 4 * index + 3);
		}
		else if (tmpbool == '0') {
			matrix[index][2] = leftRight - '0';
			//cout << *(matrix + 4 * index + 2);
		}
	}
}
void Excel::fillzero(int x) {
	for (j = pow(2, variable - 1); j < pow(2, variable); j++) {
		for (k = 2; k <= 3; k++) {
			//cout << *(matrix + 4 * j + k);
			if (matrix[j][k] != 1) {
				matrix[j][k] = 0;
				//cout << *(matrix + 4 * j + k);
			}
		}
	}
}
void Excel::simplematrix(int x) {
	int tmpvariable = variable;
	int tmpindex = 0;
	int tmpresult = 0;
	int q = 0;
	while (tmpvariable != 0) {
		for (q = pow(2, (tmpvariable - 1)); q < pow(2, tmpvariable); q++) {
			if (matrix[q][2] == matrix[q][3]) {
				tmpindex = matrix[q][0];
				tmpresult = matrix[q][2];
				matrix[q][0] = 0;					//�⭫�ƪ����檺index�ܦ�0
				matrix[q][1] = 0;
				gobacktosimple(tmpindex, tmpvariable - 1, tmpresult);
			}
		}
		--tmpvariable;
	}
}
void Excel::gobacktosimple(int tmpindex, int xtmpvariable, int result) {
	int q = 0;
	int w = 0;
	for (q = pow(2, xtmpvariable - 1); q < pow(2, xtmpvariable); q++) {
		for (w = 2; w <= 3; w++) {
			if (matrix[q][w] == tmpindex) {
				matrix[q][w] = result;
				//cout <<tmpindex<<" "<< *(matrix + 4 * q + w) << endl;
			}
		}
	}
}
void Excel::createdot(string outputfile) {
	ofstream file;
	int q = 1;
	int w = 1;
	string tmplabel;
	file.open(outputfile.c_str());  //ios::out | ios::in | ios::trunc
	if (!file.is_open()) {
		cout << "�L�koutput�ɮ�\n";
	}
	else {
		//cin.get();
		file << "digraph ROBDD{" << endl;
		for (q = 1; q <= variable; q++) {                //rank
			file << " {rank=same";
			for (w = 1; w < pow(2, variable); w++) {
				if (matrix[w][1] == q) {
					file << " " << matrix[w][0];
				}
			}
			file << "}" << endl;
		}

		file << endl;
		file << " 0 [label=\"0\",shape=box]" << endl;
		for (q = 1; q <= pow(2, variable); q++) {
			if (matrix[q][0] == pow(2, variable)) {
				file << " " << pow(2, variable) << " [label=\"1\",shape=box]" << endl;
			}
			else if (matrix[q][0] != 0) {
				tmplabel = checklabel(matrix[q][1]);
				file << " " << q << " [label=\"" << tmplabel << "\"]" << endl;
			}
		}

		file << endl;
		for (q = 1; q < pow(2, variable); q++) {
			if (matrix[q][0] != 0) {
				if (matrix[q][2] == 1) {
					file << " " << matrix[q][0] << "->" << pow(2,variable) << " [label=\"0\", style=dotted]" << endl;
					file << " " << matrix[q][0] << "->" << matrix[q][3] << " [label=\"1\", style=solid]" << endl;
				}
				else if (matrix[q][3] == 1) {
					file << " " << matrix[q][0] << "->" << matrix[q][2] << " [label=\"0\", style=dotted]" << endl;
					file << " " << matrix[q][0] << "->" << pow(2, variable) << " [label=\"1\", style=solid]" << endl;
				}
				else {
					file << " " << matrix[q][0] << "->" << matrix[q][2] << " [label=\"0\", style=dotted]" << endl;
					file << " " << matrix[q][0] << "->" << matrix[q][3] << " [label=\"1\", style=solid]" << endl;
				}
			}
		}
		file << "}";
	}
	file.close();
}
string Excel::checklabel(int q) {
	string tmplabel;
	if (q == 1) {
		tmplabel = "a";
	}
	else if (q == 2) {
		tmplabel = "b";
	}
	else if (q == 3) {
		tmplabel = "c";
	}
	else if (q == 4) {
		tmplabel = "d";
	}
	else if (q == 5) {
		tmplabel = "e";
	}
	else if (q == 6) {
		tmplabel = "f";
	}
	else if (q == 7) {
		tmplabel = "g";
	}
	else if (q == 8) {
		tmplabel = "h";
	}
	else if (q == 9) {
		tmplabel = "i";
	}
	else if (q == 10) {
		tmplabel = "j";
	}
	return tmplabel;
}


int main() {
	string inputfile="0";
	string outputfile="0";
	int level = 0;    //�P�_pla ���ƥ�
	int i = 0;
	int m = 0;
	inputfile = "t.pla";
	cout << "enter inputfile name" << endl;
	//cin >> inputfile;
	cout << "enter outputfile name" << endl;
	//cin >>outputfile;
	outputfile = "tt.dot";
	file.open(inputfile.c_str());      //ios::in
	if (!file) {
		cout << "�ɮ׵L�k�}�� \n";
	}
	else {
		while (!file.eof()) {
			i++;    //�ΨӧP�_�{�b�b�ĴX��

			file.getline(buffer, sizeof(buffer));  //�@��@���U�� �A�@����@��
			if (i == 1) {
				level = 1;
				makebasic(1);
			}
			else if (i == 5) {
				level = 2;
				makebasic(2);
			}
			else if (buffer[1] == 'e') {
				break;
			}
			else if (buffer[variable + 1] == '1') {     //�����ݳ̫�@�ӼƦr�O�_��1
				level = 3;
				makebasic(3);
			}
			//cout << buffer << "\n";
		}
	}
	excel.fillzero(1);

	excel.simplematrix(1);

	excel.createdot(outputfile);
	file.close();
	return 0;
}

void makebasic(int x) {
	int which = x;
	switch (which) {
	case 1:
		if (!buffer[4] ) {
			variable =int( buffer[3] - '0');   //-'0' �O�]����r������
			excel.makematrix(variable);
		}
		else {
			variable = int(buffer[3])-'0' * 10 + int(buffer[4]-'0');
			excel.makematrix(variable);
		}
		break;
	case 2:
		if (buffer[4] == 0) {
			boolvariable = buffer[3] - '0'; //-'0' �O�]����r������
		}
		else {
			boolvariable = int(buffer[3] - '0') * 10 + int(buffer[4] - '0');  //-'0' �O�]����r������
		}
		break;
	case 3:    //�����s���
		excel.makeboolfunctionMatrix(buffer);
		break;
	}

}
