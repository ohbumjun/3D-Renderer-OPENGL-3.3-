#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    
    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);

    float attenuation = 1.0 / (gamma ? distance * distance : distance);
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 lighting = vec3(0.0);
    
    for(int i = 0; i < 4; ++i)
        lighting += BlinnPhong(normalize(fs_in.Normal), fs_in.FragPos, lightPositions[i], lightColors[i]);
    
    color *= lighting;
    
    // Gamma Correction 을 적용하면, SRGB 형태의 Texture 를 사용하는 중이다
    // 그 말은 즉슨 실제 Texture 에서 샘플링한 값은 linear 공간으로 변환된 상태이고
    // 우리가 여기서 계산한 연산도 linear 공간에서 이루어진 것이다.
    // 그러면 이제 모니터가 출력을 할 때는 CRT gamma 공간으로 변환을 한다
    // 즉, 우리가 여기서 출력하려고 계산한 값보다 어둡게 출력해버린다.
    // 그러면 우리는 현재 linear 공간에서 계산한 값이 있는 그대로 모니터에
    // 출력되도록 하기 위해서는 감마 보정을 통해 더 밝게 최종 출력값을 변경해야 한다
    // 그러면 모니터가 우리가 최종적으로 출력한 값을, 어둡게 만들 것이고
    // 결과적으로 linear 공간에서의 값이 모니터에 출력되는 것이다.
    if(gamma)
        color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}