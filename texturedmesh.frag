#version 330

in vec2 fragpos;
in vec2 fragtc;
uniform sampler2D texture0;

out vec4 fragCOlor;
void main(){
	fragColor = texture(texture0, fragtc);
}
