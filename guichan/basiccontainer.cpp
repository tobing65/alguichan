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

#include "guichan/basiccontainer.hpp"

#include <algorithm>

#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/graphics.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
    BasicContainer::BasicContainer()
        : mLogicIsProcessing(false)
    {

    }

    BasicContainer::~BasicContainer()
    {
        clear();
    }

    void BasicContainer::moveToTop(Widget* widget)
    {
        WidgetListIterator iter;
        iter = std::find(mWidgets.begin(), mWidgets.end(), widget);

        if (iter == mWidgets.end())
        {
            throw GCN_EXCEPTION("There is no such widget in this container.");
        }

        if (mLogicIsProcessing)
            mWidgetToBeMovedToTheTop = widget;
        else
            _moveToTopWithNoChecks(widget);
    }

    void BasicContainer::moveToBottom(Widget* widget)
    {
        WidgetListIterator iter;
        iter = find(mWidgets.begin(), mWidgets.end(), widget);

        if (iter == mWidgets.end())
        {
            throw GCN_EXCEPTION("There is no such widget in this container.");
        }

         if (mLogicIsProcessing)
            mWidgetToBeMovedToTheBottom = widget;
        else
            _moveToBottomWithNoChecks(widget);
    }

    void BasicContainer::death(const Event& event)
    {
        WidgetListIterator iter;
        iter = find(mWidgets.begin(), mWidgets.end(), event.getSource());

        if (iter == mWidgets.end())
        {
            throw GCN_EXCEPTION("There is no such widget in this container.");
        }

        mWidgets.erase(iter);
    }

    Rectangle BasicContainer::getChildrenArea() const
    {
        return Rectangle(0, 0, getWidth(), getHeight());
    }

    void BasicContainer::focusNext()
    {
        WidgetListIterator it;

        for (it = mWidgets.begin(); it != mWidgets.end(); it++)
        {
            if ((*it)->isFocused())
            {
                break;
            }
        }

        WidgetListIterator end = it;

        if (it == mWidgets.end())
        {
            it = mWidgets.begin();
        }

        it++;

        for ( ; it != end; it++)
        {
            if (it == mWidgets.end())
            {
                it = mWidgets.begin();
            }

            if ((*it)->isFocusable())
            {
                (*it)->requestFocus();
                return;
            }
        }
    }

    void BasicContainer::focusPrevious()
    {
        WidgetList::reverse_iterator it = mWidgets.rbegin();

        for (; it != mWidgets.rend(); it++)
        {
            if ((*it)->isFocused())
            {
                break;
            }
        }

        const WidgetList::reverse_iterator end = it;

        it++;

        if (it == mWidgets.rend())
        {
            it = mWidgets.rbegin();
        }

        for ( ; it != end; it++)
        {
            if (it == mWidgets.rend())
            {
                it = mWidgets.rbegin();
            }

            if ((*it)->isFocusable())
            {
                (*it)->requestFocus();
                return;
            }
        }
    }

    Widget *BasicContainer::getWidgetAt(int x, int y) const
    {
        const Rectangle r = getChildrenArea();

        if (!r.isContaining(x, y))
            return nullptr;

        x -= r.x;
        y -= r.y;

        for (WidgetList::const_reverse_iterator it = mWidgets.rbegin(); it != mWidgets.rend(); it++)
        {
            if ((*it)->isVisible() && (*it)->getDimension().isContaining(x, y))
            {
                return (*it);
            }
        }

        return nullptr;
    }

    void BasicContainer::logic()
    {
        mLogicIsProcessing = true;
        mWidgetToBeMovedToTheTop = nullptr;
        mWidgetToBeMovedToTheBottom = nullptr;
        logicChildren();
        mLogicIsProcessing = false;

        if (mWidgetToBeMovedToTheTop)
        {
            _moveToTopWithNoChecks(mWidgetToBeMovedToTheTop);
            mWidgetToBeMovedToTheTop = nullptr;
        }

        if (mWidgetToBeMovedToTheBottom)
        {
            _moveToTopWithNoChecks(mWidgetToBeMovedToTheBottom);
            mWidgetToBeMovedToTheBottom = nullptr;
        }
    }

    void BasicContainer::_setFocusHandler(FocusHandler* focusHandler)
    {
        Widget::_setFocusHandler(focusHandler);

        if (mInternalFocusHandler)
        {
            return;
        }

        WidgetListIterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            (*iter)->_setFocusHandler(focusHandler);
        }
    }

    void BasicContainer::add(Widget* widget)
    {
        mWidgets.push_back(widget);

        if (!mInternalFocusHandler)
        {
            widget->_setFocusHandler(_getFocusHandler());
        }
        else
        {
            widget->_setFocusHandler(mInternalFocusHandler);
        }

        widget->_setParent(this);
        widget->addDeathListener(this);
    }

    void BasicContainer::remove(Widget* widget)
    {
        WidgetListIterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            if (*iter == widget)
            {
                mWidgets.erase(iter);
                widget->_setFocusHandler(nullptr);
                widget->_setParent(nullptr);
                widget->removeDeathListener(this);
                return;
            }
        }

        throw GCN_EXCEPTION("There is no such widget in this container.");
    }

    void BasicContainer::clear()
    {
        WidgetListIterator iter;

        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            (*iter)->_setFocusHandler(nullptr);
            (*iter)->_setParent(nullptr);
            (*iter)->removeDeathListener(this);
        }

        mWidgets.clear();
    }

    void BasicContainer::drawChildren(Graphics* graphics)
    {
        graphics->pushClipArea(getChildrenArea());

        WidgetListIterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            if ((*iter)->isVisible())
            {
                // If the widget has a frame,
                // draw it before drawing the widget
                if ((*iter)->getFrameSize() > 0)
                {
                    Rectangle rec = (*iter)->getDimension();
                    rec.x -= (*iter)->getFrameSize();
                    rec.y -= (*iter)->getFrameSize();
                    rec.width += 2 * (*iter)->getFrameSize();
                    rec.height += 2 * (*iter)->getFrameSize();
                    graphics->pushClipArea(rec);
                    (*iter)->drawFrame(graphics);
                    graphics->popClipArea();
                }

                graphics->pushClipArea((*iter)->getDimension());
                (*iter)->draw(graphics);
                graphics->popClipArea();
            }
        }

        graphics->popClipArea();
    }

    void BasicContainer::logicChildren()
    {
        WidgetListIterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            (*iter)->logic();
        }
    }

    void BasicContainer::showWidgetPart(Widget* widget, Rectangle area)
    {
        Rectangle widgetArea = getChildrenArea();

        area.x += widget->getX();
        area.y += widget->getY();
        
        if (area.x + area.width > widgetArea.width)
        {
            widget->setX(widget->getX() - area.x - area.width + widgetArea.width);
        }

        if (area.y + area.height > widgetArea.height)
        {
            widget->setY(widget->getY() - area.y - area.height + widgetArea.height);
        }

        if (area.x < 0)
        {
            widget->setX(widget->getX() - area.x);
        }

        if (area.y < 0)
        {
            widget->setY(widget->getY() - area.y);
        }
    }

    void BasicContainer::setInternalFocusHandler(FocusHandler* focusHandler)
    {
        Widget::setInternalFocusHandler(focusHandler);

        WidgetListIterator iter;
        for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
        {
            if (!mInternalFocusHandler)
            {
                (*iter)->_setFocusHandler(_getFocusHandler());
            }
            else
            {
                (*iter)->_setFocusHandler(mInternalFocusHandler);
            }
        }
    }

    void BasicContainer::_moveToTopWithNoChecks(Widget* widget)
    {
        mWidgets.remove(widget);
        mWidgets.push_back(widget);
    }

    void BasicContainer::_moveToBottomWithNoChecks(Widget* widget)
    {
        mWidgets.remove(widget);
        mWidgets.push_front(widget);
    }

    void BasicContainer::resizeToContent()
    {
        WidgetListIterator it;

        int w = 0, h = 0;
        for (it = mWidgets.begin(); it != mWidgets.end(); it++)
        {
            if ((*it)->getX() + (*it)->getWidth() > w)
            {
                w = (*it)->getX() + (*it)->getWidth();
            }

            if ((*it)->getY() + (*it)->getHeight() > h)
            {
                h = (*it)->getY() + (*it)->getHeight();
            }
        }

        setSize(w, h);
    }
}
