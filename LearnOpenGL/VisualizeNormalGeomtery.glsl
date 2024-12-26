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
// normal vector 를 visualisze 하기 위함이다.
// 각 정점별로 normal vector 를 보여줄 것이다.
// 그러면 2개의 정점을 추가로 만들어야 한다.
// 1) 기존 정점 pos
// 2) normal 방향으로의 정점
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();

// view space 상의 normal 과 pos 를 이용한다.
    gl_Position = projection * (
        gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
// 삼각형의 각 정점 별로 실행해줘야 한다.
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}