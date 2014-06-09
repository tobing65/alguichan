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

#include "guichan/widgets/radiobutton.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    RadioButton::GroupMap RadioButton::mGroupMap;

    RadioButton::RadioButton()
    {
        setSelected(false);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    RadioButton::RadioButton(const std::string& caption, const std::string& group, bool selected)
    {
      setCaption(caption);
      setGroup(group);
      setSelected(selected);

      setFocusable(true);
      addMouseListener(this);
      addKeyListener(this);

      adjustSize();
    }

    RadioButton::RadioButton(const char* caption, const char* group, bool selected)
    {
      setCaption(caption);
      setGroup(group);
      setSelected(selected);

      setFocusable(true);
      addMouseListener(this);
      addKeyListener(this);

      adjustSize();
    }

    RadioButton::~RadioButton()
    {
        // Remove us from the group list
        setGroup("");
    }

    void RadioButton::draw(Graphics* graphics)
    {
        graphics->pushClipArea(Rectangle(1,
                                         1,
                                         getWidth() - 1,
                                         getHeight() - 1));
        drawBox(graphics);
        graphics->popClipArea();

        const Style*style = getStyle();
        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());

        if (isFocused())
        {
            int fh;
            
            if (getHeight()%2 == 0)
            {
                fh = getHeight() - 4;
            }
            else
            {
                fh = getHeight() - 3;
            }

            int hh = (fh + 1) / 2;
        
            graphics->drawLine(0, hh + 1, hh + 1, 0);
            graphics->drawLine(hh + 2, 1, fh + 2, hh + 1);
            graphics->drawLine(fh + 1, hh + 2, hh + 1, fh + 2);
            graphics->drawLine(hh + 1, fh + 2, 1, hh + 2);            
        }
        
        int h = getHeight() + getHeight() / 2;

        Color textColor = getForegroundColor();
        if(!isEnabled())
            textColor.a *= style->getDisabledOpacity();
        graphics->setColor(textColor);

        graphics->drawText(getCaption(), h - 2, 0);
    }

    void RadioButton::drawBox(Graphics *graphics)
    {
        int h = getHeight();

        if(h%2 == 0)
        {
            h -= 4;
        }
        else
        {
            h -= 3;
        }

        const Style* style = getStyle();
        Color faceColor = getBaseColor();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        Color fgColor = getBackgroundColor();
        Color disabledColor = style->getDisabledColor(fgColor, getForegroundColor());
        graphics->setColor(isEnabled() ? fgColor : disabledColor);

        int i;
        int hh = (h + 1) / 2;

        for (i = 1; i <= hh; ++i)
        {
            graphics->drawLine(hh - i + 1,
                               i,
                               hh + i - 1,
                               i);
        }

        for (i = 1; i < hh; ++i)
        {
            graphics->drawLine(hh - i + 1,
                               h - i,
                               hh + i - 1,
                               h - i);
        }

        graphics->setColor(shadowColor);
        graphics->drawLine(hh, 0, 0, hh);
        graphics->drawLine(hh + 1, 1, h - 1, hh - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(1, hh + 1, hh, h);
        graphics->drawLine(hh + 1, h - 1, h, hh);

        graphics->setColor(getForegroundColor());

        int hhh = hh - 3;
        if (mSelected)
        {
            for (i = 0; i < hhh; ++i)
            {
                graphics->drawLine(hh - i, 4 + i, hh + i, 4 + i);
            }
            for (i = 0; i < hhh; ++i)
            {
                graphics->drawLine(hh - i, h - 4 - i, hh + i, h - 4 -  i);
            }

        }
    }

    bool RadioButton::isSelected() const
    {
        return mSelected;
    }

    void RadioButton::setSelected(bool selected)
    {
        if (selected && mGroup != "")
        {
            GroupIterator iter, iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup);
                 iter != iterEnd;
                 iter++)
            {
                if (iter->second->isSelected())
                {
                    iter->second->setSelected(false);
                }
            }
        }

        mSelected = selected;
    }

    const char* RadioButton::getCaption() const
    {
        return mCaption.c_str();
    }

    void RadioButton::setCaption(const std::string& caption)
    {
        mCaption=caption;
    }

    void RadioButton::setCaption(const char* caption)
    {
      if(caption)
        mCaption.assign(caption);
      else
        mCaption.clear();
    }

    void RadioButton::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter ||
            key.getValue() == Key::Space)
        {
            setSelected(true);
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void RadioButton::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            setSelected(true);
            distributeActionEvent();
        }
    }

    void RadioButton::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void RadioButton::setGroup(const std::string& group)
    {
        if (mGroup != "")
        {
            GroupIterator iter, iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup);
                 iter != iterEnd;
                 iter++)
            {
                if (iter->second == this)
                {
                    mGroupMap.erase(iter);
                    break;
                }
            }
        }

        if (group != "")
        {
            mGroupMap.insert(
                std::pair<std::string, RadioButton *>(group, this));
        }

        mGroup = group;
    }

    void RadioButton::setGroup(const char* group)
    {
      std::string tmp_group(group);
      setGroup(tmp_group);
    }

    const char* RadioButton::getGroup() const
    {
        return mGroup.c_str();
    }

    void RadioButton::adjustSize()
    {
        int height = getFont()->getHeight();

        setHeight(height);
        setWidth(getFont()->getWidth(getCaption()) + height + height/2);
    }
}
