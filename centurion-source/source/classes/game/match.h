#pragma once

#include "game.h"


	
class Match : public Game 
{
public:
	Match();

	virtual void Create() {};
	virtual void Run() {};

	~Match();
};


