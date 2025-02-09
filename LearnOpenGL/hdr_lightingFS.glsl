#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Light lights[16];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos; // ī�޶� ��ġ 

// �ټ��� ����(�ִ� 16��)�� ����Ͽ� ������ ���� ȿ���� �����ϴ� ���̴�
void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);

    // ambient
    vec3 ambient = 0.0 * color;

    // lighting
    vec3 lighting = vec3(0.0);

    for(int i = 0; i < 16; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);

        // ���� ����(lights[i].Color)�� ��ǻ�� ����(diff)�� ���� ��, �ȼ��� ����(color)�� ����
        vec3 diffuse = lights[i].Color * diff * color;      
        vec3 result = diffuse;        

        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - lights[i].Position);

        //  �Ÿ��� ���� ���� (Attenuation) ���� (�Ÿ��� ������ �ݺ���ϴ� ���� ��)
        result *= 1.0 / (distance * distance);

        lighting += result;
    }

    // ȯ�汤(ambient) + ���� ����(lighting) ���� ���� �������� ���
    FragColor = vec4(ambient + lighting, 1.0);
}