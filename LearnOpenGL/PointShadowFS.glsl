#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;


// directional shadow map �� �޸� sampler 2d �� �ƴ϶�
// cube map depth map �� ����Ѵ�.
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;


// light space ������ ����ü far plane ���� �޴´�.
uniform float far_plane;
uniform bool shadows;


// directional shadow map �� �޸� light space ������ ������ �ƴ϶�
// world space ������ frag ������ �޴´�.
float ShadowCalculation(vec3 fragPos)
{

	// world space ��, light ���κ����� �Ÿ� ������ tex coord �� Ȱ���Ͽ�
	// depth map ���� ������ ���̴�. ��Ȯ���� �ش� fragToLight �� '���� ����' ��
	// �� Ȱ��Ǿ�, cube map 3d ���� �ش� ���⺤�Ͱ� ������ texture �� ��ġ����
	// sampling �ؿ��� ���̴�.
	// direction vector �� texture �� sampling �ϱ� ���� ���� unit vector ��
	// �� �ʿ䰡 �����Ƿ�, normalize ���� �ʴ´�
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    
	// depthMap ���� ���� light �������κ��� ���� ����� object �� �Ÿ� ������
	// 0 ~ 1 ������ normalize �� ������ ����Ǿ� �ִ�.
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    
    
	// ���ʿ� 0 ~ 1 ������ ������ normalize �� ��, far plane ���� �������� ������
	// �ٽ� far_plane �� ���ؼ�, ���� �Ÿ� ������ ��ȯ�Ѵ�.
	// ��, (0,1) ������ ������ (0, far_plane) ������ ��ȯ�ϴ� ���̴�.
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    
	// ���� render ���� ���� light ������ �Ÿ��� ����Ѵ�.
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