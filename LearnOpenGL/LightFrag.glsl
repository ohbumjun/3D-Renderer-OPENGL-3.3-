#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;       // world 상의 camera position

struct Light {
    // Light 의 각 색상
vec3 position;
    // 일반적으로 ambient light 는 intensity 가 낮다.
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;

struct Material {
    // 각 light 요소의 색상을 결정한다.
vec3 ambient;     
vec3 diffuse;
vec3 specular;
float shininess;
};
uniform Material material;

void main()
{
// 참고 : 현재 learning purpose 를 위해서 빛 계산을 모두 world space 상에서 진행하고 있다.
    float specularStrength = 0.5;
    float shiness = 32; // 값이 높을 수록, specualr light 반사 정도가 더 정확해지고, 빛이 덜 scatter 된다.
    vec3 ambient = material.ambient * light.ambient;

    // pixel normal vector normalized
    vec3 norm = normalize(Normal);
    // lightPos - FragPos : frag -> lightPos 방향 벡터
    vec3 lightDir = normalize(lightPos - FragPos);

    // dot(norm, lightDir) : 두 벡터의 내적 == cos(theta)
    // - 두 벡터가 평행하면 1, 수직이면 0, 반대면 -1
    // - 두 벡터 사이의 각도가 0이면 가장 diffuse 색상이 밝고, 90도 이면 0s
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos); // pixel -> camera
    // reflect 함수의 경우, 1번째 요소가 light source -> pixel 방향이기를 기대한다. 그래서 '-' 을 붙여준다.
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 
        material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}