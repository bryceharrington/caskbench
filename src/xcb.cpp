/*
 * Copyright 2017 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xcb.h"
#include "device_config.h"

bool
createXCBContextAndWindow(xcb_state_t *state, int width, int height)
{
    xcb_screen_t *screen;
    uint32_t mask[2];
    const xcb_setup_t *setup;
    xcb_visualtype_t *visual;
    xcb_generic_event_t *event;
    xcb_screen_iterator_t screen_iter;
    xcb_depth_iterator_t depth_iter;

    // Set up XCB
    state->xcb_connection = xcb_connect(NULL, &state->xcb_screen_number);
    if (state->xcb_connection == NULL) {
        warnx("Could not create XCB context\n");
        return false;
    } else if (xcb_connection_has_error(state->xcb_connection)) {
        warnx("Could not connect to X11 server\n");
        return false;
    }

    // Set up screen
    setup = xcb_get_setup(state->xcb_connection);
    if (setup == NULL) {
        warnx("Could not create XCB setup\n");
        return false;
    }
    screen_iter = xcb_setup_roots_iterator(setup);
    for (; screen_iter.rem; --state->xcb_screen_number, xcb_screen_next(&screen_iter)) {
        if (state->xcb_screen_number == 0) {
            state->xcb_screen = screen_iter.data;
            break;
        }
    }
    if (state->xcb_screen == NULL) {
        warnx("Could not create XCB screen\n");
        return false;
    }

    // Create window
    state->xcb_window = xcb_generate_id(state->xcb_connection);
    mask[0] = 1;
    mask[1] = XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(state->xcb_connection,             // XCB connection to X11
                      XCB_COPY_FROM_PARENT,           // Window's depth
                      state->xcb_window,              // Window ID
                      state->xcb_screen->root,        // Parent window
                      20, 20,                         // X, Y position
                      width,                          // Window width
                      height,                         // Window height
                      0,                              // Border width in pixels
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // Behavior class for window
                      state->xcb_screen->root_visual, // New window's visual id
                      XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK,  // Mask type
                      mask);                          // Mask values list
    xcb_map_window(state->xcb_connection, state->xcb_window);
    xcb_flush(state->xcb_connection);

    // Get the visual
    depth_iter = xcb_screen_allowed_depths_iterator(state->xcb_screen);
    for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
        xcb_visualtype_iterator_t visual_iter;

        visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
        for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
            if (state->xcb_screen->root_visual == visual_iter.data->visual_id) {
                state->xcb_visual_type = visual_iter.data;
                return true;
            }
        }
    }

    warnx("XCB internal error, could not find visual\n");
    xcb_disconnect(state->xcb_connection);
    return false;
}

void
destroyXCBContextAndWindow (xcb_state_t *state)
{
    xcb_disconnect(state->xcb_connection);
}
 
void
cleanup_state_xcb (void *data)
{
    xcb_state_t *state = (xcb_state_t*)data;
    destroyXCBContextAndWindow(state);
    free(state);
}

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
