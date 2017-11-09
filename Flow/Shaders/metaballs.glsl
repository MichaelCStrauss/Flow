#version 150 core

in float inValue;
out float value;

void main()
{
	value = sqrt(inValue);
}