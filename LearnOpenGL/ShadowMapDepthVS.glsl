#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
// 지금 현재 vs shader 는 광원관점에서 depth map 을 그리는 shader
// 이때 사용하는 matrix 는 광원관점에서의 view * projection matrix 이다.
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}