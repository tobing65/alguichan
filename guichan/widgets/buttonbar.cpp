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

#include "guichan/widgets/buttonbar.hpp"

#include "guichan/widgets/button.hpp"
#include "guichan/widgets/checkbox.hpp"
//#include "guichan/widgets/dropdown.hpp"
#include "guichan/widgets/imagebutton.hpp"
#include "guichan/widgets/label.hpp"
#include "guichan/widgets/listbox.hpp"
#include "guichan/widgets/radiobutton.hpp"
//#include "guichan/widgets/textbox.hpp"
#include "guichan/widgets/textfield.hpp"


using namespace gcn;


//----------------------------------------------------------------------------

ButtonVBar::ButtonVBar()
: int_spacing_(1)
, border_spacing_(0)
{
    setOpaque(false);
}

void ButtonVBar::setSpacing(unsigned int int_sp, unsigned int border_sp)
{
    int_spacing_=int_sp;
    border_spacing_=border_sp;
}

void ButtonVBar::setWidth(int width)
{
    WidgetListIterator iter;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
    {
        Widget* pw = *iter;

        ButtonVBar* pb = dynamic_cast<ButtonVBar*>(pw);
        if(pb)
        {
            pb->setWidth(width - 2*border_spacing_);
        }
        else
        {
            pw->setWidth(width - 2*border_spacing_);
        }
    }
    Container::setWidth(width);
}

void ButtonVBar::adjustSizeAndButtons(ButtonBarAdjustMode show_invisible_and_disabled)
{
    WidgetListIterator iter;
    int max_width=0;
    int cur_y=border_spacing_;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
    {
        Widget*pw = *iter;
        switch(show_invisible_and_disabled)
        {
        case BBAdjustEnabled:
            if(!pw->isEnabled())
                continue;
        case BBAdjustVisible:
            if(!pw->isVisible())
                continue;
        case BBAdjustAll:
            break;
        }

        {
            gcn::Button*pb = dynamic_cast<gcn::Button*>(pw);
            if(pb)
                pb->adjustSize();
        }
        {
            gcn::ImageButton*pb = dynamic_cast<gcn::ImageButton*>(pw);
            if(pb)
                pb->adjustSize();
        }
        {
            gcn::CheckBox*pc = dynamic_cast<gcn::CheckBox*>(pw);
            if(pc)
                pc->adjustSize();
        }
        {
            //gcn::DropDown*pc = dynamic_cast<gcn::DropDown*>(pw);
            //if(pc)
            //    pc->adjustSize();
        }
        {
            gcn::Label*pl = dynamic_cast<gcn::Label*>(pw);
            if(pl)
                pl->adjustSize();
        }
        {
            //gcn::TextBox*pt = dynamic_cast<gcn::TextBox*>(pw);
            //if(pt)
            //    pt->adjustSize();
        }
        {
            gcn::TextField*pt = dynamic_cast<gcn::TextField*>(pw);
            if(pt)
                pt->adjustSize();
        }
        {
            gcn::ListBox*pl = dynamic_cast<gcn::ListBox*>(pw);
            if(pl)
                pl->adjustSize();
        }
        {
            gcn::RadioButton*pr = dynamic_cast<gcn::RadioButton*>(pw);
            if(pr)
                pr->adjustSize();
        }
        {
            ButtonHBar*pb = dynamic_cast<ButtonHBar*>(pw);
            if(pb)
                pb->adjustSizeAndButtons(show_invisible_and_disabled);
        }
        {
            ButtonVBar*pb = dynamic_cast<ButtonVBar*>(pw);
            if(pb)
                pb->adjustSizeAndButtons(show_invisible_and_disabled);
        }
        const int cur_width = pw->getWidth();
        if(max_width < cur_width)
            max_width = cur_width;
        pw->setPosition(border_spacing_,cur_y);
        cur_y += pw->getHeight();
        cur_y += int_spacing_;
    }
    setWidth(max_width+2*border_spacing_);
    setHeight(cur_y);
}

//----------------------------------------------------------------------------

ButtonHBar::ButtonHBar()
: int_spacing_(1)
, border_spacing_(0)
{
    setOpaque(false);
}

void ButtonHBar::setSpacing(unsigned int int_sp, unsigned int border_sp)
{
    int_spacing_=int_sp;
    border_spacing_=border_sp;
}

void ButtonHBar::setHeight(int height)
{
    WidgetListIterator iter;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
    {
        Widget* pw = *iter;

        ButtonHBar* pb = dynamic_cast<ButtonHBar*>(pw);
        if(pb)
        {
            pb->setHeight(height - 2*border_spacing_);
        }
        else
        {
            pw->setHeight(height - 2*border_spacing_);
        }
    }
    Container::setHeight(height);
}

void ButtonHBar::adjustSizeAndButtons(ButtonBarAdjustMode show_invisible_and_disabled)
{
    WidgetListIterator iter;
    int max_height=0;
    int cur_x=0;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
    {
        Widget*pw = *iter;
        switch(show_invisible_and_disabled)
        {
        case BBAdjustEnabled:
            if(!pw->isEnabled())
                continue;
        case BBAdjustVisible:
            if(!pw->isVisible())
                continue;
        case BBAdjustAll:
            break;
        }

        {
            gcn::Button*pb = dynamic_cast<gcn::Button*>(pw);
            if(pb)
                pb->adjustSize();
        }
        {
            gcn::ImageButton*pb = dynamic_cast<gcn::ImageButton*>(pw);
            if(pb)
                pb->adjustSize();
        }
        {
            gcn::CheckBox*pc = dynamic_cast<gcn::CheckBox*>(pw);
            if(pc)
                pc->adjustSize();
        }
        {
            gcn::Label*pl = dynamic_cast<gcn::Label*>(pw);
            if(pl)
                pl->adjustSize();
        }
        {
            gcn::ListBox*pl = dynamic_cast<gcn::ListBox*>(pw);
            if(pl)
                pl->adjustSize();
        }
        {
            gcn::RadioButton*pr = dynamic_cast<gcn::RadioButton*>(pw);
            if(pr)
                pr->adjustSize();
        }
        {
            ButtonHBar*pb = dynamic_cast<ButtonHBar*>(pw);
            if(pb)
                pb->adjustSizeAndButtons(show_invisible_and_disabled);
        }
        {
            ButtonVBar*pb = dynamic_cast<ButtonVBar*>(pw);
            if(pb)
                pb->adjustSizeAndButtons(show_invisible_and_disabled);
        }
        const int cur_height = pw->getHeight();
        if(max_height < cur_height)
            max_height = cur_height;
        pw->setPosition(cur_x,border_spacing_);
        cur_x += int_spacing_;
        cur_x += pw->getWidth();
    }
    setWidth(cur_x);
    setHeight(max_height+2*border_spacing_);
}

//----------------------------------------------------------------------------

