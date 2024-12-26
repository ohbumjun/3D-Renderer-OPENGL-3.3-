#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D depthMap;
void main()
{
// depth map texture �� �ִ� �״�� ȭ�鿡 ����Ѵ�
// single float ���� texturte �� ����� ��. �׷��� 'r' ���� ���� �����Ѵ�.
// texture() �Լ��� 4���� ���� (R, G, B, A)�� ��ȯ�ϴµ�, ���⼭ .r�� ����Ͽ� R ä���� ���� ����
float depthValue = texture(depthMap, TexCoords).r;
FragColor = vec4(vec3(depthValue), 1.0);
}