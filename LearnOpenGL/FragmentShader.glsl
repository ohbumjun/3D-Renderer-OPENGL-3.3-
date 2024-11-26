#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 TexCoord;
uniform float textureMixValue;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
	// mix : linear interpolation between two values based on 3rd value
	// ex) 0.2 : 80% of texture1, 20% of texture2
	// FragColor = mix(texture(texture1, TexCoord),texture(texture2, TexCoord), textureMixValue);
	vec2 reversedTexCoords = vec2(1.0 - TexCoord.x, TexCoord.y);
	FragColor = texture(texture1, reversedTexCoords);
};