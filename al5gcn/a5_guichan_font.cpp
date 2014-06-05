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

#include "al5gcn/a5_guichan_font.h"
#include "al5gcn/a5_guichan_graphics.h"

#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Font::Allegro5Font(ALLEGRO_FONT* font)
: mAutoFree(false)
, mAllegroFont(font)
{
	if (font == NULL)
	{
		throw GCN_EXCEPTION("Allegro font is not usable. Have you forgotten to load it?");
	}
}

Allegro5Font::Allegro5Font(const std::string& filename, int w, int h)
: mAutoFree(true)
#if ALLEGRO_SUB_VERSION >= 1
, mAllegroFont(al_load_ttf_font_stretch(filename.c_str(), w, h, 0))
#else
, mAllegroFont(al_load_ttf_font(filename.c_str(), h, 0))
#endif
{
	if (mAllegroFont == NULL)
	{
		throw GCN_EXCEPTION("Unable to load Allegro font from file.");
	}
}

Allegro5Font::~Allegro5Font()
{
	if (mAutoFree)
	{
		if (mAllegroFont != NULL)
		{
			al_destroy_font(mAllegroFont);
		}

		mAllegroFont = NULL;
	}
}

int Allegro5Font::getWidth(const std::string& text) const
{
	return al_get_text_width(mAllegroFont, text.c_str());
}

int Allegro5Font::getHeight() const
{
	return al_get_font_line_height(mAllegroFont);
}

void Allegro5Font::drawString(gcn::Graphics* graphics, const std::string& text, int x, int y)
{
	Allegro5Graphics* const allegroGraphics = dynamic_cast<Allegro5Graphics*>(graphics);
	if (!allegroGraphics)
	{
		throw GCN_EXCEPTION("Graphics is not of type AllegroGraphics");
	}

	const gcn::ClipRectangle& rec = graphics->getCurrentClipArea();

	al_draw_text(mAllegroFont, allegroGraphics->getAllegroColor(),
         float(x + rec.xOffset), float(y + rec.yOffset), 0, text.c_str());
}
