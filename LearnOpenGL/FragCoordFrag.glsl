#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

// gl_FrontFacing : ���� pixel �κ��� front facing �κ����� �ƴ��� ����
// ���� face culling �� enable �ߴٸ�, back facing �κп� ���� pixel shader ��
// �ش� pixel �� ���ؼ��� ���൵ �ȵȴ�. ��, �Ʒ��� ���൵ �ȵȴٴ� ���̴�.
void main()
{
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);
}