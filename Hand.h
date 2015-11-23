#pragma once

#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include "Deck.h"
#include <vector>

//This is the class for a hand of cards. It contains all
//variables relevant to a hand of cards e.g. total, bet,
//a vector of cards etc.
class Hand
{
	public:
		//Constructor and Destructor
		Hand();
		~Hand();

		void DrawCard(Card _card);
		
		//Setters
		void SetLive(bool _live);
		void SetBet(int _bet);
		void SetHandState(std::string _state);

		//Getters
		int GetHandSize();
		int GetHandTotal();
		int GetBet();
		std::string GetHandState();

		//Functions to check hand state
		bool HasAce();
		bool CanHandSplit();
		bool IsHandLive();

		Card SplitHand();

		//Functions for drawing hands
		void RenderPlayerHand(ALLEGRO_BITMAP *_cards, int _handNo, int _playerNo);
		void RenderDealerHand(ALLEGRO_BITMAP *_cards, bool _isVisible);

	private:
		bool m_hasAce, m_handLive;

		int m_total, m_bet;

		//String used for printing the result on the table
		std::string m_handState;

		//Cards held in the hand
		std::vector <Card> m_hand;
};

#endif