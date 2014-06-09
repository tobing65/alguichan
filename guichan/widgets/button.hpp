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

#ifndef GCN_BUTTON_HPP
#define GCN_BUTTON_HPP

#include <string>

#include "guichan/focuslistener.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
    * An implementation of a regular clickable button. 
    * Derived classes Button and ImageButton also show the button's content.
    *
    * If a button is clicked an action event will be sent to all action listener's 
    * of the button.
    *
    * @see  Button, ImageButton
    */
    class GCN_CORE_DECLSPEC ButtonBase : public Widget, public MouseListener, public KeyListener, public FocusListener
    {
    public:
        /**
        * Constructor.
        */
        ButtonBase();

        /**
        * Sets the alignment of the content.
        * The alignment is relative to the center of the button.
        *
        * @param alignment The alignment of the content.
        * @see getAlignment, Graphics
        */
        void setAlignment(Graphics::Alignment alignment);

        /**
        * Gets the alignment of the content.
        *
        * @return The alignment of the content.
        * @see setAlignment, Graphics
        */
        Graphics::Alignment getAlignment() const;

        /**
        * Sets the spacing between the border of the button and its content.
        *
        * @param spacing The default value for spacing is 4 and can be changed 
        *                using this method.
        * @see getSpacing
        */
        void setSpacing(unsigned int spacing);

        /**
        * Gets the spacing between the border of the button and its content.
        *
        * @return spacing.
        * @see setSpacing
        */
        unsigned int getSpacing() const;

        void setOpaque(bool isOpaque);


        // Inherited from FocusListener
        virtual void focusLost(const Event& event);

        // Inherited from MouseListener
        virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseEntered(MouseEvent& mouseEvent);
        virtual void mouseExited(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);

        // Inherited from KeyListener
        virtual void keyPressed(KeyEvent& keyEvent);
        virtual void keyReleased(KeyEvent& keyEvent);


        /**
        * Triggers the buttons action, as if it were pressed.
        * The action is not triggered, if the button is not enabled.
        *
        * @return true, if the action was triggered, false if not.
        */
        bool triggerAction();

        /**
        * Triggers the buttons action, as if it were pressed. 
        * The keyEvent is consumed if the action was triggered.
        *
        */
        void triggerAction(KeyEvent& keyEvent);

    protected:
        /**
        * Checks if the button is pressed. Convenient method to use
        * when overloading the draw method of the button.
        *
        * @return True if the button is pressed, false otherwise.
        */
        bool isPressed() const;

        /**
        * True if the mouse is ontop of the button, false otherwise.
        */
        bool mHasMouse;

        /**
        * True if a key has been pressed, false otherwise.
        */
        bool mKeyPressed;

        /**
        * True if a mouse has been pressed, false otherwise.
        */
        bool mMousePressed;

        bool mIsOpaque;

        /**
        * Holds the alignment of the content.
        */
        Graphics::Alignment mAlignment;

        /**
        * Holds the spacing between the border and the content.
        */
        unsigned short mSpacing;
    };







    /**
    * An implementation of a regular clickable button with a caption. 
    *
    * If a button is clicked an action event will be sent to all action listener's 
    * of the button.
    */
    class GCN_CORE_DECLSPEC Button : public ButtonBase
    {
    public:
        /**
        * Constructor.
        */
        Button();

        /**
        * Constructor. The button will be automatically resized
        * to fit the caption.
        *
        * @param caption The caption of the button.
        */
        explicit Button(const std::string& caption);

        /**
        * Constructor. The button will be automatically resized
        * to fit the caption.
        *
        * @param caption The caption of the button.
        */
        explicit Button(const char* caption);

        /**
        * Sets the caption of the button. It's advisable to call
        * adjustSize after setting of the caption to adjust the
        * button's size to fit the caption.
        *
        * @param caption The caption of the button.
        * @see getCaption, adjustSize
        */
        void setCaption(const std::string& caption);

        /**
        * Sets the caption of the button. It's advisable to call
        * adjustSize after setting of the caption to adjust the
        * button's size to fit the caption.
        *
        * @param caption The caption of the button.
        * @see getCaption, adjustSize
        */
        void setCaption(const char* caption);

        /**
        * Gets the caption of the button.
        *
        * @return The caption of the button.
        */
        const char* getCaption() const;

        /**
        * Adjusts the button's size to fit the caption.
        */
        void adjustSize();


        //Inherited from Widget

        virtual void draw(Graphics* graphics);

    protected:
        /**
        * Holds the caption of the button.
        */
        std::string mCaption;
    };

}

#endif // end GCN_BUTTON_HPP
