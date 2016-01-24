#version 330

in sampler2D texture0;
in sampler2D texture1;
in sampler2D texture2;
in vec3 posattrib;
in vec2 tcattrib;

uniform int uniint0;

out vec2 tc;

void main(){
	gl_Position = vec4(posattrib, 1.0);
	gl_Position.z = 0.0;
	tc = tcattrib;
}
