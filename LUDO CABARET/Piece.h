#pragma once
//...Forward Declarations...//
#include"All_forward_declarations.h"
class Piece
{
	public:
		//......RELATIONS.......//
		Home* home{};		// pointer to home of this piece.
		Controller_Class* controller{};
		//......PERSONAL ATTRIBUTES......//
		bool out_of_home{};		// Whether piece is in home(false) or outside(true).
	public:
		Piece();
		Piece(Home*,Controller_Class*);
		bool can_go_inside_home(int,int);
		void display(positionOnWindow, positionOnWindow);

};


