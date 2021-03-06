/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

int
ca_setup_quadratic(caskbench_context_t *ctx)
{
    cairo_set_antialias (ctx->cairo_cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width (ctx->cairo_cr, 1);
    return 1;
}

void
ca_teardown_quadratic(void)
{
}

int
ca_test_quadratic(caskbench_context_t *ctx)
{
    int w = ctx->canvas_width;
    int h = ctx->canvas_height;

    shapes_t shape;
    shape_copy(&ctx->shape_defaults, &shape);
    for (int i=0; i<ctx->size; i++) {
        shape.x  = (double)rnd()/RAND_MAX * w;
        shape.dx1 = (double)rnd()/RAND_MAX * w;
        shape.width = (double)rnd()/RAND_MAX * w - shape.x;
        shape.y  = (double)rnd()/RAND_MAX * h;
        shape.dy1 = (double)rnd()/RAND_MAX * h;
        shape.height = (double)rnd()/RAND_MAX * h - shape.y;

        cairoRandomizeColor(ctx);
        cairoDrawQuadraticCurve(ctx, &shape);
        cairo_stroke(ctx->cairo_cr);
    }

    return 1;
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
