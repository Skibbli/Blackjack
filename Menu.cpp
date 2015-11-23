#include "Menu.h"
#include "Instructions.h"
#include "Table.h"
#include <iostream>

//Constructor - loads background, registers keyboard input and sets
//the current state manager
Menu::Menu(GamestateManager *_stateManager)
{
	m_stateManager = _stateManager;
	
	m_bgImage = al_load_bitmap("Images/Blackjack Menu.jpg");

	if(m_bgImage == NULL)
		printf( "Could not load Menu BG" );

	al_install_keyboard();

	m_eventQueue = al_create_event_queue();
	al_register_event_source( m_eventQueue, al_get_keyboard_event_source());
}

//Gets next event from the queue. If it's a keyboard event it checks the switch statement
//and performs the necessary action. Function returns false if player initiates a quit of the game.

bool Menu::Update()
{
	bool gameRunning = true;

	al_get_next_event(m_eventQueue, &m_event);

	if( m_event.type == ALLEGRO_EVENT_KEY_DOWN )
	{
		switch(m_event.keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE:
				{
					gameRunning = false;
				}
				break;
			case ALLEGRO_KEY_I:
				{
					m_stateManager->AddState(new Instructions(m_stateManager));
				}
				break;
			case ALLEGRO_KEY_N:
				{
					m_stateManager->ChangeState(new Table(m_stateManager));
					return true;
				}
				break;
		}
	}

	return gameRunning;
}

//Draws the menu
void Menu::Render()
{
		al_draw_bitmap(m_bgImage, 0, 0, NULL);
}

//Destructor - cleans up to avoid memory leaks
Menu::~Menu()
{
	al_destroy_bitmap(m_bgImage);
}