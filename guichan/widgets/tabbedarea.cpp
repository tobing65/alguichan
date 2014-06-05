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

#include "guichan/widgets/tabbedarea.hpp"

#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/style.hpp"

#include "guichan/widgets/container.hpp"
#include "guichan/widgets/tab.hpp"

#include <algorithm>

namespace gcn
{
    TabbedArea::TabbedArea()
      : mSelectedTab(NULL)
      , mOpaque(false)
    {
        setFocusable(true);
        addKeyListener(this);
        addMouseListener(this);

        mTabContainer = new Container();
        mTabContainer->setOpaque(false);
        mWidgetContainer = new Container();

        add(mTabContainer);
        add(mWidgetContainer);
    }

    TabbedArea::~TabbedArea()
    {
        remove(mTabContainer);
        remove(mWidgetContainer);

        delete mTabContainer;
        delete mWidgetContainer;

        for (unsigned int i = 0; i < mTabsToDelete.size(); i++)
        {
            delete mTabsToDelete[i];
        }
    }

    void TabbedArea::addTab(const std::string& caption, Widget* widget)
    {
      Tab* tab = new Tab();
      tab->setCaption(caption);
      mTabsToDelete.push_back(tab);

      addTab(tab, widget);
    }

    void TabbedArea::addTab(const char* caption, Widget* widget)
    {
        Tab* tab = new Tab();
        tab->setCaption(caption);
        mTabsToDelete.push_back(tab);

        addTab(tab, widget);
    }

    void TabbedArea::addTab(Tab* tab, Widget* widget)
    {
        tab->setTabbedArea(this);
        tab->addActionListener(this);

        mTabContainer->add(tab);
        mTabs.push_back(std::pair<Tab*, Widget*>(tab, widget));

        if (mSelectedTab == NULL)
        {
            setSelectedTab(tab);
        }

        adjustTabPositions();
        adjustSize();
    }

    void TabbedArea::removeTabWithIndex(unsigned int index)
    {
        if (index >= mTabs.size())
        {
            throw GCN_EXCEPTION("No such tab index.");
        }

        removeTab(mTabs[index].first);
    }

    void TabbedArea::removeTab(Tab* tab)
    {
        int tabIndexToBeSelected = - 1;

        if (tab == mSelectedTab)
        {
            int index = getSelectedTabIndex();

            if (index == (int)mTabs.size() - 1
                && mTabs.size() >= 2)
            {
                tabIndexToBeSelected = index--;
            }
            else if (index == (int)mTabs.size() - 1
                     && mTabs.size() == 1)
            {
                tabIndexToBeSelected = -1;
            }
            else
            {
                tabIndexToBeSelected = index;
            }
        }

        std::vector<std::pair<Tab*, Widget*> >::iterator iter;
        for (iter = mTabs.begin(); iter != mTabs.end(); iter++)
        {
            if (iter->first == tab)
            {
                mTabContainer->remove(tab);
                mTabs.erase(iter);
                break;
            }
        }

        std::vector<Tab*>::iterator iter2;
        for (iter2 = mTabsToDelete.begin(); iter2 != mTabsToDelete.end(); iter2++)
        {
            if (*iter2 == tab)
            {
                mTabsToDelete.erase(iter2);
                delete tab;
                break;
            }
        }

        if (tabIndexToBeSelected == -1)
        {
            mSelectedTab = NULL;
            mWidgetContainer->clear();
        }
        else
        {
            setSelectedTab(tabIndexToBeSelected);
        }

        adjustSize();
        adjustTabPositions();
    }

    gcn::Tab* TabbedArea::getTab(unsigned int index) const
    {
        if (index >= mTabs.size())
        {
            throw GCN_EXCEPTION("No such tab index.");
        }

        return mTabs[index].first;
    }

    int TabbedArea::getNumberOfTabs() const
    {
        return mTabs.size();
    }

