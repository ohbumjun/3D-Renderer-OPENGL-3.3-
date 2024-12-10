#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
 layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // world pos of vertex 
    // - for light calculations in fragment shader
    FragPos = vec3(model * vec4(aPos, 1.0));

    TexCoords = aTexCoords;

    // Normal 인 경우, non-uniform scale 일 경우, 그냥 L 을 곱하면 안된다. 
    // 1) L 의 역행렬을 취해주고
    // 2) 그 역행렬의 transpose 를 취해준다.
    // 그리고 Normal 의 경우, homogeneous coordinate 가 아니라 그냥 mat3 을 쓴다는 점이다.
    // 왜냐하면 어차피 Transpose 은, Normal Vector 의 모양이나 방향에 영향을 미치지 않기 때문이다.
    // 참고 : transpose 및 inverse 의 비용은 비싸다. cpu 측에서 normal 을 계산한 다음 uniform 을 통해
    // shader 로 넘겨주는 것이 더 효율적일 수 있다.
     Normal = mat3(transpose(inverse(model))) * aNormal;
}