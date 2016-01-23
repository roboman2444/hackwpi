#version 330

in vec3 posattrib;
in vec2 tcattrib;

out vec2 fragpos;
out vec2 fragtc;

void main(){
	gl_Position = vec4(posattrib, 1.0);
	fragpos = posattrib.xy;
	fragtc = tcattrib;
}
