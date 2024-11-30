#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // pixel normal vector normalized
    vec3 norm = normalize(Normal);
    // lightPos - FragPos : light -> frag 방향 벡터
    vec3 lightDir = normalize(lightPos - FragPos);

    // dot(norm, lightDir) : 두 벡터의 내적 == cos(theta)
    // - 두 벡터가 평행하면 1, 수직이면 0, 반대면 -1
    // - 두 벡터 사이의 각도가 0이면 가장 diffuse 색상이 밝고, 90도 이면 0s
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}