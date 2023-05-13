#version 150

uniform vec3 u_light;
uniform mat4 u_mvp;
uniform mat4 u_mvpLight;

in vec4 a_pos;
in vec3 a_normal;
in vec4 a_color;

out vec4 v_color;
out vec4 v_shadowCoord;
out vec3 v_light;
out vec3 v_pos;
out vec3 v_normal;

void main()
{
    v_color = a_color;
    v_light = u_light;
    v_pos = a_pos.xyz;
    v_normal = a_normal;

    gl_Position = u_mvp * a_pos;
    v_shadowCoord = (u_mvpLight * a_pos) + 1.0f;
}
