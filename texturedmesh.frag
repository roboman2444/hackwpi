#version 130

in vec2 fragpos;
in vec2 fragtc;
uniform sampler2D texture0;

out vec4 fragColor;
void main(){
	fragColor = texture(texture0, fragtc);
	//fragColor.rgba = vec4(0.5);
}
