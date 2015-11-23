#pragma once

#ifndef HUMAN_H
#define HUMAN_H

#include "BasePlayer.h"
#include "Namespaces.h"

//This is the class for the main Blackjack player and derives from
//the Base player base, which provides the common functions used by
//both players and the dealer. It also contains all objects and variables
//relevant to the player class e.g. deck, hand total, chip totals etc
class Player : public BasePlayer
{
	private:
		//Bools to keep track of hand conditions
		bool m_canSplit, m_canDouble, m_canSurrender;

		int m_chips, m_initialBet, m_totalBet, m_result, m_activeHand;

		ALLEGRO_FONT *m_font;

	public:
		//Constructor and Destructor
		Player();
		~Player();

		//Overrided functions inherited from Baseplayer
		virtual void Render(ALLEGRO_BITMAP *_cards, int _playerNum);
		virtual bool Update(State::State _currState, Keyboard::Keyboard _keyboard, Deck *_deck);

		//Functions for main actions in a game of Blackjack
		bool Hit(Card _card);
		bool Stand();
		bool Double(Card _card);
		void Split(Card _card1, Card _card2);
		bool Surrender();

		bool IsHandLive();

		//Getters
		int GetActiveHand();
		virtual int GetHandValue();
		
		//Functions for checking and updating hand state
		bool CanDouble();
		bool CanSplit();
		bool CanSurrender();
		void UpdateHandState();
				
		void Blackjack();		
		void ResetPlayer();
		void CheckForWin(bool _dealerBust, int _dealerScore);
};

#endif