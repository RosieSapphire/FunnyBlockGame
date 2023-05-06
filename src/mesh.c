#include <glad/gl.h>
#include <malloc.h>
#include <string.h>

#include <rmath/vec4f.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimp/material.h"
#include "camera.h"
#include "mesh.h"
#include "rmath/mat4.h"
#include "rmath/vec3f.h"

static struct vertex mesh_cube_verts[] = {
	/* front */
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},
		{ 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},
		{ 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f},
		{ 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f},
		{ 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}},

	/* left */
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},
		{-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},
		{-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},
		{-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f},
		{-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	/* right */
	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},
		{ 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f},
		{ 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},
		{ 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},
		{ 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	/* back */
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},
		{ 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},
		{ 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},
		{ 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f},
		{ 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},

	/* bottom */
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},
		{ 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},
		{ 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},
		{ 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f},
		{ 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},

	/* top */
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},
		{ 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},
		{ 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f},
		{ 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},
		{ 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}},
};

static GLuint mesh_cube_indis[] = {
	0, 1, 2,
	2, 1, 3,

	4, 5, 6,
	6, 5, 7,

	8, 9, 10,
	10, 9, 11,

	12, 13, 14,
	14, 13, 15,

	16, 17, 18,
	18, 17, 19,

	20, 21, 22,
	22, 21, 23,
};

static struct vertex mesh_quad_verts[] = {
	{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
	{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
	{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f},
		{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
};

static GLuint mesh_quad_indis[6] = {
	0, 1, 2,
	2, 1, 3
};

struct mesh *mesh_create_data(struct vertex *verts, GLuint *indis,
		int num_verts, int num_indis)
{
	struct mesh *m = malloc(sizeof(*m));

	int verts_size = sizeof(struct vertex) * num_verts;
	int indis_size = sizeof(GLuint) * num_indis;

	m->num_verts = num_verts;
	m->num_indis = num_indis;
	m->verts = malloc(verts_size);
	m->indis = malloc(indis_size);
	rm_vec3f_copy(RM_VEC3F_ZERO, m->pos);
	rm_vec3f_copy(RM_VEC3F_ZERO, m->rot);
	memcpy(m->verts, verts, verts_size);
	memcpy(m->indis, indis, indis_size);

	glGenVertexArrays(1, &m->vao);
	glBindVertexArray(m->vao);
	glGenBuffers(1, &m->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);

	glBufferData(GL_ARRAY_BUFFER, verts_size, m->verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*m->verts),
			(void *)offsetof(struct vertex, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*m->verts),
			(void *)offsetof(struct vertex, uv));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(*m->verts),
			(void *)offsetof(struct vertex, norm));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(*m->verts),
			(void *)offsetof(struct vertex, col));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m->indis),
			m->indis, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return m;
}

struct mesh *mesh_create_file(const char *path)
{
	const struct aiScene *scene =
		aiImportFile(path, aiProcess_JoinIdenticalVertices |
				aiProcess_ImproveCacheLocality |
				aiProcess_Triangulate);

	if(!scene) {
		printf("Failed to load model from %s.\n", path);
		return NULL;
	}

	const struct aiMesh *ai_mesh = *scene->mMeshes;
	int num_verts = ai_mesh->mNumVertices;
	int num_faces = ai_mesh->mNumFaces;
	int num_indis = num_faces * 3;
	struct vertex *verts = malloc(num_verts * sizeof(struct vertex));
	GLuint *indis = malloc(num_indis * sizeof(GLuint));

	for(int i = 0; i < num_verts; i++) {
		const struct aiVector3D ai_pos = ai_mesh->mVertices[i];
		const struct aiVector3D ai_uv = ai_mesh->mTextureCoords[0][i];
		const struct aiVector3D ai_norm = ai_mesh->mNormals[i];
		const struct aiColor4D *ai_col = ai_mesh->mColors[0];

		verts[i].pos[0] = ai_pos.x;
		verts[i].pos[1] = ai_pos.y;
		verts[i].pos[2] = ai_pos.z;
		verts[i].uv[0] = ai_uv.x;
		verts[i].uv[1] = ai_uv.y;
		verts[i].norm[0] = ai_norm.x;
		verts[i].norm[1] = ai_norm.y;
		verts[i].norm[2] = ai_norm.z;

		if(ai_col) {
			verts[i].col[0] = ai_col[i].r;
			verts[i].col[1] = ai_col[i].g;
			verts[i].col[2] = ai_col[i].b;

			continue;
		}

		rm_vec3f_copy(RM_VEC3F_ONE, verts[i].col);
	}

	int indis_counted = 0;

	for(int i = 0; i < num_faces; i++) {
		const struct aiFace face = ai_mesh->mFaces[i];
			
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indis[indis_counted++] = face.mIndices[j];
		}
	}

	struct mesh *mesh = mesh_create_data(verts, indis,
			num_verts, num_indis);

	free(verts);
	free(indis);

	return mesh;
}

struct mesh *mesh_create_type(enum mesh_type type)
{
	struct vertex *type_verts[MESH_TYPE_COUNT] = {
		mesh_cube_verts,
		mesh_quad_verts
	};

	GLuint *type_indis[MESH_TYPE_COUNT] = {
		mesh_cube_indis,
		mesh_quad_indis
	};

	size_t type_verts_sizes[MESH_TYPE_COUNT] = {
		sizeof(mesh_cube_verts),
		sizeof(mesh_quad_verts),
	};

	size_t type_indis_sizes[MESH_TYPE_COUNT] = {
		sizeof(mesh_cube_indis),
		sizeof(mesh_quad_indis),
	};

	int verts_size = type_verts_sizes[type];
	int indis_size = type_indis_sizes[type];
	int num_verts = verts_size / sizeof(struct vertex);
	int num_indis = indis_size / sizeof(GLuint);

	return mesh_create_data(type_verts[type], type_indis[type],
			num_verts, num_indis);
}

void mesh_get_model_mat4(struct mesh m, rm_mat4 out)
{
	rm_mat4_identity(out);
	rm_mat4_rotate_x(out, m.rot[0]);
	rm_mat4_rotate_y(out, m.rot[1]);
	rm_mat4_rotate_z(out, m.rot[2]);
	rm_mat4_translate(out, m.pos);
}

void mesh_draw(struct mesh *m, GLuint shader, GLuint texture)
{
	int is_using_tex_loc = glGetUniformLocation(shader, "u_is_using_tex");

	if(is_using_tex_loc == -1) {
		printf("Cannot find uniform in shader %d"
				" 'u_is_using_tex'\n", shader);

		return;
	}

	glUseProgram(shader);
	glUniform1i(is_using_tex_loc, texture > 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(m->vao);
	glDrawElements(GL_TRIANGLES, m->num_indis, GL_UNSIGNED_INT, m->indis);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void mesh_destroy(struct mesh *m)
{
	glDeleteVertexArrays(1, &m->vao);
	glDeleteBuffers(1, &m->vbo);
	glDeleteBuffers(1, &m->ebo);
	free(m->verts);
	free(m->indis);
	free(m);
}
