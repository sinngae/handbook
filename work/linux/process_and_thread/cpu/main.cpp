#include <iostream>
#include <thread>

using namespace std;

void run()
{
	long long value = 0;
	while (true)
	{
		value++;
		//cout << value << endl;
	}
}

int main(int argc, char *argv[])
{
	const size_t n = 10;
	thread *thd[n];
	for (int i = 0; i < n; i++)
	{
		thd[i] = new thread(run);
	}

	for (int i = 0; i < n; i++)
	{
		thd[i]->join();
	}

	return 0;
}
