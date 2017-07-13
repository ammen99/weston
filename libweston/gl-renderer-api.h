/*
 * Copyright © 2017 Ilia Bozhinov
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WESTON_GL_RENDERER_API_H
#define WESTON_GL_RENDERER_API_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "plugin-registry.h"

struct weston_compositor;
struct weston_output;
struct weston_surface;

#define WESTON_GL_RENDERER_API_NAME "weston_gl_renderer_api_v1"

struct weston_gl_renderer_api {
    /* Get the renderer's EGLDisplay and EGLContext
     *
     * \param  ec The compositor instance whose states are queried
     */
    void* (*compositor_get_egl_display)(struct weston_compositor *ec);
    void* (*compositor_get_egl_context)(struct weston_compositor *ec);

    /* Get the output's EGLSurface
     *
     * \param output The output whose surface is queried
     */
    void* (*output_get_egl_surface)(struct weston_output *output);

    /* Get the gl-renderer's textures for a given surface
     *
     * \param surface The surface whose textures are queried
     * \param n_tex The number of textures for this surface
     */
    void* (*surface_get_textures)(struct weston_surface *surface, int *n_tex);
};

static inline const struct weston_gl_renderer_api *
weston_gl_renderer_get_api(struct weston_compositor *compositor)
{
	const void *api;
	api = weston_plugin_api_get(compositor, WESTON_GL_RENDERER_API_NAME,
				    sizeof(struct weston_gl_renderer_api));

	return (const struct weston_gl_renderer_api *)api;
}

#ifdef  __cplusplus
}
#endif

#endif /* WESTON_GL_RENDERER_API_H */
