//������
#include <iostream.h>


int main()
{
	Shape *ptrshape;                         //����������ָ��
	Circle circle1(20, 30, 30);              //����������Circle�Ķ���
	Rectangle rect1(40, 40, 80, 20);     //����������Rectangle�Ķ���
	circle1.dispName();
	circle1.dispArea();
	circle1.dispPos();

	ptrshape = &circle1;                     //������ָ��ָ��������Circle�Ķ���
	ptrshape->dispName();
	ptrshape->dispArea();
	ptrshape->dispPos();
	rect1.dispName();
	rect1.dispArea();
	rect1.dispPos();

	ptrshape = &rect1;               //������ָ��ָ��������Rectangle�Ķ���
	ptrshape->dispName();
	ptrshape->dispArea();
	ptrshape->dispPos();
	return 0;
}