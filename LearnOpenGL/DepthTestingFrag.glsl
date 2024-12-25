#version 330 core
out vec4 FragColor;

// �Ʒ� 3���� ���� ��� world space ���� ��ǥ�����̴�.
in vec3 Normal;
in vec3 Position;
uniform vec3 cameraPos;

uniform samplerCube skybox;

void main()
{    
	float ratio = 1.00 / 1.52;

    // camera -> ��ü�� pixel ��ġ (��, ī�޶󿡼� ��ü�� �ٶ󺸴� ����)
    vec3 I = normalize(Position - cameraPos);

	// �ݻ� vector 
	vec3 R = refract(I, normalize(Normal), ratio);

	// �ش� �ݻ� ���͸� texture coordinate �� ����Ͽ� skybox ���� ������ ������
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}