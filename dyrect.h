#ifndef DYRECT_H
#define DYRECT_H
/* This header file defines with intuitive rectangle transformations.
 *
 * The intended purpose is to ease defining single screen interfaces.
 *  Layout engines are great, but very complex to implement and master.
 *  Static layouts on the other hand tend to get very ugly, verbose and bug prone.
 *  The idea is to have a very minimalistic abstraction layer,
 *   with easy to visualize operations.
 *  This solves readability and typo-ing x to y, while being widely usable,
 *   adaptable or even reimplementable within just a few minutes.
 *
 * No doubt someone somewhere has adapted a similar approach before,
 *  however to the best of my knowledge this is the first attempt
 *  to normalize it into a library.
 *  In case I'm wrong, please throw me an email. (agvxov@gmail.com)
 */

// TODO: further macro hell x, y, width and height so they can be user overwritten too
// TODO: stretch(x|y) naming is meh
// TODO: theres a logical inconsistency between ride-hang and rock-paper; either justify it or normalize it


// ### --------------- ###
// ### SPECIALIZATIONS ###
// ### --------------- ###
/* This has to go on top, because macros. Please read on.
 */

#ifdef RAYLIB_H
# define rect_t Rectangle
static inline
rect_t get_screen_rect(void) {
    return (rect_t) {
        .x = 0,
        .y = 0,
        .width  = (float)GetScreenWidth(),
        .height = (float)GetScreenHeight(),
    };
}
#endif


// ### ------- ###
// ### General ###
// ### ------- ###

/* Our internal rectangle representation.
 *  Feel free to overwrite it with whatever suits you,
 *   just #define alias it to `rect_t`.
 */
#ifndef rect_t
typedef struct rect_t {
    float x, y, width, height;
} rect_t;
#endif

#ifdef __NCURSES_H
# define DNUNPACK(r) (int)r.height, (int)r.width, (int)r.y, (int)r.x
static inline
rect_t get_screen_rect(void) {
    return (rect_t) {
        .x = 0,
        .y = 0,
        .width  = (float)COLS,
        .height = (float)LINES,
    };
}
#endif

// tl;dr
static inline rect_t rfloor(rect_t r);
static inline rect_t get_unit_rect(void);
static inline rect_t scaley(rect_t a, float f);
static inline rect_t scalex(rect_t a, float f);
static inline rect_t scale(rect_t a, float f);
static inline rect_t balance(rect_t dest, rect_t source);
static inline rect_t buoyance(rect_t dest, rect_t source);
static inline rect_t center(rect_t dest, rect_t source);
static inline rect_t hang(rect_t dest, rect_t source);
static inline rect_t ride(rect_t dest, rect_t source);
static inline rect_t rock(rect_t dest, rect_t source);
static inline rect_t paper(rect_t dest, rect_t source);
static inline rect_t next(rect_t source, int n);
static inline rect_t after(rect_t source, int n);
static inline rect_t stretchy(rect_t dest, rect_t source);
static inline rect_t stretchx(rect_t dest, rect_t source);



/* Floor every field of a rect.
 *  Useful if next() or after() create visible gaps.
 *  NOTE: we are not actually flooring so we dont depend on <math.h>,
 *         for our ends and purposes it should just werk™
 */
static inline
rect_t rfloor(rect_t r) {
    return (rect_t) {
        .x = (long long)r.x,
        .y = (long long)r.y,
        .width  = (long long)r.width,
        .height = (long long)r.height,
    };
}


/* Return the easiest rect to transform.
 *  Coordinates (0, 0) are easy to shift.
 *  Size 1x1 is easy to scale.
 * 
 *   +-+
 *   +-+
 */
static inline
rect_t get_unit_rect(void) {
    return (rect_t) {
        .x = 0,
        .y = 0,
        .width  = 1,
        .height = 1,
    };
}


/* Modify the width by a factor.
 *
 *   +-+   __\  +---+
 *   +-+     /  +---+
 */
static inline
rect_t scalex(rect_t a, float f) {
    return (rect_t) {
        .x = a.x,
        .y = a.y,
        .width  = a.width  * f,
        .height = a.height,
    };
}


/* Modify the height by a factor.
 *
 *   +-+   __\  +-+
 *   +-+     /  | |
 *              +-+
 */
static inline
rect_t scaley(rect_t a, float f) {
    return (rect_t) {
        .x = a.x,
        .y = a.y,
        .width  = a.width,
        .height = a.height * f,
    };
}


/* Modify the height and width by a factor.
 *  Exists because its judged to be a common operation.
 *
 *   +-+   __\  +---+
 *   +-+     /  |   |
 *              +---+
 */
