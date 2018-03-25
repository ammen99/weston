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
#include "compositor.h"
#include <stdbool.h>
#include <pixman.h>

struct weston_compositor;
struct weston_output;
struct weston_surface;

#define WESTON_GL_RENDERER_API_NAME "weston_gl_renderer_api_v1"

typedef bool (*custom_renderer_func_t) (struct weston_output*, pixman_region32_t*);
typedef void (*post_render_func_t) (struct weston_output*);

enum gl_texture_format
{
	GL_TEXTURE_FORMAT_RGBA,
	GL_TEXTURE_FORMAT_RGBX,
	GL_TEXTURE_FORMAT_EGL,
	GL_TEXTURE_FORMAT_Y_UV,
	GL_TEXTURE_FORMAT_Y_U_V,
	GL_TEXTURE_FORMAT_Y_XUXV,
	GL_TEXTURE_FORMAT_SOLID
};

struct weston_gl_renderer_api {
	/* Set custom renderer function
	 *
	 * \param ec       The compositor instance
	 * \param renderer The function which should be called at each repaint cycle
	 *
	 * The renderer function should return true if it has painted the output and the
	 * in-built gl-renderer should leave the output as is. Otherwise, the gl-renderer proceeds
	 * as if there is no custom renderer.
	 */
	void (*set_custom_renderer) (struct weston_compositor *ec, custom_renderer_func_t renderer);

	/* Set custom post-renderer function
	 *
	 * \param ec   The compositor instance
	 * \param post The function which should be called right after each repaint
	 *
	 * The post function is called after each rendered frame, regardless of whether
	 * a custom renderer or the in-built gl-renderer was used
	 */
	void (*set_post_render) (struct weston_compositor *ec, post_render_func_t post);

	/* Schedule output repaint
	 *
	 * \param output The output for which a repaint is to be scheduled
	 *
	 * Unlike weston_output_schedule_repaint, this function must ensure that
	 * the gl-renderer will be invoked at the next frame. Useful when used together
	 * with custom renderer/post function, as it must bypass any optimizations in
	 * the drm-backend
	 */
	void (*schedule_repaint) (struct weston_output *output);

	/* Get the output viewport
	 *
	 * \param output The output whose viewport is queried
	 *
	 * This function returns the GL viewport taking into consideration
	 * the output borders. Useful for custom renderers when running nested
	 */
	struct weston_geometry (*get_output_gl_viewport) (struct weston_output *output);

	/* Get the gl-renderer's textures for a given surface
	 *
	 * \param surface The surface whose textures are queried
	 * \param n_tex The number of textures for this surface
	 */
	void* (*surface_get_textures)(struct weston_surface *surface, int *n_tex);

	/* Get the texture format
	 *
	 * \param surface The surface whose textures are queried
	 */
	enum gl_texture_format (*surface_get_texture_format)(struct weston_surface *surface, uint32_t *target);
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
