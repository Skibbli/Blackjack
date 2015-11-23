#pragma once

#ifndef TABLE_H
#define TABLE_H

#include "Gamestate.h"
#include "Gamestate Manager.h"
#include "Player.h"
#include "Dealer.h"
#include "Deck.h"
#include "Namespaces.h"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

//This is the class for the main Blackjack table and derives from
//the Gamestate base, which provides the common functions used by
//all Gamestates. It also contains all objects necessary to play
//the game of Blackjack
class Table : public Gamestate
{
	public:
		//Constructor and Destructor
		Table(GamestateManager *_stateManager);
		~Table();

		void DealNewHand();
		void ClearTable();

		//Overided functions derived from Gamestate
		virtual bool Update();
		virtual void Render();

		void GetKeyboardInput();
		State::State GetState();

	private:
		//Variables used by this class
		int m_numPlayers, m_activePlayer;

		Keyboard::Keyboard m_keyboard;
		State::State m_state, m_prevState;
	
		Deck m_shoe;

		Dealer m_dealer;
		std::vector < Player > m_players;

		//Allegro variables
		ALLEGRO_BITMAP *m_cardIMGs, *m_exitButton;
		ALLEGRO_FONT *m_font;
		ALLEGRO_SAMPLE *m_bgMusic;
};

#endif