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

    // Normal �� ���, non-uniform scale �� ���, �׳� L �� ���ϸ� �ȵȴ�. 
    // 1) L �� ������� �����ְ�
    // 2) �� ������� transpose �� �����ش�.
    // �׸��� Normal �� ���, homogeneous coordinate �� �ƴ϶� �׳� mat3 �� ���ٴ� ���̴�.
    // �ֳ��ϸ� ������ Transpose ��, Normal Vector �� ����̳� ���⿡ ������ ��ġ�� �ʱ� �����̴�.
    // ���� : transpose �� inverse �� ����� ��δ�. cpu ������ normal �� ����� ���� uniform �� ����
    // shader �� �Ѱ��ִ� ���� �� ȿ������ �� �ִ�.
     Normal = mat3(transpose(inverse(model))) * aNormal;
}