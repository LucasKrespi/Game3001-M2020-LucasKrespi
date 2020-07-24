#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "DisplayObject.h"

class Obstacle : public DisplayObject
{
public:
	Obstacle();
	~Obstacle();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	
};



#endif /* defined (__OBSTACLE__) */