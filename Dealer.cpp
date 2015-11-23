#include "Dealer.h"

//Constructor
Dealer::Dealer()
{
	m_isVisible = false;
	m_isBust = false;

	m_font = al_load_font("GOTHICB.ttf", 20, NULL);
}

//Function to reset the dealer for starting a new hand
void Dealer::ResetDealer()
{
	m_isVisible = false;
	m_isBust = false;
	ResetHands();
	
	m_hands.push_back(Hand());
}

void Dealer::Hit(Card _card)
{
	m_hands.at(0).DrawCard(_card);

	//Checks if hand total is over 21 and that it has no ace. If there is no ace
	//the dealer is set as bust. If there is an ace the ace check function automatically
	//reduce the value of the hand and returns true so statement does not trigger
	if(m_hands.at(0).GetHandTotal() > 21 && !m_hands.at(0).HasAce())
	{
		m_isBust = true;
	}
}

bool Dealer::IsBust()
{
	return m_isBust;
}

//Main dealer update function. Dealer will hit if they have less than 17.
//If dealer goes over 21 an ace check is initiated. If there is an ace the hand value gets
//reduced and play continues. If not the dealer is set to Bust
bool Dealer::Update(Deck *_deck)
{
	m_isVisible = true;
	
	if(m_hands.at(0).GetHandTotal() < 17)
	{
		Hit(_deck->DealCard());
		return false;
	}

	if(m_hands.at(0).GetHandTotal() > 21)
	{
		if(m_hands.at(0).HasAce())
		{
			return false;
		}

		m_isBust = true;
		return true;
	}

	return true;
}

void Dealer::SetVisible(bool _visible)
{
	m_isVisible = _visible;
}

int Dealer::GetHandValue()
{
	return m_hands.at(0).GetHandTotal();
}

//Draws dealer's hand. Only draws total of hand if dealer is playing their hand
void Dealer::Render(ALLEGRO_BITMAP *_cards)
{
	if(!m_hands.empty())
	{
			m_hands[0].RenderDealerHand(_cards, m_isVisible);
	}

	if(m_isVisible)
	{
		if(m_isBust == true)
			al_draw_textf(m_font, al_map_rgb(255, 255, 255), 85, 626, NULL, "Dealer is bust!");
		else
			al_draw_textf(m_font, al_map_rgb(255, 255, 255), 85, 626, NULL, "%i", m_hands.at(0).GetHandTotal());
	}
}

//Destructor - clean up objects to prevent memory leaks
Dealer::~Dealer()
{
	al_destroy_font(m_font);
}