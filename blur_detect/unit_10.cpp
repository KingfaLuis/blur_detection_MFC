//EXAMPLE10_3.CPP
//Դ����ʼ
#include <iostream.cpp>
using namespace std;
int main()
{
	int value = 100;
	int * iptr = &value;

	//��ʾvalueֵ
	cout << "value = " << value << endl;
	//��ʾvalue��ŵ�ַ(ָ��)
	cout << " & value = " << &value << endl;
	//��ʾiptrֵ��������value�ĵ�ַ��
	cout << "iptr = " << iptr << endl;
	//��ʾiptr�����ַ
	cout << " & iptr = " << &iptr << endl;
	return 0;
}
