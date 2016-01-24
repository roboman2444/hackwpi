#version 330

in vec3 posattrib;
in vec2 tcattrib;

uniform mat4 unimat40;
uniform sampler2D texture0;

out vec3 worldpos;
out vec2 fragtc;

void main(){
	worldpos = vec3(posattrib.xy, posattrib.z+ texture(texture0, tcattrib).r);

	gl_Position = unimat40 * vec4(worldpos, 1.0);
	fragtc = tcattrib;
}
