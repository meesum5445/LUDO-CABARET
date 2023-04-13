#pragma once
#include <SFML/Graphics.hpp>
#include<stack>
#include<queue>
#include<string>
//..Forward Declarations..//
#include"All_forward_declarations.h"
class Ludo_Cabaret
{
	public:
		//......RELATIONS.......//
		Controller_Class *controller{};			//Pointer to controller class ,so Ludo_Cabaret can be linked to controller.
		vector<Team*>teams{};
		vector<Player*>players{};				// Vector containing list of players(pointers).
		Board* board{};							// pointer to Ludo Board used in game.
		//......PERSONAL ATTRIBUTES......//
		sf::RenderWindow* ludo_window{};
		Player* turn{};
		int number_of_players{};
		int number_of_teams{};
		int number_of_pieces_per_home{};
		queue<Player*>leadership_board{};
		queue<Team*>team_leadership_board{};

	public:
		string names_of_players(int);
		string names_of_teams(int);
		int wrap_around_destination_index(int destination);
		void turn_change();
		Ludo_Cabaret();
		void initialize_everything();
		void print_turn();
		void display();
		void nenu_board0();
		void menu_board1();
		void menu_board2();
		void menu_board3();
		void menu_leadership_board();
		void draw_background();
		void play();
		void making_leadership_board();
		void start();
		bool game_end();
		~Ludo_Cabaret();
};
