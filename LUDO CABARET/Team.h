#pragma once
#include<string>
//..Forward Declarations..//
#include"All_forward_declarations.h"
class Team
{
	public:
		//......RELATIONA......//
		Controller_Class* controller;
		vector<Player*>players;
		string name{};
		int arrived_pieces{};
		//......PERSONAL ATTRIBUTES......//
	public:
		Team(string,Controller_Class*);
};
