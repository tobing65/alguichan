/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2010 - 2011 SiegeLord, Polybios and Tobias Scheuer
 *
 *
 * Tobias Scheuer a.k.a tobing  
 *
 * Visit: http://alguichan.sourceforge.net 
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "al5gcn/a5_guichan_input.h"

#include <allegro5/allegro.h>

#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Input::Allegro5Input()
: mSupressRepeat(false)
, mShiftPressed(false)
, mCtrlPressed(false)
, mAltPressed(false)
{
}

Allegro5Input::~Allegro5Input()
{
}

bool Allegro5Input::isKeyQueueEmpty()
{
	return mKeyQueue.empty();
}

KeyInput Allegro5Input::dequeueKeyInput()
{
    if (isKeyQueueEmpty())
    {
        throw GCN_EXCEPTION("Key queue is empty.");
    }

    KeyInput ki = mKeyQueue.front();
    mKeyQueue.pop();

    return ki;
}


bool Allegro5Input::isMouseQueueEmpty()
{
	return mMouseQueue.empty();
}


MouseInput Allegro5Input::dequeueMouseInput()
{
    if (isMouseQueueEmpty())
    {
        throw GCN_EXCEPTION("Mouse queue is empty.");
    }

    MouseInput mi = mMouseQueue.front();
    mMouseQueue.pop();

    return mi;
}

void Allegro5Input::pushInput(ALLEGRO_EVENT &event)
{
    switch (event.type)
    {
		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			int type=0;
			if(event.mouse.dz > 0)
			{
				type = MouseInput::WheelMovedUp;
			}
			else if(event.mouse.dz < 0)
			{
				type = MouseInput::WheelMovedDown;
			}
			else
			{
				type = MouseInput::Moved;
			}

            MouseInput mi(MouseInput::Empty, type, event.mouse.x, event.mouse.y, 0, mShiftPressed, mCtrlPressed, mAltPressed, mMetaPressed);
            mMouseQueue.push(mi);
			break;
		}
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_KEY_CHAR:
        {
            if(mSupressRepeat && event.keyboard.repeat)
                break;

            const Key keyObj = convertToKey(event.keyboard.keycode, event.keyboard.unichar);

            // this avoid duplicate events for key that generate down and char events
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.unichar == 0)
            {
                switch(keyObj.getValue())
                {
                case gcn::Key::LeftShift:
                case gcn::Key::RightShift:
                    mShiftPressed = true;
                    break;
                case gcn::Key::LeftControl:
                case gcn::Key::RightControl:
                    mCtrlPressed = true;
                    break;
                case gcn::Key::LeftMeta:
                case gcn::Key::RightMeta:
                    mMetaPressed = true;
                    break;
                case gcn::Key::LeftAlt:
                case gcn::Key::RightAlt:
                    mAltPressed = true;
                    break;
                default:
                    break;
                }
                break;
            }

            KeyInput ki(keyObj, KeyInput::Pressed);

            ki.setNumericPad(isNumericPad(event.keyboard.keycode));
            ki.setShiftPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT);
            ki.setAltPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_ALT);
            ki.setControlPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL);
            ki.setMetaPressed(event.keyboard.modifiers & (ALLEGRO_KEYMOD_COMMAND | ALLEGRO_KEYMOD_LWIN | ALLEGRO_KEYMOD_RWIN));

            mKeyQueue.push(ki);
            break;
        }

        case ALLEGRO_EVENT_KEY_UP:
        {
            Key keyObj = convertToKey(event.keyboard.keycode, event.keyboard.unichar);

            KeyInput ki(keyObj, KeyInput::Released);

            ki.setNumericPad(isNumericPad(event.keyboard.keycode));
            ki.setShiftPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT);
            ki.setAltPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_ALT);
            ki.setControlPressed(event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL);
            ki.setMetaPressed(event.keyboard.modifiers & (ALLEGRO_KEYMOD_COMMAND | ALLEGRO_KEYMOD_LWIN | ALLEGRO_KEYMOD_RWIN));

            mShiftPressed = ki.isShiftPressed();
            mCtrlPressed = ki.isControlPressed();
            mAltPressed = ki.isAltPressed();
            mMetaPressed = ki.isMetaPressed();

            mKeyQueue.push(ki);
            break;
        }

  		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
  		{
  		    int button;
			if(event.mouse.button == 1)
				button = MouseInput::Left;
			else if(event.mouse.button == 2)
				button = MouseInput::Right;
			else
				button = MouseInput::Middle;

            mMouseQueue.push(MouseInput(button, MouseInput::Pressed, event.mouse.x, event.mouse.y, 0, mShiftPressed, mCtrlPressed, mAltPressed, mMetaPressed));

			break;
  		}

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
		    int button;
			if(event.mouse.button == 1)
				button = MouseInput::Left;
			else if(event.mouse.button == 2)
				button = MouseInput::Right;
			else
				button = MouseInput::Middle;

			MouseInput mi(button, MouseInput::Released, event.mouse.x, event.mouse.y, 0, mShiftPressed, mCtrlPressed, mAltPressed, mMetaPressed);

            mMouseQueue.push(mi);
			break;
		}

	}   // end switch
}


