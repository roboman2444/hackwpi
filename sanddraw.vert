#version 330

in vec2 tcattrib;

uniform mat4 unimat40;
uniform sampler2D texture0;
uniform sampler2D texture3;
out VS_OUT {
	flat vec4 pos;
} vs_out;

void main(){
vec4 poost = vec4(texture(texture0, tcattrib).xyz , 1.0);
	float zees = texture(texture3, poost.xy /10.0 + 0.5 ).r;
	poost.z = zees + 0.1;
	vs_out.pos = unimat40 * poost;
//	vs_out.pos = vec4(tcattrib, 0.0, 0.0);
//	vs_out.pos = vec4(0.0, 0.0, 0.5, 1.0);
//	gl_PointSize = 1.0;
}
