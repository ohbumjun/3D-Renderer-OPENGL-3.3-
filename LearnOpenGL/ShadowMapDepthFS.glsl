#version 330 core

void main()
{             
    // depth buffer map �� ����� fs �̴�.
    // �츮�� depth ���� ����ϸ� �ǰ�, color ���� �ʿ����.
    // �̸� ���� �츮�� ���ʿ� frame buffer object �� ������ ��
    // color buffer �� ����, color data �� draw �ϰų� read ������ �ʴ´�.
    // �׷��� ���⼭ color �� out �� ���� �ʴ� ���̴�.
    // ����, fs shader ���� ��º��ձ� ������ �� �� depth ���� update �ȴ�.
    // gl_FragDepth = gl_FragCoord.z;
}