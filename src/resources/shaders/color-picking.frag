#version 330 core

uniform vec4 pickColor;

out vec4 FragColor;

void main() {
   FragColor = pickColor;
}
