#pragma once

#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>

//This is the class for the deck of cards. It a vector of cards
//that is created in the constructor and can be set to any number
//of decks

class Deck
{
	public:
		//Constructor and destructor
		Deck();
		~Deck();

		void ResetDeck();
		void CreateDeck();

		//Function that removes a card from the vector and returns
		//it to the caller
		Card DealCard();

		//Getter retuns number of cards in the deck
		int CardsInDeck();

	private:
		std::vector<Card> m_deck;
};

#endif