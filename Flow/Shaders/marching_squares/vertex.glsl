#version 150 core
in vec2 position;
in float[4] field;

out float[4] Field;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
	gl_PointSize = 10.0;
	Field = field;
}
