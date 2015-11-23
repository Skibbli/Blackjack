#include "Player.h"

//Constructor - sets default values for necessary variables
Player::Player()
{
	m_chips = 2000;
	m_initialBet = 0;
	m_result = 0;
	m_totalBet = 0;
	m_activeHand = 0;
	m_canDouble = true;
	m_canSplit = false;
	m_canSurrender = true;

	m_font = al_load_font("GOTHICB.ttf", 20, NULL);
}

//Function to draw a card. Checks if hand total is over 21 and that there is no ace. If over 21
//and no ace it sets the hand as not live then moves on to the next hand for this player (if there
//is one). If there is an ace the check function reduces hand value. Function returns true if player's
//turn is up
bool Player::Hit( Card _card )
{
	m_hands.at(m_activeHand).DrawCard( _card );

	if(m_hands.at(m_activeHand).GetHandTotal() < 22 && m_hands.at(m_activeHand).GetHandSize() == 5)
	{
		m_hands.at(m_activeHand).SetHandState("5 * Charlie");
		m_activeHand++;
	}
	
	else if(m_hands.at(m_activeHand).GetHandTotal() > 21 && !m_hands[m_activeHand].HasAce())
	{
		m_hands.at(m_activeHand).SetHandState("Bust");
		m_hands.at(m_activeHand).SetLive(false);
		m_result -= m_hands.at(m_activeHand).GetBet();
		m_activeHand++;
	}

	if(m_activeHand >= m_hands.size())
		return true;

	else
		return false;
}

//If player stands on their current hand play moves on to their next hand if there is one. Returns
//true if player's turn is up and false otherwise
bool Player::Stand()
{
	m_activeHand++;

	if(m_activeHand >= m_hands.size())
		return true;

	else
		return false;
}

//Checks if player's current hand is 2 cards (rule for doubling down) and if it is it draws a
//card, doubles the bet for that hand and reduces the chips accordingly. Automatically moves on
//to player's other hands if there are any. If player's turn is up returns true, otherwise false
bool Player::Double( Card _card )
{
	m_hands[m_activeHand].DrawCard( _card );
	m_hands[m_activeHand].SetBet( m_initialBet * 2 );
	m_totalBet += m_initialBet;
	m_chips -= m_initialBet;

	if(m_hands.at(m_activeHand).GetHandTotal() > 21 && !m_hands[m_activeHand].HasAce())
	{
		m_hands.at(m_activeHand).SetHandState("Bust");
		m_hands.at(m_activeHand).SetLive(false);
		m_result -= m_hands.at(m_activeHand).GetBet();
	}

	m_activeHand++;

	if(m_activeHand >= m_hands.size())
		return true;

	else
		return false;
}

//Checks if player's hand has 2 cards of the same value and that they have 3 hands or less (rules for splitting).
//If so it creates a new hand, copies the 2nd card from the original hand, adds it to the new hand, deletes the
//previous card and copies the bet from the original hand. Then draws 1 card for each of the new hands to make sure
//they both have 2 cards
void Player::Split( Card _card1, Card _card2 )
{
	Hand newHand;
	newHand.SetBet(m_initialBet);
	m_chips -= m_initialBet;
	m_totalBet += m_initialBet;
	newHand.DrawCard(m_hands.at(m_activeHand).SplitHand());
	m_hands.insert(m_hands.begin() + m_activeHand + 1, newHand);

	m_hands.at(m_activeHand).DrawCard(_card1);
	m_hands.at(m_activeHand + 1).DrawCard(_card2);

	if(m_hands.at(m_activeHand).GetHandTotal() == 21)
	{
		m_chips += m_hands.at(m_activeHand).GetBet() * 2.5;
		m_result += m_initialBet * 1.5;
		m_hands.at(m_activeHand).SetHandState("Blackjack!");
		m_hands.at(m_activeHand).SetLive(false);
	}

	if(m_hands.at(m_activeHand + 1).GetHandTotal() == 21)
	{
		m_chips += m_hands.at(m_activeHand + 1).GetBet() * 2.5;
		m_result += m_initialBet * 1.5;
		m_hands.at(m_activeHand + 1).SetHandState("Blackjack!");
		m_hands.at(m_activeHand + 1).SetLive(false);
	}

	m_canSplit = false;
}

