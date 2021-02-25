#include"gao.h"

using namespace std;
A::A()
{
	cout<<"空构造函数调用"<<endl;
}

A::A(int a):m_a(a)
{
	cout<<"有参构造函数调用"<<endl;
}

A::~A()
{
	cout<<"析构函数调用"<<endl;
}
