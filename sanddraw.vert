#version 330

in vec2 tcattrib;

uniform mat4 unimat40;
uniform sampler2D texture0;
out VS_OUT {
	flat vec4 pos;
} vs_out;

void main(){
	vs_out.pos = unimat40 * vec4(texture(texture0, tcattrib).xyz, 1.0);
//	vs_out.pos = vec4(tcattrib, 0.0, 0.0);
//	vs_out.pos = vec4(0.0, 0.0, 0.5, 1.0);
//	gl_PointSize = 1.0;
}
