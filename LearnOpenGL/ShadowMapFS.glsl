#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // fragPosLightSpace : 원근 나누기 이전 clip space 좌표 (광원 관점 좌표)

    // projCoords : perform perspective divide (result : -1 ~ 1) 적용된
    //                    최종 light 기준 frag 의 clip space
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.f;

    // frustum 0 ~ 1 이라는 near ~ far plane 사이에 있지 않고, far 보다 멀다면
    // shadow 영향을 받지 않는다고 판단한다. 애초에 light 이 영향을 미치는 범위에 없다는 것이기 때문이다.
    if(projCoords.z > 1.0)
    {
        return shadow;
    }

    // world space 에서의 light Dir
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    
    // get closest depth value from light's perspective 
    // (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow
    // shadow bias 적용 (광원으로부터 frag pos 까지의 벡터 ~ normal 벡터 사이 각도 고려)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF 고려 : 주변 texel 들 sampling 평균
    // shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // textureSize(shadowMap, 0) : shadowMap 의 width, height
    // texelSize : 1 texel 의 크기 ex)  그림자 맵의 해상도가 1024x1024라면, 텍셀 크기는 1/1024 = 0.000976
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // projCoords.xy             : 현재 텍셀의 텍스처 좌표
            // vec2(x, y) * texelSize  : 텍셀 크기(texelSize)와 x, y 오프셋을 곱하여 주변 텍셀의 좌표 오프셋을 계산
            // exture(shadowMap, ...) : 이동한 텍셀 좌표에서 그림자 맵의 깊이 값을 샘플링
            // .r : 그림자 맵의 깊이 값을 가져옴
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal); // world space normal
    vec3 lightColor = vec3(0.3);

    // ambient
    vec3 ambient = 0.3 * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    // shadow 변수 : fragment 가 shadow 가 얼만큼 '안' 존재하는가 (not)
    // ex) 1 - shadow : fragment 가 shadow 가 얼만큼 존재하는가
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}