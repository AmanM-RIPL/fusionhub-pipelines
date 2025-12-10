#version 330 core

in vec4 PickColorValue;

uniform vec4 pickColor;

out vec4 FragColor;

void main() {
   FragColor = PickColorValue;
}
