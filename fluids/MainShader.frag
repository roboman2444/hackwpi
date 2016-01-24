#version 330

in vec3 Color;
in vec2 pos;
uniform vec2 univec20; // Mouse Position

/*layout(location=0)*/out vec4 outColor;
void main()
{
	vec2 temp = univec20;
	vec2 mouseNormalised = vec2(temp.x,1-temp.y);//vec2(2*x -1, 1 - 2*y);	//Don't know why I need to do this
	
	float d = distance(gl_FragCoord.xy,vec2(univec20.x,480 - univec20.y));
//	outColor = vec4(d,d,d,1);
	if(d < 100){outColor = vec4(0,0,0,1.0);}
	else{outColor = vec4(1,1,1,1);}
	
	outColor = vec4(vec3(0.7), 1.0);	
}
