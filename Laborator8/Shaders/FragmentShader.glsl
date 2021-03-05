#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int is_spot;
uniform float cut_off_angle;

layout(location = 0) out vec4 out_color;

void main()
{	
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize( L + V );
	vec3 R = normalize(reflect(-L, world_normal));
	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max (dot(world_normal, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}
	
	float light = 0;

	if (is_spot == 0) {

		float d = distance(world_position, light_position);
		float factorAtenuare = 1 / (1 + 0.01 * d + 0.1 * d * d);
		light = ambient_light + factorAtenuare * (diffuse_light + specular_light); 

	} else if (is_spot == 1) {
	    	float cut_off = radians(cut_off_angle);
			float spot_light = dot(-L, light_direction);
			float spot_light_limit = cos(cut_off);

		if (spot_light > spot_light_limit) {

			float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else {
			light = ambient_light;
		}
	}


	vec3 color = object_color * light;

	// TODO: write pixel out color
	out_color = vec4(color, 1);
}