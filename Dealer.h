#pragma once

#ifndef DEALER_H
#define DEALER_H

#include "BasePlayer.h"

//The is the class for the dealer. It inherits common variables
//and functions from the Baseplayer base and overrides the update
//and render functions
class Dealer : public BasePlayer
{
	public:
		//Constructor and destructor
		Dealer();
		~Dealer();

		//Overrided functions inherited from the base class
		virtual void Render(ALLEGRO_BITMAP *_cards);
		virtual bool Update(Deck *_deck);

		void Hit(Card _card);
		void ResetDealer();	

		//Setters
		void SetVisible(bool _visible);

		//Getters
		int GetHandValue();
		bool IsBust();

	private:
		//Bool variables to keep track of dealer state
		bool m_isVisible;
		bool m_isBust;

		ALLEGRO_FONT *m_font;
};

#endif