#version 330 core

// vertex shader ���� ���޹޴� pritmive type 
// ex) point : GL_POINT �� �׸� ��
// ex) lines : GL_LINES, GL_LINE_STRIP
// ex)  lines_adjacency: GL_LINES_ADJACENCY or GL_LINE_STRIP_ADJACENCY (4).
// ex) triangles: GL_TRIANGLES, GL_TRIANGLE_STRIP or GL_TRIANGLE_FAN (3).
// ex) triangles_adjacency : GL_TRIANGLES_ADJACENCY or GL_TRIANGLE_STRI
//		P_ADJACENCY 
layout (points) in;

// Geomtery shader ���� ����� primitive type
// ex) points, line_strip, triangle_strip
// �Ʒ� ���� : 2���� vertex �� ������ line_strip �� ���
layout (line_strip, max_vertices = 2) out;


// geomtery build-in variable
// in gl_Vertex
// {
// vec4 gl_Position; // vertex shader output position
// float gl_PointSize;
// float gl_ClipDistance[];
// } gl_in[]; // array �� ���� : ��κ��� primitive �� ���� vertex �� �̷���� ����


void main() {
// EmitVertex & EndPrimitive : geomtery shader ���� ���ο� data �� ������� �� �ִ�
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);

// EmitVertex : gl_Position �� ������ ������ output primitive �� �߰��ȴ�
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
	EmitVertex();

// EndPrimitive : emitted �� ��� vertex ���� output ���� ������ primitive �� ���յȴ�.
// ��, �������� EmitVertex �Լ� ����, EndPrimitive �Լ��� ȣ���� ������ ���ο� 
// primitive �� �����ȴ�.
// �� ���, 1���� line strip primitive �� ������ �ʹ�. �׷��� 2���� EmitVertex �Լ��� ȣ���� ��
// EndPrimitive �Լ��� ȣ���Ѵ�.
	EndPrimitive();
}
