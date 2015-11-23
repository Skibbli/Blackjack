#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Deck.h"
#include "Hand.h"
#include <allegro5/allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

//This is the base class for the players and the dealer
//to derive from. It provides all common functions and variables
//utilised by the dealer and the players
class BasePlayer
{
	public:
		//Constructor and destructor. Destructor is virtual to ensure
		//derived classes define their own destructors
		BasePlayer();
		virtual ~BasePlayer();

		//Update and render functions that are overrided by the derived classes
		virtual void Render();
		virtual bool Update();
		
		void ResetHands();

	protected:
		std::vector <Hand> m_hands;		
};

#endif