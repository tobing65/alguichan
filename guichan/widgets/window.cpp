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

#include "guichan/widgets/window.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    Window::Window()
        : mMoved(false)
    {
        setFrameSize(getStyle()->getDefaultWindowFrameSize());
        setPadding(2);
        setTitleBarHeight(16);
        setAlignment(Graphics::Center);
        addMouseListener(this);
        setMovable(true);
        setOpaque(true);
    }

    Window::Window(const std::string& caption)
        : mMoved(false)
    {
        setFrameSize(getStyle()->getDefaultWindowFrameSize());
        setCaption(caption);
        setPadding(2);
        setTitleBarHeight(16);
        setAlignment(Graphics::Center);
        addMouseListener(this);
        setMovable(true);
        setOpaque(true);
    }

    Window::Window(const char* caption)
        : mMoved(false)
    {
        setFrameSize(getStyle()->getDefaultWindowFrameSize());
        setCaption(caption);
        setPadding(2);
        setTitleBarHeight(16);
        setAlignment(Graphics::Center);
        addMouseListener(this);
        setMovable(true);
        setOpaque(true);
    }

    Window::~Window()
    {
    }

    void Window::setPadding(unsigned int padding)
    {
        mPadding = padding;
    }

    unsigned int Window::getPadding() const
    {
        return mPadding;
    }

    void Window::setTitleBarHeight(unsigned int height)
    {
        mTitleBarHeight = height;
    }

    unsigned int Window::getTitleBarHeight() const
    {
        return mTitleBarHeight;
    }

    void Window::setCaption(const std::string& caption)
    {
        mCaption=caption;
    }

    void Window::setCaption(const char* caption)
    {
      if(caption)
        mCaption.assign(caption);
      else
        mCaption.clear();
    }

    const char* Window::getCaption() const
    {
        return mCaption.c_str();
    }

    void Window::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment Window::getAlignment() const
    {
        return mAlignment;
    }

    void Window::draw(Graphics* graphics)
    {
        const Style* style = getStyle();
        const Color &faceColor = getBaseColor();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        Rectangle d = getChildrenArea();

        // Fill the background around the content
        graphics->setColor(faceColor);
        // Fill top
        graphics->fillRectangle(0, 0, getWidth(), d.y - 1);
        // Fill left
        graphics->fillRectangle(0, d.y - 1, d.x - 1, getHeight() - d.y + 1);
        // Fill right
        graphics->fillRectangle(d.x + d.width + 1,
                                d.y - 1,
                                getWidth() - d.x - d.width - 1,
                                getHeight() - d.y + 1);
        // Fill bottom
        graphics->fillRectangle(d.x - 1,
                                d.y + d.height + 1,
                                d.width + 2,
                                getHeight() - d.height - d.y - 1);

        if (isOpaque())
        {
            graphics->fillRectangle(d.x, d.y, d.width, d.height);
        }

        // Construct a rectangle one pixel bigger than the content
        d.x -= 1;
        d.y -= 1;
        d.width += 2;
        d.height += 2;

        // Draw a border around the content
        graphics->setColor(shadowColor);
        // Top line
        graphics->drawLine(d.x,
                           d.y,
                           d.x + d.width - 2,
                           d.y);

        // Left line
        graphics->drawLine(d.x,
                           d.y + 1,
                           d.x,
                           d.y + d.height - 1);

        graphics->setColor(highlightColor);
        // Right line
        graphics->drawLine(d.x + d.width - 1,
                           d.y,
                           d.x + d.width - 1,
                           d.y + d.height - 2);
        // Bottom line
        graphics->drawLine(d.x + 1,
                           d.y + d.height - 1,
                           d.x + d.width - 1,
                           d.y + d.height - 1);

        drawChildren(graphics);

        int textX;
        int textY;

        textY = ((int)getTitleBarHeight() - getFont()->getHeight()) / 2;

        switch (getAlignment())
        {
          case Graphics::Left:
              textX = 4;
              break;
          case Graphics::Center:
              textX = getWidth() / 2;
              break;
          case Graphics::Right:
              textX = getWidth() - 4;
              break;
          default:
              throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        graphics->pushClipArea(Rectangle(0, 0, getWidth(), getTitleBarHeight() - 1));
        graphics->drawText(getCaption(), textX, textY, getAlignment());
        graphics->popClipArea();
    }

    void Window::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getSource() != this)
        {
            return;
        }

        if (getParent())
        {
            getParent()->moveToTop(this);
        }

        mDragOffsetX = mouseEvent.getX();
        mDragOffsetY = mouseEvent.getY();

        mMoved = mouseEvent.getY() <= (int)mTitleBarHeight;
    }

    void Window::mouseReleased(MouseEvent& mouseEvent)
    {
        mMoved = false;
    }

    void Window::mouseDragged(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed() || mouseEvent.getSource() != this)
        {
            return;
        }

        if (isMovable() && mMoved)
        {
            setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
                        mouseEvent.getY() - mDragOffsetY + getY());
        }

        mouseEvent.consume();
    }

    Rectangle Window::getChildrenArea() const
    {
        return Rectangle(getPadding(),
                         getTitleBarHeight(),
                         getWidth() - getPadding() * 2,
                         getHeight() - getPadding() - getTitleBarHeight());
    }

    void Window::setMovable(bool movable)
    {
        mMovable = movable;
    }

    bool Window::isMovable() const
    {
        return mMovable;
    }

    void Window::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool Window::isOpaque() const
    {
        return mOpaque;
    }

    void Window::resizeToContent()
    {
        BasicContainer::resizeToContent();
        setSize(getWidth() + 2* getPadding(), 
                getHeight() + getPadding() + getTitleBarHeight());
    }
}