    void TabbedArea::adjustSizeFromTabs()
    {
        int max_w = 0;
        int max_h = 0;
        for(size_t i=0; i<mTabs.size(); ++i)
        {
            const Widget* pw = mTabs[i].second;
            if(max_w < pw->getWidth())
                max_w = pw->getWidth();
            if(max_h < pw->getHeight())
                max_h = pw->getHeight();
        }
        setWidth(max_w);
        setHeight(max_h + getTabHeight());
    }

    bool TabbedArea::isTabSelected(unsigned int index) const
    {
        if (index >= mTabs.size())
        {
            throw GCN_EXCEPTION("No such tab index.");
        }

        return mSelectedTab == mTabs[index].first;
    }

    bool TabbedArea::isTabSelected(Tab* tab) const
    {
        return mSelectedTab == tab;
    }

    void TabbedArea::setSelectedTab(unsigned int index)
    {
        if (index >= mTabs.size())
        {
            throw GCN_EXCEPTION("No such tab index.");
        }

        setSelectedTab(mTabs[index].first);
    }

    void TabbedArea::setSelectedTab(Tab* tab)
    {
        for (size_t i = 0; i < mTabs.size(); i++)
        {
            if (mTabs[i].first == mSelectedTab)
            {
                mWidgetContainer->remove(mTabs[i].second);
            }
        }

        for (size_t i = 0; i < mTabs.size(); i++)
        {
            if (mTabs[i].first == tab)
            {
                mSelectedTab = tab;
                mWidgetContainer->add(mTabs[i].second);
            }
        }
    }

    int TabbedArea::getSelectedTabIndex() const
    {
        for (size_t i = 0; i < mTabs.size(); i++)
        {
            if (mTabs[i].first == mSelectedTab)
            {
                return i;
            }
        }

        return -1;
    }

    Tab* TabbedArea::getSelectedTab() const
    {
        return mSelectedTab;
    }

    int TabbedArea::getTabHeight() const
    {
        int maxTabHeight = 0;
        for (unsigned int i = 0; i < mTabs.size(); i++)
        {
            if (mTabs[i].first->getHeight() > maxTabHeight)
            {
                maxTabHeight = mTabs[i].first->getHeight();
            }
        }
        return maxTabHeight;
    }

    void TabbedArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool TabbedArea::isOpaque() const
    {
        return mOpaque;
    }

    void TabbedArea::draw(Graphics *graphics)
    {
        const Color &faceColor = getBaseColor();
        const Style* style = getStyle();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        // Draw a border.
        graphics->setColor(highlightColor);
        graphics->drawLine(0,
                           mTabContainer->getHeight(),
                           0,
                           getHeight() - 2);
        graphics->setColor(shadowColor);
        graphics->drawLine(getWidth() - 1,
                           mTabContainer->getHeight() + 1,
                           getWidth() - 1,
                           getHeight() - 1);
        graphics->drawLine(1,
                           getHeight() - 1,
                           getWidth() - 1,
                           getHeight() - 1);

        if (isOpaque())
        {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(1, 1,
                                    getWidth() - 2,
                                    getHeight() - 2);
        }

        // Draw a line underneath the tabs.
        graphics->setColor(highlightColor);
        graphics->drawLine(1,
                           mTabContainer->getHeight(),
                           getWidth() - 1,
                           mTabContainer->getHeight());

        // If a tab is selected, remove the line right underneath
        // the selected tab.
        if (mSelectedTab != NULL)
        {
            graphics->setColor(getBaseColor());
            graphics->drawLine(mSelectedTab->getX() + 1,
                               mTabContainer->getHeight(),
                               mSelectedTab->getX() + mSelectedTab->getWidth() - 2,
                               mTabContainer->getHeight());

        }

        drawChildren(graphics);
    }

    void TabbedArea::adjustSize()
    {
        const int maxTabHeight = getTabHeight();

        mTabContainer->setSize(getWidth() - 2,
                               maxTabHeight);

        mWidgetContainer->setPosition(1, maxTabHeight + 1);
        mWidgetContainer->setSize(getWidth() - 2,
                                  getHeight() - maxTabHeight - 2);
    }

