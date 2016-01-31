//
// Created by joe on 17/01/16.
//
// Display related classes
// Intended to abstract the display type (X, Windows, PDF, etc.) from the rest of the rendering

#ifndef OSMMAPPER_DISPLAY_H
#define OSMMAPPER_DISPLAY_H

#include "OSMTypes.h"

#include <cairommconfig.h>
#include <context.h>
#include <surface.h>
#include <refptr.h>
#include <iostream>

namespace osmMapper {
    /*! The event that occurred.
     *  This needs to be turned into a fully abstracted interface
     *  but I want to test the rest first before adding a whole bunch of serialisation */
    typedef struct {
        enum {
            KEY, BUTTON, NONE
        } type;
        int value; // Yukking coords into top and bottom words for the time being
    } event;

    /*! Base for all rendered output and UI I/O*/
    class display {
    public:
        //! DTOR
        virtual ~display() { }

        //! Sets the display to fullsize/fullscreen
        virtual void fullSize(bool full=true)=0;

        //! Gets the current width of the display in display units
        virtual int width(void) const { return m_width; }

        //! Gets the current height of the display in display units
        virtual int height(void) const { return m_height; }

        virtual void bgColour(const osmMapper::colour_t &colour) { m_background=colour; }

        virtual osmMapper::colour_t bgColour(void) const { return m_background; }

        virtual void startDrawing(void)=0;

        virtual void finishDrawing(void)=0;

        virtual void startPath(const screenPoint &startPos,
                               const colour_t &colour, const double thickness, bool filled)=0;

        virtual void endPath(void)=0;

        //! Renders a line on the display
        //! \param from The position to draw line from
        //! \param to The position to draw the line to
        virtual void drawLine(const screenPoint &from, const screenPoint &to)=0;

        virtual event getEvent(const int timeoutInMs=15000)=0;

    protected:
        //! CTOR
        //! \param x Initial width of display in display units
        //! \param y Initial height of display in display units
        display(int &x, int &y) : m_width(x), m_height(y) {
            bgColour({0.75, 0.75, 0.70});
        }

        int m_width;            //!< Width of display in display units
        int m_height;           //!< Height of display in display units
        osmMapper::colour_t m_background;  //!< Colour of the background
    };
} // namespace osmMapper

#endif //OSMMAPPER_DISPLAY_H
