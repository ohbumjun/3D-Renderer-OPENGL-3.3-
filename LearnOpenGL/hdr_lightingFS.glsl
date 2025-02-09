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
uniform vec3 viewPos; // 카메라 위치 

// 다수의 광원(최대 16개)을 고려하여 간단한 조명 효과를 적용하는 셰이더
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

        // 조명 색상(lights[i].Color)과 디퓨즈 강도(diff)를 곱한 후, 픽셀의 색상(color)에 적용
        vec3 diffuse = lights[i].Color * diff * color;      
        vec3 result = diffuse;        

        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - lights[i].Position);

        //  거리에 따른 감쇠 (Attenuation) 적용 (거리의 제곱에 반비례하는 감쇠 모델)
        result *= 1.0 / (distance * distance);

        lighting += result;
    }

    // 환경광(ambient) + 누적 조명(lighting) 값을 최종 색상으로 출력
    FragColor = vec4(ambient + lighting, 1.0);
}