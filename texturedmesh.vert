#version 330

in vec3 posattrib;
in vec2 tcattrib;

uniform mat4 unimat40;

out vec2 fragpos;
out vec2 fragtc;

void main(){
	gl_Position = unimat40 * vec4(posattrib, 1.0);
	fragpos = gl_Position.xy;
	fragtc = tcattrib;
}
