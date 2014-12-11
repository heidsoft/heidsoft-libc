/**
 * test keyword new
 * heidsoft@sina.com
 */

#include <iostream>

using namespace std;


std::string global_str;
int global_int;

int main(int argc, char const *argv[])
{
	int i ; //named, uninitialized ,int var

	int *pi = new int; // pi 指针 动态分配

	cout<< "i="<< i << endl;

	cout<<"*pi=" << *pi << endl;

	cout<<"*pi2=" << pi << endl;

	cout<<"*global_str=" << global_str << endl;
	cout<<"*global_str.size=" << global_str.size() << endl;

	cout<<"*global_int=" << global_int << endl;

	return 0;
}