#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_norm;
layout(location = 3) in vec3 a_col;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec2 o_uv;
out vec3 o_vert_col;
out vec3 o_norm;
out vec3 o_frag_pos;

void main()
{
	mat4 mvp = u_projection * u_view * u_model;
	vec4 vertex_pos = vec4(a_pos, 1.0);

	gl_Position = mvp * vertex_pos;

	vec3 frag_pos = vec3(u_model * vertex_pos);
	vec3 norm = mat3(transpose(inverse(u_model))) *
			normalize(a_norm);

	o_uv = a_uv;
	o_norm = norm;
	o_frag_pos = frag_pos;
	o_vert_col = a_col;
}
