#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 light_pos;
out vec3 view_pos;
out vec3 view_normal;

void main()
{
    view_normal = normalize(mat3(view) * normal);
    view_pos = vec3(view * world * vec4(pos,1.0f));
    light_pos = vec3(view * vec4(0.0f,0.0f,64.0f,1.0f));
    gl_Position = projection * view * world * vec4(pos,1.0f);
}
