#version 330

uniform sampler2D texture0;

in vec2 tc;
out vec4 fragColor;
void main(){
	if(length(tc) >= 1.0) discard;
	fragColor.rgba = vec4(1.0, 0.0, 0.0, 1.0);
}
