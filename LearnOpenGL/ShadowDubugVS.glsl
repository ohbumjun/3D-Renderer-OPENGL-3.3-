#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

// 여기는 depth map 을 그대로 화면 크기로 출력하는 shader 이다.
void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}