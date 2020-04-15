#include <iostream>
using namespace std;
void Throw() { throw 1; }
void NoBlockThrow() { Throw(); }
void BlockThrow() noexcept { Throw(); }

struct A {
	~A() { throw 1; }
};
struct B {
	~B() noexcept(false) { throw 2; }
};
struct C {
	B b;
};
int funA() { A a; }
int funB() { B b; }
int funC() { C c; }

int main() {
	try {
		Throw();
	} catch (...) {
		cout << "found throw..." << endl;
	}

	try {
		NoBlockThrow();
	} catch (...) {
		cout << "throw is not blocked..." << endl; // throw is not blocked
	}

	/*try {
		BlockThrow(); // terminate called after throwing an instance of 'int'\nAborted
	} catch (...) {
		cout << "find throw 1..." << endl;
	}*/

	try {
		funB();
	} catch (...) {
		cout << "catch funB..." << endl;
	}

	try {
		funC();
	} catch (...) {
		cout << "catch funC..." << endl;
	}

	/*try {
		funA(); // terminate called after throwing an instance of 'int'\nAborted
	} catch (...) {
		cout << "catch funA..." << endl; 
	}*/

}
