#include <iostream>
#include <fstream>
#include <string>

struct Identificator {
	int code = 0;
	std::string str = "";
};

void putting(int, Identificator*, std::string);
void searching(int, int, Identificator*, std::string);
void show(int, Identificator*);

int main()
{
	int i = 0;
	Identificator *List = new Identificator[100];
	std::string buffer;
	std::ifstream hash;
	hash.open("E:\\hash.txt");

	if (!hash.is_open())
		std::cout << "File is not opened!\n";
	else
		std::cout << "All is good!\n";
	
	if (hash.is_open())
	{
		std::getline(hash, buffer);
		putting(i, List, buffer);
		i = i + 1;
		while (std::getline(hash, buffer))
		{
			putting(i, List, buffer);
			i = i + 1;
		}
	}

	show(i, List);

	hash.close();
	delete[] List;
}

void show(int i, Identificator *List)
{
	for (int n = 0; n < i; n++)
	{
		std::cout << "   Code:  " << List[n].code << "  -  " << List[n].str << std::endl;
	}
};

void putting(int i, Identificator *List, std::string buffer)
{
	char first_char, middle_char;
	int code2;
	bool condition;
	first_char = buffer[0];
	middle_char = buffer[buffer.length() / 2];
	code2 = (int)first_char + (int)middle_char;	
	searching(code2, i, List, buffer);
};

void searching(int code2, int length, Identificator *List, std::string buffer)
{
	for (int n = 0; n < length; n++)
	{
		if (List[n].code == code2)
		{
			code2 = code2 + 1;
			n = 0;
		}
	}
	List[length].code = code2;
	List[length].str = buffer;
};

