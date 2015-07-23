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

#ifndef GCN_OVERLAY_HPP
#define GCN_OVERLAY_HPP

#include "guichan/widget.hpp"
#include "guichan/mouselistener.hpp"

#include "guichan/widgets/container.hpp"
#include "guichan/widgets/textbox.hpp"

#include <map>

namespace gcn
{
    //----------------------------------------------------------------------------

    enum OverlayHint { OVL_MOUSE, OVL_LEFT, OVL_RIGHT, OVL_UP, OVL_DOWN };

    class GCN_CORE_DECLSPEC OverlayCallback
    {
    public:

        OverlayCallback(OverlayHint h) : hint_(h)
        {
        }

        virtual size_t getOverlayString(char* msgbuf, size_t bufsize) const = 0;

    protected:
        friend class OverlayPlugin;

        OverlayHint hint_;
    };

    //----------------------------------------------------------------------------

    class GCN_CORE_DECLSPEC OverlayPlugin : public gcn::Widget, public gcn::MouseListener
    {
    public:
        OverlayPlugin();

        // Widget
        virtual void draw(gcn::Graphics* graphics);
        virtual void logic();

        // MouseListener
        //virtual void mouseEntered(gcn::MouseEvent& mouse_event);
        virtual void mouseExited(gcn::MouseEvent& mouse_event);
        virtual void mouseMoved(gcn::MouseEvent& mouse_event);
        virtual void mouseDragged(gcn::MouseEvent& mouse_event);
        virtual void mouseReleased(gcn::MouseEvent& mouse_event);

        void setOffset(int off);
        void setBorderWidth(int bw);

        void attachToContainer(gcn::Container* w);

    protected:
        virtual OverlayHint handleOverlayExt(gcn::Widget* w);

        gcn::Container floatmsg_box_;
        gcn::TextBox floatmsg_text_;

        int border_width_;
        int offset_;

    private:
        void handleOverlay();
        int alignX(int mx, int my, gcn::Widget* source, OverlayHint hint) const;
        int alignY(int mx, int my, gcn::Widget* source, OverlayHint hint) const;

        gcn::Container* p_container_;

        gcn::Widget* p_current_widget_;
        int mouse_x_;
        int mouse_y_;
    };

}

#endif // GCN_OVERLAY_HPP
