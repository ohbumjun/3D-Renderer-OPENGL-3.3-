/*
* >> Material 기본 설명
- 현실 세계에서는, 각 물체가 같은 light 에 대해 다르게 반응한다.
steel object 는 다른 wood object 보다 더 많은 light 를 반사한다.

- 이러한 물체의 반응을 나타내는 것이 Material 이다.
물체의 surface, 즉 표면에 이러한 material property 들을 정의하면
빛에 대해서 다르게 반응시킬 수 있다.

>> 구현
Surface 를 묘사할 때, 3가지 light 요소에 대한 material color 을 정의할 수 있다
ex) ambient, diffuse, specular
이를 통해 해당 표면의 색상이 어떻게 결정될지를 control 할 수 있다.
*/