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

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widgets/imagebutton.hpp"

#include "guichan/graphics.hpp"
#include "guichan/image.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    ImageButton::ImageButton()
        : mImage(0)
    {
        setWidth(0);
        setHeight(0);
    }

    ImageButton::ImageButton(const Image* image)
        : mImage(image)
    {
        adjustSize();
    }

    void ImageButton::setImage(const Image* image)
    {
        mImage = image;
    }

    const Image* ImageButton::getImage() const
    {
        return mImage;
    }

    void ImageButton::adjustSize()
    {
        setWidth(mImage->getWidth()+2*mSpacing);
        setHeight(mImage->getHeight()+2*mSpacing);
    }

    void ImageButton::draw(Graphics* graphics)
    {
        const Style*style = getStyle();
        Color faceColor = getBaseColor();
        Color highlightColor;
        Color shadowColor;
        if (isPressed())
        {
            faceColor = style->getActivatedColor(getBaseColor());
            highlightColor = style->getShadowColor(faceColor);
            shadowColor = style->getHighlightColor(faceColor);
        }
        else
        {
            highlightColor = style->getHighlightColor(faceColor);
            shadowColor = style->getShadowColor(faceColor);
        }

        if(mIsOpaque)
        {
            graphics->setColor(faceColor);
            graphics->fillRectangle(1, 1, getDimension().width - 1, getHeight() - 1);

            graphics->setColor(highlightColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);

            graphics->setColor(shadowColor);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
            graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

            graphics->setColor(getForegroundColor());
        }

        const int textX = (getWidth() - (mImage ? mImage->getWidth() : 0) ) / 2;
        const int textY = (getHeight() - (mImage ? mImage->getHeight() : 0) ) / 2;

        if (isPressed())
        {
            if(mImage)
                graphics->drawImage(mImage, textX + 1, textY + 1, 1.0);
        }
        else
        {
            if(mImage)
                graphics->drawImage(mImage, textX, textY, this->isEnabled() ? (float)1.0 : style->getDisabledOpacity());

            if (isFocused())
            {
                graphics->drawRectangle(2, 2, getWidth() - 4, getHeight() - 4);
            }
        }
    }
}
