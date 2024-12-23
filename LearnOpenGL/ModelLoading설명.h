/*
* >> Assimp 
- Assimp는 Open Asset Import Library의 약자로, 3D 모델을 읽어들이는 라이브러리이다.

Assimp
-> entire model 을 'scene object' 라는 형태로 load 한다.
scene object 는 import된 모든 데이터를 포함하고 있다.

->> node 의 collection 을 가지고 있다.
node 란 data 에 대한 index ? 를 가지고 있다.
node 들은 child node 를 가지고 있을 수 있다.

-> 과정
- Scene object 를 load 한다.
- 각 node 에 존재하는 mesh object 를 순회한다
- 각 mesh object 를 순회하며 vertex data, indice, material data 를 가져온다.
- 결과, mesh data 의 collection 을 얻게 된다.
- 그 값들을 single 'Model' object 에 담게 될 것이다.
*/