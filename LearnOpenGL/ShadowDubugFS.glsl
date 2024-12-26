#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D depthMap;
void main()
{
// depth map texture 를 있는 그대로 화면에 출력한다
// single float 값이 texturte 에 저장된 것. 그래서 'r' 에만 값이 존재한다.
// texture() 함수는 4차원 벡터 (R, G, B, A)를 반환하는데, 여기서 .r을 사용하여 R 채널의 값만 추출
float depthValue = texture(depthMap, TexCoords).r;
FragColor = vec4(vec3(depthValue), 1.0);
}