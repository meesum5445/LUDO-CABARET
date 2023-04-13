#include<iostream>
#include<string>
#include<vector>
#include<array>
#include"Utility_Functions.h"
using namespace std;
#include"Controller_Class.h"
//...Class/Header file  to be defined here...//
#include"Team.h"
Team::Team(string name,Controller_Class*controller):name(name),controller(controller)
{
	this->controller->teams.push_back(this);				//Adding itself to controllers class.
}