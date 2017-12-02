/*
 * Copyright 2017 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <err.h>
#include <stdlib.h>
#include <assert.h>

#include <cairo.h>
#include <cairo-xcb.h>

#include "xcb.h"
#include "device_config.h"

static xcb_state_t *state;
static cairo_surface_t *cairo_surface;

cairo_surface_t *
create_cairo_surface_xcb (const device_config_t& config)
{
    // TODO: This needs initted to 0
    state = (xcb_state_t*) malloc (sizeof (xcb_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createXCBContextAndWindow(state, config.width, config.height)) {
        cleanup_state_xcb(state);
        errx (-1, "Could not create XCB context and window\n");
    }

    assert(state->xcb_window >= 0);
    assert(state->xcb_connection);
    assert(state->xcb_visual_type);

    // TODO: Do XCB-specific setup and surface creation
    cairo_surface = cairo_xcb_surface_create(
        state->xcb_connection,     // XCB connection
        state->xcb_window,      // XCB drawable window
        state->xcb_visual_type, // Visual to use for drawing to window
        config.width,           // Window width
        config.height);         // Window height

    // TODO: Set background, clear colors, etc.

    return cairo_surface;
}

void
destroy_cairo_xcb(void)
{
    cleanup_state_xcb((void*)state);

    cairo_surface_finish(cairo_surface);

    // TODO: Do I need to destroy the surface here, or higher up?  If here, then do I need to track
    // cairo_surface in state?  Uncommented, this causes a ref count fault, so it appears to
    // already have been destroyed (maybe by _finish?)
    //cairo_surface_destroy(cairo_surface);
}

void
update_cairo_xcb(void)
{
    // TODO
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
