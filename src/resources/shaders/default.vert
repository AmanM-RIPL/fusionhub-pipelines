#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in int materialIndex;
layout (location = 4) in int textureIndex;
layout (location = 5) in int modelMatrixIndex;

uniform samplerBuffer modelMatrixBuffer;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPosition;
out vec2 TexCoord;
flat out int MaterialIndex;
flat out int TextureIndex;

void main() {

   // A mat4 takes 4 vec4 slots. We must calculate the starting offset.
   int baseIndex = modelMatrixIndex * 4;

   // Fetch the 4 columns/rows of the matrix manually
   vec4 col0 = texelFetch(modelMatrixBuffer, baseIndex + 0);
   vec4 col1 = texelFetch(modelMatrixBuffer, baseIndex + 1);
   vec4 col2 = texelFetch(modelMatrixBuffer, baseIndex + 2);
   vec4 col3 = texelFetch(modelMatrixBuffer, baseIndex + 3);

   mat4 model = mat4(col0, col1, col2, col3);

   gl_Position = projection * view * model * vec4(position.xyz, 1.0);
   FragPosition = vec3(model * vec4(position.xyz, 1.0));
   Normal = mat3(transpose(inverse(model))) * normal;
   MaterialIndex = materialIndex;
   TexCoord = uv;
   TextureIndex = textureIndex;

   // Flip Y to match Qt Quick's coordinate system
   gl_Position.y = -gl_Position.y;
}
