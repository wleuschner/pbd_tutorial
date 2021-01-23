#version 330

in vec3 light_pos;
in vec3 view_pos;
in vec3 view_normal;

layout(location=0)out vec4 color;
void main()
{
    vec3 l = normalize(view_pos - light_pos);
    vec3 e = normalize(-view_pos);
    vec3 r = normalize(reflect(l,view_normal));

    float diff = max(dot(l,view_normal),0.0f);
    diff = clamp(diff,0.0,1.0);

    float spec = max(pow(max(dot(r,e),0.0),64.0),0.0);

    color = clamp(spec*vec4(1.0f,1.0f,1.0f,1.0f)+diff*vec4(0.0,0.8,0.0,1.0)+vec4(0.0,0.4,0.0,1.0),vec4(0.0),vec4(1.0));
}
