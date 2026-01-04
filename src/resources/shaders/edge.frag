#version 330 core
flat in int MaterialIndex;
flat in bool uDashRequired;
in float vDist;
in float uDash;
in float uGap;

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

uniform samplerBuffer materialBuffer;
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

void main()
{
   // 1. generating dashes
   if (uDashRequired == true)
    {
        float pattern = uDash + uGap;
        if (mod(vDist, pattern) > uDash)
        {
            discard;
        }
    }

   // 2. get ambient part of the color only (no phong lighting model for edges)
   Material material = getMaterial(MaterialIndex);
   vec3 ambient  = light.ambient * material.ambient;

   // 3. final phong calculation
   vec4 baseColor = vec4(1.0);
   FragColor = baseColor * vec4(ambient, 1.0);
}
