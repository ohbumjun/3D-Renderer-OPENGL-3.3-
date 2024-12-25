#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

// gl_FrontFacing : 현재 pixel 부분이 front facing 부분인지 아닌지 리턴
// 만약 face culling 을 enable 했다면, back facing 부분에 대한 pixel shader 가
// 해당 pixel 에 대해서는 실행도 안된다. 즉, 아래가 실행도 안된다는 것이다.
void main()
{
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);
}