#include "Gamestate Manager.h"
#include "Menu.h"
#include "AllegroInit.h"



bool AllInit();

int main()
{
	AllegroInit mainAllegro;

	GamestateManager stateManager;

	stateManager.AddState(new Menu(&stateManager));

	//Set game loop condition and enter game loop
	bool gameRunning = true;

	while (gameRunning)
	{
		//Wait until an event enters the queue, then retrieve that event
		al_wait_for_event(mainAllegro.m_eventQueue, &mainAllegro.m_event);

		if(mainAllegro.m_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameRunning = false;
		}

		//If event is the timer that triggers once every 1/30 of a second call update
		//and render for the current state the game is in
		else if(mainAllegro.m_event.type == ALLEGRO_EVENT_TIMER)
		{
			gameRunning = stateManager.Update();
			stateManager.Render();

			//Flip buffers using double buffering then clear other buffer
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	return 0;
}