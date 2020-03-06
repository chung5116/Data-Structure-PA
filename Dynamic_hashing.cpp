#include<iostream>
#include<map>
#include<vector>
using namespace std;

class Bucket {
	int depth, size;
	map<int, string>values;
public:
	Bucket(int depth, int size);
	bool isFull();
	int insert(int key, string alphaAndnum);
	int increaseDepth(void);
	map<int, string>copy(void);
	void clear(void);
	int getDepth(void);
	void getINFfromMap(void);
	void reallyinsert(int key, string alphaAndnum);
	int checkneed2New(int key, string alphaAndnum, int bucket_no);
	bool rehash(int bucket_no, int global_depth, int key);

};

class Directory {
	int global_depth, bucket_size;
	vector<Bucket*>buckets;
	int hash(int n);
	void split(int bucket_no);
	void grow(void);
	int pairIndex(int bucket_no, int depth);
	string bucket_id(int n);
	void rehash_d(int bucket_no,int key,string alphaAndnum);

public:
	Directory(int depth, int bucket_size);		//建立最初的四個表個00 01 10 11
	void insert(int key, string alphaAndnum, bool reinserted);
	void reinsert1(int key, string alphaAndnum, bool reinserted);
	void getvaluefromIndex(int n);
	void reinsert(int key, string alphaAndnum, int oldbucket_no, int local_depth, bool reinserted);


};
/*---------------------------------------------------------------------------------------------*/
Bucket::Bucket(int depth, int size) {
	this->depth = depth;
	this->size = size;
}
bool Bucket::isFull() {
	if (values.size() == size) {
		return true;
	}
	else
		return false;
}
int Bucket::insert(int key, string alphaAndnum) {
	map<int, string>::iterator it;
	it = values.find(key);
	if (it != values.end()) {
		return -1;
	}
	if (isFull()) {
		return 0;
	}
	//values[key] = alphaAndnum;
	return 1;
}

void Bucket::reallyinsert(int key, string alphaAndnum) {
	values[key] = alphaAndnum;
}
int Bucket::increaseDepth(void) {
	depth++;
	return depth;
}

map<int, string>Bucket::copy(void) {
	map<int, string>temp(values.begin(), values.end());
	return temp;
}

void Bucket::clear(void) {
	values.clear();
}

int Bucket::getDepth(void) {
	return depth;
}

void Bucket::getINFfromMap(void) {
	if (values.size() == 0) {
		cout << "The bucket is empty." << endl;
	}
	else {
		map<int, string>::iterator it;
		for (it = values.begin(); it != values.end(); it++) {
			if (it == values.begin()) {
				cout << (*it).second;
			}
			else {
				cout << " " << (*it).second;
			}
		}
		cout << endl;
	}
}

int Bucket::checkneed2New(int key, string alphaAndnum, int bucket_no) {
	if (values.size() == 0) {
		return 0;
	}
	else {
		return values.begin()->first;
	}
}

bool Bucket::rehash(int bucket_no, int global_depth, int key) {
	bool needtoNEW = false;
		map<int, string>::iterator it;
		for (it = values.begin(); it != values.end(); it++) {
			int n = (*it).first;
			int oldbucket_no = n & ((1 << global_depth) - 1);
			if (oldbucket_no != bucket_no)
				needtoNEW = true;
		}
	
	return needtoNEW;
}

/*------------------------------------分隔線------------------------------------------------------*/
Directory::Directory(int depth, int bucket_size) {
	this->global_depth = depth;
	this->bucket_size = bucket_size;
	for (int i = 0; i < 1 << global_depth; i++) {
		buckets.push_back(new Bucket(depth, bucket_size));
	}
}

void Directory::insert(int key, string alphaAndnum, bool reinserted) {
	int bucket_no = hash(key);
	int status = buckets[bucket_no]->insert(key, alphaAndnum);
	if (status == 0) {					//滿了
		split(bucket_no);
		reinsert1(key, alphaAndnum, reinserted);
	}
	else if (status == 1) {
		
		buckets[bucket_no]->reallyinsert(key, alphaAndnum);
		if (!reinserted) {

			cout << bucket_no << endl;
		}
	}
}

