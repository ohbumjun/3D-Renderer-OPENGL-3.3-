#version 330 core
out vec4 FragColor;

void main()
{
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    // gl_FragCoord : x,y 는 screen 좌표 (왼쪽 아래에서 시작)
    // ex) window size 800x600 일때, (400, 300) 이면 중앙

    if(gl_FragCoord.x < 400)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}