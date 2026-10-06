#version 410

uniform sampler2D samp;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
in vec2 tc;     // 输入插值过的纹理坐标

out vec4 color;

void main(void)
{
    color = texture(samp, tc);
}