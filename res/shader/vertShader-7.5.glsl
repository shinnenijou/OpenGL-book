#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
out vec4 varyingColor;

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
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
    vec4 color;

    // 将顶点位置转换到视觉空间
    vec4 P = mv_matrix * vec4(pos, 1.0);
    vec3 N = normalize((norm_matrix * vec4(norm, 1.0)).xyz);
    vec3 L = normalize(light.position - P.xyz);

    // 视觉向量等于视觉空间中负顶点的位置
    vec3 V = normalize(-P.xyz);

    // R是-L相对法向量的镜像
    vec3 R = reflect(-L, N);

    // 各个光分量
    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N, L), 0);
    vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(dot(R, V), 0.0f), material.shininess);

    // 将颜色输出发送到片段着色器
    varyingColor = vec4((ambient + diffuse + specular), 1.0);

    // 将位置发送到片段着色器
    gl_Position = proj_matrix * mv_matrix * vec4(pos, 1.0);
}