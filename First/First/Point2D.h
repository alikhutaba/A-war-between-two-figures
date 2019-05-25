#pragma once
class Point2D
{
public:
	Point2D();
	~Point2D();
private:
	int x;
	int y;
	bool isvalid;
public:
	Point2D(int x, int y);
	int GetX() const;
	int GetY() const;
	void setY(int);
	void SetIsValid(bool isvalid);
	bool GetIsValid();
	void setX(int);
	bool operator==(const Point2D& other);
};

