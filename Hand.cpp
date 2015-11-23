#include "Hand.h"

//Constructor creates hand with necessary variables
Hand::Hand()
{
	m_total = 0;
	m_bet = 0;
	m_handLive = true;
	m_hasAce = false;
	m_handState = "NONE";
}

//Adds provided card to hand and updates hand total
void Hand::DrawCard(Card _card)
{
	m_total += _card.GetValue();
	m_hand.push_back(_card);
}

//GETTERS
int Hand::GetHandSize()
{
	return m_hand.size();
}

int Hand::GetHandTotal()
{
	return m_total;
}

int Hand::GetBet()
{
	return m_bet;
}

std::string Hand::GetHandState()
{
	return m_handState;
}

bool Hand::IsHandLive()
{
	return m_handLive;
}
//+++++++++++++++++++++++++++++++++++++


//SETTERS
void Hand::SetHandState(std::string _state)
{
	m_handState = _state;
}

void Hand::SetBet(int _bet)
{
	m_bet = _bet;
}

void Hand::SetLive(bool _live)
{
	m_handLive = _live;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++

//Checks if hand contains an ace. If it does it reduces the value of the hand by
//10 and sets the value of the ace to 1. Returns true is hand contains an ace
//and false if not. Function called only if player is over 21 and thus if hand contains
//ace value must be reduced
bool Hand::HasAce()
{
	for(unsigned int i = 0; i < m_hand.size(); i++)
	{
		if(m_hand.at(i).GetValue() == 11)
		{
			m_hand.at(i).SetValue(1);
			m_total -= 10;
			return true;
		}
	}

	return false;
}

//Checks if the 2 cards in the hand are the same value - returns true
//if they are and false otherwise. Only called if player chooses split
bool Hand::CanHandSplit()
{
	if(m_hand.at(0).GetValue() == m_hand.at(1).GetValue())
	{
		return true;
	}

	else
		return false;
}

//Removes 2nd card from hand and returns it to caller. Called if hand can be
//split and returns card to be added to new hand
Card Hand::SplitHand()
{
	Card newCard = m_hand.at(1);
	m_total -= m_hand.at(1).GetValue();
	m_hand.erase(m_hand.begin() + 1);
	return newCard;
}



//Draws player hand relative to what player number they are and what hand they are playing
//This affects where they are drawn on table
void Hand::RenderPlayerHand(ALLEGRO_BITMAP *_cards, int _handNo, int _playerNo)
{	
		for( unsigned int i = 0; i < m_hand.size(); i++ )
		{
			al_draw_tinted_scaled_rotated_bitmap_region(_cards, (m_hand.at(i).GetNumber() - 1) * 80,  m_hand.at(i).GetSuit() * 120, 80, 120, al_map_rgb(255,255,255), 0, 0, 495 + (_handNo * 165) + (i * 18), 80 + (_playerNo * 190) + (i * 3), 0.8, 0.8, 0, NULL);
		}
}

//Draws dealer hand - 2nd card is only drawn if dealer is playing their turn (after
//all other players have played their hands
void Hand::RenderDealerHand(ALLEGRO_BITMAP *_cards, bool _isVisible)
{
	if( _isVisible )
	{
		for( unsigned int i = 0; i < m_hand.size(); i++ )
		{
			al_draw_tinted_scaled_rotated_bitmap_region(_cards, (m_hand.at(i).GetNumber() - 1) * 80,  m_hand.at(i).GetSuit() * 120, 80, 120, al_map_rgb(255,255,255), 0, 0, 84 + i * 20, 451 + (i * 8), 0.8, 0.8, 0, NULL);
		}
	}

	else if( m_hand.size() > 0 )
	{
		al_draw_tinted_scaled_rotated_bitmap_region(_cards, 0, 4 * 120, 80, 120, al_map_rgb(255,255,255), 0, 0, 84, 451, 0.8, 0.8, 0, NULL);
		al_draw_tinted_scaled_rotated_bitmap_region(_cards, (m_hand.at(1).GetNumber() - 1) * 80,  m_hand.at(1).GetSuit() * 120, 80, 120, al_map_rgb(255,255,255), 0, 0, 104, 459, 0.8, 0.8, 0, NULL);
	}
}

Hand::~Hand()
{
	m_hand.clear();
}