#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in int materialIndex;
layout (location = 4) in int textureIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPosition;
out vec2 TexCoord;
flat out int MaterialIndex;
flat out int TextureIndex;

void main() {
   gl_Position = projection * view * model * vec4(position, 1.0);
   FragPosition = vec3(model * vec4(position, 1.0));
   Normal = mat3(transpose(inverse(model))) * normal;
   MaterialIndex = materialIndex;
   TexCoord = uv;
   TextureIndex = textureIndex;

   // Flip Y to match Qt Quick's coordinate system
   gl_Position.y = -gl_Position.y;
}