//If player chooses to surrender a hand that hand is set as dead, half their chips are returned and
//play moves on to their next hand if they have one. Returns true if player's turn is up and false otherwise
bool Player::Surrender()
{
	m_hands.at(m_activeHand).SetHandState("Gave Up");
	m_hands[m_activeHand].SetLive( false );
	m_chips += m_initialBet / 2;
	m_result -= m_initialBet / 2;
	m_totalBet -= m_initialBet / 2;
	m_activeHand++;

	if(m_activeHand >= m_hands.size())
		return true;

	else
		return false;
}

//Gets current hand player is playing
int Player::GetActiveHand()
{
	return m_activeHand;
}

int Player::GetHandValue()
{
	return m_hands.at(m_activeHand).GetHandTotal();
}

bool Player::CanDouble()
{
	return m_canDouble;
}

bool Player::CanSplit()
{
	return m_canSplit;
}

bool Player::CanSurrender()
{
	return m_canSurrender;
}

//Initiates player Blackjack. Awards chips and changes hand status to avoid
//the Win check function at the end of the hand
void Player::Blackjack()
{
	m_chips += m_initialBet * 2.5;
	m_result += m_initialBet * 1.5;
	m_hands.at(m_activeHand).SetHandState("Blackjack!");
	m_hands.at(m_activeHand).SetLive(false);
}

bool Player::IsHandLive()
{
	for(unsigned int i = 0; i < m_hands.size(); i++)
	{
		if(m_hands.at(i).IsHandLive())
		{
			return true;
		}
	}

	return false;
}

//Main player update function. Acts based upon the state of the table passed in and the key the user has pressed
//(in the keyboard enum). Each function in the update function returns true if the player has played all their
//possible hands and false otherwise. If function returns true the table moves play on to the next play if there is one
bool Player::Update(State::State _currState, Keyboard::Keyboard _keyboard, Deck *_deck)
{
	bool playerDone = false;
	
	switch(_currState)
	{
		case State::GET_BETS:
			{
				switch(_keyboard)
				{
					case Keyboard::ONE:
						{
							m_initialBet = 100;
						}
						break;

					case Keyboard::TWO:
						{
							m_initialBet = 200;
						}
						break;
					case Keyboard::THREE:
						{
							m_initialBet = 300;
						}
						break;
					case Keyboard::FOUR:
						{
							m_initialBet = 400;
						}
						break;

					case Keyboard::FIVE:
						{
							m_initialBet = 500;
						}
						break;
				}

				if(m_initialBet != 0)
				{
					if(m_initialBet > m_chips)
						m_initialBet = m_chips;
					m_hands.at(0).SetBet(m_initialBet);
					m_chips -= m_initialBet;
					m_totalBet = m_initialBet;
					playerDone = true;
				}
			}
			break;
			//Updates hand based on the player's input in the keyboard enum.
			//For each option the game checks if the player is able to do that action
			//If they can't then pressing the button does nothing
		case State::PLAYING_HAND:
			{
				if(m_hands.at(m_activeHand).IsHandLive())
				{
					UpdateHandState();

					switch(_keyboard)
					{
						case Keyboard::H:
							{
								if(Hit( _deck->DealCard()))
									playerDone = true;
							}
							break;
						case Keyboard::S:
							{
								if(Stand())
									playerDone = true;
							}
							break;
						case Keyboard::D:
							{						
								if(m_canDouble)
									if(Double( _deck->DealCard()))
										playerDone = true;
							}
							break;
						case Keyboard::P:
							{
								if(m_canSplit)
									Split( _deck->DealCard(), _deck->DealCard());
							}
							break;
						case Keyboard::G:
							{
								if(m_canSurrender)
									if(Surrender())
										playerDone = true;
							}
							break;
					}
				}

				else
				{
					m_activeHand++;

					if(m_activeHand >= m_hands.size())
						playerDone = true;
				}		
			}
			break;
		case State::DISPLAY_RESULTS:
			{
				if(_keyboard == Keyboard::ENTER)
					return true;
				else
					return false;
			}
	}

	return playerDone;
}

