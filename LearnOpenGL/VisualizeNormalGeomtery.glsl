#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 projection;

void GenerateLine(int index)
{
// normal vector �� visualisze �ϱ� �����̴�.
// �� �������� normal vector �� ������ ���̴�.
// �׷��� 2���� ������ �߰��� ������ �Ѵ�.
// 1) ���� ���� pos
// 2) normal ���������� ����
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();

// view space ���� normal �� pos �� �̿��Ѵ�.
    gl_Position = projection * (
        gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
// �ﰢ���� �� ���� ���� ��������� �Ѵ�.
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}