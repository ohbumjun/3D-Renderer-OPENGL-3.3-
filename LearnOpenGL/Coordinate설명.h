/*
* 1) Local Space

* 2) World Space

* 3) View Space * Projection Matrix => Clip Space

* 4) Clip Space 
- 1 ~ 1 사이의 좌표로 변환된다.
- perspective projection 을 사용할 경우, 원근감을 줄 수 있다.

* 5) Screen Space
- 마지막 viewport transform 을 통해서 -1 ~ 1 사이 범위의 좌표를
  screen 좌표로 변환한다.
- 이때 이용하는 함수가 바로 glViewport 함수이다.
* 
*/

/*
* >> Local Space
- 우리가 가진 고유 모델들은, (0,0,0) 을 중심으로 하는 고유 local space 에서 시작할 것이다.
*/

/*
* >> Clip Space
- vertex shader 가 실행되고 난 이후, OpenGL 은 각 정점이 clip space 에 있다라고 가정한다.
그리고 특정 범위 안에 존재하지 않는 정점들은 clip 되어 버린다.
- clip 되지 않은, 나머지 부분들만 픽셀로 나타나게 된다.
- projection matrix 를 사용해서 clip space 로 변환된다. 변환 이후, 좌표는
 NDC 공간. 즉, -1 ~ 1 사이의 좌표로 변환된다.
 만약 변환 결과 -1 ~ 1 밖에 있다면, 해당 좌표는 clip 되어 버린다.

 >> Frustum
 - Projection Matrix 가 생성하는 viewing box 를 frustum 이라고 한다.
 그리고 해당 frustum 안에 존재하는 좌표만이 user 에게 보여진다.

 - 3D 공간을 2D 화면에 투영하는 것을 projection 이라고 한다.

 >> Perspective Division
 - 모든 정점들이 clip space 에 존재하고 나면, 마지막으로 perspective division 이 이루어진다.
 vector 의 w 값으로 x,y,z 값을 나누는 것이다.

 즉, Perspective Division 란 4D Clip Space 좌표계를 3D NDC 좌표계로
 만드는 과정이다.

 이 과정은 정점 셰이더 마지막 부분에서 "자동으로" 해준다.
*/

/*
* >> Projection Matrix 종류
- view 좌표계를 clip 좌표계로 변환하는 2가지 형태가 존재한다.
그리고 각각은 고유의 frutstum 을 만들어낸다.

1) Orthographic Projection : 직교 투영

- 정육면체, 직육면체 등 "cube" 형태의 frustum 을 만들어낸다.
즉, 카메라의 시선이 평행하게 뻗어나간다.
따라서 물체의 크기가 거리에 따라 변하지 않는다.

- frustum 의 width, height, length (depth) 를 지정해주면 된다.

- 해당 frustum 안에 있는 좌표계들을, "directly" NDC 좌표계로 매핑한다.
w 좌표도 건드리지 않는다.
그래서 'w' 을 이용한 "원근 효과" 는 일어나지 않는다.

glm::ortho(
	0.0f, 800.0f, // left , right coord of frustum
	0.0f, 600.0f, // bottom, top coord of frustum
	0.1f, 100.0f // near, far coord of frustum
);

2) Perspective Projection : 원근 투영

- 원근감 : Perspective Projection Matrix 를 통해서 원근감을 부여하게 된다.

- Projection Matrix 는 frustum 범위를 clip space 로 변환한다. 뿐만 아니라
각 정점 좌표의 w 을 조작하여 원근감을 부여한다.
멀리 있는 꼭짓점의 w 컴포넌트를 크게 만들어 원근감을 조절한다.

- 좌표들이 clip space 로 변환되고 나면, -w ~ w 사이의 좌표로 변환된다.
해당 범위 밖에 있는 좌표들은 clip 되어 버린다.

- 이제 OpenGL 은 visible 좌표들이
vertex shader 수행 결과 -1 ~ 1 사이에 놓이기를 요구한다.

그러므로 좌표들이 clip space 에 위치하고 나면
perspective division 이 clip space 좌표들에 적용된다.
즉, x,y,z 좌표들은 w 로 나눠진다.
이후 NDC 좌표계로 변환된다.

ex) 
glm::mat4 proj = glm::perspective(
	glm::radians(45.0f),			// fov : field of view -> 보통 45도
	(float)width /(float)height,  // aspect ratio 
	0.1f,									// near
	100.0f								// far
);

glm::perspective : visible space 를 정의하는 거대한 frustum 을 만들어낸다.
해당 frustrum 밖에 있는 대상들은 clip space 로 보내지지 않고 clip 된다.
*/

/*
* >> 최종 정리

local -> clip 으로 변환되는 과정

Vclip = Mprojection ·Mview · Mmodel ·Vlocal

최종 vertex 는 vertex shader 에서 gl_Position 에 저장되어야 한다.
그러면 OpenGL 이 알아서 perspective division 과 clipping 을 수행하게 된다.

자. 다시 복기하자면, vertex shader 의 결과물은 clip space 좌표계에 놓여야 한다.

그 다음 이 clip space 의 좌표계들은 perspective division 을 거쳐 NDC 좌표계로 변환된다.

이후 , OpenGL 은 NDC 좌표계의 좌표들을 glViewPort 을 통해 screen 좌표계로 변환한다.
(viewport transform)

*/