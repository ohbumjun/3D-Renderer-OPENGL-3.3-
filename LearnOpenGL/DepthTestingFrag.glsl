#version 330 core
out vec4 FragColor;

// �Ʒ� 3���� ���� ��� world space ���� ��ǥ�����̴�.
in vec3 Normal;
in vec3 Position;
uniform vec3 cameraPos;

uniform samplerCube skybox;

void main()
{    
    // camera -> ��ü�� pixel ��ġ (��, ī�޶󿡼� ��ü�� �ٶ󺸴� ����)
    vec3 I = normalize(Position - cameraPos);
	// �ݻ� vector 
	vec3 R = reflect(I, normalize(Normal));
	// �ش� �ݻ� ���͸� texture coordinate �� ����Ͽ� skybox ���� ������ ������
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}