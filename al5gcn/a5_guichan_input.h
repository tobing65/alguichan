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

#ifndef ALLEGRO5_GUICHAN_INPUT
#define ALLEGRO5_GUICHAN_INPUT

#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"

#include <queue>

union ALLEGRO_EVENT;

namespace gcn
{

    class Allegro5Input : public Input
    {
    public:
        Allegro5Input();
        virtual ~Allegro5Input();

        void pushInput(ALLEGRO_EVENT &event);

        // Inherited from Input
        virtual void _pollInput() { };

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

        virtual void suppressKeyRepeat(bool new_value);
        virtual bool isKeyRepeatSuppressed() const;

    protected:
        Key convertToKey(int keycode, int unicode);
        bool isNumericPad(int keycode);

        std::queue<KeyInput> mKeyQueue;
        std::queue<MouseInput> mMouseQueue;

        bool mSupressRepeat;

        bool mShiftPressed;
        bool mCtrlPressed;
        bool mAltPressed;
        bool mMetaPressed;
    };

}

#endif
