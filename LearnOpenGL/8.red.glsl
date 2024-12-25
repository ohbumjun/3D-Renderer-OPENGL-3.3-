#version 330 core
out vec4 FragColor;

void main()
{
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    // gl_FragCoord : x,y �� screen ��ǥ (���� �Ʒ����� ����)
    // ex) window size 800x600 �϶�, (400, 300) �̸� �߾�

    if(gl_FragCoord.x < 400)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}