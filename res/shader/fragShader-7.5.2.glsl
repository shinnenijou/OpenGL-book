#version 410

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingLight2Dir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
in vec3 varyingHalf2Vector;
out vec4 fragColor;

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
    // 将顶点位置转换到视觉空间
    vec3 L = normalize(varyingLightDir);
    vec3 L2 = normalize(varyingLight2Dir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);
    vec3 H2 = normalize(varyingHalf2Vector);

    // Phong shading
    // 计算视觉向量和反射光向量的夹角
    // R是-L相对法向量的镜像
    // vec3 R = reflect(-L, N);
    // float cosPhi = dot(V, R);

    // Blinn-Phong shading
    // 用（光源方向和视觉向量的角平分线与法向量夹角）代替（视觉向量和反射光向量的夹角）
    float cosPhi = dot(H, N);
    float cosPhi2 = dot(H2, N);

    // 计算光照与法向量的夹角
    float cosTheta = dot(L, N);
    float cosTheta2 = dot(L2, N);

    // 光源1的各个光分量
    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0);
    vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(cosPhi, 0), material.shininess);

    // 光源2的各个光分量
    vec3 ambient2 = ((globalAmbient * material.ambient) + (light2.ambient * material.ambient)).xyz;
    vec3 diffuse2 = light2.diffuse.xyz * material.diffuse.xyz * max(cosTheta2, 0);
    vec3 specular2 = material.specular.xyz * light2.specular.xyz * pow(max(cosPhi2, 0), material.shininess);

    fragColor = vec4((ambient + diffuse) * 0.5 + (ambient2 + diffuse2) * 0.5 + specular + specular2, 1.0);
}