static inline
rect_t scale(rect_t a, float f) {
    return (rect_t) {
        .x = a.x,
        .y = a.y,
        .width  = a.width  * f,
        .height = a.height * f,
    };
}

/* Align to the middle horizontally
 *
 *   +---+---+---+
 *   | :         |
 *   +---+       |
 *   |   |       |
 *   |   |       |
 *   +---+       |
 *   | :         |
 *   +---+---+---+
 */
static inline
rect_t balance(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = dest.x + ((dest.width - source.width) / 2),
        .y = source.y,
        .width  = source.width,
        .height = source.height,
    };
}

/* Align to the middle vertically
 *
 *   +---+---+---+
 *   |   |   |   |
 *   | - |   | - |
 *   |   +---+   |
 *   |           |
 *   |           |
 *   |           |
 *   +---+---+---+
 */
static inline
rect_t buoyance(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = source.x,
        .y = dest.y + ((dest.height - source.height) / 2),
        .width  = source.width,
        .height = source.height,
    };
}


/* Balance and Buoyance. Align to the middle vertically and horizontally.
 *  Exists because its judged to be a common operation.
 *
 *   +-----------+
 *   |     :     |
 *   |   +---+   |
 *   | - |   | - |
 *   |   |   |   |
 *   |   +---+   |
 *   |     :     |
 *   +-----------+
 */
static inline
rect_t center(rect_t dest, rect_t source) {
    return balance(dest, buoyance(dest, source));
}


/* Dangles from the top.
 *
 *   +---+-------+
 *   |   |       |
 *   |   |       |
 *   +---+       |
 *   |           |
 *   |           |
 *   |           |
 *   +-----------+
 */
static inline
rect_t hang(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = source.x,
        .y = dest.y,
        .width  = source.width,
        .height = source.height,
    };
}


/* Places on the top.
 *
 *   +---+
 *   |   |
 *   |   |
 *   +---+-------+
 *   |           |
 *   |           |
 *   |           |
 *   |           |
 *   |           |
 *   |           |
 *   +-----------+
 */
static inline
rect_t ride(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = source.x,
        .y = dest.y - source.height,
        .width  = source.width,
        .height = source.height,
    };
}


/* Moves to the left.
 *  NOTE: this is a reference to the political compass, for easy memorization.
 *
 *   +---+-------+
 *   |   |       |
 *   |   |       |
 *   +---+       |
 *   |           |
 *   |           |
 *   |           |
 *   +-----------+
 */
static inline
rect_t rock(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = dest.x,
        .y = source.y,
        .width  = source.width,
        .height = source.height,
    };
}


/* Moves to the right.
 *  NOTE: this is a reference to the political compass, for easy memorization.
 *
 *   +-------+---+
 *   |       |   |
 *   |       |   |
 *   |       +---+
 *   |           |
 *   |           |
 *   |           |
 *   +-----------+
 */
static inline
rect_t paper(rect_t dest, rect_t source) {
    return (rect_t) {
        .x = (dest.x + dest.width) - source.width,
        .y = source.y,
        .width  = source.width,
        .height = source.height,
    };
}


/* Gets the N-th horizontal neighbour.
 *
 *   +---+---+
 *   |   |   |
 *   |   |   |
 *   +---+---+
 */
static inline
rect_t next(rect_t source, int n) {
    return (rect_t) {
        .x = source.x + (source.width * n),
        .y = source.y,
        .width  = source.width,
        .height = source.height,
    };
}

/* Gets the N-th vertical neighbour.
 *
 *   +---+    
 *   |   |
 *   |   |
 *   +---+
 *   |   |
 *   |   |
 *   +---+
 */
static inline
rect_t after(rect_t source, int n) {
    return (rect_t) {
        .x = source.x,
        .y = source.y + (source.height * n),
        .width  = source.width,
        .height = source.height,
    };
}

static inline
rect_t stretchy(rect_t dest, rect_t source) {
    return (dest.y > source.y) ?
        (rect_t) {
            .x = source.x,
            .y = source.y,
            .width  = source.width,
            .height = dest.y - source.y,
        }
    :
        (rect_t) {
            .x = source.x,
            .y = dest.y + dest.height,
            .width  = source.width,
            .height = source.height + (source.y - (dest.y + dest.height)),
        }
    ;
}

static inline
rect_t stretchx(rect_t dest, rect_t source) {
    return (dest.x > source.x) ?
        (rect_t) {
            .x = source.x,
            .y = source.y,
            .width  = dest.x - source.x,
            .height = source.height,
        }
    :
        (rect_t) {
            .x = dest.x + dest.width,
            .y = source.y,
            .width  = source.width + (source.x - (dest.x + dest.width)),
            .height = source.height,
        }
    ;
}

#endif

// This header is in the Public Domain. If you say this notice is inadequate, I will sue you.
