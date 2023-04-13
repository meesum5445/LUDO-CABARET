
#include<iostream>
#define NOMINMAX
#include<Windows.h>
#include<vector>
#include<array>
#include"Utility_Functions.h"
using namespace std;
#include"Controller_Class.h"
#include"Ludo_Cabaret.h"
#include"Player.h"
#include"Board.h"
//...Class/Header file  to be defined here...//
#include"Dice.h"
Dice::Dice(Controller_Class* controller) :controller(controller)
{
	this->controller->dice = this;			//Adding itself to controllers class.
	this->dice_last_result = 6;
}
int Dice::roll()
{
	srand(time(0));							// setting seed value to current time in seconds.
	this->dice_last_result= rand() % 6 + 1;					// %6+1 to generate random number between 1 and 6.
	return dice_last_result;
}
void Dice::display_dice(int roll_result, float angle, positionOnWindow size)
{
	sf::Texture dice_texture;
	sf::RectangleShape dice(sf::Vector2f(size.row, size.col));
	dice.setOrigin(sf::Vector2f(dice.getSize().x / 2, dice.getSize().y / 2));
	dice.setPosition(sf::Vector2f(1240, 760));
	dice.setRotation(angle);
	if (roll_result == 1)
	{
		dice_texture.loadFromFile("dice_1.png");
	}
	else if (roll_result == 2)
	{
		dice_texture.loadFromFile("dice_2.png");
	}
	else if (roll_result == 3)
	{
		dice_texture.loadFromFile("dice_3.png");
	}
	else if (roll_result == 4)
	{
		dice_texture.loadFromFile("dice_4.png");
	}
	else if (roll_result == 5)
	{
		dice_texture.loadFromFile("dice_5.png");
	}
	else if (roll_result == 6)
	{
		dice_texture.loadFromFile("dice_6.png");
	}
	else if (roll_result == 7)
	{
		dice_texture.loadFromFile("dice_animation1.png");
	}
	else if (roll_result == 8)
	{
		dice_texture.loadFromFile("dice_animation2.png");
	}
	else if (roll_result == 9)
	{
		dice_texture.loadFromFile("dice_animation3.png");
	}
	else if (roll_result == 10)
	{
		dice_texture.loadFromFile("dice_animation4.png");
	}
	else if (roll_result == 11)
	{
		dice_texture.loadFromFile("dice_animation5.png");
	}
	dice.setTexture(&dice_texture);
	this->controller->ludo_cabaret->ludo_window->draw(dice);
}
bool Dice::is_dice_clicked()
{
	auto pos=get_row_col_by_click(*this->controller->ludo_cabaret->ludo_window);
	if (pos.row >= 600 && pos.row <= 920 && pos.col >= 1080 && pos.col <= 1400)
		return true;
	return false;
}
void Dice::animation_dice_rolling()
{
	sf::Texture background_texture;
	background_texture.loadFromFile("background.jpeg");
	sf::RectangleShape background(sf::Vector2f(1500, 1000));
	background.setTexture(&background_texture);
	for(int i = 0; i < 5; i++)
	{
		
		this->controller->ludo_cabaret->ludo_window->draw(background);
		//....................................
		this->controller->ludo_cabaret->turn->display_roll_results();
		//....................................
		this->controller->ludo_cabaret->print_turn();
		//....................................
		this->controller->board->display();
		//..................................
		this->display_dice(i+7, i*10,{520,520});
		this->controller->ludo_cabaret->ludo_window->display();
	}
}