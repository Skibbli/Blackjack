#ifndef CARD_H
#define CARD_H

#include <string>
#include <allegro5/allegro.h>

//This class contains all variables and functions relevant
//to a card
class Card
{
	public:
		//Constructors and destructors with overloaded constructor
		//to take parameters when creating the deck
		Card();
		Card(int _number, int _suit);
		~Card();

		//Getters
		int GetValue();
		int GetSuit();
		int GetNumber();

		//Setters
		void SetValue(int _val);
		void SetIsVisible(bool _isVisible);

	private:
		bool m_isVisible;
		int m_number, m_value;
		
		enum Suit {CLUBS, SPADES, HEARTS, DIAMONDS};
		Suit m_suit;
};

#endif