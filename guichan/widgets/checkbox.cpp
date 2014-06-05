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

#include "guichan/widgets/checkbox.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{

    CheckBox::CheckBox()
    {
        setSelected(false);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    CheckBox::CheckBox(const std::string& caption, bool selected)
    {
        setCaption(caption);
        setSelected(selected);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    CheckBox::CheckBox(const char* caption, bool selected)
    {
        setCaption(caption);
        setSelected(selected);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    void CheckBox::draw(Graphics* graphics)
    {
        drawBox(graphics);

        graphics->setFont(getFont());

        Color textColor = getForegroundColor();
        if(!isEnabled())
            textColor.a /= 2;
        graphics->setColor(textColor);

        const int h = getHeight() + getHeight() / 2;

        graphics->drawText(getCaption(), h - 2, 0);
    }

    void CheckBox::drawBox(Graphics *graphics)
    {
        const Style* style = getStyle();
        const int h = getHeight() - 2;
        Color faceColor = getBaseColor();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        graphics->setColor(shadowColor);
        graphics->drawLine(1, 1, h, 1);
        graphics->drawLine(1, 1, 1, h);

        graphics->setColor(highlightColor);
        graphics->drawLine(h, 1, h, h);
        graphics->drawLine(1, h, h - 1, h);

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(2, 2, h - 2, h - 2);

        graphics->setColor(getForegroundColor());

        if (isFocused())
        {
            graphics->drawRectangle(0, 0, h + 2, h + 2);
        }

        if (mSelected)
        {
            graphics->drawLine(3, 5, 3, h - 2);
            graphics->drawLine(4, 5, 4, h - 2);

            graphics->drawLine(5, h - 3, h - 2, 4);
            graphics->drawLine(5, h - 4, h - 4, 5);
        }
    }

    bool CheckBox::isSelected() const
    {
        return mSelected;
    }

    void CheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    const char* CheckBox::getCaption() const
    {
        return mCaption.c_str();
    }

    void CheckBox::setCaption(const std::string& caption)
    {
        mCaption=caption;
    }

    void CheckBox::setCaption(const char* caption)
    {
        if(caption)
            mCaption.assign(caption);
        else
            mCaption.clear();
    }

    void CheckBox::keyPressed(KeyEvent& keyEvent)
    {
        const Key& key = keyEvent.getKey();

        if(key.getValue() == Key::Enter || key.getValue() == Key::Space)
        {
            toggleSelected();
            keyEvent.consume();
        }
    }

    void CheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if(mouseEvent.getButton() == MouseEvent::Left)
        {
            toggleSelected();
        }
    }

    void CheckBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void CheckBox::adjustSize()
    {
        const int height = getFont()->getHeight();

        setHeight(height);
        setWidth(getFont()->getWidth(mCaption) + height + height / 2);
    }

    void CheckBox::toggleSelected()
    {
        mSelected = !mSelected;
        distributeActionEvent();
    }
}

