#include <iostream>
using namespace std;

#define LOG(...) { \
    fprintf(stderr, "%s: Line %d: \t", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
}

void func(...) {
	int x = 3;
	LOG("x = %d", x);
	//cout << "va_args: " << __VA_ARGS__ << endl; // 仅用于宏定义
}

int main() {
	cout << "standard clib: " << __STDC_HOSTED__ << endl;
	cout << "standard C: " << __STDC__ << endl;
	//cout << "C standard version: " << __STDC_VERSION__ << endl;
	cout << "ISO/IEC " << __STDC_ISO_10646__ << endl;
	cout << "func: " << __func__ << endl;
	//cout << "date: " << __date__ << endl;

	func("test: %d", 10);
}
