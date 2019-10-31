#include <iostream>
#include <string.h>
#include <thread>

using namespace std;

void run()
{
	cout << "run ..." << endl;
}

void run1()
{
	cout << "run 1 ..." << endl;
}

int main(int argc, char *argv[])
{
	thread first(run);
	thread second(run1);
	cout << "run and run1 execute concurrently.." << endl;
	first.join();
	second.join();
	cout << "run end.. " << endl;
	return 0;
}
