#include "a5_guichan_input.h"

#include <allegro5/allegro.h>

#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Input::Allegro5Input()
{
	mKeyQueue = al_create_event_queue();
	mMouseQueue = al_create_event_queue();
	al_register_event_source(mKeyQueue, al_get_keyboard_event_source());
	al_register_event_source(mMouseQueue, al_get_mouse_event_source());
}

Allegro5Input::~Allegro5Input()
{
	al_destroy_event_queue(mKeyQueue);
	al_destroy_event_queue(mMouseQueue);
}

bool Allegro5Input::isKeyQueueEmpty()
{
	return al_event_queue_is_empty(mKeyQueue);
}

KeyInput Allegro5Input::dequeueKeyInput()
{
	if (isKeyQueueEmpty())
	{
		throw GCN_EXCEPTION("Key queue is empty.");
	}

	ALLEGRO_EVENT event;

	al_get_next_event(mKeyQueue, &event);

	Key keyObj = convertToKey(event.keyboard.keycode, event.keyboard.unichar);
	
	int type;
	if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		type = KeyInput::PRESSED;
	else if(event.type == ALLEGRO_EVENT_KEY_UP)
		type = KeyInput::RELEASED;
	else
		type = KeyInput::PRESSED;

	KeyInput ki(keyObj, type);
	
	ki.setNumericPad(isNumericPad(event.keyboard.keycode));
	ki.setShiftPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT);
	ki.setAltPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_ALT);
	ki.setControlPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL);
	ki.setMetaPressed(event.keyboard.modifiers & (ALLEGRO_KEYMOD_COMMAND |
									ALLEGRO_KEYMOD_LWIN |
									ALLEGRO_KEYMOD_RWIN));
	
	return ki;
}

bool Allegro5Input::isMouseQueueEmpty()
{
	return al_event_queue_is_empty(mMouseQueue);
}

MouseInput Allegro5Input::dequeueMouseInput()
{
	if (isMouseQueueEmpty())
	{
		throw GCN_EXCEPTION("Mouse queue is empty.");
	}

	ALLEGRO_EVENT event;

	al_get_next_event(mMouseQueue, &event);

	int button;
	int type;

	switch(event.type)
	{
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if(event.mouse.button == 1)
				button = MouseInput::LEFT;
			else if(event.mouse.button == 2)
				button = MouseInput::RIGHT;
			else
				button = MouseInput::MIDDLE;
		
			type = MouseInput::PRESSED;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			if(event.mouse.button == 1)
				button = MouseInput::LEFT;
			else if(event.mouse.button == 2)
				button = MouseInput::RIGHT;
			else
				button = MouseInput::MIDDLE;

			type = MouseInput::RELEASED;
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			button = MouseInput::EMPTY;

			if(event.mouse.dz > 0)
			{
				type = MouseInput::WHEEL_MOVED_UP;
			}
			else if(event.mouse.dz < 0)
			{
				type = MouseInput::WHEEL_MOVED_DOWN;
			}
			else
			{
				type = MouseInput::MOVED;
			}
			break;
		default:
			button = MouseInput::EMPTY;
			type = MouseInput::MOVED;
			break;
	}

	MouseInput mi(button, type, event.mouse.x, event.mouse.y, 0);

	return mi;
}

