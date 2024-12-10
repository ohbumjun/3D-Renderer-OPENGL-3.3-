#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;       // world 상의 camera position

struct Light {
// directional light 의 경우, position 이 없다. Point light 의 경우
// Light 와 Fragment 사이의 거리에 따라 attenuation 을 계산하기 위해 light pos 필요
// SpotLight 의 경우, position 과 direction 이 모두 필요하다.
    vec3 position; 

// light -> pixel 방향 벡터, directional light 는 모든 pixel 동일 방향                   
vec3 direction; 

// SpotLight 을 위한 cutOff
float cutOff;

// Light 의 각 색상
// 일반적으로 ambient light 는 intensity 가 낮다.
vec3 ambient;
vec3 diffuse;
vec3 specular;

// Point light 에서 attenuation 을 위한 변수들
 float constant;
 float linear;
 float quadratic;
};
uniform Light light;

struct Material {
     sampler2D diffuse; // diffuse map
     sampler2D specular; // specular map
     float shininess;
};
uniform Material material;

in vec2 TexCoords;

void main()
{
// 참고 : 현재 learning purpose 를 위해서 빛 계산을 모두 world space 상에서 진행하고 있다.
    float specularStrength = 0.5;
    float shiness = 32; // 값이 높을 수록, specualr light 반사 정도가 더 정확해지고, 빛이 덜 scatter 된다.
    vec3 ambient =  vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // pixel normal vector normalized
    vec3 norm = normalize(Normal);
    // lightPos - FragPos : frag -> lightPos 방향 벡터
    vec3 fragToLightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(-light.direction);
    // 해당 fragment 가 spot light 범위 안에 존재하는지 확인한다.
    float theta = dot(fragToLightDir, normalize(-light.direction));
    
    // remember that we're working with angles 
    // as cosines instead of degrees so a '>' is used.
    // 아래 if 문은 spot light 범위 안에 있다는 의미이다.
    if(theta > light.cutOff) 
    {    
        // dot(norm, lightDir) : 두 벡터의 내적 == cos(theta)
        // - 두 벡터가 평행하면 1, 수직이면 0, 반대면 -1
        // - 두 벡터 사이의 각도가 0이면 가장 diffuse 색상이 밝고, 90도 이면 0s
        float diff = max(dot(norm, fragToLightDir), 0.0);
        vec3 diffuse = (diff *  vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos); // pixel -> camera
        // reflect 함수의 경우, 1번째 요소가 light source -> pixel 방향이기를 기대한다. 그래서 '-' 을 붙여준다.
        vec3 reflectDir = reflect(-fragToLightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 
            material.shininess);
        vec3 specular = (spec * vec3(texture(material.diffuse, TexCoords))) * light.specular;

        float distance = length(light.position- FragPos);
        
        // 거리에 따라 빛의 영향력이 감소하는 것을 나타내기 위한 attenuation
        float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }
}