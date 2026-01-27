#version 330 core
layout (location = 0) in float corner; // -1 or +1
layout (location = 1) in ivec2 edge;   // v0, v1 indices
layout (location = 2) in float edgeWidth;
layout (location = 3) in int materialIndex;
layout (location = 4) in float edgeDashLength;
layout (location = 5) in float edgeGapLength;
layout (location = 6) in int edgeDash;

uniform samplerBuffer vertices; //vertex positions only
uniform samplerBuffer modelMatrixBuffer;
uniform isamplerBuffer modelMatrixIndexBuffer; // same as (location = 5) modelMatrixIndex
uniform vec2 viewport;
uniform mat4 view;
uniform mat4 projection;


out float vDist;
out float uDash;
out float uGap;
flat out int uDashRequired;
flat out int MaterialIndex;

void main()
{
    // 1. extract the verticies
    vec3 position0 = texelFetch(vertices, int(edge.x)).xyz;
    vec3 position1 = texelFetch(vertices, int(edge.y)).xyz;

    // 2. extract the model matrix (both edge's vertices will have the same model matrix
    int modelMatrixIndex = texelFetch(modelMatrixIndexBuffer, int(edge.x)).x;
    // int modelMatrixIndex = texelFetch(modelMatrixIndexBuffer, 0).x;

    // 2.1 A mat4 takes 4 vec4 slots. We must calculate the starting offset.
    int baseIndex = modelMatrixIndex * 4;

    // 2.2 Fetch the 4 columns/rows of the matrix manually
    vec4 col0 = texelFetch(modelMatrixBuffer, baseIndex + 0);
    vec4 col1 = texelFetch(modelMatrixBuffer, baseIndex + 1);
    vec4 col2 = texelFetch(modelMatrixBuffer, baseIndex + 2);
    vec4 col3 = texelFetch(modelMatrixBuffer, baseIndex + 3);

    mat4 model = mat4(col0, col1, col2, col3);

    // 3. project vertices to clip space
    vec4 clipPosition0 = projection * view * model * vec4(position0, 1.0);
    vec4 clipPosition1 = projection * view * model * vec4(position1, 1.0);

    // 4. project to ndc
    vec2 ndcPosition0 = clipPosition0.xy / clipPosition0.w;
    vec2 ndcPosition1 = clipPosition1.xy / clipPosition1.w;

    // 5. project to screen space
    vec2 screenPosition0 = (ndcPosition0 * 0.5 + 0.5) * viewport;
    vec2 screenPosition1 = (ndcPosition1 * 0.5 + 0.5) * viewport;

    // 5.1 Qt Quick FBO Y-flip
    screenPosition0.y = viewport.y - screenPosition0.y;
    screenPosition1.y = viewport.y - screenPosition1.y;

    // 6. get edge direction and normal
    vec2 dir = normalize(screenPosition1 - screenPosition0);
    vec2 normal = vec2(-dir.y, dir.x);

    // 7. Pick start or end based on vertex ID
    bool isEnd = (gl_VertexID >= 2);
    vec2 base = isEnd ? screenPosition1 : screenPosition0;

    // 8. Distance along line
    vDist = isEnd ? length(screenPosition1 - screenPosition0) : 0.0;

    // 9. Expand quad
    vec2 offset = normal * corner * (edgeWidth * 0.5);
    vec2 pos = base + offset;

    // 10. Back to clip space
    vec2 ndc = (pos / viewport) * 2.0 - 1.0;
    float z  = isEnd ? clipPosition1.z / clipPosition1.w : clipPosition0.z / clipPosition0.w;

    // 11. Generate outputs
    gl_Position = vec4(ndc, z, 1.0);
    MaterialIndex = materialIndex;
    uDash = edgeDashLength;
    uGap = edgeGapLength;
    uDashRequired = edgeDash;
}
