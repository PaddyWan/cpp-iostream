#ifndef STDUTIL_HPP
#define STDUTIL_HPP

#include "basicstream.hpp"
#include "fdbuf.hpp"

struct stdutil
{
    enum FORMAT
    {
        //set
        BOLD = 1,
        DIM = 2,
        UNDERLINE = 4,
        BLINK = 5,
        INVERTED = 7,
        HIDDEN = 8,
        //reset
        RESET = 0,
        //more here

        //colors
        //foreground, text
        FG_DEFAULT = 39,
        FG_BLACK = 30,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_MAGENTA = 35,
        FG_CYAN = 36,
        FG_LIGHT_GRAY = 37,
        FG_DARK_GRAY = 90,
        FG_LIGHT_RED = 91,
        FG_LIGHT_GREEN = 92,
        FG_LIGHT_YELLOW = 93,
        FG_LIGHT_BLUE = 94,
        FG_LIGHT_MAGENTA = 95,
        FG_LIGHT_CYAN = 96,
        FG_WHITE = 97,
        //background
        BG_DEFAULT  = 49,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_YELLOW = 43,
        BG_BLUE     = 44
                      //more here
    };

    static bool should_colorize()
    {
        char const* t = getenv("TERM");
        return isatty(STDOUT_FILENO) && (t && strcmp(t, "dumb") != 0);
    }
};

#ifndef NO_SYSTEM_STREAM_STDUTIL
extern istream<fdbuf> sis;
extern ostream<fdbuf> sos;
extern ostream<fdbuf> ses;
#endif

#endif//STDUTIL_HPP
