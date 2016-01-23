#version 330
in vec2 posattrib;

uniform mat4 unimat40;
uniform sampler2D texture0;

out VS_OUT {
	flat ivec2 bp;
} vs_out;

void main(){
	vs_out.bp = ivec2(posattrib);
}
