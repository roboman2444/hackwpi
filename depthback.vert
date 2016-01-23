#version 330
in vec3 posattrib;
in vec2 tcattrib;

uniform mat4 unimat40;
uniform sampler2D texture0;

out vec2 fragpos;
out vec2 fragtc;

void main(){
	vec3 wp;
	wp.xy = posattrib.xy;
	wp.z = posattrib.z;
	gl_Position = unimat40 * vec4(wp, 1.0);
	fragpos = gl_Position.xy;
	fragtc = tcattrib;
}