Key Allegro5Input::convertToKey(int keycode, int unicode)
{
	int keysym;
	bool pad = false;

	switch(keycode)
	{
	  case ALLEGRO_KEY_ESCAPE:
		  keysym = Key::Escape;
		  break;

	  case ALLEGRO_KEY_ALT:
		  keysym = Key::LeftAlt;
		  break;

	  case ALLEGRO_KEY_ALTGR:
		  keysym = Key::RightAlt;
		  break;

	  case ALLEGRO_KEY_LSHIFT:
		  keysym = Key::LeftShift;
		  break;

	  case ALLEGRO_KEY_RSHIFT:
		  keysym = Key::RightShift;
		  break;

	  case ALLEGRO_KEY_LCTRL:
		  keysym = Key::LeftControl;
		  break;

	  case ALLEGRO_KEY_RCTRL:
		  keysym = Key::RightControl;
		  break;

	  case ALLEGRO_KEY_LWIN:
		  keysym = Key::LeftMeta;
		  break;

	  case ALLEGRO_KEY_RWIN:
		  keysym = Key::RightMeta;
		  break;

	  case ALLEGRO_KEY_INSERT:
		  keysym = Key::Insert;
		  break;

	  case ALLEGRO_KEY_HOME:
		  keysym = Key::Home;
		  break;

	  case ALLEGRO_KEY_PGUP:
		  keysym = Key::PageUp;
		  break;

	  case ALLEGRO_KEY_PGDN:
		  keysym = Key::PageDown;
		  break;

      case ALLEGRO_KEY_PAD_DELETE:
          pad = true;
	  case ALLEGRO_KEY_DELETE:
		  keysym = Key::Delete;
		  break;

	  case ALLEGRO_KEY_END:
		  keysym = Key::End;
		  break;

	  case ALLEGRO_KEY_CAPSLOCK:
		  keysym = Key::CapsLock;
		  break;

	  case ALLEGRO_KEY_BACKSPACE:
		  keysym = Key::Backspace;
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
		  keysym = Key::PrintScreen;
		  break;

	  case ALLEGRO_KEY_PAUSE:
		  keysym = Key::Pause;
		  break;

	  case ALLEGRO_KEY_SCROLLLOCK:
		  keysym = Key::ScrollLock;
		  break;

	  case ALLEGRO_KEY_NUMLOCK:
		  keysym = Key::NumLock;
		  break;

	  case ALLEGRO_KEY_LEFT:
		  keysym = Key::Left;
		  break;

	  case ALLEGRO_KEY_RIGHT:
		  keysym = Key::Right;
		  break;

	  case ALLEGRO_KEY_UP:
		  keysym = Key::Up;
		  break;

	  case ALLEGRO_KEY_DOWN:
		  keysym = Key::Down;
		  break;

	  case ALLEGRO_KEY_PAD_ENTER:
		  pad = true;
	  case ALLEGRO_KEY_ENTER:
		  keysym = Key::Enter;
		  break;

      default:
		  keysym = unicode;
	}

	return Key(keysym);
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

void Allegro5Input::suppressKeyRepeat(bool new_value)
{
    mSupressRepeat = new_value;
}

bool Allegro5Input::isKeyRepeatSuppressed() const
{
    return mSupressRepeat;
}
