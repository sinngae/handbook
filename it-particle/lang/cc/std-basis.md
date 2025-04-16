# C++ STD
头文件名都不带.h后缀

## C library
C库头文件

### cctype
isalnum/isalpha/islower/isupper/isdigit/isxdigit/iscntrl/isgraph/isspace/isblank/isprint/ispunct/tolower/toupper

## input/output
```cpp
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
```

## multi-threading

## container
### array

## other