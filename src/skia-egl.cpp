#include <config.h>

#include <err.h>
#include <stdlib.h>

#define  SK_ATOMICS_PLATFORM_H "ports/SkAtomics_sync.h"
#define  SK_MUTEX_PLATFORM_H   "ports/SkMutex_pthread.h"
#include <GrGLInterface.h>
#include <GrRenderTarget.h>
#include <GrContext.h>
#include <SkGpuDevice.h>

#include <EGL/egl.h>
#if HAVE_GLES3_H
#include <GLES3/gl3.h>
#elif HAVE_GLES2_H
#include <GLES2/gl2.h>
#endif


#include "egl.h"
#include "device_config.h"

static egl_state_t *state;
static GrContext* ctx;

SkBaseDevice *
create_skia_device_egl (const device_config_t& config)
{
    GrBackendRenderTargetDesc desc;
    GrRenderTarget* target;
    SkGpuDevice *device;

    state = (egl_state_t*) malloc (sizeof (egl_state_t));
    if (!state) {
        warnx ("Out of memory\n");
        return NULL;
    }

    if (!createEGLContextAndWindow(state, config)) {
        cleanup_state_egl(state);
        return NULL;
    }

    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);

    desc.fWidth = config.width;
    desc.fHeight = config.height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;

    if (config.egl_sample_buffers > 0)
        desc.fSampleCnt = config.egl_samples;
    else
        desc.fSampleCnt = 0;
    desc.fStencilBits = 1;
    desc.fRenderTargetHandle = 0;
    ctx = GrContext::Create(kOpenGL_GrBackend, 0);
    if (ctx == NULL) {
        warnx("Could not establish a graphics context for Skia EGL\n");
        cleanup_state_egl(state);
        return NULL;
    }

    target = ctx->wrapBackendRenderTarget(desc);
    device = new SkGpuDevice(ctx, target);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    return device;
}

void
destroy_skia_egl(void)
{
    delete ctx;
    cleanup_state_egl(state);
}

void
update_skia_egl(void)
{
    ctx->flush();
    eglSwapBuffers(state->egl_display, state->egl_surface);
}
