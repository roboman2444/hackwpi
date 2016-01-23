#version 330

in vec2 pos;
uniform vec2 univec20; // Mouse Position
uniform vec2 univec21; // Mouse Movement
uniform sampler2D texture0; // velocity0

layout(location=1) out vec4 velocity1;

void main()
{
	//DATA
	
	vec2 velocityIn = texture(texture0,pos).xy;
	velocityIn *= 0.99;

	vec2 temp = univec20;

	vec2 mouseNormalised = vec2(temp.x,1-temp.y);

	vec2 displacement = univec21;

	float dt = 1.0/60;
	
	vec2 mouseVel = displacement/dt;

	float radius = 0.05;
	if(abs(distance(pos,mouseNormalised)) < radius)
	{
		vec2 velocityOut = 0.000001*velocityIn + 0.001*mouseVel;
		velocity1 = vec4(velocityOut,0,1.0);
	}
	else
	{
		velocity1 = texture(texture0,pos);
	}
}