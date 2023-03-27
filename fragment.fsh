#version 150

in vec3 v_normal;
in vec3 v_color;
in vec3 v_light;

const float ambient_intensity = 0.2f;

void main(void)
{
    float intensity = max(ambient_intensity, dot(v_normal, v_light));

    gl_FragColor = vec4(v_color, 1.0f) * intensity;
}
