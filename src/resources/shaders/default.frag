#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoord;
flat in int MaterialIndex;
flat in int TextureIndex;

out vec4 FragColor;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform vec3 viewPosition;
uniform samplerBuffer materialBuffer;
uniform sampler2DArray textureArray;
uniform Light light;


Material getMaterial(int id) {
   int base = id * 3; // 3 texels per material
   vec4 t0 = texelFetch(materialBuffer, base);
   vec4 t1 = texelFetch(materialBuffer, base + 1);
   vec4 t2 = texelFetch(materialBuffer, base + 2);

   Material m;
   m.ambient   = t0.xyz;
   m.diffuse   = vec3(t0.w, t1.xy);
   m.specular  = vec3(t1.zw, t2.x);
   m.shininess = t2.y;

   return m;
}


void main() {
   Material material = getMaterial(MaterialIndex);

   // ambient
   vec3 ambient  = light.ambient * material.ambient;

   // diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPosition);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);

   // specular
   vec3 viewDir = normalize(viewPosition - FragPosition);
   vec3 reflectDir = reflect(-lightDir, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);


   // adding textures
   vec4 baseColor = vec4(1.0);

   if (TextureIndex >= 0) {
      baseColor = texture(textureArray, vec3(TexCoord, TextureIndex));
   }

   // final phong calculation
   vec3 result = ambient + diffuse + specular;
   FragColor = baseColor * vec4(result, 1.0);
}
