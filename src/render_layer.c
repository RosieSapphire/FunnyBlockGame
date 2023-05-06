#include <glad/gl.h>
#include <stddef.h>
#include <rmath/vec4f.h>
#include <malloc.h>

#include "render_layer.h"
#include "mesh.h"
#include "texture.h"

struct render_layer *render_layer_create(int width, int height)
{
	struct render_layer *layer = malloc(sizeof(*layer));

	layer->quad_mesh = mesh_create_type(MESH_QUAD);
	layer->tex = texture_create_empty(width, height);

	glGenFramebuffers(1, &layer->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, layer->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, layer->tex, 0);

	glGenRenderbuffers(1, &layer->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, layer->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,
			GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, layer->rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER)
			!= GL_FRAMEBUFFER_COMPLETE) {
		printf("Failed to create framebuffer.\n");
		return NULL;
	}

	layer->width = width;
	layer->height = height;

	return layer;
}

void render_layer_draw(const struct render_layer *l, GLuint shader,
		int dst_width, int dst_height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, l->fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, dst_width, dst_height);
	glBlitFramebuffer(0, 0, l->width, l->height, 0, 0,
			dst_width, dst_height, GL_COLOR_BUFFER_BIT,
			GL_NEAREST);
	glDisable(GL_DEPTH_TEST);

	struct mesh *m = l->quad_mesh;

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, l->tex);
	glBindVertexArray(m->vao);
	glDrawElements(GL_TRIANGLES, m->num_indis, GL_UNSIGNED_INT, m->indis);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void render_layer_bind_and_clear(const struct render_layer *l,
		float r, float g, float b, float a)
{
	glBindFramebuffer(GL_FRAMEBUFFER, l->fbo);
	glViewport(0, 0, l->width, l->height);
	glClearColor(r, g, b, a);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_layer_destroy(struct render_layer *l)
{
	glDeleteFramebuffers(1, &l->fbo);
	glDeleteRenderbuffers(1, &l->rbo);
	glDeleteTextures(1, &l->tex);
	mesh_destroy(l->quad_mesh);
}
