#include<iostream>
#include<vector>
#include<array>
#include<string>
#include<SFML/Audio.hpp>
#include"Utility_Functions.h"
using namespace std;
#include"Controller_Class.h"
#include"Team.h"
#include"Player.h"
#include"Board.h"
#include"Home.h"
#include"Piece.h"
#include"Dice.h"
//...Class/Header file  to be defined here...//
#include"Ludo_Cabaret.h"
Ludo_Cabaret::Ludo_Cabaret()
{
	this->ludo_window = new sf::RenderWindow(sf::VideoMode(1500, 1000), "LUDO", sf::Style::Close);
}
string Ludo_Cabaret::names_of_players(int n)
{
	if (n == 0)return "RED UNCLE SAINT";
	else if (n == 1)return "DOLPHIN THE SMASHER";
	else if (n == 2)return "ORANGE THE SARFRAZ LEAGUE";
	else if (n == 3)return "GREEN THE HUSTLER";
	else if (n == 4)return "YELLOW NOT YOUR DIRTY FELLOW";
	else if (n == 5)return "PURPLE THE MISS CONQUER";
	else return "name out of range";
}
string  Ludo_Cabaret::names_of_teams(int n)
{
	if (n == 0)return "MADAGASCAR";
	else if (n == 1)return "GODZILLA";
	else if (n == 2)return "JUMANJI";
	else return "team name out of range";
}
void Ludo_Cabaret::initialize_everything()
{

	//.....................................
	controller = new Controller_Class();									//Creating a Controller Class
	controller->ludo_cabaret = this;										//Adding itself to Controller Class
	//....................................

	

	for (int team = 0; team < number_of_teams; team++)
	{
		this->teams.push_back(new Team(names_of_teams(team),controller));						//Creating number_of_teams objects and pushing them into vector of teams.
	}

	for (int player = 0, team = 0; player < number_of_players; player++, team = ((++team) % number_of_teams))
	{
		players.push_back(new Player(this->teams[team], player * 13,names_of_players(player), controller));
	}
	//...................................
	this->board = new Board(controller);
	srand(time(0));
	this->turn = this->players[rand() % number_of_players];		// Generating random turn 
	this->turn = this->players[1];
}
int Ludo_Cabaret::wrap_around_destination_index(int destination)
{
	return destination % (13 * 6);
}
void Ludo_Cabaret::play()
{
	while (true)
	{
		
		this->controller->dice->dice_last_result = 6;
		if (turn->arrived_pieces <number_of_pieces_per_home && turn->team->arrived_pieces<number_of_pieces_per_home*turn->team->players.size())
		{
			this->display();
			turn->roll();
			while (turn->roll_results.size() > 0)
			{
				int six_index = turn->index_of_six();
				if (turn->any_valid_move() || (six_index != -1 && turn->any_piece_inside_home()) || turn->is_any_valid_move_in_local_path())
				{
					int choice = turn->make_choice();
					if (choice == -6 && (six_index != -1 && turn->any_piece_inside_home()))
					{
						turn->take_out_piece();
						turn->roll_results.erase(turn->roll_results.begin() + six_index);
					}
					else if (choice >= 0 && turn->any_valid_move())
					{
						int floor = turn->our_piece_index(choice);
						if (floor != -1)
						{
							int dice_index = turn->select_dice_number();
							if (dice_index != -1)
							{
								int destination = wrap_around_destination_index(choice + turn->roll_results[dice_index]);
								if (this->board->path[choice][floor]->can_go_inside_home(choice, turn->roll_results[dice_index]))
								{
									turn->home->local_path[choice + turn->roll_results[dice_index] - turn->home->home_entering_index].push_back(this->board->path[choice][floor]);
									this->board->path[choice].erase(this->board->path[choice].begin() + floor);
									if (choice + turn->roll_results[dice_index] - turn->home->home_entering_index == 5) turn->arrived_pieces++;
									turn->roll_results.erase(turn->roll_results.begin() + dice_index);
								}
								else if (turn->valid_destination(destination))
								{
									turn->kill({ choice,floor }, { destination,-1 });
									this->board->move_piece({ choice,floor }, { destination,-1 });
									turn->roll_results.erase(turn->roll_results.begin() + dice_index);
								}
							}


						}

					}
					else if (choice > -6 && choice < 0 && turn->is_any_valid_move_in_local_path())
					{
						choice = -choice - 1;
						int floor = turn->our_piece_index_local_path(choice);
						if (floor != -1)
						{
							int dice_index = turn->select_dice_number();
							if (dice_index != -1)
							{
								int destination = choice + turn->roll_results[dice_index];
								if (destination < 6)
								{
									turn->home->move_inside_home({ choice,floor }, { destination,-1 });
									turn->roll_results.erase(turn->roll_results.begin() + dice_index);
								}
							}
						}
					}
				}
				else
				{
					turn->roll_results.clear();
					break;
				}
				this->display();
			}
			making_leadership_board();
		}
		if (game_end())break;
		turn_change();
	}

}
void Ludo_Cabaret::draw_background()
{
	sf::Texture background_texture;
	background_texture.loadFromFile("background.jpeg");
	sf::RectangleShape background(sf::Vector2f(1500, 1000));
	background.setTexture(&background_texture);
	this->ludo_window->draw(background);
}
void Ludo_Cabaret::display()
{
	draw_background();
	//....................................
	this->board->dice->display_dice(this->board->dice->dice_last_result, 0,{160,160});
	//....................................
	turn->display_roll_results();
	//....................................
	this->print_turn();
	//....................................
	this->board->display();
	//..................................
	this->ludo_window->display();

}
void Ludo_Cabaret::turn_change()
{
	if (turn==this->players.back())
	{
		turn = this->players.front();
	}
	else
	{
		int i = 0;
		while (turn != players[i])  i++;
		i++;
		turn = players[i];
	}
}
void Ludo_Cabaret::print_turn()
{
	sf::Font font;
	font.loadFromFile("gooddog.ttf");
	sf::Text turn_text;
	turn_text.setFont(font);
	turn_text.setCharacterSize(100);
	turn_text.setString("TURN : ");
	turn_text.setPosition(sf::Vector2f(1060,50));
	this->ludo_window->draw(turn_text);
	//.........................................
	sf::Texture turn_box_texture;
	sf::RectangleShape turn_box(sf::Vector2f(200, 200));
	turn_box.setPosition(sf::Vector2f(1280, 20));
	if (turn == this->players[0])
	{
		turn_box_texture.loadFromFile("red piece.png");
	}
	else if (turn == this->players[1])
	{
		turn_box_texture.loadFromFile("blue piece.png");
	}
	else if (turn == this->players[2])
	{
		turn_box_texture.loadFromFile("orange piece.png");
	}
	else if (turn == this->players[3])
	{
		turn_box_texture.loadFromFile("green piece.png");
	}
	else if (turn == this->players[4])
	{
		turn_box_texture.loadFromFile("yellow piece.png");
	}
	else if (turn == this->players[5])
	{
		turn_box_texture.loadFromFile("purple piece.png");
	}
	turn_box.setTexture(&turn_box_texture);
	this->ludo_window->draw(turn_box);
}
void Ludo_Cabaret::making_leadership_board()
{
	if (turn->arrived_pieces == number_of_pieces_per_home)
	{
		this->leadership_board.push(turn);
	}
	turn->arrived_pieces = 0;
	for (auto player : turn->team->players)
		turn->team->arrived_pieces += player->arrived_pieces;

	if(turn->team->arrived_pieces== number_of_pieces_per_home*turn->team->players.size())
	{
		this->team_leadership_board.push(turn->team);
	}
}
void Ludo_Cabaret::nenu_board0()
{
	draw_background();

	sf::Texture menu_board_texture;
	menu_board_texture.loadFromFile("menu_board.png");
	sf::RectangleShape menu_board(sf::Vector2f(1500, 1000));
	menu_board.setTexture(&menu_board_texture);
	this->ludo_window->draw(menu_board);
	//......................................
	sf::Font text_font;
	text_font.loadFromFile("gooddog.ttf");
	sf::Text text;
	text.setFont(text_font);
	text.setCharacterSize(100);
	//....................................
	text.setString("LUDO CABARET");
	text.setPosition(sf::Vector2f(520, 600));
	this->ludo_window->draw(text);
	//....................................
	text.setString("BY SOBIA AND MEESUM");
	text.setPosition(sf::Vector2f(400, 750));
	this->ludo_window->draw(text);
	this->ludo_window->display();
	//.....................................
	auto pos = get_row_col_by_click(*this->ludo_window);
}
void Ludo_Cabaret::menu_board1()
{
	draw_background();

	sf::Texture menu_board_texture;
	menu_board_texture.loadFromFile("menu_board.png");
	sf::RectangleShape menu_board(sf::Vector2f(1500, 1000));
	menu_board.setTexture(&menu_board_texture);
	this->ludo_window->draw(menu_board);
	//......................................
	sf::Font text_font;
	text_font.loadFromFile("gooddog.ttf");
	sf::Text text;
	text.setFont(text_font);
	text.setCharacterSize(100);
	//....................................
	text.setString("SELECT PLAYERS");
	text.setPosition(sf::Vector2f(480, 600));
	this->ludo_window->draw(text);
	//............................
	text.setCharacterSize(150);
	//.......................
	text.setString("4");
	text.setPosition(sf::Vector2f(730, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("3");
	text.setPosition(sf::Vector2f(600, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("5");
	text.setPosition(sf::Vector2f(860, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("2");
	text.setPosition(sf::Vector2f(470, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("6");
	text.setPosition(sf::Vector2f(990, 700));
	this->ludo_window->draw(text);
	//.................................
	this->ludo_window->display();
	//...............................................
	do
	{
		auto choice = get_row_col_by_click(*this->ludo_window);
		//..............................................
		int error = 30;
		if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 500 - error && choice.col < 500 + error)
			this->number_of_players = 2;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 623 - error && choice.col < 623 + error)
			this->number_of_players = 3;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 759 - error && choice.col < 759 + error)
			this->number_of_players = 4;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 893 - error && choice.col < 893 + error)
			this->number_of_players = 5;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 1025 - error && choice.col < 1025 + error)
			this->number_of_players = 6;
	} while (!(this->number_of_players >= 2 && this->number_of_players <= 6));
	
}
void Ludo_Cabaret::menu_board2()
{
	draw_background();

	sf::Texture menu_board_texture;
	menu_board_texture.loadFromFile("menu_board.png");
	sf::RectangleShape menu_board(sf::Vector2f(1500, 1000));
	menu_board.setTexture(&menu_board_texture);
	this->ludo_window->draw(menu_board);
	//......................................
	sf::Font text_font;
	text_font.loadFromFile("gooddog.ttf");
	sf::Text text;
	text.setFont(text_font);
	text.setCharacterSize(100);
	//....................................
	text.setString("SELECT TEAMS");
	text.setPosition(sf::Vector2f(480, 600));
	this->ludo_window->draw(text);

	int error = 60;
	//....................................
	if (this->number_of_players == 4)
	{
		text.setString("SOLO");
		text.setPosition(sf::Vector2f(460, 750));
		this->ludo_window->draw(text);
		//.................................
		text.setString("PAIR");
		text.setPosition(sf::Vector2f(860, 750));
		this->ludo_window->draw(text);
		//.................................
		this->ludo_window->display();
		//................................
		do
		{
			auto pos = get_row_col_by_click(*this->ludo_window);
			//..............................
			if (pos.row > 813 - error && pos.row < 813 + error && pos.col>540 - error && pos.col < 540 + error)
				this->number_of_teams = 4;
			else if (pos.row > 813 - error && pos.row < 813 + error && pos.col>942 - error && pos.col < 942 + error)
				this->number_of_teams = 2;
		} while (!(this->number_of_teams == 4 || this->number_of_teams == 2));
	}
	else if (this->number_of_players == 6)
	{
		text.setString("SOLO");
		text.setPosition(sf::Vector2f(380, 750));
		this->ludo_window->draw(text);
		//.................................
		text.setString("PAIR");
		text.setPosition(sf::Vector2f(665, 750));
		this->ludo_window->draw(text);
		//.................................
		text.setString("TRIO");
		text.setPosition(sf::Vector2f(930, 750));
		this->ludo_window->draw(text);
		//.................................
		this->ludo_window->display();
		//................................
		do
		{
			auto pos = get_row_col_by_click(*this->ludo_window);
			//..............................
			if (pos.row > 813 - error && pos.row < 813 + error && pos.col>460 - error && pos.col < 460 + error)
				this->number_of_teams = 6;
			else if (pos.row > 813 - error && pos.row < 813 + error && pos.col>745 - error && pos.col < 745 + error)
				this->number_of_teams = 3;
			else if (pos.row > 813 - error && pos.row < 813 + error && pos.col>1010 - error && pos.col < 1010 + error)
				this->number_of_teams = 2;
		} while (!(this->number_of_teams == 6 || this->number_of_teams == 2 || this->number_of_teams == 3));
	}
	else
		this->number_of_teams = this->number_of_players;
	//.................................
}
void Ludo_Cabaret::menu_board3()
{
	draw_background();

	sf::Texture menu_board_texture;
	menu_board_texture.loadFromFile("menu_board.png");
	sf::RectangleShape menu_board(sf::Vector2f(1500, 1000));
	menu_board.setTexture(&menu_board_texture);
	this->ludo_window->draw(menu_board);
	//......................................
	sf::Font text_font;
	text_font.loadFromFile("gooddog.ttf");
	sf::Text text;
	text.setFont(text_font);
	text.setCharacterSize(100);
	//....................................
	text.setString("SELECT PIECES");
	text.setPosition(sf::Vector2f(480, 600));
	this->ludo_window->draw(text);
	//............................
	text.setCharacterSize(150);
	//............................
	text.setString("2");
	text.setPosition(sf::Vector2f(630, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("3");
	text.setPosition(sf::Vector2f(780, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("1");
	text.setPosition(sf::Vector2f(500, 700));
	this->ludo_window->draw(text);
	//............................
	text.setString("4");
	text.setPosition(sf::Vector2f(900, 700));
	this->ludo_window->draw(text);
	//.................................
	this->ludo_window->display();
	//...............................................
	do
	{
		auto choice = get_row_col_by_click(*this->ludo_window);
		//..............................................
		int error = 30;
		if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 514 - error && choice.col < 514 + error)
			this->number_of_pieces_per_home = 1;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 666 - error && choice.col < 666 + error)
			this->number_of_pieces_per_home = 2;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 805 - error && choice.col < 805 + error)
			this->number_of_pieces_per_home = 3;
		else if (choice.row > 813 - error && choice.row < 813 + error && choice.col > 925 - error && choice.col < 925 + error)
			this->number_of_pieces_per_home = 4;
	} while (!(this->number_of_pieces_per_home>=1 && this->number_of_pieces_per_home<=4));

}
void Ludo_Cabaret::menu_leadership_board()
{
	draw_background();
	//....................................
	sf::Font text_font;
	text_font.loadFromFile("gooddog.ttf");
	sf::Text text;
	text.setFont(text_font);
	//....................................
	text.setCharacterSize(150);
	text.setString("LEADERSHIP BOARD");
	text.setPosition(sf::Vector2f(300, 30));
	this->ludo_window->draw(text);
	//............................
	text.setCharacterSize(80);
	if (number_of_players == number_of_teams)
	{
		for (int i = 0; i < leadership_board.size(); i++)
		{
			string pre_name;
			if (i == 0)pre_name = "st";
			else if (i == 1)pre_name = "nd";
			else if (i == 2)pre_name = "rd";
			else pre_name = "th";
			auto player = leadership_board.front();
			text.setString(to_string(i + 1) + pre_name + "     " + player->name);
			text.setPosition(sf::Vector2f(150, 210 + 130 * i));
			player->home->pieces[0]->display({ i * 125 + 270,1200}, {60,60});
			this->ludo_window->draw(text);
			leadership_board.pop();
		}
	}
	else
	{
		for (int i = 0; i < team_leadership_board.size(); i++)
		{
			string pre_name;
			if (i == 0)pre_name = "st";
			else if (i == 1)pre_name = "nd";
			else if (i == 2)pre_name = "rd";
			else pre_name = "th";
			auto team = team_leadership_board.front();
			text.setString(to_string(i + 1) + pre_name + "     " + team->name);
			text.setPosition(sf::Vector2f(150, 210 + 130 * i));
			this->ludo_window->draw(text);
			for (int j=0;j<team->players.size();j++)
				team->players[j]->home->pieces[0]->display({i*125+270,j*120+800}, {60,60});
			team_leadership_board.pop();
		}
	}
	
	//............................
	this->ludo_window->display();
	auto pos = get_row_col_by_click(*this->ludo_window);
}
void Ludo_Cabaret::start()
{
	nenu_board0();
	menu_board1();
	menu_board2();
	menu_board3();
	initialize_everything();
	play();
	menu_leadership_board();
}
bool Ludo_Cabaret::game_end()
{
	if (leadership_board.size() >= number_of_players - 1)
	{
		for (auto player : this->players)
		{
			if (player->arrived_pieces != number_of_pieces_per_home)
				leadership_board.push(player);
		}
		return true;
	}
	else if (team_leadership_board.size() >= number_of_teams - 1)
	{
		for (auto team : this->teams)
		{
			if (team->arrived_pieces != team->players.size() * number_of_pieces_per_home)
				team_leadership_board.push(team);
		}
		return true;
	}
	return false;
}
Ludo_Cabaret::~Ludo_Cabaret()
{
	delete this->controller;
	delete this->ludo_window;
	for (auto team: this->teams)
		delete team;
	for (auto player : this->players)
		delete player;	
	delete board;
}