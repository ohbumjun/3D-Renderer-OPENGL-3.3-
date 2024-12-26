#version 330 core
layout (triangles) in;

// 기존 삼각형을 복사해서 새로운 삼각형을 만든다
// 이후 해당 삼각형을 normal 방향으로 왔다갔다 시키는 것이다.
// 즉, 기존 삼각형 위치에 새로운 삼각형을 만들고 해당 새로운 삼각형의
// 위치를 변경하는 것이다.
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords; 

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
// 정점들을 normal 방향으로 조금씩 이동시킨다.
    float magnitude = 2.0;
// (sin(time) + 1.0) / 2 : 0 ~ 1 사이의 범위의 값을 얻고 싶다
// 정점이 normal 밖 방향으로 이동하기를 원하고, 안쪽으로는 이동하기를
// 원하지 않기 때문이다.
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}