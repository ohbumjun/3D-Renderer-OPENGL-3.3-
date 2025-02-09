#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

// diffuseMap, normalMap, depthMap을 입력으로 받습니다
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

// Parallax 효과의 강도를 조절하는 데 사용
uniform float heightScale;

// viewDir : tangent space 에서 view -> frag 방향 벡터
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
	// 깊이 map 의 값을 가져온다
	// 값 low  : 깊이가 깊다 == 높이가 높다
	// 값 high : 깊이가 낮다 == 높이가 낮다
    float height =  texture(depthMap, texCoords).r;     
    
	// 1) viewDir.xy 를 viewDir.z 로 나눈다 : 원근에 따른 보정
	// - 카메라 시점에서 보았을 때 텍스처 좌표가 이동해야 하는지의 방향
	// 2) 이후 H(A) 값을 곱한다
	// - 해당 방향으로 얼마나 이동해야 하는가
	// 3) 마지막으로 설정한 height_scale 값도 곱해준다 : 이것을 해주지 않으면
	//    일반적으로 조정효과가 너무 크다
	// p : 변위 벡터
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);     

    // 조정된 tex coord 값을 리턴한다
	// 해당 값을 이용하여 normal map, diffuse map 에서 값을 추출할 것이다
	return texCoords - p;
}

void main()
{           
	// tangent space view dir 벡터를 구한다
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    
	// Parallax Mapping 을 통해 조정된 tex coord
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}