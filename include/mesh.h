#ifndef FBG_MESH_H
#define FBG_MESH_H

#include <rmath/vec3f.h>
#include <rmath/vec2f.h>

#include "camera.h"

struct vertex {
	rm_vec3f pos;
	rm_vec2f uv;
	rm_vec3f norm;
	rm_vec3f col;
};

struct mesh {
	int num_verts, num_indis;
	struct vertex *verts;
	unsigned int *indis;
	unsigned int vao, vbo, ebo;
	rm_vec3f pos, rot;
};

enum mesh_type {
	MESH_CUBE = 0,
	MESH_QUAD,
	MESH_TYPE_COUNT
};

struct mesh *mesh_create_data(struct vertex *verts, GLuint *indis,
		int num_verts, int num_indis);
struct mesh *mesh_create_file(const char *path);
struct mesh *mesh_create_type(enum mesh_type type);
void mesh_get_model_mat4(struct mesh m, rm_mat4 out);
void mesh_draw(struct mesh *m, GLuint shader, GLuint texture);
void mesh_destroy(struct mesh *m);

#endif
