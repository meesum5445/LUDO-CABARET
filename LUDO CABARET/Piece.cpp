#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<array>
#include"Utility_Functions.h"
using namespace std;
#include"Home.h"
#include"Controller_Class.h"
#include"Ludo_Cabaret.h"
#include"Player.h"
//...Class/Header file  to be defined here...//
#include"Piece.h"
Piece::Piece()
{

}
Piece::Piece(Home*home,Controller_Class *controller) :home(home),controller(controller)
{
	this->controller->pieces.push_back(this);					//Adding itself to controllers class.
}
void Piece::display(positionOnWindow P, positionOnWindow size)
{
	sf::RectangleShape pictures(sf::Vector2f(size.col, size.row));
	sf::Texture piece_texture;
	
	
	if (this->home->player == this->controller->players[0])
	{
		piece_texture.loadFromFile("red piece.png");
	}
	else if (this->home->player == this->controller->players[1])
	{
		piece_texture.loadFromFile("blue piece.png");
	}
	else if (this->home->player == this->controller->players[2])
	{
		piece_texture.loadFromFile("orange piece.png");
	}
	else if (this->home->player == this->controller->players[3]) 
	{
		piece_texture.loadFromFile("green piece.png");
	}
	else if (this->home->player == this->controller->players[4])
	{
		piece_texture.loadFromFile("yellow piece.png");
	}
	else if (this->home->player == this->controller->players[5])
	{
		piece_texture.loadFromFile("purple piece.png");
	}
	pictures.setTexture(&piece_texture);
	pictures.setOrigin(size.row/2, size.col/2);
	pictures.setPosition(sf::Vector2f(P.col, P.row));
	this->controller->ludo_cabaret->ludo_window->draw(pictures);
		
	
	
}
bool Piece::can_go_inside_home(int source,int dice_value)
{
	if (this->home->player->killed_enemy && source + dice_value >= this->home->home_entering_index && source < this->home->home_entering_index)
		return true;
	return false;
	
}