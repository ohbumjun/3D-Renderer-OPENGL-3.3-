#version 330 core

void main()
{             
    // depth buffer map 을 만드는 fs 이다.
    // 우리는 depth 값만 기록하면 되고, color 값은 필요없다.
    // 이를 위해 우리는 애초에 frame buffer object 를 생성할 때
    // color buffer 도 없고, color data 를 draw 하거나 read 하지도 않는다.
    // 그래서 여기서 color 에 out 을 하지 않는 것이다.
    // 한편, fs shader 이후 출력병합기 측으로 갈 때 depth 값을 update 된다.
    // gl_FragDepth = gl_FragCoord.z;
}