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

#include "al5gcn/a5_guichan_image.h"

#include "guichan/exception.hpp"

using namespace gcn;

Allegro5Image::Allegro5Image(ALLEGRO_BITMAP* bitmap, bool autoFree)
: mBitmap(bitmap)
, mAutoFree(autoFree)
{
}

Allegro5Image::~Allegro5Image()
{
	if (mAutoFree)
	{
		free();
	}
}

ALLEGRO_BITMAP* Allegro5Image::getBitmap() const
{
	return mBitmap;
}

void Allegro5Image::free()
{
    if(mBitmap)
    {
        al_destroy_bitmap(mBitmap);
        mBitmap = 0;
    }
}

void Allegro5Image::setBitmap(ALLEGRO_BITMAP* bitmap, bool autoFree)
{
    free();
    mBitmap = bitmap;
    mAutoFree = autoFree;
}

int Allegro5Image::getWidth() const
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to get the width of a non loaded image.");
	}

	return al_get_bitmap_width(mBitmap);
}

int Allegro5Image::getHeight() const
{
	if (mBitmap == NULL)
	{
		GCN_EXCEPTION("Trying to get the height of a non loaded image.");
	}

	return al_get_bitmap_height(mBitmap);
}

Color Allegro5Image::getPixel(int x, int y)
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to get a pixel from a non loaded image.");
	}

	ALLEGRO_COLOR c = al_get_pixel(mBitmap, x, y);

	unsigned char r,g,b,a;
	al_unmap_rgba(c, &r, &g, &b, &a);

	return Color(r, g, b, a);
}

void Allegro5Image::putPixel(int x, int y, const Color& color)
{
	if (mBitmap == NULL)
	{
		throw GCN_EXCEPTION("Trying to put a pixel in a non loaded image.");
	}

	ALLEGRO_COLOR c = al_map_rgba(color.r, color.g, color.b, color.a);

	ALLEGRO_BITMAP* old_target = al_get_target_bitmap();

	al_set_target_bitmap(mBitmap);
	al_put_pixel(x, y, c);
	al_set_target_bitmap(old_target);
}

void Allegro5Image::convertToDisplayFormat()
{
	if (mBitmap == NULL)
	{
		GCN_EXCEPTION("Trying to convert a non loaded image to display format.");
	}

    // for allegro 5, there's nothing to do: the image loader will always load correctly
#if 0
	int old_flags = al_get_new_bitmap_flags();
	int old_format = al_get_new_bitmap_format();

	al_set_new_bitmap_flags(0);
	al_set_new_bitmap_format(0);

	ALLEGRO_BITMAP *bmp = al_clone_bitmap(mBitmap);

	al_set_new_bitmap_flags(old_flags);
	al_set_new_bitmap_format(old_format);

	al_destroy_bitmap(mBitmap);
	mBitmap = bmp;
#endif
}
