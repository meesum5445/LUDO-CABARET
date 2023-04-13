#pragma once
//...Forward Declarations...//
#include"All_forward_declarations.h"

class Home
{
	public:
		//......RELATIONS.......//
		Controller_Class* controller{};		//pointer to controller class ,so Home can be linked to controller.
		Player* player{};					//owner of home / player who is in charge of this home and its pieces.
		vector<Piece*>pieces{};				// vector list of pieces in home.
		//......PERSONAL ATTRIBUTES......//
		positionOnWindow home_coordinate{};
		vector<positionOnWindow>coordinates_of_screen_for_rooms{};
		//.............................................................
		int home_entering_index{};
		vector<vector<Piece*>>local_path{};
		vector<positionOnWindow>coordinates_of_screen_for_local_path{};
	public:
		Home();
		void display();
		void move_inside_home(Position,Position);
		Home(Player*, positionOnWindow,int,vector<positionOnWindow>, vector<positionOnWindow>,Controller_Class*);
		~Home();
		
};
