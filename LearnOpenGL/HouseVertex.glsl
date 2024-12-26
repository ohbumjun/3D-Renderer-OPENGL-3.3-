#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;


//   float housePoints[] = {
//     -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // top-left
//     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // top-right
//     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
//     -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
// };

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
// 정점 정보로 아예 정점별 color 도 같이 넘겨주는 것을 알 수 있다.
    vs_out.color = aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}