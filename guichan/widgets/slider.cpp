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

#include "guichan/widgets/slider.hpp"

#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/style.hpp"

namespace gcn
{
    Slider::Slider(double scaleEnd)
        : mDragged(false)
        , mScaleStart(0)
        , mScaleEnd(scaleEnd)
    {
        setFrameSize(getStyle()->getDefaultAreaFrameSize());
        setFocusable(true);
        setOrientation(Horizontal);
        setValue(0);
        setStepLength(scaleEnd / 10);
        setMarkerLength(10);

        addMouseListener(this);
        addKeyListener(this);
    }

    Slider::Slider(double scaleStart, double scaleEnd)
        : mDragged(false)
        , mScaleStart(scaleStart)
        , mScaleEnd(scaleEnd)
    {
        setFrameSize(getStyle()->getDefaultAreaFrameSize());
        setFocusable(true);
        setOrientation(Horizontal);
        setValue(scaleStart);
        setStepLength((scaleEnd  - scaleStart)/ 10);
        setMarkerLength(10);

        addMouseListener(this);
        addKeyListener(this);
    }

    void Slider::setScale(double scaleStart, double scaleEnd)
    {
        mScaleStart = scaleStart;
        mScaleEnd = scaleEnd;
    }

    double Slider::getScaleStart() const
    {
        return mScaleStart;
    }

    void Slider::setScaleStart(double scaleStart)
    {
        mScaleStart = scaleStart;
    }

    double Slider::getScaleEnd() const
    {
        return mScaleEnd;
    }

    void Slider::setScaleEnd(double scaleEnd)
    {
        mScaleEnd = scaleEnd;
    }

    void Slider::draw(gcn::Graphics* graphics)
    {
        const Style* style = getStyle();
        Color shadowColor = style->getDarkerColor(getBaseColor());

        graphics->setColor(shadowColor);
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        drawMarker(graphics);
    }

    void Slider::drawMarker(gcn::Graphics* graphics)
    {
        gcn::Color faceColor = getBaseColor();
        const Style* style = getStyle();
        Color highlightColor = style->getHighlightColor(faceColor);
        Color shadowColor = style->getShadowColor(faceColor);

        graphics->setColor(faceColor);

        if (getOrientation() == Horizontal)
        {
            int v = getMarkerPosition();
            graphics->fillRectangle(v + 1, 1, getMarkerLength() - 2, getHeight() - 2);
            graphics->setColor(highlightColor);
            graphics->drawLine(v, 0, v + getMarkerLength() - 1,0);
            graphics->drawLine(v, 0, v, getHeight() - 1);
            graphics->setColor(shadowColor);
            graphics->drawLine(v + getMarkerLength() - 1, 1, v + getMarkerLength() - 1, getHeight() - 1);
            graphics->drawLine(v + 1, getHeight() - 1, v + getMarkerLength() - 1, getHeight() - 1);

            if (isFocused())
            {
                graphics->setColor(getForegroundColor());
                graphics->drawRectangle(v + 2, 2, getMarkerLength() - 4, getHeight() - 4);
            }
        }
        else
        {
            int v = (getHeight() - getMarkerLength()) - getMarkerPosition();
            graphics->fillRectangle(1, v + 1, getWidth() - 2, getMarkerLength() - 2);
            graphics->setColor(highlightColor);
            graphics->drawLine(0, v, 0, v + getMarkerLength() - 1);
            graphics->drawLine(0, v, getWidth() - 1, v);
            graphics->setColor(shadowColor);
            graphics->drawLine(1, v + getMarkerLength() - 1, getWidth() - 1, v + getMarkerLength() - 1);
            graphics->drawLine(getWidth() - 1, v + 1, getWidth() - 1, v + getMarkerLength() - 1);

            if (isFocused())
            {
                graphics->setColor(getForegroundColor());
                graphics->drawRectangle(2, v + 2, getWidth() - 4, getMarkerLength() - 4);
            }
        }
    }

    void Slider::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == gcn::MouseEvent::Left
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() <= getWidth()
            && mouseEvent.getY() >= 0
            && mouseEvent.getY() <= getHeight())
        {
            if (getOrientation() == Horizontal)
            {
                setValue(markerPositionToValue(mouseEvent.getX() - getMarkerLength() / 2));
            }
            else
            {
                setValue(markerPositionToValue(getHeight() - mouseEvent.getY() - getMarkerLength() / 2));
            }

            distributeActionEvent();
        }
    }

    void Slider::mouseDragged(MouseEvent& mouseEvent)
    {
        if (getOrientation() == Horizontal)
        {
            setValue(markerPositionToValue(mouseEvent.getX() - getMarkerLength() / 2));
        }
        else
        {
            setValue(markerPositionToValue(getHeight() - mouseEvent.getY() - getMarkerLength() / 2));
        }

        distributeActionEvent();

        mouseEvent.consume();
    }

    void Slider::setValue(double value)
    {
        if (value > getScaleEnd())
        {
            mValue = getScaleEnd();
            return;
        }

        if (value < getScaleStart())
        {
            mValue = getScaleStart();
            return;
        }

        mValue = value;
    }

    double Slider::getValue() const
    {
        return mValue;
    }

    int Slider::getMarkerLength() const
    {
        return mMarkerLength;
    }

    void Slider::setMarkerLength(int length)
    {
        mMarkerLength = length;
    }

    void Slider::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (getOrientation() == Horizontal)
        {
            if (key.getValue() == Key::Right)
            {
                setValue(getValue() + getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
            else if (key.getValue() == Key::Left)
            {
                setValue(getValue() - getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }            
        }
        else
        {
            if (key.getValue() == Key::Up)
            {
                setValue(getValue() + getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
            else if (key.getValue() == Key::Down)
            {
                setValue(getValue() - getStepLength());
                distributeActionEvent();
                keyEvent.consume();
            }
        }
    }

    void Slider::setOrientation(Slider::Orientation orientation)
    {
        mOrientation = orientation;
    }

    Slider::Orientation Slider::getOrientation() const
    {
        return mOrientation;
    }

    double Slider::markerPositionToValue(int v) const
    {
        int w;
        if (getOrientation() == Horizontal)
        {
            w = getWidth();
        }
        else
        {
            w = getHeight();
        }

        double pos = v / ((double)w - getMarkerLength());
        return (1.0 - pos) * getScaleStart() + pos * getScaleEnd();

    }

    int Slider::valueToMarkerPosition(double value) const
    {
        int v;
        if (getOrientation() == Horizontal)
        {
            v = getWidth();
        }
        else
        {
            v = getHeight();
        }

        int w =  (int)((v - getMarkerLength())
                       * (value  - getScaleStart())
                       / (getScaleEnd() - getScaleStart()));

        if (w < 0)
        {
            return 0;
        }

        if (w > v - getMarkerLength())
        {
            return v - getMarkerLength();
        }

        return w;
    }

    void Slider::setStepLength(double length)
    {
        mStepLength = length;
    }

    double Slider::getStepLength() const
    {
        return mStepLength;
    }

    int Slider::getMarkerPosition() const
    {
        return valueToMarkerPosition(getValue());
    }

    void Slider::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        setValue(getValue() + getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }

    void Slider::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        setValue(getValue() - getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }
}
