/*
 * Copyright 2017 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#ifndef XCB_H_    // -*- mode: c++ -*-
#define XCB_H_

#include <xcb/xcb.h>

typedef struct {
    int               xcb_screen_number;
    xcb_screen_t     *xcb_screen;
    xcb_window_t      xcb_window;
    xcb_visualtype_t *xcb_visual_type;
    xcb_connection_t *xcb_connection;
} xcb_state_t;

bool createXCBContextAndWindow(xcb_state_t *state, int width, int height);
void destroyXCBContextAndWindow(xcb_state_t *state);
void cleanup_state_xcb(void *state);

#endif // XCB_H_
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
