#pragma once
//..Forward Declarations..//
#include"All_forward_declarations.h"

class Controller_Class
{
	public:
		vector<Team*>teams{};
		vector<Player*>players{};
		vector<Home*>homes{};
		vector<Piece*>pieces{};
		Ludo_Cabaret*ludo_cabaret {};
		Board* board{};		
		Dice* dice{};
	public:
		Controller_Class();
}; 
