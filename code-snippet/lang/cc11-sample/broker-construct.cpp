#include <iostream>
#include <list>
#include <vector>
#include <deque>
using namespace std;

class Construct {
	template<class T> Construct(T first, T last) 
		: l(first, last) { throw 0; }
	list<int> l;
public:
	Construct(vector<short> &v) 
		try : Construct(v.begin(), v.end()) {}
		catch (...) { cout << "catch exception" << endl; }
	Construct(deque<int> &d) 
		: Construct(d.begin(), d.end()) {}
};

int main() {
	vector<short> v = {0, 1, 2};
	Construct cst(v);
	return 0;
}
