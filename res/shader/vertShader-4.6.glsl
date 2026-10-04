#version 410

layout (location = 0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;   // 用于设置不同立方体位置和动画的时间因子

out vec4 varyingColor;

// 构建平移矩阵
mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
          x,   y,   z, 1.0
    );
    return trans;
}

// 构建绕x轴旋转的旋转矩阵
mat4 buildRotateX(float rad)
{
    mat4 trans = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(rad), -sin(rad), 0.0,
        0.0, sin(rad), cos(rad), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    return trans;
}

// 构建绕y轴旋转的旋转矩阵
mat4 buildRotateY(float rad)
{
    mat4 trans = mat4(
        cos(rad), 0.0, -sin(rad), 0.0,
        0.0, 1.0, 0.0, 0.0,
        sin(rad), 0.0, cos(rad), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    return trans;
}

// 构建绕z轴旋转的旋转矩阵
mat4 buildRotateZ(float rad)
{
    mat4 trans = mat4(
        cos(rad), -sin(rad), 0.0, 0.0,
        sin(rad), cos(rad), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    return trans;
}

// 构建缩放矩阵
mat4 buildScale(float x, float y, float z)
{
    mat4 trans = mat4(
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    return trans;
}

void main(void){
    float distance = 10.0;
    float i = gl_InstanceID + tf;               // 基于时间因子让不同实例有不同位置
    float a = sin(0.35 * i) * distance;         // 用来平移的x, y, z分量
    float b = cos(0.52 * i) * distance;
    float c = sin(0.7 * i) * distance;

    // 构建旋转和平移矩阵
    mat4 localScale = buildScale(0.2, 0.2, 0.2);
    mat4 localRotX = buildRotateX(1.75 * i);
    mat4 localRotY = buildRotateY(1.75 * i);
    mat4 localRotZ = buildRotateZ(1.75 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    // 构建MV矩阵
    mat4 m_matrix = localScale * localTrans * localRotX * localRotY * localRotZ;
    mat4 mv_matrix = v_matrix * m_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}