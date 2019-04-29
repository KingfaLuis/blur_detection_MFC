#pragma once
#include <iostream.h>
#include <string.h>
class Shape      //声明抽象类作为基类
{
protected:
	double x;
	double y;
public:
	Shape() {}
	Shape(double xx, double yy);
	~Shape() {}
	virtual void dispName() = 0;           //声明纯虚函数
	virtual void dispArea() = 0;            //声明纯虚函数
	virtual void dispPos() = 0;              //声明纯虚函数
};
class Circle :public Shape             //声明派生类Circle
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

class Rectangle :public Shape       //声明派生类Rectangle
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


