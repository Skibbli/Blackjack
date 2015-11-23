#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5\allegro.h>

class AllegroInit
{
public:

private:
	ALLEGRO_DISPLAY *display = NULL;

	ALLEGRO_BITMAP *screen = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_EVENT event;
}

#endif