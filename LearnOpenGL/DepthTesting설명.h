/* p177
>> Depth Testing 
- 자 depth buffer 는 0 ~ 1 사이의 값을 지닌다.
그러면 여기 depth buffer 에 맺히는 값은
각 object 의 z 값이다.

그런데 z 값은 0 ~ 1 로 변환되기 전에
near ~ far plane 사이의 값이다.

그리고 이 값을 0 ~ 1로 맞춰주는 것이다.

1) linear
- z 값에 따라서 linear 하게 0 ~ 1 로 맺히는 값이 변하게 하는 방법도 사용
하지만 이것은 거의 사용되지 않는다

2) non linear 
- 즉, 0.5 라는 depth buffer 값은 해당 pixel 의 z-value 가
frustum 의 중간에 있다는 것을 의미하지 않는다는  것이다.
linear 하지 않으니까

사실 near plan 에 조금 더 가깝다
depth buffer 값이 0 ~ 0.5 로 갈 때까지는, 조금만 실제 z pos 값이 올라가도
depth buffer 값은 빠르게 증가한다.

반면 0.5 ~ 1 로 갈 때는, 실제 z pos 값이 크게 증가해도
depth buffer 값은 느리게 증가한다.
*/