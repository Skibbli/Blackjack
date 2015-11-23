#include "Table.h"
#include "Menu.h"
#include <iostream>

//Constructor - loads background and buttons, registers keyboard input and sets
//the current state manager. Also initialises necessary variables
Table::Table(GamestateManager *_stateManager)
{
	m_stateManager = _stateManager;
	
	m_bgImage = al_load_bitmap("Images/Blackjack Table.jpg");

	if(m_bgImage == NULL)
		printf("Could not load Menu BG");

	m_cardIMGs = al_load_bitmap("Images/Playing Cards.png");

	if( m_cardIMGs == NULL )
		printf("Could not load Playing Cards");

	m_exitButton = al_load_bitmap("Images/Exit Button.png");

	if( m_exitButton == NULL )
		printf("Could not load Exit Button");

	m_eventQueue = al_create_event_queue();
	al_register_event_source( m_eventQueue, al_get_keyboard_event_source());

	al_reserve_samples(1);
	m_bgMusic = al_load_sample("The_Lounge.ogg");

	if( m_bgMusic == NULL )
		printf("Could not load BG Music");

	al_play_sample(m_bgMusic,1,1,1,ALLEGRO_PLAYMODE_LOOP,NULL);

	m_font = al_load_font("GOTHICB.ttf", 26, 0);

	m_state = State::State::GET_PLAYERS;
	m_keyboard = Keyboard::Keyboard::NONE;
	m_activePlayer = 0;
	m_numPlayers = 0;
}

//Deals a new hand by dealing 1 card to the dealer and 1 to each player, twice
void Table::DealNewHand()
{
	int n = 0;

	while(n < 2)
	{
		m_dealer.Hit(m_shoe.DealCard());

		for(unsigned int i = 0; i < m_players.size(); i++)
		{
			m_players.at(i).Hit(m_shoe.DealCard());
		}

		n++;
	}
}

//Clears table by resetting all player and dealer hands and all necessary variables
void Table::ClearTable()
{
	m_dealer.ResetDealer();

	for(int i = 0; i < m_numPlayers; i++)
		m_players.at(i).ResetPlayer();

	if(m_shoe.CardsInDeck() < ( m_numPlayers + 1 ) * 6)
		m_shoe.ResetDeck();

	m_activePlayer = 0;
}

//This gets keyboard input from the player and updates a Keyboard enum. The keyboard
//enum can then be passed in to other objects to check player input without having to
//create a whole new event queue
void Table::GetKeyboardInput()
{
	al_get_next_event(m_eventQueue, &m_event);

	if(m_event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch(m_event.keyboard.keycode)
		{
			case ALLEGRO_KEY_H: m_keyboard = Keyboard::Keyboard::H; break;
			case ALLEGRO_KEY_S: m_keyboard = Keyboard::Keyboard::S; break;
			case ALLEGRO_KEY_D: m_keyboard = Keyboard::Keyboard::D; break;
			case ALLEGRO_KEY_P: m_keyboard = Keyboard::Keyboard::P; break;
			case ALLEGRO_KEY_G: m_keyboard = Keyboard::Keyboard::G; break;
			case ALLEGRO_KEY_ESCAPE: m_keyboard = Keyboard::Keyboard::ESCAPE; break;
			case ALLEGRO_KEY_ENTER: m_keyboard = Keyboard::Keyboard::ENTER; break;
			case ALLEGRO_KEY_Y: m_keyboard = Keyboard::Keyboard::Y; break;
			case ALLEGRO_KEY_N: m_keyboard = Keyboard::Keyboard::N; break;
			case ALLEGRO_KEY_1: m_keyboard = Keyboard::Keyboard::ONE; break;
			case ALLEGRO_KEY_2: m_keyboard = Keyboard::Keyboard::TWO; break;
			case ALLEGRO_KEY_3: m_keyboard = Keyboard::Keyboard::THREE; break;
			case ALLEGRO_KEY_4: m_keyboard = Keyboard::Keyboard::FOUR; break;
			case ALLEGRO_KEY_5: m_keyboard = Keyboard::Keyboard::FIVE; break;
		}
	}

	if(m_event.type == ALLEGRO_EVENT_KEY_UP)
	{
		m_keyboard = Keyboard::Keyboard::NONE;
	}
}

