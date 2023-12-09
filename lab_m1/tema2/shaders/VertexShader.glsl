#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform float hp;

// Output
// TODO(student): Output values to fragment shader
out vec3 f_position;
out vec3 f_normal;
out vec2 f_coord;
out vec3 f_color;
out vec3 color;

void main()
{
    f_position = v_position;
    f_color = object_color;
    f_normal = v_normal;
    f_coord = v_coord;
	color = object_color;
    gl_Position = Projection * View * Model * vec4(v_position,1.0);
	

}