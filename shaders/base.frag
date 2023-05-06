#version 330 core

in vec2 o_uv;
in vec3 o_ambient;
in vec3 o_diffuse;
in vec3 o_light_col;
in vec3 o_vert_col;

uniform sampler2D u_tex;
uniform vec3 u_view_pos;

out vec4 frag_color;

void main()
{
	vec3 base = texture(u_tex, o_uv).xyz * o_vert_col;

	frag_color = vec4((o_ambient + o_diffuse) * base, 1.0);
}
