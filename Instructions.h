#pragma once

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Gamestate.h"
#include "Gamestate Manager.h"

//This is the class for the instructions and derives from
//the Gamestate base, which provides the common functions used by
//all Gamestates.
class Instructions : public Gamestate
{
	public:
		Instructions(GamestateManager *_stateManager);
		~Instructions();

		virtual bool Update();
		virtual void Render();
	private:
};

#endif

//Constructor - loads background, registers keyboard input and sets
//the current state manager
Instructions::Instructions(GamestateManager *_stateManager)
{
	m_stateManager = _stateManager;
	
	m_bgImage = al_load_bitmap("Images/Blackjack Instructions.jpg");

	if(m_bgImage == NULL)
		printf("Could not load Menu BG");

	m_eventQueue = al_create_event_queue();
	al_register_event_source(m_eventQueue, al_get_keyboard_event_source());
}

//Gets next event from the queue. The event queue is only set up to the
//keyboard so will only trigger when a key is pressed
bool Instructions::Update()
{
	al_get_next_event(m_eventQueue, &m_event);
	
	if(m_event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch(m_event.keyboard.keycode)
		{
			case ALLEGRO_KEY_ENTER:
			{
				m_stateManager->RemoveLastState();
				return true;
			}
			break;
		}
	}
	
	return true;
}

//Overrided function derived from the base state
//Draws the background image to the screen
void Instructions::Render()
{
	al_draw_bitmap(m_bgImage, 0 , 0, NULL);
}

//Destructor - cleans up to avoid memory leaks
Instructions::~Instructions()
{
	al_destroy_bitmap(m_bgImage);
}