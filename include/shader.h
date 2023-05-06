#ifndef FBG_SHADER_H
#define FBG_SHADER_H

#include <rmath/mat4.h>

unsigned int shader_compile(const char *path, int type);
unsigned int shader_create(const char *vert_path, const char *frag_path);
unsigned int shader_get_loc(unsigned int s, const char *loc);
void shader_bind(unsigned int s);
void shader_uni_mat4(unsigned int loc, rm_mat4 m4);
void shader_uni_vec3f(unsigned int loc, rm_vec3f v3f);
void shader_uni_int(unsigned int loc, int i);

#endif
