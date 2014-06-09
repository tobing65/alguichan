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

#include "guichan/widgets/button.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    ButtonBase::ButtonBase()
        : mHasMouse(false)
        , mKeyPressed(false)
        , mMousePressed(false)
        , mIsOpaque(true)
        , mAlignment(Graphics::Center)
        , mSpacing(4)
    {
        setFrameSize(getStyle()->getDefaultButtonFrameSize());
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    void ButtonBase::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment ButtonBase::getAlignment() const
    {
        return mAlignment;
    }

    void ButtonBase::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
    }

    unsigned int ButtonBase::getSpacing() const
    {
        return mSpacing;
    }

    void ButtonBase::setOpaque(bool isOpaque)
    {
        mIsOpaque = isOpaque;
    }

    bool ButtonBase::isPressed() const
    {
        if (mMousePressed)
        {
            return mHasMouse;
        }
        else
        {
            return mKeyPressed;
        }
    }

    void ButtonBase::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void ButtonBase::mouseExited(MouseEvent& mouseEvent)
    {
        mHasMouse = false;
    }

    void ButtonBase::mouseEntered(MouseEvent& mouseEvent)
    {
        mHasMouse = true;
    }

    void ButtonBase::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left
            && mMousePressed
            && mHasMouse)
        {
            mMousePressed = false;
            distributeActionEvent();
            mouseEvent.consume();
        }
        else if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

    void ButtonBase::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ButtonBase::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter
            || key.getValue() == Key::Space)
        {
            mKeyPressed = true;
            keyEvent.consume();
        }
    }

    void ButtonBase::keyReleased(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter
            || key.getValue() == Key::Space)
            && mKeyPressed)
        {
            mKeyPressed = false;
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void ButtonBase::focusLost(const Event& event)
    {
        mMousePressed = false;
        mKeyPressed = false;
    }

    bool ButtonBase::triggerAction()
    {
        if(isEnabled())
        {
            distributeActionEvent();
            return true;
        }
        return false;
    }

    void ButtonBase::triggerAction(KeyEvent& keyEvent)
    {
        if(isEnabled())
        {
            distributeActionEvent();
            keyEvent.consume();
        }
    }







    Button::Button()
        : ButtonBase()
    {
        adjustSize();
    }

    Button::Button(const std::string& caption)
        : ButtonBase()
        , mCaption(caption)
    {
        adjustSize();
    }

    Button::Button(const char* caption)
        : ButtonBase()
        , mCaption()
    {
        if(caption)
            mCaption.assign(caption);
        adjustSize();
    }

    void Button::setCaption(const std::string& caption)
    {
        mCaption=caption;
    }

    void Button::setCaption(const char* caption)
    {
        if(caption)
            mCaption.assign(caption);
        else
            mCaption.clear();
    }

    const char* Button::getCaption() const
    {
        return mCaption.c_str();
    }

    void Button::adjustSize()
    {
        setWidth(getFont()->getWidth(mCaption) + 2*mSpacing);
        setHeight(getFont()->getHeight() + 2*mSpacing);
    }

    void Button::draw(Graphics* graphics)
    {
        const Style*style = getStyle();
        if(mIsOpaque)
        {
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

            graphics->setColor(faceColor);
            graphics->fillRectangle(1, 1, getDimension().width-1, getHeight() - 1);

            graphics->setColor(highlightColor);
            graphics->drawLine(0, 0, getWidth() - 1, 0);
            graphics->drawLine(0, 1, 0, getHeight() - 1);

            graphics->setColor(shadowColor);
            graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
            graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
        }

        Color textColor = getForegroundColor();
        if(!isEnabled())
            textColor.a *= style->getDisabledOpacity();
        graphics->setColor(textColor);

        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        switch (getAlignment())
        {
        case Graphics::Left:
            textX = mSpacing;
            break;
        case Graphics::Center:
            textX = getWidth() / 2;
            break;
        case Graphics::Right:
            textX = getWidth() - mSpacing;
            break;
        default:
            throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setFont(getFont());

        if (isPressed())
        {
            graphics->drawText(getCaption(), textX + 1, textY + 1, getAlignment());
        }
        else
        {
            graphics->drawText(getCaption(), textX, textY, getAlignment());
            if (isFocused())
            {
                graphics->drawRectangle(2, 2, getWidth()-4, getHeight()-4);
            }
        }
    }
}
