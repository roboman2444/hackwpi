#version 330

uniform sampler2D texture0; // Pressure 0
in vec2 pos;
in vec2 OFFSET;
layout(location=2) out vec4 pressure1;

void main()
{
	pressure1 = texture(texture0, pos + OFFSET);
}