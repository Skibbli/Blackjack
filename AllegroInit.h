#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

class AllegroInit
{
	public:
		AllegroInit();
		~AllegroInit();

		ALLEGRO_DISPLAY *m_display;

		ALLEGRO_TIMER *m_timer;
		ALLEGRO_EVENT_QUEUE *m_eventQueue;
		ALLEGRO_EVENT m_event;

	private:
		//Create display variables
		int m_screenW;
		int m_screenH;
};

#endif

AllegroInit::AllegroInit()
{
	al_init();

	m_screenW = 1000;
	m_screenH = 800;
	
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();

	m_display = al_create_display(m_screenW, m_screenH);

	m_timer = al_create_timer(0.05);

	m_eventQueue = al_create_event_queue();
	al_register_event_source(m_eventQueue, al_get_display_event_source(m_display));
	al_register_event_source(m_eventQueue, al_get_timer_event_source(m_timer));

	al_start_timer(m_timer);
}

AllegroInit::~AllegroInit()
{
	al_destroy_display(m_display);
	al_destroy_timer(m_timer);
}