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

#include "guichan/widgets/tab.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/style.hpp"

#include "guichan/widgets/button.hpp"
#include "guichan/widgets/label.hpp"
#include "guichan/widgets/tabbedarea.hpp"

namespace gcn
{
    Tab::Tab()
        : mHasMouse(false)
        , mTabbedArea(nullptr)
    {
        mLabel = new Label();
        mLabel->setPosition(3, 3);
        mLabel->setFrameSize(0);
        add(mLabel);

        addMouseListener(this);
    }

    Tab::~Tab()
    {
        delete mLabel;
    }

    void Tab::adjustSize()
    {
        mLabel->adjustSize();
        setSize(mLabel->getWidth() + 8,
                mLabel->getHeight() + 8);

        if (mTabbedArea)
            mTabbedArea->adjustTabPositions();
    }

    void Tab::setTabbedArea(TabbedArea* tabbedArea)
    {
        mTabbedArea = tabbedArea;
    }

    TabbedArea* Tab::getTabbedArea()
    {
        return mTabbedArea;
    }

    void Tab::setCaption(const std::string& caption)
    {
      mLabel->setCaption(caption);
      mLabel->adjustSize();
      adjustSize();
    }

    void Tab::setCaption(const char* caption)
    {
        mLabel->setCaption(caption);
        mLabel->adjustSize();
        adjustSize();
    }

    const char* Tab::getCaption() const
    {
        return mLabel->getCaption();
    }

    void Tab::draw(Graphics *graphics)
    {
        const Color &faceColor = getBaseColor();
        const Style* style = getStyle();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        Color borderColor;
        Color baseColor;

        if((mTabbedArea && mTabbedArea->isTabSelected(this)) || mHasMouse)
        {
            // Draw a border.
            graphics->setColor(highlightColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);
            graphics->setColor(shadowColor);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);

            borderColor = highlightColor;
            baseColor = getBaseColor();
        }
        else
        {
            // Draw a border.
            graphics->setColor(shadowColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);

            baseColor = style->getDarkerColor(getBaseColor());
        }

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 1));
        const Rectangle currentClipArea = graphics->getCurrentClipArea();

        graphics->setColor(baseColor);
        graphics->fillRectangle(0,
                                0,
                                currentClipArea.width,
                                currentClipArea.height);

        drawChildren(graphics);

        if(mTabbedArea && mTabbedArea->isFocused() && mTabbedArea->isTabSelected(this))
        {
            graphics->setColor(Color(0x000000));
            graphics->drawRectangle(2,
                                    2,
                                    currentClipArea.width - 5,
                                    currentClipArea.height - 5);
        }

        graphics->popClipArea();
    }

    void Tab::mouseEntered(MouseEvent& mouseEvent)
    {
        mHasMouse = true;
    }

    void Tab::mouseExited(MouseEvent& mouseEvent)
    {
        mHasMouse = false;
    }
}

