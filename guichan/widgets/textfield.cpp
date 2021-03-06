/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2009 Olof Naess�n and Per Larsson
 * Copyright (c) 2010 - 2011 Olof Naess�n, Per Larsson and Tobias Scheuer
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

#include "guichan/widgets/textfield.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    TextField::TextField()
        : mCaretPosition(0)
        , mXScroll(0)
        , mXDisplacement(0)
        , mEditable(true)
        , mAlignment(Graphics::Left)
    {
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    TextField::TextField(const std::string& text)
        : mCaretPosition(0)
        , mXScroll(0)
        , mXDisplacement(0)
        , mEditable(true)
        , mAlignment(Graphics::Left)
    {
        mText = text;
        adjustSize();

        setFocusable(true);
        fixScroll();

        addMouseListener(this);
        addKeyListener(this);
    }

    void TextField::setText(const std::string& text)
    {
        if(text.size() < mCaretPosition )
        {
            mCaretPosition = text.size();
        }

        mText = text;
        fixScroll();
    }

    void TextField::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment=alignment;
        fixScroll();
    }

    Graphics::Alignment TextField::getAlignment() const
    {
        return mAlignment;
    }

    void TextField::draw(Graphics* graphics)
    {
        const Color& faceColor = getBaseColor();
        const Style* style = getStyle();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        // Draw a border.
        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, getHeight() - 2);
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(0, getHeight() - 1, getWidth() - 1, getHeight() - 1);

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if (isFocused())
        {
            graphics->setColor(getSelectionColor());
            graphics->drawRectangle(0, 0, getWidth() - 2, getHeight() - 2);
            graphics->drawRectangle(1, 1, getWidth() - 4, getHeight() - 4);
        }

        if (isFocused() && isEditable())
        {
            drawCaret(graphics, getFont()->getWidth(mText.substr(0, mCaretPosition)) - mXDisplacement - mXScroll);
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        graphics->drawText(mText, 1 - mXDisplacement - mXScroll, 1);

        graphics->popClipArea();
    }

    void TextField::drawCaret(Graphics* graphics, int x)
    {
        const bool blink=getStyle()->getBlink();
        if(blink)
            return;

        // Check the current clip area as a clip area with a different
        // size than the widget might have been pushed (which is the
        // case in the draw method when we push a clip area after we have
        // drawn a border).
        const Rectangle clipArea = graphics->getCurrentClipArea();

        graphics->setColor(getForegroundColor());
        const int caretWidth = getStyle()->getCaretWidth();
        for(int i=0; i<caretWidth; ++i)
        {
            graphics->drawLine(x, clipArea.height - 2, x, 1);
            ++x;
        }
    }

    void TextField::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mCaretPosition = getFont()->getStringIndexAt(mText, mouseEvent.getX() + mXScroll + mXDisplacement);
            fixScroll();
        }
    }

    void TextField::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextField::keyPressed(KeyEvent& keyEvent)
    {
        const Key key = keyEvent.getKey();

        if (key.getValue() == Key::Left && mCaretPosition > 0)
        {
            --mCaretPosition;
        }
        else if (key.getValue() == Key::Right && mCaretPosition < mText.size())
        {
            ++mCaretPosition;
        }
        else if (key.getValue() == Key::Delete && mCaretPosition < mText.size() && mEditable)
        {
            mText.erase(mCaretPosition, 1);
        }
        else if (key.getValue() == Key::Backspace && mCaretPosition > 0 && mEditable)
        {
            mText.erase(mCaretPosition - 1, 1);
            --mCaretPosition;
        }
        else if (key.getValue() == Key::Enter)
        {
            distributeActionEvent();
        }
        else if (key.getValue() == Key::Home)
        {
            mCaretPosition = 0;
        }
        else if (key.getValue() == Key::End)
        {
            mCaretPosition = mText.size();
        }
        else if (key.isCharacter() && key.getValue() != Key::Tab && mEditable)
        {
            mText.insert(mCaretPosition, std::string(1,(char)key.getValue()));
            ++mCaretPosition;
        }

        if (key.getValue() != Key::Tab)
        {
            keyEvent.consume();
        }

        fixScroll();
    }

    void TextField::adjustSize()
    {
        setWidth(getFont()->getWidth(mText) + 6);
        adjustHeight();

        fixScroll();
    }

    void TextField::adjustHeight()
    {
        setHeight(getFont()->getHeight() + 4);
    }

    void TextField::fixScroll()
    {
        switch(mAlignment)
        {
        case Graphics::Left:
            mXDisplacement=0;
            break;
        case Graphics::Center:
            mXDisplacement=(getFont()->getWidth(mText) - getWidth())/2;
            break;
        case Graphics::Right:
            mXDisplacement=getFont()->getWidth(mText) - getWidth();
            break;
        }

        if(isFocused())
        {

            const int caretX = getFont()->getWidth(mText.substr(0, mCaretPosition));

            if (caretX - mXDisplacement - mXScroll >= getWidth() - 4)
            {
                mXScroll = caretX - mXDisplacement - getWidth() + 4;
            }
            else if (caretX - mXDisplacement - mXScroll <= 0)
            {
                mXScroll = caretX - mXDisplacement - getWidth() / 2;

                if (mXScroll < -mXDisplacement)
                {
                    mXScroll = -mXDisplacement;
                }
            }
        }
    }

    void TextField::setCaretPosition(unsigned int position)
    {
        if (position > mText.size())
        {
            mCaretPosition = mText.size();
        }
        else
        {
            mCaretPosition = position;
        }

        fixScroll();
    }

    unsigned int TextField::getCaretPosition() const
    {
        return mCaretPosition;
    }

    const std::string& TextField::getText() const
    {
        return mText;
    }

    void TextField::fontChanged()
    {
        fixScroll();
    }

    bool TextField::isEditable() const
    {
        return mEditable;
    }

    void TextField::setEditable(bool editable)
    {
        mEditable = editable;
    }
}
