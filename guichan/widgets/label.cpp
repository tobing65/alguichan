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

#include "guichan/widgets/label.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    Label::Label()
        : mAlignment(Graphics::Left)
        , mOpaque(false)
    {
        setFrameSize(getStyle()->getDefaultFrameSize());
    }

    Label::Label(const std::string& caption)
        : mCaption(caption)
        , mAlignment(Graphics::Left)
        , mOpaque(false)
    {
        adjustSize();
    }

    Label::Label(const char* caption)
        : mAlignment(Graphics::Left)
        , mOpaque(false)
    {
        if(caption)
            mCaption.assign(caption);

        adjustSize();
    }

    const char* Label::getCaption() const
    {
        return mCaption.c_str();
    }

    void Label::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    void Label::setCaption(const char* caption)
    {
        if(caption)
            mCaption.assign(caption);
        else
            mCaption.clear();
    }

    void Label::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Label::getAlignment() const
    {
        return mAlignment;
    }

    bool Label::isOpaque() const
    {
        return mOpaque;
    }

    void Label::setOpaque(bool opaque)
    {
        mOpaque=opaque;
    }

    void Label::draw(Graphics* graphics)
    {
        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        if (mOpaque)
        {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(0, 0, getWidth(), getHeight());
        }

        switch (getAlignment())
        {
        case Graphics::Left:
            textX = 0;
            break;
        case Graphics::Center:
            textX = getWidth() / 2;
            break;
        case Graphics::Right:
            textX = getWidth();
            break;
        default:
            throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(getCaption(), textX, textY, getAlignment());
    }

    void Label::adjustSize()
    {
        setWidth(getFont()->getWidth(getCaption()));
        setHeight(getFont()->getHeight());
    }
}
