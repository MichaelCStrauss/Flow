#version 150 core

layout(points) in;
layout(points, max_vertices = 1) out;

in vec4 Field[];
out vec3 fColor;

void main()
{
	if (Field[0].x > 1)
		fColor = vec3(0, 0, 1);
	else
		fColor = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = gl_in[0].gl_PointSize;
	EmitVertex();
	EndPrimitive();
}