    void TabbedArea::adjustTabPositions()
    {
        const int maxTabHeight = getTabHeight();

        int x = 0;
        for (size_t i = 0; i < mTabs.size(); i++)
        {
            Tab* tab = mTabs[i].first;
            tab->setPosition(x, maxTabHeight - tab->getHeight());
            x += tab->getWidth();
        }
    }

    void TabbedArea::setWidth(int width)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setWidth as
        // Widget::setWidth might distribute a resize event.
        gcn::Rectangle dim = mDimension;
        mDimension.width = width;
        adjustSize();
        mDimension = dim;
        Widget::setWidth(width);
    }

    void TabbedArea::setHeight(int height)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setHeight as
        // Widget::setHeight might distribute a resize event.
        gcn::Rectangle dim = mDimension;
        mDimension.height = height;
        adjustSize();
        mDimension = dim;
        Widget::setHeight(height);
    }

    void TabbedArea::setSize(int width, int height)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setSize as
        // Widget::setSize might distribute a resize event.
        gcn::Rectangle dim = mDimension;
        mDimension.width = width;
        mDimension.height = height;
        adjustSize();
        mDimension = dim;
        Widget::setSize(width, height);
    }

    void TabbedArea::setDimension(const Rectangle& dimension)
    {
        // This may seem odd, but we want the TabbedArea to adjust
        // it's size properly before we call Widget::setDimension as
        // Widget::setDimension might distribute a resize event.
        gcn::Rectangle dim = mDimension;
        mDimension = dimension;
        adjustSize();
        mDimension = dim;
        Widget::setDimension(dimension);      
    }

    void TabbedArea::keyPressed(KeyEvent& keyEvent)
    {
        if (keyEvent.isConsumed() || !isFocused())
        {
            return;
        }

        if (keyEvent.getKey().getValue() == Key::Left)
        {
            int index = getSelectedTabIndex();
            index--;

            if (index < 0)
            {
                return;
            }
            else
            {
                setSelectedTab(mTabs[index].first);
            }

            keyEvent.consume();
        }
        else if (keyEvent.getKey().getValue() == Key::Right)
        {
            int index = getSelectedTabIndex();
            index++;

            if (index >= (int)mTabs.size())
            {
                return;
            }
            else
            {
                setSelectedTab(mTabs[index].first);
            }

            keyEvent.consume();
        }
    }


    void TabbedArea::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
        {
            return;
        }

        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            Widget* widget = mTabContainer->getWidgetAt(mouseEvent.getX(), mouseEvent.getY());
            Tab* tab = dynamic_cast<Tab*>(widget);

            if (tab != NULL)
            {
                setSelectedTab(tab);
            }
        }

        // Request focus only if the source of the event
        // is not focusble. If the source of the event
        // is focused we don't want to steal the focus.
        if (!mouseEvent.getSource()->isFocusable())
        {
            requestFocus();
        }
    }

    void TabbedArea::death(const Event& event)
    {
        Widget* source = event.getSource();
        Tab* tab = dynamic_cast<Tab*>(source);

        if (tab != NULL)
        {
            removeTab(tab);
        }
        else
        {
            BasicContainer::death(event);
        }
    }

    void TabbedArea::action(const ActionEvent& actionEvent)
    {
        Widget* source = actionEvent.getSource();
        Tab* tab = dynamic_cast<Tab*>(source);

        if (tab == NULL)
        {
            throw GCN_EXCEPTION("Received an action from a widget that's not a tab!");
        }

        setSelectedTab(tab);
    }

    void TabbedArea::setBaseColor(const Color& color)
    {
        Widget::setBaseColor(color);
        mWidgetContainer->setBaseColor(color);
        mTabContainer->setBaseColor(color);
    }
}
