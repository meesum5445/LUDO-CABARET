#include<iostream>

#include<vector>
#include<array>
#include"Utility_Functions.h"
using namespace std;
#include"Piece.h"
#include"Player.h"
#include"Controller_Class.h"
#include"Ludo_Cabaret.h"
//...Class/Header file  to be defined here...//
#include"Home.h"
Home::Home()
{

}
Home::Home(Player* player, positionOnWindow home_coord, int home_entering_index, vector<positionOnWindow>local_path_coord, vector<positionOnWindow>room_coord, Controller_Class* controller) :controller(controller), home_coordinate(home_coord), home_entering_index(home_entering_index), coordinates_of_screen_for_local_path(local_path_coord), coordinates_of_screen_for_rooms(room_coord), player(player)
{
	this->controller->homes.push_back(this);			//Adding itself to controllers class.
	player->home = this;								//Making relation with player of this home by adding itself into player. 

	local_path = vector<vector<Piece*>>(6);
	for (int i = 0; i < this->controller->ludo_cabaret->number_of_pieces_per_home; i++)
		this->pieces.push_back(new Piece(this, controller));				//Creating piece and pushing back them to pieces of home.
}
void Home::display()
{
	for (int i = 0; i < local_path.size(); i++)
	{
		for (int j = 0; j < local_path[i].size(); j++)
		{
			local_path[i][j]->display({ coordinates_of_screen_for_local_path[i].row, coordinates_of_screen_for_local_path[i].col + j * 5 }, { 45,45 });
		}
	}
	for (int i = 0; i < this->controller->ludo_cabaret->number_of_pieces_per_home; i++)
	{
		if (this->pieces[i]->out_of_home == false)
			this->pieces[i]->display(coordinates_of_screen_for_rooms[i], { 45,45 });
	}
}
void Home::move_inside_home(Position source, Position destination)
{
	for (int i = source.path_index; i < destination.path_index; i++)
	{
		this->local_path[i + 1].push_back(this->local_path[i][source.floor]);
		this->local_path[i].erase(this->local_path[i].begin() + source.floor);				// make source empty by erasing
		this->controller->ludo_cabaret->display();
	}

	if (destination.path_index == 5)
		this->player->arrived_pieces++;
}
Home::~Home()
{
	for (auto piece : this->pieces)
		delete piece;
}