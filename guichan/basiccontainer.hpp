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

#ifndef GCN_BASICCONTAINER_HPP
#define GCN_BASICCONTAINER_HPP

#include <list>

#include "guichan/deathlistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
     * A base class for containers. The class implements the most
     * common things for a container. If you are implementing a 
     * container, consider inheriting from this class.
     *
     * @see Container
     * @since 0.6.0
     */
    class GCN_CORE_DECLSPEC BasicContainer : public Widget, public DeathListener
    {
    public:
        /**
         * Constructor.
         */
        BasicContainer();

        /**
         * Destructor
         */
        virtual ~BasicContainer();

        /**
         * Shows a certain part of a widget in the basic container.
         * Used when widgets want a specific part to be visible in
         * its parent. An example is a TextArea that wants a specific
         * part of its text to be visible when a TextArea is a child
         * of a ScrollArea.
         *
         * @param widget The widget whom wants a specific part of
         *               itself to be visible.
         * @param rectangle The rectangle to be visible.
         */
        virtual void showWidgetPart(Widget* widget, Rectangle area);

        
        // Inherited from Widget

        virtual void moveToTop(Widget* widget);

        virtual void moveToBottom(Widget* widget);

        virtual Rectangle getChildrenArea() const;

        virtual void focusNext();

        virtual void focusPrevious();

        virtual void logic();

        virtual void _setFocusHandler(FocusHandler* focusHandler);

        void setInternalFocusHandler(FocusHandler* focusHandler);

        virtual Widget *getWidgetAt(int x, int y) const;


        // Inherited from DeathListener

        virtual void death(const Event& event);

    protected:
        /**
         * Moves a widget to the top. Normally one wants to use
         * Widget::moveToTop instead.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param widget The widget to move to the top.
         * @since 0.9.0
         */
        void _moveToTopWithNoChecks(Widget* widget);

         /**
         * Moves a widget to the bottom. Normally one wants to use
         * Widget::moveToBottom instead.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param The widget to move to the bottom.
         * @since 0.9.0
         */
        void _moveToBottomWithNoChecks(Widget* widget);

        /**
         * Adds a widget to the basic container.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param widget The widget to add.
         * @see remove, clear
         */
        void add(Widget* widget);

        /**
         * Removes a widget from the basic container.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @param widget The widget to remove.
         * @see add, clear
         */
        virtual void remove(Widget* widget);

        /**
         * Clears the basic container from all widgets.
         *
         * THIS METHOD IS NOT SAFE TO CALL INSIDE A WIDGETS LOGIC FUNCTION
         * INSIDE ANY LISTER FUNCTIONS!
         *
         * @see remove, clear
         */
        virtual void clear();
        
        /**
         * Draws the children widgets of the basic container.
         *
         * @param graphics A graphics object to draw with.
         */
        virtual void drawChildren(Graphics* graphics);

        /**
         * Calls logic for the children widgets of the basic
         * container.
         */
        virtual void logicChildren();

        /**
         * Resizes the BasicContainer to fit it's content exactly.
         */
        void resizeToContent();

        /**
         * Typedef.
         */
        typedef std::list<Widget*> WidgetList;

         /**
         * Typedef.
         */
        typedef WidgetList::iterator WidgetListIterator;

        /**
         * Holds all widgets of the basic container.
         */
        WidgetList mWidgets;

        /**
         * Holds a widget that should be moved to the top.
         */
        Widget* mWidgetToBeMovedToTheTop;

        /**
         * Holds a widget that should be moved to the bottom.
         */
        Widget* mWidgetToBeMovedToTheBottom;

        /**
         * True if logic is currently being processed, false otherwise.
         */
        bool mLogicIsProcessing;
    };
}

#endif // end GCN_BASICCONTAINER_HPP
