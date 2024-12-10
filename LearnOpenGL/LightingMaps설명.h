/*
>> Diffuse Map
- 각 object 별로  3D 표면의 각 'point' 에 대한 색상을 정의하는 texture map 이다.
이 color 정보는, object 가 light 에 hit 할 때 어떤 색상으로 보일지를 결정한다.
일종의 object 를 감싼 3D digital paint 같은 것이다.

>> How ?
1) 2D Texture 생성
- 해당 이미지는 3d object 의 surface 에 매핑된다.
- 이미지 상의 각 픽셀은, object 평면에 맺힌 각 색상값을 나타내게 된다.

2) Fragment Shader 에서 사용
- Fragment Shader 는, 각 fragment 에 대한 최종 색상을 결정해야 한다.
- 각 색상을 결정하기 위해서, diffuse map 에서 해당 pixel 에 대응되는
색상값을 가져온다. 이때 pixel 에 대응되는 object 표면상의 위치를 사용하는 것 같다
- 이후, 그 색상값은 해당 픽셀에 대한 최종 색상값을 결정할 때 사용된다.
- 
*/