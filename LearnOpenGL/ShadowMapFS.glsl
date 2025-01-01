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
    // fragPosLightSpace : ���� ������ ���� clip space ��ǥ (���� ���� ��ǥ)

    // projCoords : perform perspective divide (result : -1 ~ 1) �����
    //                    ���� light ���� frag �� clip space
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.f;

    // frustum 0 ~ 1 �̶�� near ~ far plane ���̿� ���� �ʰ�, far ���� �ִٸ�
    // shadow ������ ���� �ʴ´ٰ� �Ǵ��Ѵ�. ���ʿ� light �� ������ ��ġ�� ������ ���ٴ� ���̱� �����̴�.
    if(projCoords.z > 1.0)
    {
        return shadow;
    }

    // world space ������ light Dir
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    
    // get closest depth value from light's perspective 
    // (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow
    // shadow bias ���� (�������κ��� frag pos ������ ���� ~ normal ���� ���� ���� ���)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF ��� : �ֺ� texel �� sampling ���
    // shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // textureSize(shadowMap, 0) : shadowMap �� width, height
    // texelSize : 1 texel �� ũ�� ex)  �׸��� ���� �ػ󵵰� 1024x1024���, �ؼ� ũ��� 1/1024 = 0.000976
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // projCoords.xy             : ���� �ؼ��� �ؽ�ó ��ǥ
            // vec2(x, y) * texelSize  : �ؼ� ũ��(texelSize)�� x, y �������� ���Ͽ� �ֺ� �ؼ��� ��ǥ �������� ���
            // exture(shadowMap, ...) : �̵��� �ؼ� ��ǥ���� �׸��� ���� ���� ���� ���ø�
            // .r : �׸��� ���� ���� ���� ������
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
    // shadow ���� : fragment �� shadow �� ��ŭ '��' �����ϴ°� (not)
    // ex) 1 - shadow : fragment �� shadow �� ��ŭ �����ϴ°�
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}