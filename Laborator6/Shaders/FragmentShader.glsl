#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_tex;
in vec3 frag_color;

layout(location = 3) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec3 out_tex;
layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(frag_normal, 1.0);
	out_normal = vec4(frag_normal, 1.0);
	out_tex = vec3(frag_tex, 1.0);
	out_position = vec4(frag_position, 1.0);
}