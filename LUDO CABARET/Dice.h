#pragma once
//...Forward Declarations...//
#include"All_forward_declarations.h"


class Dice
{
public:
	Controller_Class* controller{};			//pointer to controller class ,so Dice can be linked to controller.
	int dice_last_result{};
public:
	Dice(Controller_Class*);	
	void display_dice(int,float, positionOnWindow);
	bool is_dice_clicked();
	void animation_dice_rolling();
	int roll();
};


