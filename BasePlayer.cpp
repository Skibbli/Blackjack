#include "BasePlayer.h"

//Constructor that creates a hand and adds it to the vector
//of hands when the player is created to ensure there is a hand
//present when cards are dealt
BasePlayer::BasePlayer()
{
	m_hands.push_back( Hand() );
}

void BasePlayer::Render() {}

bool BasePlayer::Update() { return false; }

//Resets hand and resizes containing vector to 0
void BasePlayer::ResetHands()
{
	m_hands.clear();
	m_hands.resize( 0 );
}

//Destructor
BasePlayer::~BasePlayer()
{
	m_hands.clear();
}