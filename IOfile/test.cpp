#include<fstream>
#include<iostream>
using namespace std;
int main()
{
	char data[100];
	
	//以写模式打开文件
	ofstream outfile;
	outfile.open("hello.txt");
	
	cout<<"Writing to the file"<<endl;
	cout<<"Enter your name:"<<endl;
	cin.getline(data,100);
	
	outfile<<data<<endl;
	
	cout<<"Enter your age:";
	cin>>data;
	cin.ignore();
	
	outfile<<data<<endl;

	//关闭打开的文件
	outfile.close();
	
	//以读模式打开文件
	ifstream infile;
	infile.open("hello.txt");
	
	cout<<"Reading from the file"<<endl;
	//在屏幕上写数据
	infile>>data;
	cout<<data<<endl;
	
	infile>>data;
	cout<<data<<endl;
	
	//关闭打开文件
	infile.close();
	
	return 0;
	
}
