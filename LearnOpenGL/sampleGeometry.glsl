#version 330 core

// vertex shader 에서 전달받는 pritmive type 
// ex) point : GL_POINT 를 그릴 때
// ex) lines : GL_LINES, GL_LINE_STRIP
// ex)  lines_adjacency: GL_LINES_ADJACENCY or GL_LINE_STRIP_ADJACENCY (4).
// ex) triangles: GL_TRIANGLES, GL_TRIANGLE_STRIP or GL_TRIANGLE_FAN (3).
// ex) triangles_adjacency : GL_TRIANGLES_ADJACENCY or GL_TRIANGLE_STRI
//		P_ADJACENCY 
layout (points) in;

// Geomtery shader 에서 출력할 primitive type
// ex) points, line_strip, triangle_strip
// 아래 예시 : 2개의 vertex 를 가지는 line_strip 을 출력
layout (line_strip, max_vertices = 2) out;


// geomtery build-in variable
// in gl_Vertex
// {
// vec4 gl_Position; // vertex shader output position
// float gl_PointSize;
// float gl_ClipDistance[];
// } gl_in[]; // array 인 이유 : 대부분의 primitive 가 여러 vertex 로 이루어져 있음


void main() {
// EmitVertex & EndPrimitive : geomtery shader 에서 새로운 data 를 만들어잴 수 있다
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);

// EmitVertex : gl_Position 에 설정된 정점이 output primitive 에 추가된다
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
	EmitVertex();

// EndPrimitive : emitted 된 모든 vertex 들이 output 으로 설정된 primitive 로 결합된다.
// 즉, 여러번의 EmitVertex 함수 이후, EndPrimitive 함수를 호출할 때마다 새로운 
// primitive 가 생성된다.
// 이 경우, 1개의 line strip primitive 를 만들어내고 싶다. 그래서 2개의 EmitVertex 함수를 호출한 후
// EndPrimitive 함수를 호출한다.
	EndPrimitive();
}