void Directory::reinsert1(int key, string alphaAndnum, bool reinserted) {
	int bucket_no = hash(key);
	int status = buckets[bucket_no]->insert(key, alphaAndnum);
	if (status == 0) {					//滿了

		
		int oldkey = buckets[bucket_no]->checkneed2New(key, alphaAndnum, bucket_no);
		int oldbucket_no = hash(oldkey);
		int tmp_depth = buckets[oldbucket_no]->getDepth();
		if (oldkey != 0) {
			if (oldbucket_no != bucket_no) {
				buckets[bucket_no] = new Bucket(tmp_depth, 2);
			}
			else {
				split(bucket_no);
			}
		}

		insert(key, alphaAndnum, reinserted);
	}
	else if (status == 1) {
		int oldkey = buckets[bucket_no]->checkneed2New(key, alphaAndnum, bucket_no);
		if (oldkey != 0) {
			int oldbucket_no = hash(oldkey);
			if (oldbucket_no != bucket_no) {
				int tmp_depth = buckets[bucket_no]->getDepth();
				buckets[bucket_no] = new Bucket(tmp_depth, 2);
			}
		}
		buckets[bucket_no]->reallyinsert(key, alphaAndnum);
		if (!reinserted) {
		
			cout << bucket_no << endl;
		}
		

	}
}

void Directory::reinsert(int key, string alphaAndnum, int old_bucket_no, int local_depth, bool reinserted) {
	int status = 0;
	int bucket_no = hash(key);
	if (bucket_no == old_bucket_no) {
		status = buckets[bucket_no]->insert(key, alphaAndnum);
	}
	else {
		buckets[bucket_no] = new Bucket(local_depth, bucket_size);
		status = buckets[bucket_no]->insert(key, alphaAndnum);
	}
	if (status == 0) {					//滿了
		split(bucket_no);
		insert(key, alphaAndnum, reinserted);
	}
	else if (status == 1) {
		buckets[bucket_no]->reallyinsert(key, alphaAndnum);
		if (!reinserted) {
		
			cout << bucket_no << endl;
		}
		

	}

}

void Directory::rehash_d(int bucket_no, int key, string alphaAndnum) {
	map<int, string>temp;
	map<int, string>::iterator it;
	temp = buckets[bucket_no]->copy();
	int tmp_depth = buckets[bucket_no]->getDepth();
	buckets[bucket_no]->clear();
	buckets[bucket_no] = new Bucket(tmp_depth, 2);
	for (it = temp.begin(); it != temp.end(); it++) {
		insert((*it).first, (*it).second, 1);
	}
}

int Directory::hash(int n) {
	return n & ((1 << global_depth) - 1);
}

void Directory::split(int bucket_no) {
	int local_depth, pair_index, index_diff, dir_size, i;
	map<int, string>temp;
	map<int, string>::iterator it;
	local_depth = buckets[bucket_no]->increaseDepth();   //這裡看
	if (local_depth > global_depth) {
		grow();
	}

	temp = buckets[bucket_no]->copy();
	buckets[bucket_no]->clear();

	for (it = temp.begin(); it != temp.end(); it++) {
		reinsert1((*it).first, (*it).second, 1);
	}
}




void Directory::grow(void) {
	for (int i = 0; i < 1 << global_depth; i++)
		buckets.push_back(buckets[i]);
	global_depth++;
}

int Directory::pairIndex(int bucket_no, int depth) {
	return bucket_no ^ (1 << (depth - 1));
}

string Directory::bucket_id(int n) {
	int d;
	string s;
	d = buckets[n]->getDepth();
	s = "";
	while (n > 0 && d > 0) {
		s = (n % 2 == 0 ? "0" : "1") + s;
		n /= 2;
		d--;
	}
	while (d > 0) {
		s = "0" + s;
		d--;
	}
	return s;
}

void Directory::getvaluefromIndex(int n) {
	if (n >= (1 << global_depth)) {
		cout << "Out of range." << endl;
	}
	else {
		buckets[n]->getINFfromMap();
	}
}

/*-----------------------------------------------------------------*/
int main() {
	char in[5];
	string alphaAndNum;
	int index;
	Directory d(2, 2);		//initialize the original chart;
	while (cin >> in) {
		if (in[0] == 'p') {
			cin >> alphaAndNum;
			int  indexNum = (int)(32 + (alphaAndNum[0] - 65) * 8) + (alphaAndNum[1] - '0'); //轉成數字
			d.insert(indexNum, alphaAndNum, 0);
		}
		else if (in[0] == 'g') {
			cin >> index;
			d.getvaluefromIndex(index);
		}
		else if (in[0] == 'e') {
			return 0;
		}
		else
			break;
	}


	return 0;
}