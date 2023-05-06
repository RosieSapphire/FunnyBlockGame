#include <stdio.h>
#include <malloc.h>
#include <glad/gl.h>
#include "shader.h"

static char *file_read_text(const char *path)
{
	FILE *file = fopen(path, "r");
	size_t file_size = 0L;
	char *buffer = NULL;

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);
	buffer = malloc(file_size);
	fread(buffer, sizeof(char), file_size, file);
	fclose(file);
	buffer[--file_size] = 0;

	return buffer;
}

GLuint shader_compile(const char *path, int type)
{
	GLuint shader = glCreateShader(type);
	char *source = file_read_text(path);
	GLint status;

	glShaderSource(shader, 1, (const char **)&source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if(status)
		return shader;

	char log[512];

	glGetShaderInfoLog(shader, 512, NULL, log);
	printf("Failed to create shader from '%s': %s\n", path, log);
	free(source);

	return 0;
}

GLuint shader_create(const char *vert_path, const char *frag_path)
{
	GLuint vert_shader = shader_compile(vert_path, GL_VERTEX_SHADER);
	GLuint frag_shader = shader_compile(frag_path, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	return program;
}

GLuint shader_get_loc(unsigned int s, const char *loc)
{
	return glGetUniformLocation(s, loc);
}

void shader_bind(unsigned int s)
{
	glUseProgram(s);
}

void shader_uni_mat4(unsigned int loc, rm_mat4 m4)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const float *)m4);
}

void shader_uni_vec3f(unsigned int loc, rm_vec3f v3f)
{
	glUniform3fv(loc, 1, (const float *)v3f);
}

void shader_uni_int(unsigned int loc, int i)
{
	glUniform1i(loc, i);
}
