#include "Card.h"

//Defining constructors - one for a blank card and one for
//parameters to create the deck
Card::Card()
{
}

Card::Card( int _number, int _suit )
{
	m_number = _number;

	if( _number == 1 )
		m_value = 11;

	else if( _number > 10 )
		m_value = 10;

	else
		m_value = _number;

	m_suit = (Suit)_suit;
}

//Function to se whether card will display or not when render is called
void Card::SetIsVisible( bool _isVisible )
{
	m_isVisible = _isVisible;
}

int Card::GetSuit()
{
	return (int)m_suit;
}

int Card::GetNumber()
{
	return m_number;
}

int Card::GetValue()
{
	return m_value;
}

void Card::SetValue( int _val )
{
	m_value = _val;
}

//Destructor
Card::~Card()
{
}