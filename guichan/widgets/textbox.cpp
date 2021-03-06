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

#include <cstring>

#include "guichan/widgets/textbox.hpp"

#include "guichan/basiccontainer.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    TextBox::TextBox()
        : mCaretColumn(0)
        , mCaretRow(0)
        , mAlignment(Graphics::Left)
        , mEditable(true)
        , mOpaque(true)
    {
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSize();
    }

    TextBox::TextBox(const std::string& text)
        : mCaretColumn(0)
        , mCaretRow(0)
        , mAlignment(Graphics::Left)
        , mEditable(true)
        , mOpaque(true)
    {
        setText(text);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSize();
    }

    TextBox::TextBox(const char* text)
        : mCaretColumn(0)
        , mCaretRow(0)
        , mAlignment(Graphics::Left)
        , mEditable(true)
        , mOpaque(true)
    {
        setText(text);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSize();
    }

    void TextBox::setText(const std::string& text)
    {
        mCaretColumn = 0;
        mCaretRow = 0;

        mTextRows.clear();

        std::string::size_type pos, lastPos = 0;
        int length;
        do
        {
            pos = text.find("\n", lastPos);

            if (pos != std::string::npos)
            {
                length = pos - lastPos;
            }
            else
            {
                length = text.size() - lastPos;
            }
            std::string sub = text.substr(lastPos, length);
            mTextRows.push_back(sub);
            lastPos = pos + 1;

        } while (pos != std::string::npos);

        adjustSize();
    }

    void TextBox::setText(const char* text)
    {
        mTextRows.clear();
        addRow(text);
    }

    void TextBox::addRow(const char* text)
    {
        mCaretColumn = 0;
        mCaretRow = 0;

        if(text)
        {
            const int sz=strlen(text);
            if(sz > 0)
            {
                char*const pbuf=new char[sz+1];
                strcpy(pbuf, text);

                char*curr_p=pbuf;
                const char*last_p=curr_p;
                for(int i=0; i<=sz; ++i)
                {
                    if(*curr_p=='\n' || *curr_p==0)
                    {
                        *curr_p=0;
                        std::string sub(last_p);
                        mTextRows.push_back(sub);
                        ++curr_p;
                        last_p=curr_p;
                    }
                    else
                        ++curr_p;
                }

                delete pbuf;
            }
        }

        adjustSize();
    }

    void TextBox::setTextAutoWrap(const char* text, int width_hint)
    {
        mCaretColumn = 0;
        mCaretRow = 0;

        mTextRows.clear();

        const int sz=strlen(text);
        char*const pbuf=new char[sz+1];
        strcpy(pbuf, text);

        char*curr_p=pbuf;
        const char*last_p=curr_p;
        for(int i=0; i<=sz; ++i)
        {
            if(*curr_p=='\n' || *curr_p==0)
            {
                *curr_p=0;
                std::string sub(last_p);
                mTextRows.push_back(sub);
                ++curr_p;
                last_p=curr_p;
            }
            else if(*curr_p==' ')
            {
                *curr_p=0;
                const int w=getFont()->getWidth(last_p);
                if(w>=width_hint)
                {
                    std::string sub(last_p);
                    mTextRows.push_back(sub);
                    ++curr_p;
                    last_p=curr_p;
                }
                else
                {
                    *curr_p=' ';
                    ++curr_p;
                }
            }
            else
                ++curr_p;
        }

        delete pbuf;

        adjustSize();
    }

    void TextBox::draw(Graphics* graphics)
    {
        if(mOpaque)
        {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(0, 0, getWidth(), getHeight());
        }

        if(isFocused() && isEditable())
        {
            drawCaret(graphics, getFont()->getWidth(mTextRows[mCaretRow].substr(0, mCaretColumn)), mCaretRow * getFont()->getHeight());
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        for(unsigned int i = 0; i < mTextRows.size(); i++)
        {
            // Move the text one pixel so we can have a caret before a letter.
            if(!isEditable() && mAlignment == gcn::Graphics::Right)
                graphics->drawText(mTextRows[i], getWidth(), i * getFont()->getHeight(), mAlignment);
            else if(!isEditable() && mAlignment == gcn::Graphics::Center)
                graphics->drawText(mTextRows[i], getWidth()/2, i * getFont()->getHeight(), mAlignment);
            else
                graphics->drawText(mTextRows[i], 1, i * getFont()->getHeight());
        }
    }

    void TextBox::drawCaret(Graphics* graphics, int x, int y)
    {
        const Style* style = getStyle();
        const bool blink = style->getBlink();
        if(blink)
            return;

        graphics->setColor(getForegroundColor());
        const int caretWidth = style->getCaretWidth();
        for(int i=0; i<caretWidth; ++i)
        {
            graphics->drawLine(x, getFont()->getHeight() + y, x, y);
            ++x;
        }
    }

    void TextBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mCaretRow = mouseEvent.getY() / getFont()->getHeight();

            if (mCaretRow >= (int)mTextRows.size())
            {
                mCaretRow = mTextRows.size() - 1;
            }

            mCaretColumn = getFont()->getStringIndexAt(mTextRows[mCaretRow], mouseEvent.getX());
        }
    }

    void TextBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Left)
        {
            --mCaretColumn;
            if (mCaretColumn < 0)
            {
                --mCaretRow;

                if (mCaretRow < 0)
                {
                    mCaretRow = 0;
                    mCaretColumn = 0;
                }
                else
                {
                    mCaretColumn = mTextRows[mCaretRow].size();
                }
            }
        }

        else if (key.getValue() == Key::Right)
        {
            ++mCaretColumn;
            if (mCaretColumn > (int)mTextRows[mCaretRow].size())
            {
                ++mCaretRow;

                if (mCaretRow >= (int)mTextRows.size())
                {
                    mCaretRow = mTextRows.size() - 1;
                    if (mCaretRow < 0)
                    {
                        mCaretRow = 0;
                    }

                    mCaretColumn = mTextRows[mCaretRow].size();
                }
                else
                {
                    mCaretColumn = 0;
                }
            }
        }

        else if (key.getValue() == Key::Down)
        {
            setCaretRow(mCaretRow + 1);
        }

        else if (key.getValue() == Key::Up)
        {
            setCaretRow(mCaretRow - 1);
        }

        else if (key.getValue() == Key::Home)
        {
            mCaretColumn = 0;
        }

        else if (key.getValue() == Key::End)
        {
            mCaretColumn = mTextRows[mCaretRow].size();
        }

        else if (key.getValue() == Key::Enter && mEditable)
        {
            mTextRows.insert(mTextRows.begin() + mCaretRow + 1,
                             mTextRows[mCaretRow].substr(mCaretColumn, mTextRows[mCaretRow].size() - mCaretColumn));
            mTextRows[mCaretRow].resize(mCaretColumn);
            ++mCaretRow;
            mCaretColumn = 0;
        }

        else if (key.getValue() == Key::Backspace
                 && mCaretColumn != 0
                 && mEditable)
        {
            mTextRows[mCaretRow].erase(mCaretColumn - 1, 1);
            --mCaretColumn;
        }

        else if (key.getValue() == Key::Backspace
                 && mCaretColumn == 0
                 && mCaretRow != 0
                 && mEditable)
        {
            mCaretColumn = mTextRows[mCaretRow - 1].size();
            mTextRows[mCaretRow - 1] += mTextRows[mCaretRow];
            mTextRows.erase(mTextRows.begin() + mCaretRow);
            --mCaretRow;
        }

        else if (key.getValue() == Key::Delete
                 && mCaretColumn < (int)mTextRows[mCaretRow].size()
                 && mEditable)
        {
            mTextRows[mCaretRow].erase(mCaretColumn, 1);
        }

        else if (key.getValue() == Key::Delete
                 && mCaretColumn == (int)mTextRows[mCaretRow].size()
                 && mCaretRow < ((int)mTextRows.size() - 1)
                 && mEditable)
        {
            mTextRows[mCaretRow] += mTextRows[mCaretRow + 1];
            mTextRows.erase(mTextRows.begin() + mCaretRow + 1);
        }

        else if(key.getValue() == Key::PageUp)
        {
            Widget* parent = getParent();

            if (parent)
            {
                int rowsPerPage = parent->getChildrenArea().height / getFont()->getHeight();
                mCaretRow -= rowsPerPage;

                if (mCaretRow < 0)
                {
                    mCaretRow = 0;
                }
            }
        }

        else if(key.getValue() == Key::PageDown)
        {
            Widget* parent = getParent();

            if (parent)
            {
                int rowsPerPage = parent->getChildrenArea().height / getFont()->getHeight();
                mCaretRow += rowsPerPage;

                if (mCaretRow >= (int)mTextRows.size())
                {
                    mCaretRow = mTextRows.size() - 1;
                }
            }
        }

        else if(key.getValue() == Key::Tab
                && mEditable)
        {
            mTextRows[mCaretRow].insert(mCaretColumn,std::string("    "));
            mCaretColumn += 4;
        }

        else if (key.isCharacter()
                 && mEditable)
        {
            mTextRows[mCaretRow].insert(mCaretColumn,std::string(1,(char)key.getValue()));
            ++mCaretColumn;
        }

        adjustSize();
        scrollToCaret();

        keyEvent.consume();
    }

    void TextBox::adjustSize()
    {
        unsigned int i;
        int width = 0;
        for (i = 0; i < mTextRows.size(); ++i)
        {
            int w = getFont()->getWidth(mTextRows[i]);
            if (width < w)
            {
                width = w;
            }
        }

        setWidth(width + 1);
        setHeight(getFont()->getHeight() * mTextRows.size());
    }

    void TextBox::setCaretPosition(unsigned int position)
    {
        int row;

        for (row = 0; row < (int)mTextRows.size(); row++)
        {
            if (position <= mTextRows[row].size())
            {
                mCaretRow = row;
                mCaretColumn = position;
                return; // we are done
            }
            else
            {
                position--;
            }
        }

        // position beyond end of text
        mCaretRow = mTextRows.size() - 1;
        mCaretColumn = mTextRows[mCaretRow].size();
    }

    unsigned int TextBox::getCaretPosition() const
    {
        int pos = 0, row;

        for (row = 0; row < mCaretRow; row++)
        {
            pos += mTextRows[row].size();
        }

        return pos + mCaretColumn;
    }

    void TextBox::setCaretRowColumn(int row, int column)
    {
        setCaretRow(row);
        setCaretColumn(column);
    }

    void TextBox::setCaretRow(int row)
    {
        mCaretRow = row;

        if (mCaretRow >= (int)mTextRows.size())
        {
            mCaretRow = mTextRows.size() - 1;
        }

        if (mCaretRow < 0)
        {
            mCaretRow = 0;
        }

        setCaretColumn(mCaretColumn);
    }

    unsigned int TextBox::getCaretRow() const
    {
        return mCaretRow;
    }

    void TextBox::setCaretColumn(int column)
    {
        mCaretColumn = column;

        if (mCaretColumn > (int)mTextRows[mCaretRow].size())
        {
            mCaretColumn = mTextRows[mCaretRow].size();
        }

        if (mCaretColumn < 0)
        {
            mCaretColumn = 0;
        }
    }

    unsigned int TextBox::getCaretColumn() const
    {
        return mCaretColumn;
    }

    const std::string& TextBox::getTextRow(int row) const
    {        
        return mTextRows[row];
    }

    void TextBox::setTextRow(int row, const std::string& text)
    {
        mTextRows[row] = text;

        if (mCaretRow == row)
        {
            setCaretColumn(mCaretColumn);
        }

        adjustSize();
    }

    unsigned int TextBox::getNumberOfRows() const
    {
        return mTextRows.size();
    }

    std::string TextBox::getText() const
    {
        if (mTextRows.size() == 0)
        {
            return std::string("");
        }

        int i;
        std::string text;

        for (i = 0; i < (int)mTextRows.size() - 1; ++i)
        {
            text = text + mTextRows[i] + "\n";
        }

        text = text + mTextRows[i];

        return text;
    }

    void TextBox::fontChanged()
    {
        adjustSize();
    }

    void TextBox::scrollToCaret()
    {
        Rectangle scroll;
        scroll.x = getFont()->getWidth(mTextRows[mCaretRow].substr(0, mCaretColumn));
        scroll.y = getFont()->getHeight() * mCaretRow;
        scroll.width = getFont()->getWidth(" ");
        scroll.height = getFont()->getHeight() + 2; // add 2 for some extra space

        showPart(scroll);
    }

    void TextBox::setEditable(bool editable)
    {
        mEditable = editable;
    }

    bool TextBox::isEditable() const
    {
        return mEditable;
    }

    void TextBox::addRow(const std::string &row)
    {
        addRow(row.c_str());
    }

    void TextBox::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment TextBox::getAlignment() const
    {
        return mAlignment;
    }

    bool TextBox::isOpaque() const
    {
        return mOpaque;
    }

    void TextBox::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }
}
