#version 330 core

in vec2 o_uv;
in vec3 o_vert_col;
in vec3 o_norm;
in vec3 o_frag_pos;

uniform sampler2D u_tex;
uniform vec3 u_view_pos;
uniform bool u_is_using_tex;

out vec4 frag_color;

void main()
{
	vec3 light_pos = vec3(2);
	vec3 light_dir = normalize(light_pos - o_frag_pos);
	vec3 light_col = vec3(1);
	vec3 ambient = 0.1 * light_col;
	vec3 diffuse = max(dot(o_norm, light_dir), 0.0) * light_col;

	vec3 base = vec3(1);

	if(u_is_using_tex) {
 		base = texture(u_tex, o_uv).xyz;
	}

 	base *= o_vert_col;

	frag_color = vec4((ambient + diffuse) * base, 1.0);
}
