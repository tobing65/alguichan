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

#include "guichan/ext/overlay.hpp"

using namespace gcn;



//----------------------------------------------------------------------------

OverlayPlugin::OverlayPlugin()
    : p_container_(0)
    , border_width_(1)
    , offset_(24)
    , p_current_widget_(0)
    , mouse_x_(0)
    , mouse_y_(0)
{
    floatmsg_text_.setEditable(false);
    floatmsg_text_.setPosition(border_width_, border_width_);
    floatmsg_box_.add(&floatmsg_text_);
}

void OverlayPlugin::draw(gcn::Graphics* graphics)
{
}

void OverlayPlugin::logic()
{
    if(p_current_widget_)
        handleOverlay();
}

void OverlayPlugin::mouseExited(gcn::MouseEvent& mouse_event)
{
    p_current_widget_ = 0;
    floatmsg_box_.setVisible(false);
}

void OverlayPlugin::mouseMoved(gcn::MouseEvent& mouse_event)
{
    p_current_widget_ = mouse_event.getSource();
    mouse_x_ = mouse_event.getX();
    mouse_y_ = mouse_event.getY();
    handleOverlay();
}

void OverlayPlugin::mouseDragged(gcn::MouseEvent& mouse_event)
{
    p_current_widget_ = mouse_event.getSource();
    mouse_x_ = mouse_event.getX();
    mouse_y_ = mouse_event.getY();
    floatmsg_box_.setVisible(false);
    //handleOverlay();
}

void OverlayPlugin::mouseReleased(gcn::MouseEvent& mouse_event)
{
    p_current_widget_ = mouse_event.getSource();
    mouse_x_ = mouse_event.getX();
    mouse_y_ = mouse_event.getY();
    handleOverlay();
}

int OverlayPlugin::alignX(int mx, int my, gcn::Widget* p, OverlayHint hint) const
{
    int px = 0;
    int py = 0;
    p->getAbsolutePosition(px, py);

    if(hint == OVL_UP || hint == OVL_DOWN)
    {
        return px;
    }
    else if(hint == OVL_RIGHT)
    {
        int posx = px + p->getWidth() + offset_;
        if(posx + floatmsg_box_.getWidth() + offset_ <= p_container_->getWidth())
            return posx;
    }
    else if(hint == OVL_LEFT)
    {
        int posx = px - offset_ - floatmsg_box_.getWidth();
        if(posx >= offset_)
            return posx;
    }

    int posx = mx + offset_;
    if(posx + floatmsg_box_.getWidth() + offset_ <= p_container_->getWidth())
        return posx;

    posx = p_container_->getWidth() - floatmsg_box_.getWidth() - offset_;
    return posx;
}

int OverlayPlugin::alignY(int mx, int my, gcn::Widget* p, OverlayHint hint) const
{
    int px = 0;
    int py = 0;
    p->getAbsolutePosition(px, py);

    if(hint == OVL_LEFT || hint == OVL_RIGHT)
    {
        return py;
    }
    else if(hint == OVL_DOWN)
    {
        int posy = py + p->getHeight() + offset_;
        if(posy + floatmsg_box_.getHeight() + offset_ <= p_container_->getHeight())
            return posy;
    }
    else if(hint == OVL_UP)
    {
        int posy = py - offset_ - floatmsg_box_.getHeight();
        if(posy >= offset_)
            return posy;
    }

    int posy = my + offset_;
    if(posy + floatmsg_box_.getHeight() + offset_ <= p_container_->getHeight())
        return posy;

    posy = p_container_->getHeight() - floatmsg_box_.getHeight() - offset_;
    return posy;
}

void OverlayPlugin::handleOverlay()
{
    OverlayHint hint = OVL_MOUSE;
    floatmsg_text_.setText(0);

    Widget*w = p_current_widget_;

    {
        OverlayCallback*p = dynamic_cast<OverlayCallback*>(w);
        if(p)
        {
            char omsg[16*1024] = "";
            size_t sz = p->getOverlayString(omsg, sizeof(omsg));
            if(sz > 0)
            {
                floatmsg_text_.setText(omsg);
                hint = p->hint_;
            }
        }
    }

    if(floatmsg_text_.getNumberOfRows() == 0)
    {
        hint = handleOverlayExt(w);
    }

    if(floatmsg_text_.getNumberOfRows() > 0)
    {
        floatmsg_box_.setSize(floatmsg_text_.getWidth() + 2 * border_width_, floatmsg_text_.getHeight() + 2 * border_width_);

        // find position
        int posx = alignX(mouse_x_, mouse_y_, w, hint);
        int posy = alignY(mouse_x_, mouse_y_, w, hint);
        floatmsg_box_.setPosition(posx, posy);
        floatmsg_box_.setVisible(true);
        floatmsg_text_.setBackgroundColor(getBackgroundColor());
        floatmsg_box_.setBaseColor(getBaseColor());
    }
    else
    {
        floatmsg_box_.setVisible(false);
    }
}

OverlayHint OverlayPlugin::handleOverlayExt(gcn::Widget* w)
{
    return OVL_MOUSE;
}

void OverlayPlugin::attachToContainer(gcn::Container* w)
{
    p_container_ = w;
    p_container_->addMouseListener(this);
    p_container_->add(&floatmsg_box_);
    p_container_->add(this);
    floatmsg_text_.setPosition(border_width_, border_width_);
    floatmsg_box_.setVisible(false);
}