//Main update function for this game state. Returns true if the game is still running
//and false if the user quits the game
bool Table::Update()
{
	bool gameRunning = true;	

	m_keyboard = Keyboard::Keyboard::NONE;
	GetKeyboardInput();
	
	if(m_keyboard == Keyboard::ESCAPE)
	{
		m_prevState = m_state;
		m_state = State::State::EXIT;
	}

	//This is the main switch statement for the FSM for this game state. Based on the state the
	//game is currently in only the code relevant to that state will get used
	switch( m_state )
	{
		if(m_keyboard != Keyboard::NONE)
		{
			//Gets number of players playing at the table. Creates players for the vector
			//of players and then changes to next state
			case State::GET_PLAYERS:
				{					
					switch(m_keyboard)
					{
						case Keyboard::ONE: m_numPlayers = 1;
							break;
						case Keyboard::TWO: m_numPlayers = 2;
							break;
						case Keyboard::THREE: m_numPlayers = 3;
							break;
						case Keyboard::FOUR: m_numPlayers = 4;
							break;
						case Keyboard::ESCAPE: m_state = State::State::EXIT;
							break;
					}

					if( m_numPlayers != 0 )
					{
						m_state = State::State::GET_BETS;
					
						for(int i = 0; i < m_numPlayers; i++)
						{
							m_players.push_back(Player());
						}
					}
				}
				break;

				//Gets bets for each player. Once one player chooses their bet automatically
				//moves on to the next player. Once all players have bets hand is dealer and
				//state is changed to next state
			case State::GET_BETS:
				{
					if(m_players.at(m_activePlayer).Update(m_state, m_keyboard, &m_shoe))
						m_activePlayer++;
					

					if(m_activePlayer >= m_players.size())
					{
						DealNewHand();
						
						//If the dealer has Blackjack the hand automatically ends. If the player has
						//BJ as well this gets picked up in the DISPLAY_RESULTS state and will get their
						//bet returned there
						if(m_dealer.GetHandValue() == 21)
						{
							m_state = State::State::DEALER_PLAYING;
						}	

						//Checks players for BJ - if BJ then hand gets paid and hand status is set to not live
						else
						{
							for(int i = 0; i < m_numPlayers; i++)
							{
								if(m_players.at(i).GetHandValue() == 21)
								{
									m_players.at(i).Blackjack();
								}
							}

							m_state = State::State::PLAYING_HAND;
							m_activePlayer = 0;
						}				
					}		
				}
				break;

				//Gets player input for hand decision. For each choice it performs the necessary action and
				//returns if the player's turn is over. If it is play automatically moves on to the next player.
				//If there are no more players the state is changed and the dealer plays their hand
			case State::PLAYING_HAND:
				{
					if(m_players.at(m_activePlayer).Update(m_state, m_keyboard, &m_shoe))
						m_activePlayer++;

					if(m_activePlayer >= m_numPlayers)
						m_state = State::State::DEALER_PLAYING;
				}
				break;

			case State::DISPLAY_RESULTS:
				{
					if(m_players.at(0).Update(m_state, m_keyboard, &m_shoe))
						m_state = State::State::END_HAND;
				}
				break;
				//If player chooses exit above this prompts to confirm they wish to exit
				//If they do state changes to menu - if not play resumes
			case State::EXIT:
				{
					switch(m_keyboard)
					{
						case Keyboard::Y:
							{
								m_stateManager->ChangeState(new Menu(m_stateManager));
							}
							break;
						case Keyboard::N:
							{
								m_state = m_prevState;
							}
							break;
					}
				}
				break;
				
			}
			//Checks if it's time for the dealer to play. If it is their hand is revealed. If the dealer's hand is under
			//17 they draw a card. If hand is not under 17 then play ends and state is changed to the next one
			case State::DEALER_PLAYING:
				{
					bool playersActive = false;

					//Checks if any players are still active. Only plays their hand if their are players still in the hand
					for(int i = 0; i < m_numPlayers; i++)
					{
						if(m_players.at(i).IsHandLive())
							playersActive = true;
					}

					if(playersActive)
					{				
						if(m_dealer.Update(&m_shoe))
						{
							m_state = State::State::DISPLAY_RESULTS;

							for(unsigned int i = 0; i < m_numPlayers; i++)
							{
								m_players.at(i).CheckForWin(m_dealer.IsBust(), m_dealer.GetHandValue());
							}
						}
					}

					else
					{
						m_state = State::State::DISPLAY_RESULTS;
						m_dealer.SetVisible(true);

						for(unsigned int i = 0; i < m_numPlayers; i++)
						{
							m_players.at(i).CheckForWin(m_dealer.IsBust(), m_dealer.GetHandValue());
						}
					}
				}
				break;
				//Checks if each player has won. Cycles though all player hands and checks if they won. If they did their chips
				//are adjusted, the table is cleared and a new hand starts
			case State::END_HAND:
				{
					ClearTable();
					m_state = State::State::GET_BETS;
				}
				break;		
	}
	
	return gameRunning;
}

