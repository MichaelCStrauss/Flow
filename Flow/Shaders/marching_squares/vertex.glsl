#version 150 core
in vec2 position;
in vec4 field;

out vec4 Field;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
	gl_PointSize = 5.0;
	Field = field;
}
