#version 330 core
out vec4 FragColor;

// 아래 3개의 값은 모두 world space 기준 좌표값들이다.
in vec3 Normal;
in vec3 Position;
uniform vec3 cameraPos;

uniform samplerCube skybox;

void main()
{    
    // camera -> 물체의 pixel 위치 (즉, 카메라에서 물체를 바라보는 방향)
    vec3 I = normalize(Position - cameraPos);
	// 반사 vector 
	vec3 R = reflect(I, normalize(Normal));
	// 해당 반사 벡터를 texture coordinate 로 사용하여 skybox 에서 색상을 가져옴
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}