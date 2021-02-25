#include<stdio.h>

inline void printLine()
{
	printf("LINE:%d\n",__LINE__);
}

//void test01(int a, int b)
//{
//	printf("this is int int !\n");
//}
//
//void test01(double a, double b)
//{
//	printf("this is double double !\n");
//}

void test02(const int a)
{
	printf("hello");
}

int main()
{
	int a = 10;
	int b = 3;
	//double c = 1/3;
	//printf("double c=%f\n",c);
	//test01(10,2);
	test02(21.1);
	printLine();
	
	return 0;
}
