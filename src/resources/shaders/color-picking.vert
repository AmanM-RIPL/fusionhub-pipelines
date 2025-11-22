#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   gl_Position = projection * view * model * vec4(position, 1.0);


   // Flip Y to match Qt Quick's coordinate system
   gl_Position.y = -gl_Position.y;
}
