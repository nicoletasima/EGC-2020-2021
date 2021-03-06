#version 330

// TODO: get vertex attributes from each location
	layout(location = 0) in vec3 v_position;
	layout(location = 1) in vec3 v_normal;
	layout(location = 2) in vec2 tex_coord;
	layout(location = 3) in vec3 v_color;
	

// Uniform properties
	uniform mat4 Model;
	uniform mat4 View;
	uniform mat4 Projection;
	uniform float Time;

// TODO: output values to fragment shader
	out vec3 frag_position;
	out vec3 frag_normal;
	out vec2 frag_tex;
	out vec3 frag_color;
	

void main()
{
	// TODO: send output to fragment shader
	frag_position = v_position;
	frag_normal = v_normal + vec3(sin(Time), cos(Time), sin(Time));
	frag_tex = tex_coord;
	frag_color = v_color;

	// TODO: compute gl_Position -> predefinita, salveaza pozitia vertexului curent
	vec4 newPos = Model * vec4(v_position, 1.0) + vec4(0.0, sin(Time), 0.0, 0.0);
	gl_Position = Projection * View * newPos;
}
