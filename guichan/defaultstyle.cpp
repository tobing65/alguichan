/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2009 Olof Naessén and Per Larsson
 * Copyright (c) 2010 - 2011 Olof Naessén, Per Larsson and Tobias Scheuer
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 * Tobias Scheuer a.k.a tobing  
 *
 * Visit: http://guichan.sourceforge.net for older versions
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

#include "guichan/defaultstyle.hpp"

#include "guichan/color.hpp"
#include "guichan/widget.hpp"

#include <time.h>

using namespace gcn;

bool DefaultStyle::getBlink() const
{
    const bool blink=((int)(clock()/(0.6*CLOCKS_PER_SEC)))&1;
    return blink;
}

int DefaultStyle::getCaretWidth() const
{
    return 2;
}

Color DefaultStyle::getActivatedColor(const Color&oldColor) const
{
    Color newColor = oldColor;
    newColor.r = newColor.r * 0.8;
    newColor.g = newColor.g * 0.8;
    newColor.b = newColor.b * 0.8;
    return newColor;
}

Color DefaultStyle::getDarkerColor(const Color&oldColor) const
{
    Color newColor = oldColor;
    newColor.r = newColor.r * 0.9;
    newColor.g = newColor.g * 0.9;
    newColor.b = newColor.b * 0.9;
    return newColor;
}

Color DefaultStyle::getShadowColor(const Color&oldColor) const
{
    Color newColor = oldColor;
    newColor.r = newColor.r * 0.7;
    newColor.g = newColor.g * 0.7;
    newColor.b = newColor.b * 0.7;
    return newColor;
}

Color DefaultStyle::getHighlightColor(const Color&oldColor) const
{
    Color newColor = oldColor;
    newColor.r = oldColor.r * 0.7 + 255 * 0.3;
    newColor.g = oldColor.g * 0.7 + 255 * 0.3;
    newColor.b = oldColor.b * 0.7 + 255 * 0.3;
    return newColor;
}

Color DefaultStyle::getDisabledColor(const Color&fgColor, const Color&bgColor) const
{
    Color newColor = fgColor;
    newColor.r = (3*newColor.r + bgColor.r) / 4;
    newColor.g = (3*newColor.g + bgColor.g) / 4;
    newColor.b = (3*newColor.b + bgColor.b) / 4;
    return newColor;
}

Color DefaultStyle::getDefaultForegroundColor() const
{
    return Color(0,0,0);
}

Color DefaultStyle::getDefaultBackgroundColor() const
{
    return Color(255,255,255);
}

Color DefaultStyle::getDefaultBaseColor() const
{
    return Color(128,128,128);
}

short DefaultStyle::getDefaultFrameSize() const
{
    return 0;
}

short DefaultStyle::getDefaultButtonFrameSize() const
{
    return 0;
}

short DefaultStyle::getDefaultAreaFrameSize() const
{
    return 0;
}

short DefaultStyle::getDefaultWindowFrameSize() const
{
    return -1;
}

