#include<iostream>
using namespace std;

class String{
public:
	String();
	String(const char* init);
	~String();
	friend istream& operator>>(istream& is, String& string);
	friend ostream& operator<<(ostream& os, String& string);
	friend bool operator==(const String& lhs, const String& rhs);
	ostream& Frequency(ostream& os);
	int Find(const String& str);
	void Delete(const unsigned int& start, const unsigned int& length);
	void CharDelete(const char& c);
private:
	char* str;
	unsigned int length;
};

String::String() {
	str = nullptr;
	length = 0;
}

String::String(const char* init) {
	for (this->length = 0; init[this->length] != '\0'; this->length++);
	this->str = new char[this->length];
	for (int i = 0; i < this->length; i++) {
		this->str[i] = init[i];
	}
}

String::~String() {
	if (this->str != nullptr) {
		delete[] this->str;
		this->str = nullptr;
		this->length = 0;
	}
}

istream& operator>>(istream& is, String& string) {
	char input[25];
	is >> input;

	int len = 0;
	for (len = 0; input[len] != '\0'; len++);
	string.~String();
	string.str = new char[len];
	string.length = len;
	for (int i = 0; i < len; i++) {
		string.str[i] = input[i];
	}

	return is;
}

ostream& operator<<(ostream& os, String& string) {
	for (int i = 0; i < string.length; i++) {
		os << string.str[i];
	}
	return os;
}

bool operator==(const String& lhs, const String& rhs) {
	if (lhs.length != rhs.length) {
		return false;
	}
	for (int i = 0; i < lhs.length; i++) {
		if (lhs.str[i] != rhs.str[i]) {
			return false;
		}
	}
	return true;
}

ostream& String::Frequency(ostream& os) {
	int f[26] = { 0 };
	for (int i = 0; i < this->length; i++) {
		f[str[i] - 'a']++;
	}
	for (int i = 0; i < 26; i++) {
		if (f[i]) {
			cout << (char)(i + 'a') << " " << f[i] << endl;
		}
	}
	return os;
}

int String::Find(const String& str) {
	for (int i = 0; i < this->length; i++) {
		bool check = true;
		for (int pos = 0; pos < str.length; pos++) {
			if (i + pos >= this->length || this->str[i + pos] != str.str[pos])
			{
				check = false;
				break;
			}
		}
		if (check)
		{
			return i;
		}
	}
	return -1;
}

void String::Delete(const unsigned int& start, const unsigned int& length) {
	if (this->length - start - length < 0)
	{
		return;
	}
	int j = 0;
	for (int i = 0, l = length; i < this->length; i++)
	{
		if (i < start)
		{
			j++;
			continue;
		}
		if (i >= start && l) {
			l--;
			continue;
		}
		this->str[j++] = this->str[i];
	}

	char* dst = new char[j];
	for (int i = 0; i < j; i++)
	{
		dst[i] = this->str[i];
	}
	delete[] this->str;
	this->str = dst;
	this->length = j;
}



void String::CharDelete(const char& c)
{
	for (int i = this->length; i >= 0; i--)
	{
		if (this->str[i] == c)
		{
			this->Delete(i, 1);
		}
	}
}



int main() {
	String freq("freq"), find("find"), del("del"), chdel("chdel");
	String input, command;

	cin >> input;
	while (cin >> command) {
		if (command == freq) {
			input.Frequency(cout);
		}
		else if (command == find) {
			String str;
			cin >> str;
			cout << input.Find(str) << endl;
		}
		else if (command == del) {
			unsigned int start, length;
			cin >> start >> length;
			input.Delete(start, length);
			cout << input << endl;
		}
		else if (command == chdel) {
			char c;
			cin >> c;
			input.CharDelete(c);
			std::cout << input << std::endl;
		}


	}
}