#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

uniform vec3 colors[10];
uniform int color_bounds[9];

uniform vec3 test;

void main(){

    fColor = colors[0];

    gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
}