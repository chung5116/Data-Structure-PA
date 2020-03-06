// Project : String Pattern
// Create : Pei-Ru Wang
// Date : 20190929
// E-mail : M10815012@mail.ntust.edu.tw

#include <iostream>

#define STRING_MAX_LENGTH 255

using namespace std;

class String
{
public:
	// You need to initialize this->str to *init.
	String();
	String(const char* init);
	~String();
	friend istream& operator>>(istream& is, String& string);
	friend ostream& operator<<(ostream& os, String& string);
	friend bool operator==(const String& lhs, const String& rhs);
	// Return the number of characters in *this;
	unsigned int Length() { return this->length; };
	// return string str first position in the string. (0 is the first position).
	int Find(const String& str);
	// Output the frequency of each of the distinct characters in the string. (Only show the characters who's frequency is non-zero.)
	ostream& Frequency(ostream& os);
	// Remove length characters beginning at start.
	void Delete(const unsigned int& start, const unsigned int& length);
	// Remove char c in string.
	void CharDelete(const char& c);
private:
	char* str;
	unsigned int length;
};

int main()
{
	String freq("freq"), find("find"), del("del"), chdel("chdel");
	String input, command;

	cin >> input;
	while (cin >> command)
	{
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

String::String()
{
	str = nullptr;
	length = 0;
}

String::String(const char* init)
{
	for (this->length = 0; init[this->length] != '\0'; this->length++);
	this->str = new char[this->length];
	for (int i = 0; i < this->length; i++)
	{
		this->str[i] = init[i];
	}
}

String::~String()
{
	if (this->str != nullptr)
	{
		delete[] this->str;
		this->str = nullptr;
		this->length = 0;
	}
}

istream& operator>>(istream& is, String& string)
{
	char input[STRING_MAX_LENGTH];
	is >> input;

	int len = 0;
	for (len = 0; input[len] != '\0'; len++);
	string.~String();
	string.str = new char[len];
	string.length = len;
	for (int i = 0; i < len; i++)
	{
		string.str[i] = input[i];
	}

	return is;
}

ostream& operator<<(ostream& os, String& string)
{
	for (int i = 0; i < string.length; i++)
	{
		os << string.str[i];
	}
	return os;
}

bool operator==(const String& lhs, const String& rhs)
{
	if (lhs.length != rhs.length)
	{
		return false;
	}
	for (int i = 0; i < lhs.length; i++)
	{
		if (lhs.str[i] != rhs.str[i])
		{
			return false;
		}
	}
	return true;
}

int String::Find(const String& str)
{
	for (int i = 0; i < this->length; i++)
	{
		bool check = true;

		for (int pos = 0; pos < str.length; pos++)
		{
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

ostream& String::Frequency(ostream& os)
{
	int ct[26] = { 0 };
	for (int i = 0; i < this->length; i++)
	{
		ct[str[i] - 'a']++;
	}
	for (int i = 0; i < 26; i++)
	{
		if (ct[i])
		{
			os << (char)(i + 'a') << " " << ct[i] << endl;
		}
	}

	return os;
}

void String::Delete(const unsigned int& start, const unsigned int& length)
{
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