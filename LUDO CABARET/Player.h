#pragma once
#include<string>
//...Forward Declarations...//
#include"All_forward_declarations.h"
class Player
{
public:
	//......RELATIONS.......//
	Controller_Class* controller{};			//pointer to controller class ,so Player can be linked to controller.
	Team* team{};
	Home* home{};
	//......PERSONAL ATTRIBUTES......//
	string name{};
	vector<int>roll_results{};
	int starting_location{};
	bool killed_enemy{};
	int arrived_pieces{};
public:
	Player(Team*, int, string, Controller_Class*);
	void roll();
	int index_of_six();
	int our_piece_index(int);
	int our_piece_index_local_path(int);
	bool valid_destination(int);
	void take_out_piece();
	int make_choice();
	int select_dice_number();
	//bool is_any_piece_on_local_path();
	void kill(Position, Position);
	void display_roll_results();
	bool any_valid_move();
	bool is_any_valid_move_in_local_path();
	bool is_any_valid_move_to_go_inside_home();
	bool any_piece_inside_home();

};
