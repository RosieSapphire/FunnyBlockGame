#ifndef FBG_RENDER_LAYER_H
#define FBG_RENDER_LAYER_H

#include "mesh.h"

struct render_layer {
	struct mesh *quad_mesh;
	unsigned int fbo, rbo, tex, width, height;
};

struct render_layer *render_layer_create(int width, int height);
void render_layer_bind_and_clear(const struct render_layer *l,
		float r, float g, float b, float a);
void render_layer_draw(const struct render_layer *l, GLuint shader,
		int dst_width, int dst_height);
void render_layer_destroy(struct render_layer *l);

#endif
