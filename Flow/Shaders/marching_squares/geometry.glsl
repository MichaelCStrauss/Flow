#version 150 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform float cellW;
uniform float cellH;

in vec4 Field[];
out vec3 fColor;

vec4 NE()
{
	return gl_in[0].gl_Position;
}
vec4 NW()
{
	return vec4(gl_in[0].gl_Position.x - cellW, gl_in[0].gl_Position.y, 0, 1);
}
vec4 SW()
{
	return vec4(gl_in[0].gl_Position.x - cellW, gl_in[0].gl_Position.y - cellH, 0, 1);
}
vec4 SE()
{
	return vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y - cellH, 0, 1);
}
vec4 S()
{
	vec4 s = vec4(gl_in[0].gl_Position.x - cellW * 0.5f, gl_in[0].gl_Position.y - cellH, 0, 1);
	s.x = SW().x + (SE().x - SW().x) * (1 - Field[0].w) / (Field[0].y - Field[0].w);
	return s;
}
vec4 E()
{
	vec4 e = vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y - 0.5f * cellH, 0, 1);
	e.y = SE().y + (NE().y - SE().y) * (1 - Field[0].y) / (Field[0].x - Field[0].y);
	return e;
}
vec4 W()
{
	vec4 w = vec4(gl_in[0].gl_Position.x - cellW, gl_in[0].gl_Position.y - 0.5f * cellH, 0, 1);
	w.y = SW().y + (NW().y - SW().y) * (1 - Field[0].w) / (Field[0].z - Field[0].w);
	return w;
}
vec4 N()
{
	vec4 n = vec4(gl_in[0].gl_Position.x - cellW * 0.5f, gl_in[0].gl_Position.y, 0, 1);
	n.x = NW().x + (NE().x - NW().x) * (1 - Field[0].z) / (Field[0].x - Field[0].z);
	return n;
}

void main()
{
	int val =	int(Field[0].w > 1) +
			2 * int(Field[0].y > 1) +
			4 * int(Field[0].x > 1) +
			8 * int(Field[0].z > 1);

	fColor = vec3(0, 0, 1);

	switch (val) {
		case 0:
			break;
		case 1:
			gl_Position = SW();
			EmitVertex();
			gl_Position = W();
			EmitVertex();
			gl_Position = S();
			EmitVertex();
			break;
		case 2:
			gl_Position = SE();
			EmitVertex();
			gl_Position = S();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			break;
		case 3:
			gl_Position = W();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = SW();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			break;
		case 4:
			gl_Position = N();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			break;
		case 5:
			gl_Position = SW();
			EmitVertex();
			gl_Position = W();
			EmitVertex();
			gl_Position = S();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
		case 6:
			gl_Position = S();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
		case 7:
			gl_Position = SW();
			EmitVertex();
			gl_Position = W();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
		case 8:
			gl_Position = W();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			break;
		case 9:
			gl_Position = SW();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			gl_Position = S();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			break;
		case 10:
			gl_Position = SE();
			EmitVertex();
			gl_Position = S();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = W();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			break;
		case 11:
			gl_Position = NW();
			EmitVertex();
			gl_Position = N();
			EmitVertex();
			gl_Position = SW();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			break;
		case 12:
			gl_Position = W();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
		case 13:
			gl_Position = S();
			EmitVertex();
			gl_Position = SW();
			EmitVertex();
			gl_Position = E();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
		case 14:
			gl_Position = S();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			gl_Position = W();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			break;
		case 15:
			gl_Position = SW();
			EmitVertex();
			gl_Position = NW();
			EmitVertex();
			gl_Position = SE();
			EmitVertex();
			gl_Position = NE();
			EmitVertex();
			break;
	}
	EndPrimitive();
}
