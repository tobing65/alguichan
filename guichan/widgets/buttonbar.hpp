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

#ifndef GCN_BUTTONBAR_HPP
#define GCN_BUTTONBAR_HPP

#include "guichan/widgets/container.hpp"

namespace gcn
{

    //----------------------------------------------------------------------------

    /**
    Controls which widgets are used to find positions and sizes 
    in the ButtonVBar and ButtonHBar containers. 
    */
    enum ButtonBarAdjustMode
    {
        BBAdjustAll, BBAdjustEnabled, BBAdjustVisible
    };

    //----------------------------------------------------------------------------

    /**
    Special container that holds and ajusts its children vertically. 

    Children are adjusted in a column, resizing them all to the maximum width. 

    @see ButtonHBar
    */
    class GCN_CORE_DECLSPEC ButtonVBar : public Container
    {
    public:
        ButtonVBar();

        /**
        Adjusts all children vertically to new positions and resizes them to the maximum width. 
        The parameter controls which children are part of this.
        */
        void adjustSizeAndButtons(ButtonBarAdjustMode show_invisible_and_disabled);

        /**
        Set spacing to be used during adjustment.
        @param int_sp spacing between children
        @param border_sp spacing between children and border
        */
        void setSpacing(unsigned int int_sp, unsigned int border_sp);

        /**
        Set spacing to be used during adjustment.
        @param int_sp spacing between children
        @param border_width_sp spacing between children and border left and right
        @param border_height_sp spacing between children and border top and bottom
        */
        void setSpacing(unsigned int int_sp, unsigned int border_width_sp, unsigned int border_height_sp);

        /**
        Set width of all child widgets recursively.
        */
        void setWidth(int width);

    private:
        unsigned short int_spacing_;
        unsigned short border_w_spacing_;
        unsigned short border_h_spacing_;
    };

    //----------------------------------------------------------------------------

    /**
    Special container that holds and ajusts its children horizontally. 

    Children are adjusted in a row, resizing them all to the maximum height. 

    @see ButtonVBar
    */
    class GCN_CORE_DECLSPEC ButtonHBar : public Container
    {
    public:
        ButtonHBar();

        /**
        Adjusts all children horizontally to new positions and resizes them to the maximum height. 
        The parameter controls which children are part of this.
        */
        void adjustSizeAndButtons(ButtonBarAdjustMode show_invisible_and_disabled);

        /**
        Set spacing to be used during adjustment.
        @param int_sp spacing between children
        @param border_sp spacing between children and border
        */
        void setSpacing(unsigned int int_sp, unsigned int border_sp);

        /**
        Set spacing to be used during adjustment.
        @param int_sp spacing between children
        @param border_width_sp spacing between children and border left and right
        @param border_height_sp spacing between children and border top and bottom
        */
        void setSpacing(unsigned int int_sp, unsigned int border_width_sp, unsigned int border_height_sp);

        /**
        Set Height of all child widgets recursively.
        */
        void setHeight(int height);

    private:
        unsigned char int_spacing_;
        unsigned char border_w_spacing_;
        unsigned char border_h_spacing_;
    };

    //----------------------------------------------------------------------------

}

#endif // GCN_BUTTONBAR_HPP
