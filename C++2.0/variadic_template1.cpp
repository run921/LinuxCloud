#include<iostream>
#include<bitset>
using namespace std;

void func(){}

template<typename T,typename... Type>
void func(const T& firstArg,const Type&... args)
{
	cout<<firstArg<<endl;
	func(args...);
}

int main()
{
	func(7.5,"hello",bitset<16>(377),42);
	return 0;
}