Key Allegro5Input::convertToKey(int keycode, int unicode)
{
	int keysym;
	bool pad = false;

	switch(keycode)
	{
	  case ALLEGRO_KEY_ESCAPE:
		  keysym = Key::ESCAPE;
		  break;

	  case ALLEGRO_KEY_ALT:
		  keysym = Key::LEFT_ALT;
		  break;

	  case ALLEGRO_KEY_ALTGR:
		  keysym = Key::RIGHT_ALT;
		  break;

	  case ALLEGRO_KEY_LSHIFT:
		  keysym = Key::LEFT_SHIFT;
		  break;

	  case ALLEGRO_KEY_RSHIFT:
		  keysym = Key::RIGHT_SHIFT;
		  break;

	  case ALLEGRO_KEY_LCTRL:
		  keysym = Key::LEFT_CONTROL;
		  break;

	  case ALLEGRO_KEY_RCTRL:
		  keysym = Key::RIGHT_CONTROL;
		  break;

	  case ALLEGRO_KEY_LWIN:
		  keysym = Key::LEFT_META;
		  break;

	  case ALLEGRO_KEY_RWIN:
		  keysym = Key::RIGHT_META;
		  break;

	  case ALLEGRO_KEY_INSERT:
		  keysym = Key::INSERT;
		  break;

	  case ALLEGRO_KEY_HOME:
		  keysym = Key::HOME;
		  break;

	  case ALLEGRO_KEY_PGUP:
		  keysym = Key::PAGE_UP;
		  break;

	  case ALLEGRO_KEY_PGDN:
		  keysym = Key::PAGE_DOWN;
		  break;

	  case ALLEGRO_KEY_DELETE:
		  keysym = Key::DELETE;
		  break;

	  case ALLEGRO_KEY_PAD_DELETE:
		  keysym = Key::DELETE;
		  pad = true;
		  break;

	  case ALLEGRO_KEY_END:
		  keysym = Key::END;
		  break;

	  case ALLEGRO_KEY_CAPSLOCK:
		  keysym = Key::CAPS_LOCK;
		  break;

	  case ALLEGRO_KEY_BACKSPACE:
		  keysym = Key::BACKSPACE;
		  break;

	  case ALLEGRO_KEY_F1:
		  keysym = Key::F1;
		  break;

	  case ALLEGRO_KEY_F2:
		  keysym = Key::F2;
		  break;

	  case ALLEGRO_KEY_F3:
		  keysym = Key::F3;
		  break;

	  case ALLEGRO_KEY_F4:
		  keysym = Key::F4;
		  break;

	  case ALLEGRO_KEY_F5:
		  keysym = Key::F5;
		  break;

	  case ALLEGRO_KEY_F6:
		  keysym = Key::F6;
		  break;

	  case ALLEGRO_KEY_F7:
		  keysym = Key::F7;
		  break;

	  case ALLEGRO_KEY_F8:
		  keysym = Key::F8;
		  break;

	  case ALLEGRO_KEY_F9:
		  keysym = Key::F9;
		  break;

	  case ALLEGRO_KEY_F10:
		  keysym = Key::F10;
		  break;

	  case ALLEGRO_KEY_F11:
		  keysym = Key::F11;
		  break;

	  case ALLEGRO_KEY_F12:
		  keysym = Key::F12;
		  break;

	  case ALLEGRO_KEY_PRINTSCREEN:
		  keysym = Key::PRINT_SCREEN;
		  break;

	  case ALLEGRO_KEY_PAUSE:
		  keysym = Key::PAUSE;
		  break;

	  case ALLEGRO_KEY_SCROLLLOCK:
		  keysym = Key::SCROLL_LOCK;
		  break;

	  case ALLEGRO_KEY_NUMLOCK:
		  keysym = Key::NUM_LOCK;
		  break;

	  case ALLEGRO_KEY_LEFT:
		  keysym = Key::LEFT;
		  break;

	  case ALLEGRO_KEY_RIGHT:
		  keysym = Key::RIGHT;
		  break;

	  case ALLEGRO_KEY_UP:
		  keysym = Key::UP;
		  break;

	  case ALLEGRO_KEY_DOWN:
		  keysym = Key::DOWN;
		  break;

	  case ALLEGRO_KEY_PAD_ENTER:
		  pad = true;
	  case ALLEGRO_KEY_ENTER:
		  keysym = Key::ENTER;
		  break;
 default:
		  keysym = unicode;
	}

	Key k = Key(keysym);

	return k;
}

bool Allegro5Input::isNumericPad(int keycode)
{
	switch (keycode)
	{
	  case ALLEGRO_KEY_PAD_0:
	  case ALLEGRO_KEY_PAD_1:
	  case ALLEGRO_KEY_PAD_2:
	  case ALLEGRO_KEY_PAD_3:
	  case ALLEGRO_KEY_PAD_4:
	  case ALLEGRO_KEY_PAD_5:
	  case ALLEGRO_KEY_PAD_6:
	  case ALLEGRO_KEY_PAD_7:
	  case ALLEGRO_KEY_PAD_8:
	  case ALLEGRO_KEY_PAD_9:
	  case ALLEGRO_KEY_PAD_SLASH:
	  case ALLEGRO_KEY_PAD_MINUS:
	  case ALLEGRO_KEY_PAD_PLUS:
		  return true;
	  default:
		  return false;
	}
}
