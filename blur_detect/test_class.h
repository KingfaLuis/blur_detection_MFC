#pragma once
#include <iostream.h>
#include <string.h>
class Shape      //������������Ϊ����
{
protected:
	double x;
	double y;
public:
	Shape() {}
	Shape(double xx, double yy);
	~Shape() {}
	virtual void dispName() = 0;           //�������麯��
	virtual void dispArea() = 0;            //�������麯��
	virtual void dispPos() = 0;              //�������麯��
};
class Circle :public Shape             //����������Circle
{
private:
	double radius;
	double area;
public:
	Circle(double cx = 1, double cy = 1, double rad = 1);
	~Circle();
	void dispName();
	void dispArea();
	void dispPos();
};

class Rectangle :public Shape       //����������Rectangle
{
private:
	double length;
	double width;
	double area;
public:
	Rectangle(double cx = 1, double cy = 1, double len = 2, double wid = 1);
	~Rectangle();
	void dispName();
	void dispArea();
	void dispPos();
};


