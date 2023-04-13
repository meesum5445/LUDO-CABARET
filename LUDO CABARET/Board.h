#pragma once
//..Forward Declarations../
#include"All_forward_declarations.h"

class Board
{
	public:
		//......RELATIONS.......//
		Controller_Class* controller{};			//pointer to controller class ,so board can be linked to controller.
		vector<Home*>homes{};
		Dice* dice{};
		//......PERSONAL ATTRIBUTES......//
		int number_of_homes{};
		vector<vector<Piece*>>path{};
		vector<vector<int>>coordinates_of_screen_for_path{};
		//..................;



	public:
		Board();
		Board(Controller_Class*);
		void display();
		void init_coordinate_of_path();
		void move_piece(Position ,Position);
		vector<positionOnWindow> local_path_coordinates(int);
		vector<positionOnWindow>rooms_coodinates(int home);
		~Board();

};