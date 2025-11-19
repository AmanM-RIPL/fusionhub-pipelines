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
uniform Material materials[2];
uniform sampler2D textures[1];
uniform Light light;

void main() {
   Material material = materials[MaterialIndex];

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
      baseColor = texture(textures[TextureIndex], TexCoord);
   }

   // final phong calculation
   vec3 result = ambient + diffuse + specular;
   FragColor = baseColor * vec4(result, 1.0);
}
