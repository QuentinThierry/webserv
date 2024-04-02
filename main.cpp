#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	fstream s;

	s.open(argv[1], ios::in);
	if (!s.is_open())
	{
		cout << "Failed to open '" << argv[1] << "'" << endl;
		return 0;
	}
	string buffer;
	int i = 0;
	while (true)
	{
		getline(s, buffer);
		if (s.fail())
		{
			cout << "Failed to read '" << argv[1] << "'" << endl;
			return 1;
		}
		if (s.eof())
			return 0;
		cout << buffer << endl;
		s.clear();
	}
}