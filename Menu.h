#pragma once

#ifndef MENU_H
#define MENU_H

#include "Gamestate.h"
#include "Gamestate Manager.h"

//This is the class for the main Menu and derives from
//the Gamestate base, which provides the common functions used by
//all Gamestates. It contains no additional variables as it
//inherits all necessary variables from the base
class Menu : public Gamestate
{
	public:
		//Constructor and Destructor
		Menu(GamestateManager *_gametateManager);
		~Menu();

		//Overrided functions inherited from Gamestate
		virtual bool Update();
		virtual void Render();
	private:
};

#endif