#version 150

in vec4 a_pos;
in vec4 a_normal;
in vec4 a_color;

uniform mat4 u_mvp;

out vec4 v_color;

void main()
{
    v_color = abs(a_normal);
    gl_Position = u_mvp * a_pos;
}
