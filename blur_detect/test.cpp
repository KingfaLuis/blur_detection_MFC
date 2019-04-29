//主程序
#include <iostream.h>


int main()
{
	Shape *ptrshape;                         //声明抽象类指针
	Circle circle1(20, 30, 30);              //声明派生类Circle的对象
	Rectangle rect1(40, 40, 80, 20);     //声明派生类Rectangle的对象
	circle1.dispName();
	circle1.dispArea();
	circle1.dispPos();

	ptrshape = &circle1;                     //抽象类指针指向派生类Circle的对象
	ptrshape->dispName();
	ptrshape->dispArea();
	ptrshape->dispPos();
	rect1.dispName();
	rect1.dispArea();
	rect1.dispPos();

	ptrshape = &rect1;               //抽象类指针指向派生类Rectangle的对象
	ptrshape->dispName();
	ptrshape->dispArea();
	ptrshape->dispPos();
	return 0;
}