#include <iostream>

using namespace std;

int main() {
	auto var = {0, 1, 2, 3};
	//cout << decltype(var) << endl; // error, decltype没有左值右值，仅仅是静态编译出的标识
	//cout << var << endl;
	for (auto item : var) {
		cout << item << endl;
	}
	return 0;
}
