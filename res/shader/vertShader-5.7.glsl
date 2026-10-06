#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D samp;

out vec2 tc;    // 输出到光栅着色器进行插值

void main(void)
{
    gl_Position = proj_matrix * mv_matrix * vec4(pos, 1.0);
    tc = texCoord;
}