//Draws each of the player's hands and other important player information - draws on screen relative to what number player they are
void Player::Render( ALLEGRO_BITMAP *_cards, int _playerNum )
{
	al_draw_textf(m_font, al_map_rgb(255, 255, 255), 330, 33 + (_playerNum * 188), NULL, "Player %i", _playerNum + 1);
	al_draw_textf(m_font, al_map_rgb(255, 255, 255), 330, 73 + (_playerNum * 188), NULL, "Chips %i", m_chips);
	al_draw_textf(m_font, al_map_rgb(255, 255, 255), 330, 113 + (_playerNum * 188), NULL, "Bet %i", m_totalBet);
	al_draw_textf(m_font, al_map_rgb(255, 255, 255), 330, 153 + (_playerNum * 188), NULL, "Result %i", m_result);
	
	for( unsigned int i = 0; i < m_hands.size(); i++ )
	{
		//al_draw_textf( m_font, al_map_rgb( 255, 255, 255 ), 20 + (_playerNum * 250), 375, NULL, "Player %i bet: %i", _playerNum + 1, m_hands.at(i).GetBet());
		m_hands.at(i).RenderPlayerHand( _cards, i, _playerNum );

		if(m_hands.at(i).GetHandState() == "NONE")
			al_draw_textf(m_font, al_map_rgb(255, 255, 255), 499 + (167 * i), 33 + (_playerNum * 188), NULL, "%i", m_hands.at(i).GetHandTotal());

		else
			al_draw_textf(m_font, al_map_rgb(255, 255, 255), 499 + (167 * i), 33 + (_playerNum * 188), NULL, "%s", m_hands.at(i).GetHandState().c_str());
	}
}

//Resets the player's important variables e.g. hands, bets etc
void Player::ResetPlayer()
{
	ResetHands();
	m_hands.push_back( Hand() );
	m_initialBet = 0;
	m_totalBet = 0;
	m_result = 0;
	m_activeHand = 0;
	m_canDouble = true;
	m_canSplit = false;
	m_canSurrender = true;
}

//Updates the state of the hand for double, split and surrender which is called
//each update of the player
void Player::UpdateHandState()
{
	if(m_hands.at(m_activeHand).GetHandSize() == 2)
	{
		m_canSurrender = true;

		if(m_chips >= m_initialBet)
		{
			m_canDouble = true;

			if(m_hands.at(m_activeHand).CanHandSplit() && m_hands.size() < 3)
				m_canSplit = true;
		}

		else
		{
			m_canDouble = false;
			m_canSplit = false;
		}
	}

	else
	{
		m_canDouble = false;
		m_canSurrender = false;
		m_canSplit = false;
	}
}

//Checks if player has won. Cycles through all hands and if they are live checks it
//against state of the dealer. If dealer is bust or player total is greater than dealer
//total player wins and receives double their original bet amount. If player and dealer
//totals are the same player receives their chips back. Anything else means player loses
void Player::CheckForWin(bool _dealerBust, int _dealerScore)
{
	for(unsigned int i = 0; i < m_hands.size(); i++)
	{
		if(m_hands.at(i).IsHandLive())
		{
			if(m_hands.at(i).GetHandState() == "5 * Charlie!")
			{
				m_chips += m_hands.at(i).GetBet() * 2;
				m_result += m_hands.at(i).GetBet();
			}
			
			else if(_dealerBust)
			{
				m_chips += m_hands.at(i).GetBet() * 2;
				m_result += m_hands.at(i).GetBet();
				m_hands.at(i).SetHandState("Win!");
			}

			else if( m_hands.at(i).GetHandTotal() > _dealerScore )
			{
				m_chips += m_hands.at(i).GetBet() * 2;
				m_result += m_hands.at(i).GetBet();
				m_hands.at(i).SetHandState("Win!");
			}

			else if( m_hands.at(i).GetHandTotal() == _dealerScore )
			{
				m_chips += m_hands.at(i).GetBet();
				m_result += 0;
				m_hands.at(i).SetHandState("Push");
			}

			else
			{
				m_result -= m_hands.at(i).GetBet();
				m_hands.at(i).SetHandState("Lose");
			}
		}
	}
}

//Destructor
Player::~Player()
{

}