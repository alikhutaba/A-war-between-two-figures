#include "Point2D.h"



Point2D::Point2D()
	: x(0)
{
}


Point2D::~Point2D()
{
}


Point2D::Point2D(int x, int y)
{
	this->x = x;
	this->y = y;
}



int Point2D::GetX() const
{
	return x;
}


int Point2D::GetY() const
{
	return y;
}

void Point2D::setY(int y)
{
	this->y = y;
}

void Point2D::setX(int x)
{
	this->x = x;
}

void Point2D::SetIsValid(bool isvalid)
{
	this->isvalid = isvalid;
}
bool Point2D::GetIsValid()
{
	return this->isvalid;
}

bool Point2D::operator==(const Point2D& other)
{
	return x==other.x && y== other.y;
}
