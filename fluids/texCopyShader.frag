#version 330

in vec2 pos;
uniform sampler2D texture1; // Velocity 1
layout(location=0) out vec4 velocity0;

void main()
{
	velocity0 = texture(texture1,pos);
}