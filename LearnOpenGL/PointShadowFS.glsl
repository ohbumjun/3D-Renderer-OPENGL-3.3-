#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;


// directional shadow map 과 달리 sampler 2d 가 아니라
// cube map depth map 을 사용한다.
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;


// light space 에서의 절두체 far plane 값도 받는다.
uniform float far_plane;
uniform bool shadows;


// directional shadow map 과 달리 light space 에서의 정보가 아니라
// world space 에서의 frag 정보를 받는다.
float ShadowCalculation(vec3 fragPos)
{

	// world space 상, light 으로부터의 거리 정보를 tex coord 로 활용하여
	// depth map 에서 가져올 것이다. 정확히는 해당 fragToLight 은 '방향 벡터' 로
	// 서 활용되어, cube map 3d 에서 해당 방향벡터가 맺히는 texture 의 위치값을
	// sampling 해오는 것이다.
	// direction vector 는 texture 로 sampling 하기 위해 굳이 unit vector 가
	// 될 필요가 없으므로, normalize 하지 않는다
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    
	// depthMap 에는 현재 light 광원으로부터 가장 가까운 object 의 거리 정보가
	// 0 ~ 1 사이의 normalize 된 값으로 저장되어 있다.
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    
    
	// 애초에 0 ~ 1 사이의 값으로 normalize 할 때, far plane 으로 나누었기 때문에
	// 다시 far_plane 을 곱해서, 실제 거리 값으로 변환한다.
	// 즉, (0,1) 사이의 범위를 (0, far_plane) 범위로 변환하는 것이다.
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    
	// 현재 render 중인 대상과 light 사이의 거리를 계산한다.
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        

    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);

    // ambient
    vec3 ambient = 0.3 * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}