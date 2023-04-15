#include<SFML/Graphics.hpp>
#include<iostream>

#include<vector>
#include<array>
#include"Utility_Functions.h"
using namespace std;
#include"Dice.h"
#include"Home.h"
#include"Piece.h"
#include"Controller_Class.h"
#include"Ludo_Cabaret.h"
#include"Player.h"
//...Class/Header file to be defined here...//
#include"Board.h"


Board::Board(Controller_Class* controller) :number_of_homes(this->controller->ludo_cabaret->number_of_players), controller(controller)
{
	this->controller->board = this;				//Adding itself to controllers class.


	path = vector<vector<Piece*>>(13 * 6);
	this->dice = new Dice(controller);

	vector<positionOnWindow>homes_coordinates{ {215,500},{355,745},{635,745},{770,495},{630,250},{355,250} };
	vector<int>home_entering_indexes{ 77,12,25,38,51,64 };

	for (int i = 0; i < number_of_homes; i++)
	{
		this->homes.push_back(new Home(this->controller->players[i], homes_coordinates[i], home_entering_indexes[i], local_path_coordinates(i), rooms_coodinates(i), controller));
	}
	init_coordinate_of_path();
}
void Board::init_coordinate_of_path()
{
	coordinates_of_screen_for_path = vector<vector<int>>{ {145,357},{191,383},{240,409},{279,434},{325,462},{327,532},{282,563},{235,585},{194,611},{144,640},{102,668},{123,712},
		{153,758},{194,734},{243,706},{287,681},{330,656},{375,628},{435,664},{441,718},{442,771},{441,822},{440,875},{442,929},{490,931},{544,928},
		{544,878},{545,821},{546,768},{545,716},{546,665},{606,629},{651,654},{695,682},{743,706},{788,739},{829,760},{853,714},{884,667},{839,642},
		{791,613},{747,587},{704,560},{656,536},{661,460},{707,433},{748,410},{795,385},{838,354},{882,330},{860,283},{833,238},{789,267},{741,289},
		{698,316},{652,341},{606,367},{541,331},{544,278},{547,224},{547,172},{548,119},{544,65},{488,69},{438,64},{434,121},{437,173},{438,225},{442,279},
		{440,332},{374,367},{332,340},{290,315},{245,289},{199,263},{150,237},{121,282},{99,330} };
}
vector<positionOnWindow> Board::local_path_coordinates(int home_number)
{
	if (home_number == 0)
	{
		return { {167,311},{216,349},{262,363},{306,389},{350,418},{396,443} };
	}
	else if (home_number == 1)
	{
		return { {171,688},{216,661},{263,632},{307,605},{350,583},{395,555} };
	}
	else if (home_number == 2)
	{
		return { {492,876},{492,823},{492,767},{492,715},{492,664},{492,609} };
	}
	else if (home_number == 3)
	{
		return { {814,689},{767,661},{722,637},{677,609},{633,583},{587,555} };
	}
	else if (home_number == 4)
	{
		return { {811,307},{767,334},{723,362},{678,388},{634,415},{585,438} };
	}
	else if (home_number == 5)
	{
		return { {494,117},{494,171},{493,225},{493,278},{481,330},{491,382} };
	}
	return {};
}
vector<positionOnWindow> Board::rooms_coodinates(int home)
{
	if (home == 0)
		return { {240,500},{188,447},{186,499},{187,549} };
	else if (home == 1)
		return { {364,719},{296,742},{339,769},{383,792} };
	else if (home == 2)
		return { {620,721},{603,796},{645,768},{687,743} };
	else if (home == 3)
		return{ {746,497},{799,547},{797,498},{799,450} };
	else if (home == 4)
		return { {615,277},{686,256},{643,229},{601,205} };
	else if (home == 5)
		return { {365,278},{385,204},{341,227},{298,252} };
	else
		return{};

}
void Board::display()
{
	sf::RectangleShape ludoBoard(sf::Vector2f(1000, 1000));
	sf::Texture ludoBoard_texture;
	ludoBoard_texture.loadFromFile("ludo board.jpg");
	ludoBoard.setTexture(&ludoBoard_texture);
	this->controller->ludo_cabaret->ludo_window->draw(ludoBoard);

	for (int i = 0; i < path.size(); i++)
	{
		for (int j = 0; j < path[i].size(); j++)
		{
			path[i][j]->display({ this->coordinates_of_screen_for_path[i][0], this->coordinates_of_screen_for_path[i][1] + j * 5 }, { 45,45 });
		}
	}
	for (auto home : this->homes)
		home->display();
}
void Board::move_piece(Position source, Position destination)
{
	int len{};
	if (source.path_index < destination.path_index)
		len = destination.path_index - source.path_index + 1;
	else
		len = 78 - source.path_index + destination.path_index + 1;
	for (int i = 0; i < len - 1; i++)
	{
		int des = this->controller->ludo_cabaret->wrap_around_destination_index(source.path_index + i + 1);
		int prev_des = this->controller->ludo_cabaret->wrap_around_destination_index(source.path_index + i);
		int floor = this->controller->ludo_cabaret->turn->our_piece_index(prev_des);
		this->path[des].push_back(this->path[prev_des][floor]);
		this->path[prev_des].erase(this->path[prev_des].begin() + floor);				// make source empty by erasing
		this->controller->ludo_cabaret->display();
	}
}
void Board::go_inside_home(Position source, Position destination)
{
	Piece* temp = this->path[source.path_index][source.floor];

	this->move_piece(source, { temp->home->home_entering_index - 1, -1 });

	temp->home->local_path[0].push_back(temp);
	this->path[temp->home->home_entering_index - 1].erase(this->path[temp->home->home_entering_index - 1].begin());
	this->controller->ludo_cabaret->display();

	temp->home->move_inside_home({ 0,0 }, destination);
}
Board::~Board()
{
	delete this->dice;
	for (auto home : this->homes)
		delete home;
}