#include "Deck.h"
#include <time.h>

//Constructor
Deck::Deck()
{
	CreateDeck();
}

//Function to create the deck. This is separate from the constructor
//as the deck will need to be recreated if it gets low on cards
void Deck::CreateDeck()
{
	for(unsigned int i = 1, j = 0; i < 14 && j < 4; i++)
	{
		Card newCard( i, j );
		m_deck.push_back(newCard);

		if(i == 13)
		{
			j++;
			i = 0;
		}
	}
}

//Function to clear the deck and create a new one
void Deck::ResetDeck()
{
	m_deck.clear();
	CreateDeck();
};

//Function selects a random card from the deck and returns it to the caller
Card Deck::DealCard()
{
	Card newCard;
	srand( time(NULL) );
	int r = rand() % (m_deck.size());
	newCard = m_deck.at(r);
	m_deck.erase(m_deck.begin() + r);

	return newCard;
}

int Deck::CardsInDeck()
{
	return m_deck.size();
}

//Destructor - destroys cards when deck is destroyed (Allegro Bitmap issue)
Deck::~Deck()
{
	m_deck.clear();
}