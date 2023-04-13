#include<iostream>
#include<vector>
#include<array>
#include"Utility_Functions.h"
#include<SFML/Audio.hpp>
using namespace std;
#include"Piece.h"
#include"Team.h"
#include"Controller_Class.h"
#include"Dice.h"
#include"Board.h"
#include"Team.h"
#include"Home.h"
#include"Ludo_Cabaret.h"
//...Class/Header file  to be defined here...//
#include"Player.h"
Player::Player(Team *team,int starting_location,string name,Controller_Class* controller):team(team),starting_location(starting_location),name(name), controller(controller)
{
	this->controller->players.push_back(this);				//Adding itself to controllers class.
	this->team->players.push_back(this);	
}
void Player::roll()
{
	sf::SoundBuffer roll_sound_buffer;
	roll_sound_buffer.loadFromFile("dice_roll_sound.wav");
	sf::Sound dice_roll_sound;
	dice_roll_sound.setBuffer(roll_sound_buffer);
	roll_results.clear();
	int i = 0;
	do
	{

		if (i == 3)
		{
			roll_results.clear();
			break;
		}
		while (!this->controller->dice->is_dice_clicked());
		dice_roll_sound.play();
		this->controller->dice->animation_dice_rolling();
		this->roll_results.push_back(this->controller->dice->roll());
		i++;
		this->controller->ludo_cabaret->display();
	} while (roll_results[i - 1] == 6);
			
}
int Player::index_of_six()
{
	for (int result_index=0; result_index<this->roll_results.size(); result_index++)
	{
		if (this->roll_results[result_index] == 6)
			return result_index;
	}
	return -1;
}
int Player::our_piece_index(int source)
{
	for (int i=0; i < this->controller->board->path[source].size(); i++)
	{
		if (this->controller->board->path[source][i]->home->player->team == this->team)
		{
			return i;
		}
	}
	return -1;
}
int Player::our_piece_index_local_path(int source)
{
	for (int i = 0; i < this->home->local_path[source].size(); i++)
	{
		if (this->home->local_path[source][i]->home->player->team == this->team)
		{
			return i;
		}
	}
	return -1;
}
bool Player::valid_destination(int destination)
{
	return !(this->controller->board->path[destination].size() > 1 && this->controller->board->path[destination][0]->home->player->team != this->team);
}
int Player::make_choice()
{
	positionOnWindow temp = get_row_col_by_click(*this->controller->ludo_cabaret->ludo_window);
	int zero_error = 20;
	int i = 0;
	for (auto index_coordinates : this->controller->board->coordinates_of_screen_for_path)
	{
		if (temp.col <= index_coordinates[1] + zero_error && temp.col >= index_coordinates[1] - zero_error && temp.row <= index_coordinates[0] + zero_error && temp.row >= index_coordinates[0] - zero_error)
			return i;
		i++;
	}
	i = -1;
	for (auto index_coordinates : this->home->coordinates_of_screen_for_local_path)
	{
		if (temp.col <= index_coordinates.col + zero_error && temp.col >= index_coordinates.col - zero_error && temp.row <= index_coordinates.row + zero_error && temp.row >= index_coordinates.row - zero_error)
			return i;
		if (--i == -6)break;
	}
	zero_error = 50;
	if (temp.col <= this->home->home_coordinate.col + zero_error && temp.col >= this->home->home_coordinate.col - zero_error && temp.row <= this->home->home_coordinate.row + zero_error && temp.row >= this->home->home_coordinate.row - zero_error)
	{
		return -6;
	}
	return -7;
}
int Player::select_dice_number()
{
	positionOnWindow pos = get_row_col_by_click(*this->controller->ludo_cabaret->ludo_window);
	for (int i = 0; i < this->roll_results.size(); i++)
	{
		if (pos.row >= 300 && pos.row <= 400 && pos.col >= 1080 + i * 110 && pos.col <= 1080 + i * 110 + 100)		
			return i;
					
	}
	return -1;
}
void Player::display_roll_results()
{
	sf::Texture dice_result_texture;
	int n = 0;
	for (auto dice_result : this->roll_results)
	{
		switch (dice_result)
		{
		case 1:
			dice_result_texture.loadFromFile("dice_1.png");
			break;
		case 2:
			dice_result_texture.loadFromFile("dice_2.png");
			break;
		case 3:
			dice_result_texture.loadFromFile("dice_3.png");
			break;
		case 4:
			dice_result_texture.loadFromFile("dice_4.png");
			break;
		case 5:
			dice_result_texture.loadFromFile("dice_5.png");
			break;
		case 6:
			dice_result_texture.loadFromFile("dice_6.png");
			break;
		}
		int gap = n * 10;
		sf::RectangleShape dice_result(sf::Vector2f(100, 100));
		dice_result.setTexture(&dice_result_texture);
		dice_result.setPosition(1080 + n * 110, 300);
		this->controller->ludo_cabaret->ludo_window->draw(dice_result);
		n++;
	}
}
void Player::take_out_piece()
{

	for (auto piece : this->home->pieces)
	{
		if (!piece->out_of_home)
		{
			this->controller->board->path[this->starting_location].push_back(piece);
			piece->out_of_home = true;
			return;
		}
			
	}	
	

}
void Player::kill(Position source,Position destination)
{
	if (destination.path_index % 13==0 || this->controller->ludo_cabaret->wrap_around_destination_index(destination.path_index+4)%13==0)
		return;

	if (this->controller->board->path[destination.path_index].size() == 1 && this->controller->board->path[destination.path_index][0]->home->player->team != this->team)
	{
		this->controller->board->path[destination.path_index][0]->out_of_home = false;
		this->controller->board->path[destination.path_index].clear();
		this->controller->board->path[source.path_index][source.floor]->home->player->killed_enemy = true;
	}

}
bool Player::any_valid_move()
{
	for (int i = 0; i < this->controller->board->path.size();i++)
	{
		int floor = this->our_piece_index(i);
		if (floor != -1)
		{
			for (auto j : this->roll_results)
			{
				int destination =this->controller->ludo_cabaret->wrap_around_destination_index(i+j);
				if (this->valid_destination(destination))
				{
					return true;
				}
			}
		}
	}


	return false;
	
}
bool Player::is_any_valid_move_in_local_path()
{
	for (int i = 0; i<this->home->local_path.size();i++)
	{
		int floor = this->our_piece_index_local_path(i);
		if (floor != -1)
		{
			for (auto result : this->roll_results)
			{
				if (i + result < 6)
					return true;
			}
		}
	}
	return false;
}
bool Player::any_piece_inside_home()
{
	for (auto piece : this->home->pieces)
	{
		if (piece->out_of_home == false)		
			return true;
					
	}
	return false;
}