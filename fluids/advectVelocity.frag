#version 330

uniform sampler2D texture1; // Velocity 1
uniform sampler2D texture0; // Ink
in vec2 pos;
layout(location=0) out vec4 velocity0;

void main()
{
	float dt = 1.0/100;

	vec2 FragPos = pos - dt*(texture(texture1,pos).xy);

	velocity0 = vec4(texture(texture0, FragPos).xy , 0 ,1);

//	velocity0 = texture(Ink, pos);
}