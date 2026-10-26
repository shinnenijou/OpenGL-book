#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingLight2Dir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;
out vec3 varyingHalf2Vector;

struct PositionLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform vec4 globalAmbient;
uniform PositionLight light;
uniform PositionLight light2;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
    varyingVertPos = (mv_matrix * vec4(pos, 1.0)).xyz;
    varyingNormal = (norm_matrix * vec4(norm, 1.0)).xyz;
    varyingLightDir = light.position - varyingVertPos;
    varyingHalfVector = varyingLightDir + (- varyingVertPos);
    varyingLight2Dir = light2.position - varyingVertPos;
    varyingHalf2Vector = varyingLight2Dir + (- varyingVertPos);

    // 将位置发送到片段着色器
    gl_Position = proj_matrix * mv_matrix * vec4(pos, 1.0);
}