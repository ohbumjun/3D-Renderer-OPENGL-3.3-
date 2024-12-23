#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// uniform vec3 objectColor;
// uniform vec3 lightColor;
uniform vec3 viewPos;       // world 상의 camera position

struct DirLight {
    // light -> pixel 방향 벡터, directional light 는 모든 pixel 동일 방향                   
    vec3 direction; 
    // Light 의 각 색상
    // 일반적으로 ambient light 는 intensity 가 낮다.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    // directional light 의 경우, position 이 없다. Point light 의 경우
    // Light 와 Fragment 사이의 거리에 따라 attenuation 을 계산하기 위해 light pos 필요
    // SpotLight 의 경우, position 과 direction 이 모두 필요하다.
    vec3 position; 

    // light -> pixel 방향 벡터               
    vec3 direction; 

    // SpotLight 을 위한 cutOff
    float cutOff;

    // 점점 smooth edge 가 되도록 하는 값
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    // Light 의 각 색상
    // 일반적으로 ambient light 는 intensity 가 낮다.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    // directional light 의 경우, position 이 없다. Point light 의 경우
    // Light 와 Fragment 사이의 거리에 따라 attenuation 을 계산하기 위해 light pos 필요
    // SpotLight 의 경우, position 과 direction 이 모두 필요하다.
    vec3 position; 
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

#define NR_POINT_LIGHTS 4

uniform PointLight[4] pointLights;
uniform SpotLight spotLight;
uniform DirLight dirLight;

struct Material {
     sampler2D diffuse; // diffuse map
     sampler2D specular; // specular map
     float shininess;
};
uniform Material material;

in vec2 TexCoords;

vec3 CalSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    // lightPos - FragPos : frag -> lightPos 방향 벡터
    vec3 fragToLightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, fragToLightDir), 0.0);

    vec3 reflectDir = reflect(-fragToLightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);

     // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // 해당 fragment 가 spot light 범위 안에 존재하는지 확인한다.
    // spotlight intensity
    float theta = dot(fragToLightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff; // smooth edge
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
   // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

// viewDir : 사용자 시점 -> frag pos
vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
    // lightPos - FragPos : frag -> lightPos 방향 벡터
    vec3 fragToLightDir = -light.direction;

    float diff = max(dot(norm, fragToLightDir), 0.0);

    vec3 diffuse = (diff *  vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    vec3 ambient =  vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // reflect 함수의 경우, 1번째 요소가 light source -> pixel 방향이기를 기대한다. 그래서 '-' 을 붙여준다.
    
    vec3 reflectDir = reflect(-light.direction, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);

    vec3 specular = (spec * vec3(texture(material.diffuse, TexCoords))) * light.specular;

    // 여러개의 light 을 사용하는 경우, object color 는 여기서 계산 x
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = (ambient + diffuse + specular);

    return result;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 fragPos)
{
    // lightPos - FragPos : frag -> lightPos 방향 벡터
    vec3 fragToLightDir = normalize(light.position - fragPos);

    float diff = max(dot(norm, fragToLightDir), 0.0);

    vec3 diffuse = (diff *  vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    vec3 ambient =  vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // reflect 함수의 경우, 1번째 요소가 light source -> pixel 방향이기를 기대한다. 그래서 '-' 을 붙여준다.
    
    vec3 reflectDir = reflect(-fragToLightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);

    vec3 specular = (spec * vec3(texture(material.diffuse, TexCoords))) * light.specular;

    float distance = length(light.position- fragPos);
    
    // 거리에 따라 빛의 영향력이 감소하는 것을 나타내기 위한 attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // 여러개의 light 을 사용하는 경우, object color 는 여기서 계산 x
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = (ambient + diffuse + specular);
    return result;
}

void main()
{
    // 참고 : 현재 learning purpose 를 위해서 빛 계산을 모두 world space 상에서 진행하고 있다.
    float specularStrength = 0.5;
    float shiness = 32; // 값이 높을 수록, specualr light 반사 정도가 더 정확해지고, 빛이 덜 scatter 된다.
    
    // pixel normal vector normalized
    vec3 norm = normalize(Normal);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos); // pixel -> camera

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        // result += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
        result += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
    }

    // phase 3: Spot light
    result += CalSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}