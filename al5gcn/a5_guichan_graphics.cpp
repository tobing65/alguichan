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

#include "al5gcn/a5_guichan_graphics.h"
#include "al5gcn/a5_guichan_image.h"

#include "guichan/rectangle.hpp"
#include "guichan/exception.hpp"
#include "guichan/cliprectangle.hpp"
#include "guichan/color.hpp"

#include "allegro5/allegro_primitives.h"

#include <math.h>

#ifdef ALLEGRO_MSVC
#define hypotf(x, y) _hypotf((x), (y))
#endif

using namespace gcn;

static ALLEGRO_COLOR white = {1,1,1,1};

Allegro5Graphics::Allegro5Graphics()
    : mTintColor(white)
{
}

Allegro5Graphics::~Allegro5Graphics()
{
}

void Allegro5Graphics::_beginDraw()
{
    ALLEGRO_BITMAP* target = al_get_target_bitmap();
    if(!target)
    {
        throw GCN_EXCEPTION("Target BITMAP is null, set it with setTarget first.");
    }

    //push a clip area the size of the target bitmap
    pushClipArea(gcn::Rectangle(0, 0, al_get_bitmap_width(target), al_get_bitmap_height(target)));
}

void Allegro5Graphics::_endDraw()
{
    //pop the clip area pushed in _beginDraw
    popClipArea();
}

bool Allegro5Graphics::pushClipArea(gcn::Rectangle area)
{
    bool result = Graphics::pushClipArea(area);

    const ClipRectangle& cr = mClipStack.top();

    al_set_clipping_rectangle(cr.x, cr.y, cr.width, cr.height);

    return result;
}

void Allegro5Graphics::popClipArea()
{
    Graphics::popClipArea();

    if (mClipStack.empty())
    {
        return;
    }

    const ClipRectangle& cr = mClipStack.top();

    al_set_clipping_rectangle(cr.x, cr.y, cr.width, cr.height);
}

void Allegro5Graphics::drawImage(const Image* image,
                                 int srcX,
                                 int srcY,
                                 int srcW,
                                 int srcH,
                                 int dstX,
                                 int dstY,
                                 int dstW,
                                 int dstH,
                                 float opacity)
{
    if (mClipStack.empty())
    {
        throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
            "outside of _beginDraw() and _endDraw()?");
    }

    const int xOffset = mClipStack.top().xOffset;
    const int yOffset = mClipStack.top().yOffset;

    const Allegro5Image* srcImage = dynamic_cast<const Allegro5Image*>(image);

    if (srcImage == NULL)
    {
        throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an Allegro5Image.");
    }

    if(opacity < 0.99)
    {
        int op, src, dst;
        al_get_blender(&op, &src, &dst);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
        mTintColor.a = opacity;
        al_draw_tinted_scaled_bitmap(srcImage->getBitmap(), mTintColor,
            srcX, srcY, srcW, srcH,
            dstX + xOffset, dstY + yOffset, dstW, dstH,
            0
            );
        al_set_blender(op, src, dst);
    }
    else
    {
        al_draw_scaled_bitmap(srcImage->getBitmap(),
            srcX, srcY, srcW, srcH,
            dstX + xOffset, dstY + yOffset, dstW, dstH,
            0
            );
    }

#if 0
    al_draw_bitmap_region(srcImage->getBitmap(),
        srcX,
        srcY,
        width,
        height,
        dstX + xOffset,
        dstY + yOffset,
        0
        );
#endif
}

void Allegro5Graphics::drawPoint(int x, int y)
{
    if (mClipStack.empty())
    {
        throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
            "outside of _beginDraw() and _endDraw()?");
    }

    const int xOffset = mClipStack.top().xOffset;
    const int yOffset = mClipStack.top().yOffset;

    al_draw_pixel(x + xOffset + 0.5f, y + yOffset + 0.5f, mAllegroColor);
}

//A function to draw a line with A4 semantics, which is what GuiChan expects
void a4_line(float x1, float y1, float x2, float y2, ALLEGRO_COLOR color)
{
    ALLEGRO_VERTEX vtx[4];

    float len = hypotf(x2 - x1, y2 - y1);

    if (len == 0)
    {
        al_draw_pixel(x1 + 0.5f, y1 + 0.5f, color);
        return;
    }

    float tx = 0.5f * (y2 - y1) / len;
    float ty = 0.5f * -(x2 - x1) / len;
    float lx = 0.5f * (x2 - x1) / len;
    float ly = 0.5f * (y2 - y1) / len;

    vtx[0].x = 0.5f + x1 + tx - lx; vtx[0].y = 0.5f + y1 + ty - ly;
    vtx[1].x = 0.5f + x1 - tx - lx; vtx[1].y = 0.5f + y1 - ty - ly;
    vtx[2].x = 0.5f + x2 - tx + lx; vtx[2].y = 0.5f + y2 - ty + ly;
    vtx[3].x = 0.5f + x2 + tx + lx; vtx[3].y = 0.5f + y2 + ty + ly;

    for (int ii = 0; ii < 4; ii++)
    {
        vtx[ii].color = color;
        vtx[ii].z = 0;
    }

    al_draw_prim(vtx, 0, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
}

void Allegro5Graphics::drawLine(int x1, int y1, int x2, int y2)
{
    if (mClipStack.empty())
    {
        throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
            "outside of _beginDraw() and _endDraw()?");
    }

    const int xOffset = mClipStack.top().xOffset;
    const int yOffset = mClipStack.top().yOffset;

    a4_line(x1 + xOffset, y1 + yOffset, x2 + xOffset, y2 + yOffset, mAllegroColor);
}

void Allegro5Graphics::drawRectangle(int x, int y, int w, int h)
{
    if (mClipStack.empty())
    {
        throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
            "outside of _beginDraw() and _endDraw()?");
    }

    const int xOffset = mClipStack.top().xOffset;
    const int yOffset = mClipStack.top().yOffset;

    al_draw_rectangle(
        float(x + xOffset) + 0.5,
        float(y + yOffset) + 0.5,
        float(x + w - 1 + xOffset) + 0.5,
        float(y + h - 1 + yOffset) + 0.5,
        mAllegroColor, 1);
}

void Allegro5Graphics::fillRectangle(int x, int y, int w, int h)
{
    if (mClipStack.empty())
    {
        throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion "
            "outside of _beginDraw() and _endDraw()?");
    }

    const int xOffset = mClipStack.top().xOffset;
    const int yOffset = mClipStack.top().yOffset;

    al_draw_filled_rectangle(
        x + xOffset,
        y + yOffset,
        x + w + xOffset,
        y + h + yOffset,
        mAllegroColor);
}

void Allegro5Graphics::setColor(const Color& color)
{
    mColor = color;
    mAllegroColor = al_map_rgba(color.r, color.g, color.b, color.a);
}

const Color& Allegro5Graphics::getColor() const
{
    return mColor;
}

const ALLEGRO_COLOR& Allegro5Graphics::getAllegroColor() const
{
    return mAllegroColor;
}

