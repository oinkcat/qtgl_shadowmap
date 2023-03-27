#version 150

in vec4 a_pos;
in vec3 a_normal;
in vec3 a_color;

uniform vec3 u_light;
uniform mat4 u_mvp;

out vec3 v_normal;
out vec3 v_color;
out vec3 v_light;

void main(void)
{
    v_normal = a_normal;
    v_color = a_color;
    v_light = u_light;

    gl_Position = u_mvp * a_pos;
}
