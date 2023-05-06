#version 330 core

in vec2 o_uv;

uniform sampler2D u_tex;

out vec4 frag_color;

void main()
{
	frag_color = vec4(texture(u_tex, o_uv).xyz, 1.0);
}