//Draws the table dependant on what state the table is in
void Table::Render()
{
	al_draw_bitmap(m_bgImage, 0, 0, NULL);

	//Always draws dealer and all players - if they have no hand then no hand is drawn
	m_dealer.Render(m_cardIMGs);

	for(int i = 0; i < m_numPlayers; i++)
	{
		m_players.at(i).Render(m_cardIMGs, i);
	}

	switch( m_state)
	{
		case State::GET_PLAYERS:
			{
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 43, NULL, "Enter Number");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 68, NULL, "of Players");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 103, NULL, "1 = 1 players");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 128, NULL, "2 = 2 players");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 153, NULL, "3 = 3 players");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 178, NULL, "4 = 4 players");
			}
			break;
		case State::GET_BETS:
			{
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 43, NULL, "Player %i", m_activePlayer + 1);
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 68, NULL, "enter your bet");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 103, NULL, "1 = 100 chips");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 128, NULL, "2 = 200 chips");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 153, NULL, "3 = 300 chips");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 178, NULL, "4 = 400 chips");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 203, NULL, "5 = 500 chips");
			}
			break;
		case State::START_HAND:
			break;
			//Draws options available to the player e.g. Hit, Double etc
		case State::PLAYING_HAND:
			{
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 43, NULL, "Player %i", m_activePlayer + 1);
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 68, NULL, "choose an option");
				
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 103, NULL, "H = Hit");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 128, NULL, "S = Stand");
				if(m_players.at(m_activePlayer).CanDouble())
					al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 153, NULL, "D = Double");
				if(m_players.at(m_activePlayer).CanSplit())
					al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 178, NULL, "P = Split");	
				if(m_players.at(m_activePlayer).CanSurrender())
					al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 203, NULL, "G = Surrender");

				al_draw_filled_rectangle(495 + (m_players[m_activePlayer].GetActiveHand() * 165), 198 + (m_activePlayer * 189),
					495 + (m_players[m_activePlayer].GetActiveHand() * 165) + 136, 200 + (m_activePlayer * 192) + 8, al_map_rgb(255,0,0));
			}
			break;
		case State::DISPLAY_RESULTS:
			{
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 43, NULL, "Press Enter to");
				al_draw_textf(m_font, al_map_rgb(255, 255, 255), 46, 68, NULL, "start next hand");
			}
		case State::END_HAND:
			{

			}
			break;
		case State::EXIT:
			{
				al_draw_scaled_bitmap(m_exitButton, 0, 0, 633, 471, 300, 250, 400, 299, NULL);
			}
			break;
	}
}

//Cleans up when state is changed, preventing memory leaks
Table::~Table()
{
	al_destroy_bitmap(m_bgImage);
	al_destroy_sample(m_bgMusic);
	al_destroy_bitmap(m_exitButton);
	al_destroy_font(m_font);
}