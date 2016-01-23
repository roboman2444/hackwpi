#version 330

uniform sampler2D texture1; // Velocity 1
in vec2 pos;
in vec2 OFFSET;
layout(location=0) out vec4 velocity0;

void main()
{
	velocity0 = texture(texture1, pos + OFFSET)*(-1.0);
}