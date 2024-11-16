/*
Texture Wrapping

texture coord 은 0 ~ 1 사이이다
그런데 만약 이보다 큰 감ㅅ을 specify 한 경우 ?
이 경우를 어떻게 처리할지에 대한 개념이다.

• GL_REPEAT: 
	The default behavior for textures. Repeats the texture image.
• GL_MIRRORED_REPEAT: 
	Same as GL_REPEAT but mirrors the image with each repeat.
• GL_CLAMP_TO_EDGE:
	Clamps the coordinates between 0 and 1. The result is that higher
	coordinates become clamped to the edge, resulting in a stretched edge pattern.
• GL_CLAMP_TO_BORDER: 
	Coordinates outside the range are now given a user-specified
	border color.

// s,t : 좌우, 위 아래를 의미
// 만약 3d texture 라면 x,y,z 을 사용한다.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


// GL_CLAMP_TO_BORDER 을 사용할 겨우 border color 를 지정해야 한다.
float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
*/


/*
Texture Filtering

pixel 을, texture 상 어떤 coordinate 에 mapping 할지를 결정해야 한다.
이때 사용하는 방법이 Texture Filtering 이다.

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/

/*
* Mipmaps

- 멀리에 있는 object 는 자세한 texture 를 입힐 필요가 없다.
- 그래서, 멀리에 있는 object 에는 low resolution 의 texture 를 입히는 것이 좋다.
- 이때 사용하는 것이 Mipmaps 이다.

Mipmap 은, texture 이미지들을, 여러 단계로 만든다
하위 단계로 갈 수록 resolution 이 낮아진다.

opengl 에서 mimp level 사이에 변경이 일어날 때
2개의 mip map level 사이에 shared edge 갑 ㅗ이게 되는 경우가 있다.
이또한 filtering 을 통해 해결할 수 있다.

one of the following four options:
• GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and
uses nearest neighbor interpolation for texture sampling.
• GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level
using linear interpolation.
• GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that
most closely match the size of a pixel and samples the interpolated level via nearest neighbor
interpolation.
• GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps
and samples the interpolated level via linear interpolation.

ex) 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/