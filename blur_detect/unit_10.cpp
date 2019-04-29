//EXAMPLE10_3.CPP
//源程序开始
#include <iostream.cpp>
using namespace std;
int main()
{
	int value = 100;
	int * iptr = &value;

	//显示value值
	cout << "value = " << value << endl;
	//显示value存放地址(指针)
	cout << " & value = " << &value << endl;
	//显示iptr值（保存了value的地址）
	cout << "iptr = " << iptr << endl;
	//显示iptr自身地址
	cout << " & iptr = " << &iptr << endl;
	return 0;
}
