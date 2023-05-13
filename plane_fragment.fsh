#version 150

uniform sampler2DShadow s_shadow;

in vec4 v_color;
in vec3 v_pos;
in vec3 v_light;
in vec3 v_normal;
in vec4 v_shadowCoord;

out vec4 outColor;

const float AMBIENT = 0.2f;
const float MAX_DISTANCE = 15.0f;
const float FRAGMENT_STEP = 0.005f;

float get_avg_shadow_value(vec4 coord)
{
    const float K = 1.0f / 9.0f;

    float val = 0.0f;

    for(int rx = -1; rx <= 1; rx++) {
        for(int ry = -1; ry <= 1; ry++) {
            float dx = FRAGMENT_STEP * rx * coord.w;
            float dy = FRAGMENT_STEP * ry * coord.w;
            vec4 offset = vec4(dx, dy, 0.0f, 0.0f);

            val += textureProj(s_shadow, v_shadowCoord + offset);
        }
    }

    return val * K;
}

float get_spotlight_value()
{
    float light_dist = length(v_pos - v_light);
    vec3 norm_light = normalize(v_light);

    float light_intensity = dot(v_normal, norm_light)
                            * (1 - (light_dist / MAX_DISTANCE));

    return light_intensity;
}

void main()
{
    float spot_value = get_spotlight_value();
    float shadow_value = get_avg_shadow_value(v_shadowCoord);
    float light_value = spot_value * shadow_value;

    outColor = max(AMBIENT, light_value) * v_color;
}
