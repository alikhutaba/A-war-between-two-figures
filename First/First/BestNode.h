
#pragma once
#include "Point2D.h"
class BestNode
{
public:
	BestNode();
	BestNode(Point2D*, Point2D*);
	~BestNode();
private:
	Point2D* point;
	Point2D* target_point;
	BestNode* parent;
	int f;
	int g;
public:
	Point2D* GetPoint();
	Point2D* GetTargetPoint();
	void setTargetPoint(Point2D*);
public:
	int getF();
	int getG();
	void setG(int);
	void setF();
	void setPoint(int, int);
	int h();
	void move_up();
	void move_down();
	void move_right();
	void move_left();
	void setParent(BestNode*);
	BestNode* getParent();
	bool operator==(const BestNode&);
	BestNode::BestNode(const BestNode&);
};

