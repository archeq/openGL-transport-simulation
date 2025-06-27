#version 460 core

layout (location=0) in vec3 pos;
layout (location=1) in vec3 norm;
layout (location=2) in vec2 texture_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    // object position in camera cpace
    gl_Position = projection * view * model * vec4(pos, 1.0);

}