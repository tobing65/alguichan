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

#ifndef GCN_TEXTBOX_HPP
#define GCN_TEXTBOX_HPP

#include <ctime>
#include <string>
#include <vector>

#include "guichan/graphics.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    /**
     * An implementation of a text box where a user can enter text that contains of many lines.
     */
    class GCN_CORE_DECLSPEC TextBox: public Widget, public MouseListener, public KeyListener
    {
    public:
        /**
         * Constructor.
         */
        TextBox();

        /**
         * Constructor.
         *
         * @param text The default text of the text box.
         */
        explicit TextBox(const std::string& text);

        /**
         * Constructor.
         *
         * @param text The default text of the text box.
         */
        explicit TextBox(const char* text);

        /**
         * Sets the text of the text box. 
         * New line characters are translated into new rows.
         *
         * @param text The text of the text box.
         * @see getText
         */
        void setText(const std::string& text);

        /**
         * Sets the text of the text box. 
         * New line characters are translated into new rows.
         *
         * @param text The text of the text box.
         * @see getText
         */
        void setText(const char* text);

        /**
         * Sets the text of the text box. 
         * Automatically wraps long lines at width_hint pixels. The default font for this widget is used.
         *
         * @param text The text of the text box.
         * @see setText, getText
         */
        void setTextAutoWrap(const char* text, int width_hint);

        /**
         * Gets the text of the text box.
         *
         * @return The text of the text box.
         * @see setText
         */
        std::string getText() const;

        /**
         * Gets a certain row from the text.
         *
         * @param row The number of the row to get from the text.
         * @return A row from the text of the text box.
         * @see setTextRow
         */
        const std::string& getTextRow(int row) const;

        /**
         * Sets the text of a certain row of the text.
         *
         * @param row The number of the row to set in the text.
         * @param text The text to set in the given row number.
         * @see getTextRow
         */
        void setTextRow(int row, const std::string& text);

        /**
         * Gets the number of rows in the text.
         *
         * @return The number of rows in the text.
         */
        unsigned int getNumberOfRows() const;

        /**
         * Gets the caret position in the text.
         *
         * @return The caret position in the text.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;

        /**
         * Sets the position of the caret in the text.
         *
         * @param position the positon of the caret.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the row number where the caret is currently located.
         *
         * @return The row number where the caret is currently located.
         * @see setCaretRow
         */
        unsigned int getCaretRow() const;

        /**
         * Sets the row where the caret should be currently located.
         *
         * @param The row where the caret should be currently located.
         * @see getCaretRow
         */
        void setCaretRow(int row);

        /**
         * Gets the column where the caret is currently located.
         *
         * @return The column where the caret is currently located.
         * @see setCaretColumn
         */
        unsigned int getCaretColumn() const;

        /**
         * Sets the column where the caret should be currently located.
         *
         * @param The column where the caret should be currently located.
         * @see getCaretColumn
         */
        void setCaretColumn(int column);

        /**
         * Sets the row and the column where the caret should be curretly
         * located.
         *
         * @param row The row where the caret should be currently located.
         * @param column The column where the caret should be currently located.
         * @see getCaretRow, getCaretColumn
         */
        void setCaretRowColumn(int row, int column);

        /**
         * Scrolls the text to the caret if the text box is in a scroll area.
         * 
         * @see ScrollArea
         */
        virtual void scrollToCaret();

        /**
         * Checks if the text box is editable.
         *
         * @return True it the text box is editable, false otherwise.
         * @see setEditable
         */
        bool isEditable() const;

        /**
         * Sets the text box to be editable or not.
         *
         * @param editable True if the text box should be editable, false otherwise.
         */
        void setEditable(bool editable);

        /**
         * Adds a row of text to the end of the text.
         * New line characters are translated into new rows.
         *
         * @param row The row to add.
         */
        virtual void addRow(const std::string &row);

        /**
         * Adds a row of text to the end of the text.
         * New line characters are translated into new rows.
         *
         * @param row The row to add.
         */
        virtual void addRow(const char* text);

        /**
         * Sets the alignment of the text. The alignment is relative to the center of the widget.
         *
         * Note that the alignment has no effect if the TextBox is editable, because alignment is not implemented for the caret.
         * So only if the TextBox is used as non-editable, the alignment has the desired effect.
         *
         * @param alignemnt The alignment of the text of the widget.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the text. The alignment is relative to
         * the center of the widget.
         *
         * @return The alignment of the text of the widget.
         * @see setAlignmentm Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Checks if the text box is opaque. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @return True if the text box is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const;

        /**
         * Sets the text box to be opaque or not. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @param opaque True if the text box should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);


        // Inherited from Widget

        virtual void draw(Graphics* graphics);

        virtual void fontChanged();


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

    protected:
        /**
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics a Graphics object to draw with.
         * @param x the x position.
         * @param y the y position.
         */
        virtual void drawCaret(Graphics* graphics, int x, int y);

        /**
         * Adjusts the text box's size to fit the text.
         */
        virtual void adjustSize();

        /**
         * Holds all the rows of the text.
         */
        std::vector<std::string> mTextRows;

        /**
         * Holds the current column of the caret.
         */
        int mCaretColumn;

        /**
         * Holds the current row of the caret.
         */
        int mCaretRow;

        /**
         * Holds the alignment of the text.
         */
        Graphics::Alignment mAlignment;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mEditable;

        /**
         * True if the text box is opaque, false otherwise.
         */
        bool mOpaque;
    };
}

#endif // end GCN_TEXTBOX_HPP
