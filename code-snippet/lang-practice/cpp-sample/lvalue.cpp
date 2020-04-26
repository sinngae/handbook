#include <iostream>
using namespace std;

struct Copyable {
	Copyable() {}
	Copyable(const Copyable &o) {
		cout << "Copied" << endl;
	}
};

Copyable ReturnRvalue() { return Copyable(); }
void AcceptVal(Copyable c) {
	cout << &c << endl;
}
void AcceptRef(const Copyable &) {}

int main() {
	AcceptVal(ReturnRvalue());
	AcceptRef(ReturnRvalue());
}
