#include <fstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	fstream fs;
	fs.open("test.txt", fstream::in|fstream::out|fstream::binary);
	while (true)
	{
		int i = rand()%10 + 1;
		fs << i;
	}
	fs.close();
	return 0;
}
