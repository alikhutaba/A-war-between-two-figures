#pragma once

#include "BestNode.h"

class CompareBestNodes
{
public:
	CompareBestNodes();
	~CompareBestNodes();
	bool operator () (BestNode &b1, BestNode &b2)
	{
		if (b1.getF() > b2.getF())
			return true;
		else
			return false;
	}
};

