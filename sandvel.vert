#version 330

in vec3 posattrib;
in vec2 tcattrib;

out vec2 tc;

void main(){
	gl_Position = vec4(posattrib, 1.0);
	gl_Position.z = 0.0;
	tc = tcattrib;
}
