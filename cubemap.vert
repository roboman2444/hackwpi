#version 330

in vec3 posattrib;

uniform mat4 unimat40;

out vec3 fragtc;

void main(){
	gl_Position = unimat40 * vec4(posattrib, 1.0);
//	gl_Position.z = 1.0;
	fragtc = posattrib;
}
